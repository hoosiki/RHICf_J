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

    
    G4String SMDHCHDE = "SMDHLeftToRight_DE";// Deposit energy
    G4String SMDVCHDE = "SMDVTopToDown_DE";
    G4String ZDC1CHDE = "ZDC1FrontToRear_DE";
    G4String ZDC2CHDE = "ZDC2FrontToRear_DE";
    G4String ZDC3CHDE = "ZDC3FrontToRear_DE";
    G4String ZDC1WCHDE = "ZDC1WFrontToReal_DE";
    G4String ZDC2WCHDE = "ZDC2WFrontToReal_DE";
    G4String ZDC3WCHDE = "ZDC3WFrontToReal_DE";
    G4String SMDHTDE   = "SMDHTotalDE";
    G4String SMDVTDE   = "SMDVTotalDE";
    G4String ZDCTDE    = "ZDCTotalDE";
    G4String W_PLTDE   = "TungstenTotalDE";
    G4String I_PLTDE   = "IronTotalDE";
    G4String SMDHCHNOP = "LeftToRight_NOP";// Number of photon
    G4String SMDVCHNOP = "TopToDown_NOP";
    G4String ZDC1CHNOP = "ZDC1FrontToRear_NOP";
    G4String ZDC2CHNOP = "ZDC2FrontToRear_NOP";
    G4String ZDC3CHNOP = "ZDC3FrontToRear_NOP";



    
    if( fEventAction )
    {
    
        fAnalysisManager->CreateNtuple("RHICf_J", "Hits");
        fAnalysisManager->CreateNtupleDColumn("DEinZDC1");// colum id=1

        for(

        
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


