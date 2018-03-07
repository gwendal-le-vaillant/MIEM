/*
  ==============================================================================

    ControlPresenter.cpp
    Created: 6 Jun 2017 10:49:25pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "ControlPresenter.h"

#include "GraphicControlSessionManager.h"

#include "ControlModel.h"

#include "AppPurpose.h"

ControlPresenter::ControlPresenter(ControlView* view_)
:
view(view_)
{
    lastSpatStatesTree = std::make_shared<bptree::ptree>();
    lastSpatScenesTree = std::make_shared<bptree::ptree>();
}

void ControlPresenter::CompleteInitialisation(GraphicControlSessionManager* _graphicSessionManager, ControlModel* _model)
{
    // Attributs privés
    graphicSessionManager = _graphicSessionManager;
    model = _model;
    
    // Init des sous-modules
    graphicSessionManager->CompleteInitialisation(model->GetInterpolator());
}


// = = = = = XML import/export = = = = =

void ControlPresenter::LoadSession(std::string filename)
{
    bptree::ptree xmlTree, miamTree, spatTree, graphicSessionTree, settingsTree;
    try {
        // Lecture
        bptree::read_xml(filename, xmlTree);
        // puis Séparation des grandes parties du fichier
        miamTree = xmlTree.get_child("miam");
        spatTree = miamTree.get_child("spat");
        graphicSessionTree = miamTree.get_child("graphicsession");
        settingsTree = miamTree.get_child("settings");
    }
    catch(bptree::ptree_error &e) {
        throw XmlReadException("Cannot load session: ", e);
    }
    
    // S'il n'y a pas eu d'erreur de lecture : le nom/chemin de session est celui de celle
    // qui vient d'être chargée
    lastFilename = filename;
    
    // Envoi de chaque grande partie à la sous-partie de spat concernée
    // Si n'importe laquelle échoue : on annule tout....
    try {
        // Envoie une XmlReadException s'il y a une erreur
        App::CheckSessionVersionNumber(miamTree);
        
        // Config modèle puis modèle
        model->SetConfigurationFromTree(settingsTree.get_child("model"));
        model->GetInterpolator()->SetStatesFromTree(spatTree);
        // Config graphique puis presenter
        this->SetConfigurationFromTree(settingsTree);
        graphicSessionManager->SetFromTree(graphicSessionTree);
    }
    catch (XmlReadException& e)
    {
        // Remise à zéro de tous les modules, commandée par arbre vide
        bptree::ptree emptyTree = bptree::ptree();
        model->GetInterpolator()->SetStatesFromTree(emptyTree);
        graphicSessionManager->SetFromTree(emptyTree);
        
        // Pour l'instant : on laisse la configuration dans son état précédent....
        // Pas de réinitialisation
        
        // Remise à zéro des états locaux internes
        lastFilename = "";
        
        // Renvoi pour affichage graphique
        throw e;
    }
    
    // Update graphique
    view->SetTitle(lastFilename + " - " + ProjectInfo::projectName);
}
void ControlPresenter::SaveSession(std::string _filename, bool /*forceDataRefresh*/)
{
    // Checks about the filename :
    // If not empty, the argument passed becomes the actual filename
    if (! _filename.empty() )
        lastFilename = _filename;
    // Else, we continue only if a filename is currently in use
    else if (lastFilename.empty())
    {
        LoadFileChooser fileChooser({App::GetPurpose()});
#ifndef __MIAMOBILE
        if ( fileChooser.browseForFileToSave(true) )
        {
            File resultFile = fileChooser.getResult();
            lastFilename = resultFile.getFullPathName().toStdString();
        }
        // Sinon, si l'utilisateur ne choisit rien : on quitte juste la fonction via une exception
        // (dont le contenu sera affiché par le presenter réel, s'il peut)
        else
            throw XmlWriteException("File not saved");

#else
        /* This function uses pop-ups and
         * must not be executed form a mobile platform.
         */
        assert(0);
#endif
    }
    
    // Whole properties tree reconstruction
    bptree::ptree miamChildrenTree;
    miamChildrenTree.put("<xmlattr>.appVersion", ProjectInfo::versionNumber);
    miamChildrenTree.put("<xmlattr>.appPurpose", App::GetPurposeName(App::GetPurpose()));
    // Les settings sont ajoutés catégorie par catégorie
    bptree::ptree settingsTree;
    settingsTree.add_child("model", *(model->GetConfigurationTree()) );
    settingsTree.add_child("presenter", *(this->GetConfigurationTree()) );
    miamChildrenTree.add_child("settings", settingsTree);
    // Puis on sauvegarde les données des modules
    miamChildrenTree.add_child("spat", *lastSpatStatesTree);
    miamChildrenTree.add_child("graphicsession", *lastSpatScenesTree);
    
    auto wholeMiamTree = std::make_shared<bptree::ptree>();
    wholeMiamTree->add_child("miam", miamChildrenTree);
    
    // Actual XML data writing into file
    bptree::xml_writer_settings<std::string> xmlSettings(' ', 4);
    try {
        bptree::write_xml(lastFilename, *wholeMiamTree, std::locale(), xmlSettings);
    }
    catch (bptree::xml_parser::xml_parser_error& e) {
        throw XmlWriteException(e.what());
    }
    
    // Update graphique
    view->SetTitle(lastFilename + " - " + ProjectInfo::projectName);
}
void ControlPresenter::updateSpatStatesTree(std::shared_ptr<bptree::ptree> newTree, bool autoSave)
{
    lastSpatStatesTree = newTree;
    if (autoSave)
        SaveSession();
}
void ControlPresenter::updateSpatScenesTree(std::shared_ptr<bptree::ptree> newTree, bool autoSave)
{
    lastSpatScenesTree = newTree;
    if (autoSave)
        SaveSession();
}
