#include "RHICFEventAction.hh"
#include "RHICFRunAction.hh"
#include "RHICFDetectorConstruction.hh"
#include "B5PrimaryGeneratorAction.hh"
///////////////////////////////////////////////////////////////////////////////
#include "G4Event.hh"
#include "G4PhysicalVolumeStore.hh"
#include "g4root.hh"
#include "G4RunManager.hh"
#include "RHICFManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "FileManager.hh"
#include "Randomize.hh"
#include <fstream>
#include <cstdlib>
#include <vector>
using namespace std;
///////////////////////////////////////////////////////////////////////////////

RHICFEventAction::RHICFEventAction(B5PrimaryGeneratorAction* B5G): G4UserEventAction(),  fB5Primary(B5G)
{

}


RHICFEventAction::~RHICFEventAction()
{
}



void RHICFEventAction::BeginOfEventAction(const G4Event*)
{



    //Junsang****RHICFDetectorConstruction* fConstruction = new RHICFDetectorConstruction();// Detector construction for get information about SD
//Junsang****
//Junsang****
    //Junsang****// Get SD number for each sensitive detector
    //Junsang****if(NbSMDH == -1)
    //Junsang****{
//Junsang****
        //Junsang****G4SDManager* fSDManager = G4SDManager::GetSDMpointer();
//Junsang****
//Junsang****
        //Junsang****if(fConstruction->GetSDforWInZDC())
        //Junsang****{
            //Junsang****NbW_1          = fSDManager -> GetCollectionID("W_PL_1Logical/DE");//Collection ID for deposit energy
            //Junsang****NbW_2          = fSDManager -> GetCollectionID("W_PL_2Logical/DE");
            //Junsang****NbW_3          = fSDManager -> GetCollectionID("W_PL_3Logical/DE");
//Junsang****
        //Junsang****}
        //Junsang****if(fConstruction->GetSDforSMD())
        //Junsang****{
            //Junsang****NbSMDH         = fSDManager -> GetCollectionID("SMDHLogical/DE");
            //Junsang****NbSMDV         = fSDManager -> GetCollectionID("SMDVLogical/DE");
            //Junsang****NbNOPSMDH         = fSDManager -> GetCollectionID("SMDHLogical/NOP");
            //Junsang****NbNOPSMDV         = fSDManager -> GetCollectionID("SMDVLogical/NOP");
        //Junsang****}
        //Junsang****if(fConstruction->GetSDforPMMA())
        //Junsang****{
            //Junsang****NbGAP_1        = fSDManager -> GetCollectionID("GAPF_1Logical/DE");
            //Junsang****NbGAP_2        = fSDManager -> GetCollectionID("GAPF_2Logical/DE");
            //Junsang****NbGAP_3        = fSDManager -> GetCollectionID("GAPF_3Logical/DE");
            //Junsang****NbNOPGAP_1     = fSDManager -> GetCollectionID("GAPF_1Logical/NOP");//Collection ID for Number of Photon
            //Junsang****NbNOPGAP_2     = fSDManager -> GetCollectionID("GAPF_2Logical/NOP");
            //Junsang****NbNOPGAP_3     = fSDManager -> GetCollectionID("GAPF_3Logical/NOP");
        //Junsang****}
        //Junsang****if(fConstruction->GetSDforI_PL())
        //Junsang****{
            //Junsang****NbI_PL         = fSDManager -> GetCollectionID("I_PLLogical/DE");
//Junsang****
        //Junsang****}
        //Junsang****if(fConstruction->GetSDforWInARM1())
        //Junsang****{
    //Junsang****/*-*/G4String calNameForARM1[19] = {"LargeW_PLLogical", "SmallW_PLLogical", "WHolder_1Logical", "WHolder_2Logical", "GSO_PLHolderLogical", "GSOBarHolderLogical", "AlFrame1Logical", "AlFrame2Logical", "SidePanelLogical", "FrontPanelLogical", "LargeGSO_PLLogical", "SmallGSO_PLLogical", "LightGuideLargeLogical", "LightGuideSmallLogical", "GSORightSmallBarLogical", "GSOLeftSmallBarLogical", "GSORightLargeBarLogical", "GSOLeftLargeBarLogical", "ARM1Logical"};
            //Junsang****NbLargeW_PL     = fSDManager -> GetCollectionID("LargeW_PLLogical/DE");
            //Junsang****NbSmallW_PL     = fSDManager -> GetCollectionID("SmallW_PLLogical/DE");
            //Junsang****G4cout << "NLW_PL:" << NbLargeW_PL << G4endl;
            //Junsang****G4cout << "NSW_PL:" << NbSmallW_PL << G4endl;
        //Junsang****}
        //Junsang****if(fConstruction->GetSDforHolder())
        //Junsang****{
            //Junsang****NbW_1Holder     = fSDManager -> GetCollectionID("WHolder_1Logical/DE");
            //Junsang****NbW_2Holder     = fSDManager -> GetCollectionID("WHolder_2Logical/DE");
            //Junsang****NbGSO_PLHolder  = fSDManager -> GetCollectionID("GSO_PLHolderLogical/DE");
            //Junsang****NbGSOBarHolder  = fSDManager -> GetCollectionID("GSOBarHolderLogical/DE");
            //Junsang****NbGSOBarHolder  = fSDManager -> GetCollectionID("LargeGSO_PLLogical/NOP");
            //Junsang****G4cout << "holder1: " << NbW_1Holder << G4endl;
            //Junsang****G4cout << "holder2: " << NbW_1Holder << G4endl;
//Junsang****G4cout << "NOP: " << NbGSOBarHolder << G4endl;
        //Junsang****}
        //Junsang****if(fConstruction->GetSDforFrame())
        //Junsang****{
            //Junsang****//Junsang****NbAlFrame
//Junsang****
        //Junsang****}
    //Junsang****}


}     


void RHICFEventAction::EndOfEventAction(const G4Event* event)
{

    G4HCofThisEvent* fHCE = event -> GetHCofThisEvent();

    if(G4PhysicalVolumeStore::GetInstance()->GetVolume("ARM1Physical",false))
    {
        ExtractDEValue(fHCE, event);
        ExtractNOPValue(fHCE);
    }
   

    //Junsang****G4cout << "DET: " << GetDEValue(fHCE, "ARM1Logical/DE", 0)/MeV << G4endl;
    //Junsang****G4cout << "DE0: " << GetDEValue(fHCE, "LargeW_PLLogical/DE", 0)/MeV << G4endl;
    //Junsang****G4cout << "DE1: " << GetDEValue(fHCE, "LargeW_PLLogical/DE", 1)/MeV << G4endl;
    //Junsang****G4cout << "DE2: " << GetDEValue(fHCE, "LargeW_PLLogical/DE", 2)/MeV << G4endl;
    //Junsang****G4cout << "DE3: " << GetDEValue(fHCE, "LargeW_PLLogical/DE", 3)/MeV << G4endl;
    //Junsang****G4cout << "DE4: " << GetDEValue(fHCE, "LargeW_PLLogical/DE", 4)/MeV << G4endl;
    //Junsang****G4cout << "DE5: " << GetDEValue(fHCE, "LargeW_PLLogical/DE", 5)/MeV << G4endl;
    //Junsang****G4cout << "DE6: " << GetDEValue(fHCE, "LargeW_PLLogical/DE", 6)/MeV << G4endl;
    //Junsang****G4cout << "DE7: " << GetDEValue(fHCE, "LargeW_PLLogical/DE", 7)/MeV << G4endl;
    //Junsang****G4cout << "DE8: " << GetDEValue(fHCE, "LargeW_PLLogical/DE", 8)/MeV << G4endl;
    //Junsang****G4cout << "DE9: " << GetDEValue(fHCE, "LargeW_PLLogical/DE", 9)/MeV << G4endl;
    //Junsang****G4cout << "DE9: " << GetDEValue(fHCE, "LargeW_PLLogical/DE", 9)/MeV << G4endl;
    //Junsang****G4cout << "NOP1: " << GetDEValue(fHCE, "LargeGSO_PLLogical/NOP", 1) << G4endl;
    //Junsang****G4cout << "NOP2: " << GetDEValue(fHCE, "LargeGSO_PLLogical/NOP", 2) << G4endl;
    //Junsang****G4cout << "NOP3: " << GetDEValue(fHCE, "LargeGSO_PLLogical/NOP", 3) << G4endl;
    //Junsang****G4cout << "NOP4: " << GetDEValue(fHCE, "LargeGSO_PLLogical/NOP", 4) << G4endl;
    //Junsang****G4cout << "NOP5: " << GetDEValue(fHCE, "LargeGSO_PLLogical/NOP", 5) << G4endl;
    //Junsang****G4cout << "NOP6: " << GetDEValue(fHCE, "LargeGSO_PLLogical/NOP", 6) << G4endl;
    //Junsang****G4cout << "NOP7: " << GetDEValue(fHCE, "LargeGSO_PLLogical/NOP", 7) << G4endl;
    //Junsang****G4cout << "NOP8: " << GetDEValue(fHCE, "LargeGSO_PLLogical/NOP", 8) << G4endl;
    //Junsang****G4cout << "NOP9: " << GetDEValue(fHCE, "LargeGSO_PLLogical/NOP", 9) << G4endl;
    //Junsang****G4cout << "NOP10: " << GetDEValue(fHCE, "LargeGSO_PLLogical/NOP", 10) << G4endl;
    //Junsang****G4cout << "NOP11: " << GetDEValue(fHCE, "LargeGSO_PLLogical/NOP", 11) << G4endl;
    //Junsang****G4cout << "DE: " << GetDEValue(fHCE, "LargeGSO_PLLogical/DE", 0)/MeV << G4endl;
    //Junsang****G4cout << "ALDE: " << GetDEValue(fHCE, "AlFrame1Logical/DE", 0)/MeV << G4endl;
    //Junsang****RHICFDetectorConstruction* fConstruction = new RHICFDetectorConstruction();
    //Junsang****//Variables for total energy and #of Photon
    //Junsang****G4double TDE_ZDC=0;//TDE:Total deposit energy
    //Junsang****G4double TDE_W_1=0;
    //Junsang****G4double TDE_W_2=0;
    //Junsang****G4double TDE_W_3=0;
    //Junsang****G4double TDE_W=0;
    //Junsang****G4double TDE_SMDH=0;
    //Junsang****G4double TDE_SMDV=0;
    //Junsang****G4double TDE_I_PL=0;
    //Junsang****G4double TDE_GAP_1=0;
    //Junsang****G4double TDE_GAP_2=0;
    //Junsang****G4double TDE_GAP_3=0;
    //Junsang****G4int    TNOP_SMDH=0;//NOP:Number of Photons
    //Junsang****G4int    TNOP_SMDV=0;
    //Junsang****G4int    TNOP_GAP_1=0;
    //Junsang****G4int    TNOP_GAP_2=0;
    //Junsang****G4int    TNOP_GAP_3=0;
    //Junsang****G4int    TNOP_ZDC=0;
//Junsang****
//Junsang****
//Junsang****
    //Junsang****//Total HitCollection in each event
//Junsang****
//Junsang****
//Junsang****
//Junsang****
    //Junsang****//#ofMapW_1,2,3 [1-27]
    //Junsang****G4THitsMap<G4double>* fEvMapForW_1;
    //Junsang****G4THitsMap<G4double>* fEvMapForW_2;
    //Junsang****G4THitsMap<G4double>* fEvMapForW_3;
    //Junsang****//#ofMapSMDH [1-8]
    //Junsang****G4THitsMap<G4double>* fEvMapForSMDH;  
    //Junsang****//#ofMapSMDV [1-7]
    //Junsang****G4THitsMap<G4double>* fEvMapForSMDV; 
    //Junsang****//#ofMapSMDH,V [1-8],[1-7] for Number
    //Junsang****G4THitsMap<G4double>* fNOPMapForSMDH; 
    //Junsang****G4THitsMap<G4double>* fNOPMapForSMDV;
    //Junsang****//#ofMapI_PL [0]
    //Junsang****G4THitsMap<G4double>* fEvMapForI_PL;
    //Junsang****//#ofMapGAP_1,2,3 [1-26]
    //Junsang****G4THitsMap<G4double>* fEvMapForGAP_1; 
    //Junsang****G4THitsMap<G4double>* fEvMapForGAP_2;
    //Junsang****G4THitsMap<G4double>* fEvMapForGAP_3;
    //Junsang****//#ofMapGAP_1,2,3 [1-26] for Number o
    //Junsang****G4THitsMap<G4double>* fNOPMapForGAP_1;
    //Junsang****G4THitsMap<G4double>* fNOPMapForGAP_2;
    //Junsang****G4THitsMap<G4double>* fNOPMapForGAP_3;
//Junsang****
//Junsang****
    //Junsang****if(fConstruction->GetSDforWInZDC())
    //Junsang****{
        //Junsang****//#ofMapW_1,2,3 [1-27]
        //Junsang****fEvMapForW_1          = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbW_1));
        //Junsang****fEvMapForW_2          = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbW_2));
        //Junsang****fEvMapForW_3          = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbW_3));
//Junsang****
    //Junsang****}
    //Junsang****if(fConstruction->GetSDforSMD())
    //Junsang****{
        //Junsang****//#ofMapSMDH [1-8]
        //Junsang****fEvMapForSMDH         = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbSMDH));
        //Junsang****//#ofMapSMDV [1-7]
        //Junsang****fEvMapForSMDV         = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbSMDV));
        //Junsang****//#ofMapSMDH,V [1-8],[1-7] for Number of Photon
        //Junsang****fNOPMapForSMDH         = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbNOPSMDH));
        //Junsang****fNOPMapForSMDV         = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbNOPSMDV));
//Junsang****
    //Junsang****}
    //Junsang****if(fConstruction->GetSDforI_PL())
    //Junsang****{
        //Junsang****//#ofMapI_PL [0]
        //Junsang****fEvMapForI_PL         = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbI_PL));
//Junsang****
    //Junsang****}
    //Junsang****if(fConstruction->GetSDforPMMA())
    //Junsang****{
        //Junsang****//#ofMapGAP_1,2,3 [1-26]
        //Junsang****fEvMapForGAP_1        = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbGAP_1));
        //Junsang****fEvMapForGAP_2        = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbGAP_2));
        //Junsang****fEvMapForGAP_3        = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbGAP_3));
        //Junsang****//#ofMapGAP_1,2,3 [1-26] for Number of Photon
        //Junsang****fNOPMapForGAP_1        = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbNOPGAP_1));
        //Junsang****fNOPMapForGAP_2        = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbNOPGAP_2));
        //Junsang****fNOPMapForGAP_3        = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbNOPGAP_3));
//Junsang****
    //Junsang****}
    //Junsang****
//Junsang****
//Junsang****
//Junsang****
//Junsang****
//Junsang****//----------------------------------------------Define HitsMaps and Map HitsMaps to DE/NOP value in each components---------------------------
//Junsang****//--------------------------------------------------------------------------------------------------------------------------------------------
//Junsang****
//Junsang****
//Junsang****
    //Junsang****G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();
//Junsang****
    //Junsang****if(fConstruction->GetSDforI_PL())
    //Junsang****{
        //Junsang****// Deposit energy in iron plate(I_PL) START
        //Junsang****G4double* kDepI_PL = (*fEvMapForI_PL)[0];
        //Junsang****if(!kDepI_PL) kDepI_PL = new G4double(0.0);
        //Junsang****TDE_I_PL = *kDepI_PL;
        //Junsang****// Deposit energy in iron plate(I_PL) END
//Junsang****
    //Junsang****}
//Junsang****
//Junsang****
    //Junsang****if(fConstruction->GetSDforSMD())
    //Junsang****{
        //Junsang****// Deposit energy & number of photon in Horizontal SMD STRIP START-------------------------------------------------------
        //Junsang****// Deposit energy in SMDH-vertical
        //Junsang****G4double* kDepSMDH[32];//[8]:PHENIX ZDC, [32]:STARZDC
        //Junsang****for(G4int i=0; i<32; i++)//[8]:PHENIX ZDC, [32]:STARZDC
        //Junsang****{
            //Junsang****kDepSMDH[i] = (*fEvMapForSMDH)[i+1];
            //Junsang****if(!kDepSMDH[i]) kDepSMDH[i] = new G4double(0.0);
//Junsang****
            //Junsang****fAnalysisManager -> FillNtupleDColumn(i, (G4double)*kDepSMDH[i]/MeV);
            //Junsang****//Junsang****G4cout << i+1 <<  "SMDH: " << *kDepSMDH[i] << G4endl;
            //Junsang****TDE_SMDH += *kDepSMDH[i];
        //Junsang****}
        //Junsang****// Number of photon in SMDH-vertical
        //Junsang****G4double* kNOPSMDH[32];//[8]:PHENIX ZDC, [32]:STARZDC
//Junsang****
        //Junsang****for(G4int i=0; i<32; i++)//[8]:PHENIX ZDC, [32]:STARZDC
        //Junsang****{
            //Junsang****kNOPSMDH[i] = (*fNOPMapForSMDH)[i+1];
            //Junsang****if(!kNOPSMDH[i]) kNOPSMDH[i] = new G4double(0.0);
//Junsang****
            //Junsang****fAnalysisManager -> FillNtupleIColumn(32+i, (G4int)*kNOPSMDH[i]);
            //Junsang****//Junsang****G4cout << i+1 <<  "SMDH: " << *kNOPSMDH[i] << G4endl;
            //Junsang****TNOP_SMDH += *kNOPSMDH[i];
        //Junsang****}
//Junsang****
        //Junsang****// Deposit energy & number of photon in Horizontal SMD STRIP END-------------------------------------------------------
        //Junsang****// Deposit energy & number of photon in Vertical SMD STRIP START-------------------------------------------------------
        //Junsang****// Deposit energy in SMDV-vertical
        //Junsang****G4double* kDepSMDV[21];//[7]:PHENIX ZDC, [21]:STARZDC
//Junsang****
        //Junsang****for(G4int i=0; i<21; i++)//[7]:PHENIX ZDC, [21]:STARZDC
        //Junsang****{
            //Junsang****kDepSMDV[i] = (*fEvMapForSMDV)[i+1];
            //Junsang****if(!kDepSMDV[i]) kDepSMDV[i] = new G4double(0.0);
//Junsang****
            //Junsang****fAnalysisManager -> FillNtupleDColumn(64+i, (G4double)*kDepSMDV[i]/MeV);
            //Junsang****//Junsang****G4cout << i+1 <<  "SMDV: " << *kDepSMDV[i] << G4endl;
            //Junsang****TDE_SMDV += *kDepSMDV[i];
        //Junsang****}
        //Junsang****// Number of photon in SMDV-vertical
        //Junsang****G4double* kNOPSMDV[21];//[7]:PHENIX ZDC, [21]:STARZDC
//Junsang****
        //Junsang****for(G4int i=0; i<21; i++)//[7]:PHENIX ZDC, [21]:STARZDC
        //Junsang****{
            //Junsang****kNOPSMDV[i] = (*fNOPMapForSMDV)[i+1];
            //Junsang****if(!kNOPSMDV[i]) kNOPSMDV[i] = new G4double(0.0);
//Junsang****
            //Junsang****fAnalysisManager -> FillNtupleIColumn(85+i, (G4int)*kNOPSMDV[i]);
            //Junsang****//Junsang****G4cout << i+1 <<  "SMDV: " << *kNOPSMDV[i] << G4endl;
            //Junsang****TNOP_SMDV += *kNOPSMDV[i];
        //Junsang****}
//Junsang****
        //Junsang****//Vertical SMD STRIP deposit energy & number of photon END-------------------------------------------------------
//Junsang****
    //Junsang****}
//Junsang****
//Junsang****
    //Junsang****if(fConstruction->GetSDforPMMA())
    //Junsang****{
        //Junsang****//GAP deposit energy & number of photon START-------------------------------------------------------
        //Junsang****// Deposit energy in GAP_1
        //Junsang****G4double* kDepGAP_1[26];
//Junsang****
        //Junsang****for(G4int i=0; i<26; i++)
        //Junsang****{
            //Junsang****kDepGAP_1[i] = (*fEvMapForGAP_1)[i+1];
            //Junsang****if(!kDepGAP_1[i]) kDepGAP_1[i] = new G4double(0.0);
//Junsang****
            //Junsang****fAnalysisManager -> FillNtupleDColumn(106+i, (G4double)*kDepGAP_1[i]/MeV);
            //Junsang****//Junsang****G4cout << i+1 <<  "GAP_1: " << *kDepGAP_1[i] << G4endl;
            //Junsang****TDE_GAP_1 += *kDepGAP_1[i];
//Junsang****
        //Junsang****}
        //Junsang****// Number of photon in GAP_1
        //Junsang****G4double* kNOPGAP_1[26];
//Junsang****
        //Junsang****for(G4int i=0; i<26; i++)
        //Junsang****{
            //Junsang****kNOPGAP_1[i] = (*fNOPMapForGAP_1)[i+1];
            //Junsang****if(!kNOPGAP_1[i]) kNOPGAP_1[i] = new G4double(0.0);
//Junsang****
            //Junsang****fAnalysisManager -> FillNtupleIColumn(132+i, (G4int)*kNOPGAP_1[i]);
            //Junsang****//Junsang****G4cout << i+1 <<  "GAP_1: " << *kNOPGAP_1[i] << G4endl;
            //Junsang****TNOP_GAP_1 += *kNOPGAP_1[i];
//Junsang****
        //Junsang****}
//Junsang****
//Junsang****
        //Junsang****// Deposit energy in GAP_2
        //Junsang****G4double* kDepGAP_2[26];
//Junsang****
        //Junsang****for(G4int i=0; i<26; i++)
        //Junsang****{
            //Junsang****kDepGAP_2[i] = (*fEvMapForGAP_2)[i+1];
            //Junsang****if(!kDepGAP_2[i]) kDepGAP_2[i] = new G4double(0.0);
//Junsang****
            //Junsang****fAnalysisManager -> FillNtupleDColumn(158+i, (G4double)*kDepGAP_2[i]/MeV);
            //Junsang****//Junsang****G4cout << i+1 <<  "GAP_2: " << *kDepGAP_2[i] << G4endl;
            //Junsang****TDE_GAP_2 += *kDepGAP_2[i];
//Junsang****
        //Junsang****}
//Junsang****
        //Junsang****// Number of photon in GAP_2
        //Junsang****G4double* kNOPGAP_2[26];
//Junsang****
        //Junsang****for(G4int i=0; i<26; i++)
        //Junsang****{
            //Junsang****kNOPGAP_2[i] = (*fNOPMapForGAP_2)[i+1];
            //Junsang****if(!kNOPGAP_2[i]) kNOPGAP_2[i] = new G4double(0.0);
//Junsang****
            //Junsang****fAnalysisManager -> FillNtupleIColumn(184+i, (G4int)*kNOPGAP_2[i]);
            //Junsang****//Junsang****G4cout << i+1 <<  "GAP_2: " << *kNOPGAP_2[i] << G4endl;
            //Junsang****TNOP_GAP_2 += *kNOPGAP_2[i];
//Junsang****
        //Junsang****}
//Junsang****
        //Junsang****// Deposit energy in GAP_3
        //Junsang****G4double* kDepGAP_3[26];
//Junsang****
        //Junsang****for(G4int i=0; i<26; i++)
        //Junsang****{
            //Junsang****kDepGAP_3[i] = (*fEvMapForGAP_3)[i+1];
            //Junsang****if(!kDepGAP_3[i]) kDepGAP_3[i] = new G4double(0.0);
//Junsang****
            //Junsang****fAnalysisManager -> FillNtupleDColumn(210+i, (G4double)*kDepGAP_3[i]/MeV);
            //Junsang****//Junsang****G4cout << i+1 <<  "GAP_3: " << *kDepGAP_3[i] << G4endl;
            //Junsang****TDE_GAP_3 += *kDepGAP_3[i];
//Junsang****
        //Junsang****}
//Junsang****
        //Junsang****// Number of photon in GAP_3
        //Junsang****G4double* kNOPGAP_3[26];
//Junsang****
        //Junsang****for(G4int i=0; i<26; i++)
        //Junsang****{
            //Junsang****kNOPGAP_3[i] = (*fNOPMapForGAP_3)[i+1];
            //Junsang****if(!kNOPGAP_3[i]) kNOPGAP_3[i] = new G4double(0.0);
//Junsang****
            //Junsang****fAnalysisManager -> FillNtupleIColumn(236+i, (G4int)*kNOPGAP_3[i]);
            //Junsang****//Junsang****G4cout << i+1 <<  "GAP_3: " << *kNOPGAP_3[i] << G4endl;
            //Junsang****TNOP_GAP_3 += *kNOPGAP_3[i];
//Junsang****
        //Junsang****}
        //Junsang****//PMMI(GAP) deposit energy & number of photon END-------------------------------------------------------
//Junsang****
    //Junsang****}
//Junsang****
    //Junsang****if(fConstruction->GetSDforWInZDC())
    //Junsang****{
        //Junsang****//Tunsten(W) deposit energy & number of photon START-------------------------------------------------------
//Junsang****
        //Junsang****// Deposit energy in W_1
        //Junsang****G4double* kDepW_1[27];
//Junsang****
        //Junsang****for(G4int i=0; i<27; i++)
        //Junsang****{
            //Junsang****kDepW_1[i] = (*fEvMapForW_1)[i+1];
            //Junsang****if(!kDepW_1[i]) kDepW_1[i] = new G4double(0.0);
//Junsang****
            //Junsang****fAnalysisManager -> FillNtupleDColumn(262+i, (G4double)*kDepW_1[i]/GeV);
            //Junsang****//Junsang****G4cout << i+1 <<  "W_1: " << *kDepW_1[i] << G4endl;
            //Junsang****TDE_W_1 += *kDepW_1[i];
//Junsang****
        //Junsang****}
//Junsang****
        //Junsang****// Deposit energy in W_2
        //Junsang****G4double* kDepW_2[27];
//Junsang****
        //Junsang****for(G4int i=0; i<27; i++)
        //Junsang****{
            //Junsang****kDepW_2[i] = (*fEvMapForW_2)[i+1];
            //Junsang****if(!kDepW_2[i]) kDepW_2[i] = new G4double(0.0);
//Junsang****
            //Junsang****fAnalysisManager -> FillNtupleDColumn(289+i, (G4double)*kDepW_2[i]/GeV);
            //Junsang****//Junsang****G4cout << i+1 <<  "W_2: " << *kDepW_2[i] << G4endl;
            //Junsang****TDE_W_2 += *kDepW_2[i];
//Junsang****
        //Junsang****}
//Junsang****
        //Junsang****// Deposit energy in W_3
        //Junsang****G4double* kDepW_3[27];
//Junsang****
        //Junsang****for(G4int i=0; i<27; i++)
        //Junsang****{
            //Junsang****kDepW_3[i] = (*fEvMapForW_3)[i+1];
            //Junsang****if(!kDepW_3[i]) kDepW_3[i] = new G4double(0.0);
//Junsang****
            //Junsang****fAnalysisManager -> FillNtupleDColumn(316+i, (G4double)*kDepW_3[i]/GeV);
            //Junsang****//Junsang****G4cout << i+1 <<  "W_3: " << *kDepW_3[i] << G4endl;
            //Junsang****TDE_W_3 += *kDepW_3[i];
//Junsang****
            //Junsang****
        //Junsang****}
        //Junsang****//Tunsten(W) deposit energy & number of photon END-------------------------------------------------------
//Junsang****
    //Junsang****}
//Junsang****
    //Junsang****TDE_W = TDE_W_1 + TDE_W_2 + TDE_W_3;
    //Junsang****TDE_ZDC = TDE_W_1+ TDE_W_2+ TDE_W_3+ TDE_SMDH+ TDE_SMDV+ TDE_I_PL+ TDE_GAP_1+ TDE_GAP_2+ TDE_GAP_3;
    //Junsang****TNOP_ZDC = TNOP_GAP_1 + TNOP_GAP_2 + TNOP_GAP_3;
//Junsang****
    //Junsang****fAnalysisManager -> FillNtupleDColumn(343, (G4double)TDE_ZDC/GeV);
    //Junsang****fAnalysisManager -> FillNtupleIColumn(344, (G4int)TNOP_ZDC);
    //Junsang****fAnalysisManager -> FillNtupleDColumn(345, (G4double)TDE_I_PL/GeV);
    //Junsang****fAnalysisManager -> FillNtupleDColumn(346, (G4double)TDE_W/GeV);
    //Junsang****fAnalysisManager -> FillNtupleDColumn(347, (G4double)TDE_SMDH/MeV);
    //Junsang****fAnalysisManager -> FillNtupleIColumn(348, (G4int)TNOP_SMDH);
    //Junsang****fAnalysisManager -> FillNtupleDColumn(349, (G4double)TDE_SMDV/MeV);
    //Junsang****fAnalysisManager -> FillNtupleIColumn(350, (G4int)TNOP_SMDV);


    //Junsang****G4cout << "TDE_ZDC: " << TDE_ZDC/GeV << "GeV" << G4endl;
    //Junsang****G4cout << "TDE_W_1: " << TDE_W_1/GeV << "GeV" << G4endl;
    //Junsang****G4cout << "TDE_W_2: " << TDE_W_2/GeV << "GeV" << G4endl;
    //Junsang****G4cout << "TDE_W_3: " << TDE_W_3/GeV << "GeV" << G4endl;
    //Junsang****G4cout << "TDE_W: "   << TDE_W/GeV << "GeV" << G4endl;
    //Junsang****G4cout << "TDE_SMDH: " << TDE_SMDH/MeV << "MeV" << G4endl;
    //Junsang****G4cout << "TNOP_SMDH: " << TNOP_SMDH << G4endl;
    //Junsang****G4cout << "TDE_SMDV: " << TDE_SMDV/MeV << "MeV" << G4endl;
    //Junsang****G4cout << "TNOP_SMDV: " << TNOP_SMDV << G4endl;
    //Junsang****G4cout << "TDE_I_PL: " << TDE_I_PL << G4endl;
    //Junsang****G4cout << "TDE_GAP_1: " << TDE_GAP_1/MeV << "MeV" << G4endl;
    //Junsang****G4cout << "TNOP_GAP_1: " << TNOP_GAP_1 << G4endl;
    //Junsang****G4cout << "TDE_GAP_2: " << TDE_GAP_2/MeV << "MeV" << G4endl;
    //Junsang****G4cout << "TNOP_GAP_2: " << TNOP_GAP_2 << G4endl;
    //Junsang****G4cout << "TDE_GAP_3: " << TDE_GAP_3/MeV << "MeV" << G4endl;
    //Junsang****G4cout << "TNOP_GAP_3: " << TNOP_GAP_3 << G4endl;
              



    //Junsang****std::map<G4int, G4double*>::iterator itr;
    //Junsang****std::map<G4int, G4double*>::iterator itr1;

    //Junsang****itr1 = fEventMap_2->GetMap()->begin();
    
    //Junsang****for(itr = fEventMap_1->GetMap()->begin(); itr != fEventMap_1->GetMap()->end(); itr++)
    //Junsang****{
//Junsang****
        //Junsang****G4int copyNb = (itr->first);
        //Junsang****G4double edep = *(itr->second);
//Junsang****
        //Junsang****G4cout << "\n smdh" << copyNb << ": " << edep/GeV << "GeV";
//Junsang****
//Junsang****
    //Junsang****}
    





    //Junsang****G4double* kDepSMDH0 = (*fEvMapForSMDH)[0];
    //Junsang****G4double* kDepSMDH1 = (*fEvMapForSMDH)[1];
    //Junsang****G4double* kDepSMDH2 = (*fEvMapForSMDH)[2];
    //Junsang****G4double* kDepSMDH3 = (*fEvMapForSMDH)[3];
    //Junsang****G4double* kDepSMDH4 = (*fEvMapForSMDH)[4];
    //Junsang****G4double* kDepSMDH5 = (*fEvMapForSMDH)[5];
    //Junsang****G4double* kDepSMDH6 = (*fEvMapForSMDH)[6];
    //Junsang****G4double* kDepSMDH7 = (*fEvMapForSMDH)[7];
    //Junsang****G4double* kDepSMDH8 = (*fEvMapForSMDH)[8];
//Junsang****
    //Junsang****if(!kDepSMDH0) kDepSMDH0 = new G4double(0.0);
    //Junsang****if(!kDepSMDH1) kDepSMDH1 = new G4double(0.0);
    //Junsang****if(!kDepSMDH2) kDepSMDH2 = new G4double(0.0);
    //Junsang****if(!kDepSMDH3) kDepSMDH3 = new G4double(0.0);
    //Junsang****if(!kDepSMDH4) kDepSMDH4 = new G4double(0.0);
    //Junsang****if(!kDepSMDH5) kDepSMDH5 = new G4double(0.0);
    //Junsang****if(!kDepSMDH6) kDepSMDH6 = new G4double(0.0);
    //Junsang****if(!kDepSMDH7) kDepSMDH7 = new G4double(0.0);
    //Junsang****if(!kDepSMDH8) kDepSMDH8 = new G4double(0.0);
//Junsang****
    //Junsang****cout << "0: " << *kDepSMDH0 << endl;
    //Junsang****cout << "1: " << *kDepSMDH1 << endl;
    //Junsang****cout << "2: " << *kDepSMDH2 << endl;
    //Junsang****cout << "3: " << *kDepSMDH3 << endl;
    //Junsang****cout << "4: " << *kDepSMDH4 << endl;
    //Junsang****cout << "5: " << *kDepSMDH5 << endl;
    //Junsang****cout << "6: " << *kDepSMDH6 << endl;
    //Junsang****cout << "7: " << *kDepSMDH7 << endl;
    //Junsang****cout << "8: " << *kDepSMDH8 << endl;

    //Junsang****fAnalysisManager -> AddNtupleRow();

    G4AnalysisManager::Instance()->AddNtupleRow(0);
    G4AnalysisManager::Instance()->AddNtupleRow(1);
    G4AnalysisManager::Instance()->AddNtupleRow(2);
}

G4double RHICFEventAction::GetDEValue(G4HCofThisEvent* hc, G4String detectorname, int channel)
{

    G4THitsMap<G4double>* tmpHitMap = (G4THitsMap<G4double>*)(hc->GetHC(G4SDManager::GetSDMpointer()->GetCollectionID(detectorname+"/DE")));
    //Junsang****G4cout << "Num " << G4SDManager::GetSDMpointer()->GetCollectionID(detectorname) << G4endl;
    G4double* tmpDE = (*tmpHitMap)[channel];
    if(!tmpDE) tmpDE = new G4double(0.0);
    return (G4double)*tmpDE;
}

G4int RHICFEventAction::GetNOPValue(G4HCofThisEvent* hc, G4String detectorname, int channel)
{
    G4THitsMap<G4double>* tmpHitMap = (G4THitsMap<G4double>*)(hc->GetHC(G4SDManager::GetSDMpointer()->GetCollectionID(detectorname+"/NOP")));
    //Junsang****G4cout << "Num " << G4SDManager::GetSDMpointer()->GetCollectionID(detectorname) << G4endl;
    G4double* tmpDE = (*tmpHitMap)[channel];
    if(!tmpDE) tmpDE = new G4double(0.0);
    return (G4double)*tmpDE;
}

void RHICFEventAction::ExtractDEValue(G4HCofThisEvent* hc, const G4Event* event)
{
    //ARM1 GSO_PL
    for (int i = 0; i < 16; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleDColumn(0, i, GetDEValue(hc, "SmallGSO_PLLogical", i)/MeV);
    } 
    for (int i = 16; i < 32; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleDColumn(0, i, GetDEValue(hc, "LargeGSO_PLLogical", i)/MeV);
    } 

    G4double tmpDE = 0.;
    for (int i = 0; i < 16; i++) 
    {
        tmpDE += GetDEValue(hc, "GSO_PLHolderLogical", i);
        tmpDE += GetDEValue(hc, "LightGuideLargeLogical", i);
        tmpDE += GetDEValue(hc, "LightGuideSmallLogical", i);
    }
    for (int i = 0; i < 80; i++) 
    {
        tmpDE += GetDEValue(hc, "GSORightSmallBarLogical", i);
        tmpDE += GetDEValue(hc, "GSOLeftSmallBarLogical", i);
    }
    for (int i = 0; i < 160; i++) 
    {
        tmpDE += GetDEValue(hc, "GSORightLargeBarLogical", i);
        tmpDE += GetDEValue(hc, "GSOLeftLargeBarLogical", i);
    }
    for (int i = 0; i < 18; i++) 
    {
        tmpDE += GetDEValue(hc, "LargeW_PLLogical", i);
        tmpDE += GetDEValue(hc, "SmallW_PLLogical", i);
        tmpDE += GetDEValue(hc, "WHolder_1Logical", i);
    }
    for (int i = 0; i < 4; i++) 
    {
        tmpDE += GetDEValue(hc, "WHolder_2Logical", i);
        tmpDE += GetDEValue(hc, "GSOBarHolderLogical", i);
    }
    tmpDE += GetDEValue(hc, "AlFrame1Logical", 0);
    tmpDE += GetDEValue(hc, "AlFrame2Logical", 0);

    G4AnalysisManager::Instance()->FillNtupleDColumn(0, 64, tmpDE/GeV); 
    G4AnalysisManager::Instance()->FillNtupleIColumn(0, 65, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID())); 
    G4AnalysisManager::Instance()->FillNtupleIColumn(0, 66, event->GetEventID()); 



    //ARM1 GSO_BAR
    for (int i = 0; i < 80; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i, GetDEValue(hc, "GSOLeftSmallBarLogical", i)/MeV);
    } 
    for (int i = 80; i < 240; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i, GetDEValue(hc, "GSOLeftLargeBarLogical", i)/MeV);
    } 
    G4AnalysisManager::Instance()->FillNtupleIColumn(1, 480, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID())); 
    G4AnalysisManager::Instance()->FillNtupleIColumn(1, 481, event->GetEventID()); 

    //FC
}

void RHICFEventAction::ExtractNOPValue(G4HCofThisEvent* hc)
{
    for (int i = 32; i < 48; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleIColumn(0, i, GetNOPValue(hc, "SmallGSO_PLLogical", i));
    } 
    for (int i = 48; i < 64; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleIColumn(0, i, GetNOPValue(hc, "SmallGSO_PLLogical", i));
    } 
    for (int i = 240; i < 320; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i, GetNOPValue(hc, "SmallGSO_PLLogical", i));
    } 
    for (int i = 320; i < 480; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i, GetNOPValue(hc, "SmallGSO_PLLogical", i));
    } 

}

