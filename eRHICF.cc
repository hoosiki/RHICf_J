// Edited by Junsang Park. 2014.12.10
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
#include "G4UICommand.hh"
#include "QGSP_BERT.hh"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif
#include <ctime>
/////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
/////////////////////////////////////////////////////////////////////////////////
{

    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    time_t systime = time(NULL);

    G4long myseed = systime*G4UniformRand();


#ifdef G4MULTITHREADED
    G4MTRunManager *runManager = new G4MTRunManager;
    runManager -> SetNumberOfThreads(4);
#else
    G4RunManager* runManager = new G4RunManager;
#endif

    G4Random::setTheSeed(myseed);

    G4VUserDetectorConstruction* RHICFDC = new RHICFDetectorConstruction();


    


    runManager -> SetUserInitialization(RHICFDC);

    G4VModularPhysicsList* physicsList = new RHICFPhysicsList("QGSP_BERT");

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
    
#ifdef G4VIS_USE
    delete visManager;
#endif

 
    delete runManager;

    return 0;
}
