// Edited by Junsang Park. 2014.12.10
#include "RHICFRunAction.hh"
#include "RHICFDetectorConstruction.hh"
////////////////////////////////////////////////////////////////////////////////
#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include <ctime>
#include <unistd.h>
#include <string>
////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////
RHICFRunAction::RHICFRunAction(RHICFEventAction* eventAction): G4UserRunAction(), fEventAction(eventAction)
////////////////////////////////////////////////////////////////////////////////
{

    //Junsang****G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();
    //Junsang****fAnalysisManager->SetVerboseLevel(1);
    //Junsang****fAnalysisManager->SetFileName("RHICf_J");
//Junsang****
//Junsang****/*-*/// Define tree name in root file
//Junsang****/*-*/G4String SMDHCHDE = "SMDHLeftToRight_DE";// Deposit energy
//Junsang****/*-*/G4String SMDVCHDE = "SMDVTopToDown_DE";
//Junsang****/*-*/G4String ZDC1CHDE = "ZDC1FrontToRear_DE";
//Junsang****/*-*/G4String ZDC2CHDE = "ZDC2FrontToRear_DE";
//Junsang****/*-*/G4String ZDC3CHDE = "ZDC3FrontToRear_DE";
//Junsang****/*-*/G4String ZDC1WCHDE = "ZDC1WFrontToReal_DE";
//Junsang****/*-*/G4String ZDC2WCHDE = "ZDC2WFrontToReal_DE";
//Junsang****/*-*/G4String ZDC3WCHDE = "ZDC3WFrontToReal_DE";
//Junsang****/*-*/G4String SMDHTDE   = "SMDHTotalDE";
//Junsang****/*-*/G4String SMDVTDE   = "SMDVTotalDE";
//Junsang****/*-*/G4String ZDCTDE    = "ZDCTotalDE";
//Junsang****/*-*/G4String W_PLTDE   = "TungstenTotalDE";
//Junsang****/*-*/G4String I_PLTDE   = "IronTotalDE";
//Junsang****/*-*/G4String SMDHCHNOP = "SMDHLeftToRight_NOP";// Number of photon
//Junsang****/*-*/G4String SMDVCHNOP = "SMDVTopToDown_NOP";
//Junsang****/*-*/G4String ZDC1CHNOP = "ZDC1FrontToRear_NOP";
//Junsang****/*-*/G4String ZDC2CHNOP = "ZDC2FrontToRear_NOP";
//Junsang****/*-*/G4String ZDC3CHNOP = "ZDC3FrontToRear_NOP";
//Junsang****/*-*/G4String ZDCTNOP   = "ZDCTotalNOP";
//Junsang****/*-*/G4String SMDHTNOP   = "SMDHTotalNOP";
//Junsang****/*-*/G4String SMDVTNOP  = "SMDVTotalNOP";
//Junsang****
//Junsang****
//Junsang****
    //Junsang****
    //Junsang****if( fEventAction )
    //Junsang****{
    //Junsang****
        //Junsang****fAnalysisManager->CreateNtuple("RHICf_J", "Hits");
//Junsang****
//Junsang****
        //Junsang****// Leaf for SMDH ID:[0-63]
        //Junsang****for(G4int i=1; i<33; i++)
        //Junsang****{
//Junsang****
            //Junsang****G4String tmpstr = SMDHCHDE + std::to_string(i);
            //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
            //Junsang****fAnalysisManager->CreateNtupleDColumn(tmpstr);
        //Junsang****}
        //Junsang****for(G4int i=1; i<33; i++)
        //Junsang****{
//Junsang****
            //Junsang****G4String tmpstr = SMDHCHNOP + std::to_string(i);
            //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
            //Junsang****fAnalysisManager->CreateNtupleIColumn(tmpstr);
        //Junsang****}
        //Junsang****// Leaf for SMDV ID:[64-105]
        //Junsang****for(G4int i=1; i<22; i++)
        //Junsang****{
//Junsang****
            //Junsang****G4String tmpstr = SMDVCHDE + std::to_string(i);
            //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
            //Junsang****fAnalysisManager->CreateNtupleDColumn(tmpstr);
        //Junsang****}
        //Junsang****for(G4int i=1; i<22; i++)
        //Junsang****{
//Junsang****
            //Junsang****G4String tmpstr = SMDVCHNOP + std::to_string(i);
            //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
            //Junsang****fAnalysisManager->CreateNtupleIColumn(tmpstr);
        //Junsang****}
        //Junsang****// Leaf for ZDC1 ID:[106-157]
        //Junsang****for(G4int i=1; i<27; i++)
        //Junsang****{
//Junsang****
            //Junsang****G4String tmpstr = ZDC1CHDE + std::to_string(i);
            //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
            //Junsang****fAnalysisManager->CreateNtupleDColumn(tmpstr);
        //Junsang****}
        //Junsang****for(G4int i=1; i<27; i++)
        //Junsang****{
//Junsang****
            //Junsang****G4String tmpstr = ZDC1CHNOP + std::to_string(i);
            //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
            //Junsang****fAnalysisManager->CreateNtupleIColumn(tmpstr);
        //Junsang****}
        //Junsang****// Leaf for ZDC2 ID:[158-209]
        //Junsang****for(G4int i=1; i<27; i++)
        //Junsang****{
//Junsang****
            //Junsang****G4String tmpstr = ZDC2CHDE + std::to_string(i);
            //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
            //Junsang****fAnalysisManager->CreateNtupleDColumn(tmpstr);
        //Junsang****}
        //Junsang****for(G4int i=1; i<27; i++)
        //Junsang****{
//Junsang****
            //Junsang****G4String tmpstr = ZDC2CHNOP + std::to_string(i);
            //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
            //Junsang****fAnalysisManager->CreateNtupleIColumn(tmpstr);
        //Junsang****}
        //Junsang****// Leaf for ZDC3 ID:[210-261]
        //Junsang****for(G4int i=1; i<27; i++)
        //Junsang****{
//Junsang****
            //Junsang****G4String tmpstr = ZDC3CHDE + std::to_string(i);
            //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
            //Junsang****fAnalysisManager->CreateNtupleDColumn(tmpstr);
        //Junsang****}
        //Junsang****for(G4int i=1; i<27; i++)
        //Junsang****{
//Junsang****
            //Junsang****G4String tmpstr = ZDC3CHNOP + std::to_string(i);
            //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
            //Junsang****fAnalysisManager->CreateNtupleIColumn(tmpstr);
        //Junsang****}
        //Junsang****// Leaf for ZDC1W ID:[262-288]
        //Junsang****for(G4int i=1; i<28; i++)
        //Junsang****{
//Junsang****
            //Junsang****G4String tmpstr = ZDC1WCHDE + std::to_string(i);
            //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
            //Junsang****fAnalysisManager->CreateNtupleDColumn(tmpstr);
        //Junsang****}
        //Junsang****// Leaf for ZDC2W ID:[289-315]
        //Junsang****for(G4int i=1; i<28; i++)
        //Junsang****{
//Junsang****
            //Junsang****G4String tmpstr = ZDC2WCHDE + std::to_string(i);
            //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
            //Junsang****fAnalysisManager->CreateNtupleDColumn(tmpstr);
        //Junsang****}
        //Junsang****// Leaf for ZDC3W ID:[316-342]
        //Junsang****for(G4int i=1; i<28; i++)
        //Junsang****{
//Junsang****
            //Junsang****G4String tmpstr = ZDC3WCHDE + std::to_string(i);
            //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
            //Junsang****fAnalysisManager->CreateNtupleDColumn(tmpstr);
        //Junsang****}
//Junsang****
        //Junsang****fAnalysisManager->CreateNtupleDColumn(ZDCTDE); // ID:343
        //Junsang****fAnalysisManager->CreateNtupleIColumn(ZDCTNOP);// ID:344
        //Junsang****fAnalysisManager->CreateNtupleDColumn(I_PLTDE);// ID:345
        //Junsang****fAnalysisManager->CreateNtupleDColumn(W_PLTDE);// ID:346
        //Junsang****fAnalysisManager->CreateNtupleDColumn(SMDHTDE);// ID:347
        //Junsang****fAnalysisManager->CreateNtupleIColumn(SMDHTNOP);// ID:348
        //Junsang****fAnalysisManager->CreateNtupleDColumn(SMDVTDE);// ID:349
        //Junsang****fAnalysisManager->CreateNtupleIColumn(SMDVTNOP);// ID:350
//Junsang****
        //Junsang****
//Junsang****
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


    //Junsang****RHICFDetectorConstruction* fConstruction = new RHICFDetectorConstruction();
                                                                            //Junsang****
    //Junsang****G4cout << "SDforWInZDC  : " << fConstruction->GetSDforWInZDC() << G4endl;
    //Junsang****G4cout << "SDforI_PL    : " << fConstruction->GetSDforI_PL() << G4endl;
    //Junsang****G4cout << "SDforPMMA    : " << fConstruction->GetSDforPMMA() << G4endl;
    //Junsang****G4cout << "SDforSMD     : " << fConstruction->GetSDforSMD() << G4endl;
    //Junsang****G4cout << "SDforWInARM1 : " << fConstruction->GetSDforWInARM1() << G4endl;
    //Junsang****G4cout << "SDforWHolder : " << fConstruction->GetSDforHolder() << G4endl;
    //Junsang****G4cout << "SDforGSOBar  : " << fConstruction->GetSDforGSOBar() << G4endl;
    //Junsang****G4cout << "SDforGSOPlate: " << fConstruction->GetSDforGSOPlate() << G4endl;
    //Junsang****G4cout << "SDforAlFrame : " << fConstruction->GetSDforFrame() << G4endl;
    //Junsang****G4cout << "SDforPanels  : " << fConstruction->GetSDforPanels() << G4endl;
//Junsang****
//Junsang****
//Junsang****
    //Junsang****long seeds[2];
//Junsang****
    //Junsang****time_t systime = time(NULL) + getpid();
    //Junsang****seeds[1] = (long) (systime*G4UniformRand());
//Junsang****
    //Junsang****G4Random::setTheSeeds(seeds);
//Junsang****
//Junsang****
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


