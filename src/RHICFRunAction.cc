// Edited by Junsang Park. 2014.12.10
#include "RHICFRunAction.hh"
////////////////////////////////////////////////////////////////////////////////
#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "g4root.hh"
#include <ctime>
#include <unistd.h>
////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////
RHICFRunAction::RHICFRunAction(RHICFEventAction* eventAction): G4UserRunAction(), fEventAction(eventAction)
////////////////////////////////////////////////////////////////////////////////
{

    //Junsang****G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();
    //Junsang****fAnalysisManager->SetVerboseLevel(1);
    //Junsang****fAnalysisManager->SetFileName("RHICf_J");
//Junsang****
    //Junsang****if( fEventAction )
    //Junsang****{
    //Junsang****
        //Junsang****fAnalysisManager->CreateNtuple("RHICf_J", "Hits");
        //Junsang****fAnalysisManager->CreateNtupleDColumn("DEinZDC1");
        //Junsang****fAnalysisManager->CreateNtupleDColumn("DEinZDC2");
        //Junsang****fAnalysisManager->CreateNtupleDColumn("DEinZDC3");
        //Junsang****fAnalysisManager->CreateNtupleDColumn("TotalDepositE");
        //Junsang****fAnalysisManager->CreateNtupleDColumn("XinSMD");
        //Junsang****fAnalysisManager->CreateNtupleDColumn("YinSMD");
        //Junsang****fAnalysisManager->FinishNtuple();
    //Junsang****}

}

////////////////////////////////////////////////////////////////////////////////
RHICFRunAction::~RHICFRunAction()
////////////////////////////////////////////////////////////////////////////////
{
    //Junsang****delete G4AnalysisManager::Instance();
}


////////////////////////////////////////////////////////////////////////////////
void RHICFRunAction::BeginOfRunAction(const G4Run* run)
////////////////////////////////////////////////////////////////////////////////
{

    G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");

    long seeds[2];

    time_t systime = time(NULL) + getpid();
    seeds[0] = (long) systime;
    seeds[1] = (long) (systime*G4UniformRand());

    G4Random::setTheSeeds(seeds);


    //Junsang****G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();
//Junsang****
    //Junsang****fAnalysisManager->OpenFile();


}



////////////////////////////////////////////////////////////////////////////////
void RHICFRunAction::EndOfRunAction(const G4Run* run)
////////////////////////////////////////////////////////////////////////////////
{

    //Junsang****G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();
    //Junsang****fAnalysisManager->Write();
    //Junsang****fAnalysisManager->CloseFile();

 
}


