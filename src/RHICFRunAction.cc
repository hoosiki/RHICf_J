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

    G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();
    fAnalysisManager->SetVerboseLevel(1);
    fAnalysisManager->SetFileName("RHICf_J");

    if( fEventAction )
    {
    
        fAnalysisManager->CreateNtuple("RHICf_J", "Hits");
        fAnalysisManager->CreateNtupleIColumn("NOfPinZDC1");
        fAnalysisManager->CreateNtupleIColumn("NOfPinZDC2");
        fAnalysisManager->CreateNtupleIColumn("NOfPinZDC3");
        fAnalysisManager->CreateNtupleIColumn("TotalNumberOfPhotons");
        fAnalysisManager->CreateNtupleDColumn("DEinZDC1");
        fAnalysisManager->CreateNtupleDColumn("DEinZDC2");
        fAnalysisManager->CreateNtupleDColumn("DEinZDC3");
        fAnalysisManager->CreateNtupleDColumn("TotalDepositE");
        fAnalysisManager->CreateNtupleDColumn("XinSMD");
        fAnalysisManager->CreateNtupleDColumn("YinSMD");
        fAnalysisManager->FinishNtuple();
    }

}

////////////////////////////////////////////////////////////////////////////////
RHICFRunAction::~RHICFRunAction()
////////////////////////////////////////////////////////////////////////////////
{
    delete G4AnalysisManager::Instance();
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


    G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();

    fAnalysisManager->OpenFile();


}



////////////////////////////////////////////////////////////////////////////////
void RHICFRunAction::EndOfRunAction(const G4Run* run)
////////////////////////////////////////////////////////////////////////////////
{

    G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();
    fAnalysisManager->Write();
    fAnalysisManager->CloseFile();

 
}


