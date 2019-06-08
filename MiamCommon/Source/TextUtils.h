/*
  ==============================================================================

    TextUtils.h
    Created: 11 Oct 2017 8:36:32pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <string>
#include <algorithm> // std::min

#include "JuceHeader.h"

#include "AudioDefines.h"
#include "MiamExceptions.h"


// - - - - - - OSC CUSTOM ADDRESS TAGS - - - - - - -
// - - - - - - OSC CUSTOM ADDRESS TAGS - - - - - - -
#define Miam_OscIntInAddressTag     "[int]"
#define Miam_OscFloatInAddressTag   "[float]"
// - - - - - - OSC CUSTOM ADDRESS TAGS - - - - - - -
// - - - - - - OSC CUSTOM ADDRESS TAGS - - - - - - -



namespace Miam {
    
    class TextUtils
    {
        private :
        static std::string oscIntInAddressTag;
        static std::string oscFloatInAddressTag;
        
        public :
        
        /// \brief Finds a filename from a single-line set of arguments given to the program as
        /// command-line arguments. The filename is the value given after the "-session" option.
        ///
        /// \returns The filename without quote marks, or an empty string if nothing was found
        static std::string FindFilenameInCommandLineArguments(std::string commandLineToParse);
        
        /// \brief Sépare une chaîne de caractères du type "/adresseOSC/et/des/choses [int] 2
        /// [int] 647 .... [float] 45.2 [float] 32.5 ...." vers un message Osc
        ///
        /// Seul l'espace peut servir de séparateur.
        ///
        /// Might throw a Miam::ParseException if necessary
        static OSCMessage ParseStringToJuceOscMessage(const std::string& stringToParse);
        
        template<typename T>
        static std::u16string ConvertNumberToU16string(T number, int numberOfCharactersToDisplay = -1)
        {
            std::u16string stringU16 = u"";
            std::string stringASCII = std::to_string(number);
            
            size_t u16StringFinalLength = stringASCII.length();
            if (numberOfCharactersToDisplay >= 0)
                u16StringFinalLength = std::min(u16StringFinalLength, (size_t(numberOfCharactersToDisplay)));
                
            for (int i = 0; i < u16StringFinalLength; ++i)
                stringU16 += char16_t(stringASCII[i]);
            return stringU16;
        }
        
        /// \brief Tries to parse the string into a valid UDP port number,
        /// or sends a Miam::ParseException
        static int ParseUdpPortNumber(const std::string& udpPort);
        
        
        /// \brief Returns a string that is made from numberOfDigits digits, not
        /// couting the dot of the floating-point number nor the + or - sign.
        ///
        /// There is always a zero for numbers between 0.0 and 0.9999...
        static std::string GetLimitedDigitsString(double value, int numberOfDigits);
        
        /// \brief Converts a linear numeric value to a decibels string objects, with the
        /// specified parameters.
        ///
        /// The number of digits might be different pour minus infinity dB.
        static std::string GetAmplitude_dB_string_from_Linear(double linearValue, int numberOfDigits);
        
        
        /// \brief Returns the IP address as a string, or an empty string if given character sequence
        /// is not valid. Makes the textbox's text bold is parsed succeeded.
        static std::string TryParseAndBoldenIpAddress(juce::TextEditor* textEditor);
        /// \brief Returns the UDP port as a positive int, or -1 if user input was not valid.
        ///  Makes the textbox's text bold is parsed succeeded.
        static int TryParseAndBoldenUdpPort(juce::TextEditor* textEditor);
        
        
        /// \brief Returns a string "CmdX" (with the actual apple knot cmd sign)
        /// or "Ctrl X" for win/linux, automatically depending on the platform
        static juce::String GetCommandKeyDescription(int keyCode);
        static juce::String GetCommandShiftKeyDescription(int keyCode);
        /// \brief Return a description of a key. (normal or action)
        static juce::String GetKeyDescription(int keyCode);
    };
}
