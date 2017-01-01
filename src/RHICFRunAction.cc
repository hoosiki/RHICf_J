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
RHICFRunAction::RHICFRunAction(): G4UserRunAction()
////////////////////////////////////////////////////////////////////////////////
{
    /*
    G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();
    fAnalysisManager -> SetVerboseLevel(1);
    fAnalysisManager -> SetFileName("ZDC");
    

    fAnalysisManager -> CreateNtuple("ZDC", "Numberofphoton");
*/


/*    
    fAnalysisManager -> CreateNtupleIColumn("FCSCHits");//Id = 0
    fAnalysisManager -> CreateNtupleIColumn("RCSCHits");//Id = 1
    fAnalysisManager -> CreateNtupleIColumn("SMDHits");//Id = 2
    fAnalysisManager -> CreateNtupleDColumn("FCSCEnergy");//Id = 3
    fAnalysisManager -> CreateNtupleDColumn("RCSCEnergy");//Id = 4
    fAnalysisManager -> CreateNtupleDColumn("SMDEnergy");//Id = 5
    fAnalysisManager -> CreateNtupleDColumn("FCSCX");//Id = 6
    fAnalysisManager -> CreateNtupleDColumn("FCSCY");//Id = 7
    fAnalysisManager -> CreateNtupleDColumn("RCSCX");//Id = 8
    fAnalysisManager -> CreateNtupleDColumn("RCSCY");//Id = 9
    fAnalysisManager -> CreateNtupleDColumn("FCSCZ");//Id = 10
    fAnalysisManager -> CreateNtupleDColumn("FIBREnergy");//Id = 11
    fAnalysisManager -> CreateNtupleIColumn("FIBRHits");//Id = 12
*/ 


/*
    fAnalysisManager -> CreateNtupleDColumn("Number");
    fAnalysisManager -> CreateNtupleDColumn("DepositEnergy");
    fAnalysisManager -> CreateNtupleDColumn("DepEW");
    fAnalysisManager -> FinishNtuple();
    */

}

////////////////////////////////////////////////////////////////////////////////
RHICFRunAction::~RHICFRunAction()
////////////////////////////////////////////////////////////////////////////////
{
    //delete G4AnalysisManager::Instance();
}


////////////////////////////////////////////////////////////////////////////////
void RHICFRunAction::BeginOfRunAction(const G4Run* run)
////////////////////////////////////////////////////////////////////////////////
{
    //G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();
    //fAnalysisManager -> OpenFile();

    G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");

    long seeds[2];

    time_t systime = time(NULL) + getpid();
    seeds[0] = (long) systime;
    seeds[1] = (long) (systime*G4UniformRand());

    G4Random::setTheSeeds(seeds);
    //G4Random::showEngineStatus(); 


}



////////////////////////////////////////////////////////////////////////////////
void RHICFRunAction::EndOfRunAction(const G4Run* run)
////////////////////////////////////////////////////////////////////////////////
{


  // print the marker of end of run
  //
  //
  /*
    G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();
    fAnalysisManager -> Write();
    fAnalysisManager -> CloseFile();
    */
 
}


