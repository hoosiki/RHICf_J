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
#include <string>
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
    G4String SMDHCHNOP = "SMDHLeftToRight_NOP";// Number of photon
    G4String SMDVCHNOP = "SMDVTopToDown_NOP";
    G4String ZDC1CHNOP = "ZDC1FrontToRear_NOP";
    G4String ZDC2CHNOP = "ZDC2FrontToRear_NOP";
    G4String ZDC3CHNOP = "ZDC3FrontToRear_NOP";
    G4String ZDCTNOP   = "ZDCTotalNOP";
    G4String SMDHTNOP   = "SMDHTotalNOP";
    G4String SMDVTNOP  = "SMDVTotalNOP";



    
    if( fEventAction )
    {
    
        fAnalysisManager->CreateNtuple("RHICf_J", "Hits");
        fAnalysisManager->CreateNtupleDColumn("DEinZDC1");// colum id=1


        // Leaf for SMDH ID:[0-63]
        for(G4int i=1; i<33; i++)
        {

            G4String tmpstr = SMDHCHDE + std::to_string(i);
            G4cout << "tmpstr: " << tmpstr << G4endl;
            fAnalysisManager->CreateNtupleDColumn(tmpstr);
        }
        for(G4int i=1; i<33; i++)
        {

            G4String tmpstr = SMDHCHNOP + std::to_string(i);
            G4cout << "tmpstr: " << tmpstr << G4endl;
            fAnalysisManager->CreateNtupleIColumn(tmpstr);
        }
        // Leaf for SMDV ID:[64-105]
        for(G4int i=1; i<22; i++)
        {

            G4String tmpstr = SMDVCHDE + std::to_string(i);
            G4cout << "tmpstr: " << tmpstr << G4endl;
            fAnalysisManager->CreateNtupleDColumn(tmpstr);
        }
        for(G4int i=1; i<22; i++)
        {

            G4String tmpstr = SMDVCHNOP + std::to_string(i);
            G4cout << "tmpstr: " << tmpstr << G4endl;
            fAnalysisManager->CreateNtupleIColumn(tmpstr);
        }
        // Leaf for ZDC1 ID:[106-157]
        for(G4int i=1; i<27; i++)
        {

            G4String tmpstr = ZDC1CHDE + std::to_string(i);
            G4cout << "tmpstr: " << tmpstr << G4endl;
            fAnalysisManager->CreateNtupleDColumn(tmpstr);
        }
        for(G4int i=1; i<27; i++)
        {

            G4String tmpstr = ZDC1CHNOP + std::to_string(i);
            G4cout << "tmpstr: " << tmpstr << G4endl;
            fAnalysisManager->CreateNtupleIColumn(tmpstr);
        }
        // Leaf for ZDC2 ID:[158-209]
        for(G4int i=1; i<27; i++)
        {

            G4String tmpstr = ZDC2CHDE + std::to_string(i);
            G4cout << "tmpstr: " << tmpstr << G4endl;
            fAnalysisManager->CreateNtupleDColumn(tmpstr);
        }
        for(G4int i=1; i<27; i++)
        {

            G4String tmpstr = ZDC2CHNOP + std::to_string(i);
            G4cout << "tmpstr: " << tmpstr << G4endl;
            fAnalysisManager->CreateNtupleIColumn(tmpstr);
        }
        // Leaf for ZDC3 ID:[210-261]
        for(G4int i=1; i<27; i++)
        {

            G4String tmpstr = ZDC3CHDE + std::to_string(i);
            G4cout << "tmpstr: " << tmpstr << G4endl;
            fAnalysisManager->CreateNtupleDColumn(tmpstr);
        }
        for(G4int i=1; i<27; i++)
        {

            G4String tmpstr = ZDC3CHNOP + std::to_string(i);
            G4cout << "tmpstr: " << tmpstr << G4endl;
            fAnalysisManager->CreateNtupleIColumn(tmpstr);
        }
        // Leaf for ZDC1W ID:[262-288]
        for(G4int i=1; i<28; i++)
        {

            G4String tmpstr = ZDC1WCHDE + std::to_string(i);
            G4cout << "tmpstr: " << tmpstr << G4endl;
            fAnalysisManager->CreateNtupleDColumn(tmpstr);
        }
        // Leaf for ZDC2W ID:[289-315]
        for(G4int i=1; i<28; i++)
        {

            G4String tmpstr = ZDC2WCHDE + std::to_string(i);
            G4cout << "tmpstr: " << tmpstr << G4endl;
            fAnalysisManager->CreateNtupleDColumn(tmpstr);
        }
        // Leaf for ZDC3W ID:[316-342]
        for(G4int i=1; i<28; i++)
        {

            G4String tmpstr = ZDC3WCHDE + std::to_string(i);
            G4cout << "tmpstr: " << tmpstr << G4endl;
            fAnalysisManager->CreateNtupleDColumn(tmpstr);
        }

        fAnalysisManager->CreateNtupleDColumn(ZDCTDE); // ID:343
        fAnalysisManager->CreateNtupleIColumn(ZDCTNOP);// ID:344
        fAnalysisManager->CreateNtupleDColumn(I_PLTDE);// ID:345
        fAnalysisManager->CreateNtupleDColumn(W_PLTDE);// ID:346
        fAnalysisManager->CreateNtupleDColumn(SMDHTDE);// ID:347
        fAnalysisManager->CreateNtupleDColumn(SMDHTNOP);// ID:348
        fAnalysisManager->CreateNtupleDColumn(SMDVTDE);// ID:349
        fAnalysisManager->CreateNtupleDColumn(SMDVTNOP);// ID:350

        

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


