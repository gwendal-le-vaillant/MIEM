/*
  ==============================================================================

    ReaperOscController.cpp
    Created: 13 Feb 2019 5:35:34pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <thread>
#include <cmath>

#include "boost/lexical_cast.hpp"

#include "ReaperOscController.h"

#include "OSCRecorder.h"

#include "MonitoringServer.h"


ReaperOscController::ReaperOscController(int _tracksCount)
:
tracksCount(_tracksCount)
{
    // vérification qu'on peut bien actualiser tous les mutes
    // avant de re-lancer une phase d'écoute
    // (pour 25 tracks dans Reaper)
    assert((25 * ForcedRefreshInterMessageDelay_ms)
           < (OSCRecorder::ListenAutoTriggerDelay_s * 1000));
    
    // resize of vectors for optimize memory mute systems
    tracksMuteStates.resize(tracksCount, TrackMuteState::Undefined);
    tracksMuteStates_waitingForReaperResponse.resize(tracksCount, false);
    
    // Sender setup
    if (! sender.connect(hostIp, reaperUdpPort))
        displayErrorAndThrowException(TRANS("Cannot connect OSC sender to Reaper at ") + String(hostIp) + ":" + String(reaperUdpPort) + TRANS(". Please check parameters."));
    
    // receiver : does not need to be real-time. The UI can wait a bit for REAPER's responses
    if (! receiver.connect(listeningUdpPort))
        displayErrorAndThrowException(TRANS("Cannot connect OSC receiver from Reaper on port ") +  String(listeningUdpPort) + TRANS(". Please check parameters."));
    
    receiver.addListener(this);
}



void ReaperOscController::oscMessageReceived (const OSCMessage &message)
{
    //std::cout << "NORMAL OSC msg received from REAPER" << std::endl;
    //assert(false); // no treated... REAPER seems to send bundles only
}
void ReaperOscController::oscBundleReceived (const OSCBundle & oscBundle)
{
    //std::cout << "Reaper BUNDLE received" << std::endl;
}



void ReaperOscController::RestartAndPlay(float tempo)
{
    MonitoringServer::SendLog("[OSC -> REAPER]: REPLAY, TEMPO = "
                                  + boost::lexical_cast<std::string>(tempo));
    
    // TEMPO set before playing
    String oscAddress = "/tempo/raw";
    OSCMessage oscMsgTempo = OSCMessage(OSCAddressPattern(oscAddress));
    oscMsgTempo.addFloat32(tempo);
    sendMessageOrThrowException(oscMsgTempo);
    
    // Délimitation de la boucle (à recalculer pour 16*4 temps, selon tempo)
    oscAddress = "/loop/start/time";
    OSCMessage oscMsgLoop1 = OSCMessage(OSCAddressPattern(oscAddress));
    oscMsgLoop1.addFloat32(0.0);
    sendMessageOrThrowException(oscMsgLoop1);
    float loopLength_s = (float)(16 * 4) / (tempo / 60.0f) ;
    oscAddress = "/loop/end/time";
    OSCMessage oscMsgLoop2 = OSCMessage(OSCAddressPattern(oscAddress));
    oscMsgLoop2.addFloat32(loopLength_s);
    sendMessageOrThrowException(oscMsgLoop2);
    
    // Then : RESTART
    oscAddress = "/time";
    OSCMessage oscMsgTime = OSCMessage(OSCAddressPattern(oscAddress));
    oscMsgTime.addFloat32(0.0f); // works ?
    sendMessageOrThrowException(oscMsgTime);
    
    // DE-MUTE après avoir placé la tête de lecture
    oscAddress = "/master/volume";
    OSCMessage oscVolumeMsg = OSCMessage(OSCAddressPattern(oscAddress));
    oscVolumeMsg.addFloat32(0.715f);
    sendMessageOrThrowException(oscVolumeMsg);
    
    // At last : PLAY
    oscAddress = "/play";
    OSCMessage oscTriggerMsg = OSCMessage(OSCAddressPattern(oscAddress));
    oscTriggerMsg.addInt32(1); // trigger forced to 1 (not a real trigger then...)
    sendMessageOrThrowException(oscTriggerMsg);
}
void ReaperOscController::Stop()
{
    MonitoringServer::SendLog("[OSC -> REAPER]: STOP");
    
    // STOP
    String oscAddress = "/stop";
    OSCMessage oscTriggerMsg = OSCMessage(OSCAddressPattern(oscAddress));
    oscTriggerMsg.addInt32(1); // trigger forced to 1 (not a real trigger then...)
    sendMessageOrThrowException(oscTriggerMsg);
    
    // RE-MUTE après un petit délai,
    // pour éviter les échos plus tard
    Timer::callAfterDelay(/*(int) std::round((double)(OSCRecorder::delayAfterFinished_ms) * 0.9)*/ 200,
                          [this]
                          {
                              String oscAddress = "/master/volume";
                              OSCMessage oscVolumeMsg = OSCMessage(OSCAddressPattern(oscAddress));
                              oscVolumeMsg.addFloat32(0.0f);
                              sendMessageOrThrowException(oscVolumeMsg);
                          });
}


void ReaperOscController::SetTrackSolo_usingMutes(int trackToUnmute, bool forceResendAllMutes)
{
    if (trackToUnmute <= 0)
        MonitoringServer::SendLog("[OSC -> REAPER]: mute pour TOUTES les tracks");
    else
        MonitoringServer::SendLog("[OSC -> REAPER]: track "
                                      + boost::lexical_cast<std::string>(trackToUnmute)
                                      + " Solo.");
    
    // Envoi bourrin de tous les mutes (actualisation forcée)
    if (forceResendAllMutes)
    {
        MonitoringServer::SendLog("[OSC -> REAPER] (...forced and delayed refresh of all tracks mute states...)");
        tracksMuteStates.clear();
        tracksMuteStates.resize(tracksCount, TrackMuteState::Undefined);
    }
    
    // envoi optimisé (ou non...) à partir des valeurs précédentes
    int forcedRefreshMessagesCount = 0; ///< Number of force-refresh messages already sent
    for (int i=1 ; i<=tracksCount ; i++)
    {
        // tracks à muter (si nécessaire)
        if (i != trackToUnmute)
        {
            if (tracksMuteStates[i - 1] != TrackMuteState::Muted)
            {
                if (! forceResendAllMutes)
                    setTrackMuteState(i, true);
                else
                {
                    Timer::callAfterDelay(forcedRefreshMessagesCount
                                          * ForcedRefreshInterMessageDelay_ms,
                                          [this, i]
                                          {setTrackMuteState(i, true);});
                    forcedRefreshMessagesCount++;
                }
            }
        }
        // track à dé-muter (si nécessaire)
        else
        {
            if (tracksMuteStates[i - 1] != TrackMuteState::Unmuted)
            {
                if (! forceResendAllMutes)
                    setTrackMuteState(i, false);
                else
                {
                    Timer::callAfterDelay(forcedRefreshMessagesCount
                                          * ForcedRefreshInterMessageDelay_ms,
                                          [this, i]
                                          {setTrackMuteState(i, false);});
                    forcedRefreshMessagesCount++;
                }
            }
        }
    }
}



void ReaperOscController::setTrackMuteState(int trackNumber, bool shouldBeMuted)
{
    String oscAddress = "/track/";
    oscAddress += String(trackNumber);
    oscAddress += "/mute";
    OSCMessage oscMsg = OSCMessage(OSCAddressPattern(oscAddress));
    if (shouldBeMuted)
        oscMsg.addInt32(1);
    else
        oscMsg.addInt32(0);
    
    sendMessageOrThrowException(oscMsg);
    
    // actualisation du système de mémoire interne
    if (shouldBeMuted)
        tracksMuteStates[trackNumber - 1] = TrackMuteState::Muted;
    else
        tracksMuteStates[trackNumber - 1] = TrackMuteState::Unmuted;
}

void ReaperOscController::waitForMissingReaperResponses()
{
    // en fait... on dirait que Reaper ne fait que transmettre les modifs utilisateur
    // dans le doute : on va renvoyer tous les paquets OSC nécessaires à chaque changement !
}


void ReaperOscController::sendMessageOrThrowException(OSCMessage& oscMessage)
{
    if ( ! sender.send(oscMessage) )
        displayErrorAndThrowException(TRANS("Impossible to send an OSC message to Reaper. Check call stack to find the type of message being sent."));
}

void ReaperOscController::displayErrorAndThrowException(String errorStr)
{
    MonitoringServer::SendLog(errorStr.toStdString());
    std::this_thread::sleep_for(std::chrono::seconds(10)); // time before looking at the console
    throw std::runtime_error(errorStr.toStdString());
}
