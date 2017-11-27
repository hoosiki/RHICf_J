#include "RHICFDetectorConstruction.hh"
#include "RHICFActionInitialization.hh"
#include "RHICFPhysicsList.hh"
#include "Randomize.hh"
/////////////////////////////////////////////////////////////////////////////////
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4UImanager.hh"
#include "G4ParallelWorldPhysics.hh"
#include "QGSP_BERT.hh"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif
#include <ctime>
#include <cstdlib>
#include "FileManager.hh"

int main(int argc, char** argv)
{
    system("date");
#ifdef G4MULTITHREADED
    G4cout << 28 << G4endl;
    G4MTRunManager *runManager = new G4MTRunManager;
#else
    G4RunManager* runManager = new G4RunManager;
#endif
    G4cout << 33 << G4endl;
    G4VUserDetectorConstruction* RHICFDC = new RHICFDetectorConstruction();
    G4cout << 35 << G4endl;
    runManager -> SetUserInitialization(RHICFDC);

    G4cout << 38 << G4endl;
    G4VModularPhysicsList* physicsList = new RHICFPhysicsList("QGSP_BERT");

    G4cout << 41 << G4endl;
    runManager -> SetUserInitialization(physicsList);
    G4VUserActionInitialization* actions = new RHICFActionInitialization;
    runManager -> SetUserInitialization(actions);
    runManager -> Initialize();
#ifdef G4VIS_USE
    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive();
    visManager -> Initialize();
#endif
    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();


    if(argc!=1) // batch mode
    {
        G4String command = "/control/execute ";
        G4String macro = argv[1];
        UImanager -> ApplyCommand(command+macro);
    }else
    {
        // interactive mode : define UI session
#ifdef G4UI_USE
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
        UImanager -> ApplyCommand("/control/execute init_vis.mac");
#else
        UImanager -> ApplyCommand("/control/execute init.mac");
#endif
        if(ui -> IsGUI())
            UImanager -> ApplyCommand("/control/execute gui.mac");
        ui -> SessionStart();
        delete ui;
#endif
    }

    FileManager* fFileManager = FileManager::GetInstance();
    std::string Command1 = "mv " + fFileManager->GetPathFortmp()+"/"+fFileManager->GetFileName()+".root " +fFileManager->GetPathForSIMDATA()+"/"+fFileManager->GetFileName()+".root";
    system(Command1.c_str());


#ifdef G4VIS_USE
    delete visManager;
#endif
    delete runManager;

    system("date");
    return 0;
}
