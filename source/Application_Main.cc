#include "Geometry.hh"
#include "UserActionInitialization.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "FTFP_BERT.hh"

#include <iostream>

int main(int argc, char** argv){
    G4RunManager* runManager = new G4RunManager;
    runManager->SetUserInitialization(new Geometry);
    runManager->SetUserInitialization(new FTFP_BERT);
    runManager->SetUserInitialization(new UserActionInitialization);
    runManager->Initialize();

    G4VisExecutive *visManager = new G4VisExecutive;
    visManager->Initialize();

    G4UImanager* uiManager = G4UImanager::GetUIpointer();

    if (argc == 1){
        // interactive mode
        G4UIExecutive* uiExec = new G4UIExecutive(argc, argv);
        // vis_init
        uiManager->ApplyCommand("/control/execute visSetup_Simplex.mac");
        uiExec->SessionStart();
        delete uiExec;
    } else {
        // macro mode
        G4String macroName = argv[1];
        uiManager->ApplyCommand("/control/execute" + macroName);
    }

    delete runManager;
}