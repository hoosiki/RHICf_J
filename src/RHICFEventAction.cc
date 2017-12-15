#include "RHICFEventAction.hh"
#include "RHICFRunAction.hh"
#include "RHICFDetectorConstruction.hh"
#include "RHICFPrimaryGeneratorAction.hh"
///////////////////////////////////////////////////////////////////////////////
#include "G4Event.hh"
#include "G4PhysicalVolumeStore.hh"
#include "g4root.hh"
#include "RHICFManager.hh"
#include "G4RunManager.hh"
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

RHICFEventAction::RHICFEventAction(): G4UserEventAction()
{

}


RHICFEventAction::~RHICFEventAction()
{
}



void RHICFEventAction::BeginOfEventAction(const G4Event*)
{
}     


void RHICFEventAction::EndOfEventAction(const G4Event* event)
{

    G4HCofThisEvent* fHCE = event -> GetHCofThisEvent();

    if(G4PhysicalVolumeStore::GetInstance()->GetVolume("ARM1Physical",false))
    {
        ExtractDEValueForARM1(fHCE, event);
        ExtractNOPValueForARM1(fHCE);
    }
    if(G4PhysicalVolumeStore::GetInstance()->GetVolume("STARZDCPhysical",false))
    {
        ExtractValueForSTARZDC(fHCE, event);
    }
   


    G4AnalysisManager::Instance()->AddNtupleRow(0);//ARM1PL
    G4AnalysisManager::Instance()->AddNtupleRow(1);//ARM1BAR
    G4AnalysisManager::Instance()->AddNtupleRow(2);//FCInfo DE & NOP
    G4AnalysisManager::Instance()->AddNtupleRow(4);//PMMA
    G4AnalysisManager::Instance()->AddNtupleRow(5);//SMD
    ClearIPInfo();
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

void RHICFEventAction::ExtractDEValueForARM1(G4HCofThisEvent* hc, const G4Event* event)
{
    //ARM1 GSO_PL
    //[0-15] : DE FOR SMALL GSO PLATE
    //[16-31] : DE FOR LARGE GSO PLATE
    //[32-47] : NOP FOR SMALL GSO PLATE
    //[48-63] : NOP FOR LRAGE GSO PLATE
    //64 : TOTAL DE IN ARM1
    //65 : RUN NUMBER
    //66 : EVENT NUMBER
    for (int i = 0; i < 16; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleDColumn(0, i, GetDEValue(hc, "SmallGSO_PLLogical", i)/MeV);
    } 
    for (int i = 16; i < 32; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleDColumn(0, i, GetDEValue(hc, "LargeGSO_PLLogical", i-16)/MeV);
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
    //[0-479] : DE
    //[480-959] : NOP
    //960 : RUN NUMBER
    //961 : EVENT NUMBER
    //DE
    for (int i = 0; i < 80; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i, GetDEValue(hc, "GSOLeftSmallBarLogical", i)/MeV);
    } 
    for (int i = 80; i < 240; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i, GetDEValue(hc, "GSOLeftLargeBarLogical", i-80)/MeV);
    } 
    for (int i = 240; i < 320; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i, GetDEValue(hc, "GSORightSmallBarLogical", i-240)/MeV);
    } 
    for (int i = 320; i < 480; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleDColumn(1, i, GetDEValue(hc, "GSORightLargeBarLogical", i-320)/MeV);
    } 
    //NOP
    for (int i = 480; i < 560; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i, GetNOPValue(hc, "GSOLeftSmallBarLogical", i-480)/MeV);
    } 
    for (int i = 560; i < 720; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i, GetNOPValue(hc, "GSOLeftLargeBarLogical", i-560)/MeV);
    } 
    for (int i = 720; i < 800; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i, GetNOPValue(hc, "GSORightSmallBarLogical", i-720)/MeV);
    } 
    for (int i = 800; i < 960; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i, GetNOPValue(hc, "GSORightLargeBarLogical", i-800)/MeV);
    } 
    G4AnalysisManager::Instance()->FillNtupleIColumn(1, 960, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID())); 
    G4AnalysisManager::Instance()->FillNtupleIColumn(1, 961, event->GetEventID()); 

    //FC
    G4AnalysisManager::Instance()->FillNtupleDColumn(2, 0, GetDEValue(hc, "FrontCounterSmallLogical", 0)/MeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(2, 1, GetDEValue(hc, "FrontCounterLargeLogical", 0)/MeV);
    G4AnalysisManager::Instance()->FillNtupleIColumn(2, 4, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID())); 
    G4AnalysisManager::Instance()->FillNtupleIColumn(2, 5, event->GetEventID()); 
}

void RHICFEventAction::ExtractNOPValueForARM1(G4HCofThisEvent* hc)
{
    for (int i = 32; i < 48; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleIColumn(0, i, GetNOPValue(hc, "SmallGSO_PLLogical", i-32));
    } 
    for (int i = 48; i < 64; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleIColumn(0, i, GetNOPValue(hc, "SmallGSO_PLLogical", i-48));
    } 
    for (int i = 480; i < 560; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i, GetNOPValue(hc, "GSOLeftSmallBarLogical", i-480));
    } 
    for (int i = 560; i < 720; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i, GetNOPValue(hc, "GSOLeftSmallBarLogical", i-560));
    } 
    for (int i = 720; i < 800; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i, GetNOPValue(hc, "GSORightSmallBarLogical", i-720));
    } 
    for (int i = 800; i < 960; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleIColumn(1, i, GetNOPValue(hc, "GSORightSmallBarLogical", i-800));
    } 

    G4AnalysisManager::Instance()->FillNtupleIColumn(2, 2, GetNOPValue(hc, "FrontCounterSmallLogical", 0));
    G4AnalysisManager::Instance()->FillNtupleIColumn(2, 3, GetNOPValue(hc, "FrontCounterLargeLogical", 0));

}

void RHICFEventAction::ExtractValueForSTARZDC(G4HCofThisEvent* hc, const G4Event* event)
{
    //STARZDC PMMA LAYER
    //[0-2] : DE FOR 1st-3rd ZDC PMMA LAYERS
    //[3-5] : NOP FOR 1st-3rd ZDC PMMA LAYERS
    //[6-8] : DE FOR 1st-3rd ZDC MODULE
    //9 : TOTAL DE IN STARZDC
    //10 : RUN NUMBER
    //11 : EVENT NUMBER
    G4AnalysisManager::Instance()->FillNtupleDColumn(4, 0, GetDEValue(hc, "GAPF_1Logical", 0)/MeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(4, 1, GetDEValue(hc, "GAPF_2Logical", 0)/MeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(4, 2, GetDEValue(hc, "GAPF_3Logical", 0)/MeV);
    G4AnalysisManager::Instance()->FillNtupleIColumn(4, 3, GetNOPValue(hc, "GAPF_1Logical", 0));
    G4AnalysisManager::Instance()->FillNtupleIColumn(4, 4, GetNOPValue(hc, "GAPF_2Logical", 0));
    G4AnalysisManager::Instance()->FillNtupleIColumn(4, 5, GetNOPValue(hc, "GAPF_3Logical", 0));
    G4AnalysisManager::Instance()->FillNtupleDColumn(4, 6, GetDEValue(hc, "W_PL_1Logical", 0)/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(4, 7, GetDEValue(hc, "W_PL_2Logical", 0)/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(4, 8, GetDEValue(hc, "W_PL_3Logical", 0)/GeV);
    G4double tmpDE = 0.;
    tmpDE += GetDEValue(hc, "GAPF_1Logical", 0);
    tmpDE += GetDEValue(hc, "GAPF_2Logical", 0);
    tmpDE += GetDEValue(hc, "GAPF_3Logical", 0);
    tmpDE += GetDEValue(hc, "W_PL_1Logical", 0);
    tmpDE += GetDEValue(hc, "W_PL_2Logical", 0);
    tmpDE += GetDEValue(hc, "W_PL_3Logical", 0);
    tmpDE += GetDEValue(hc, "I_PLLogical", 0);
    G4AnalysisManager::Instance()->FillNtupleDColumn(4, 9, tmpDE/GeV); 
    G4AnalysisManager::Instance()->FillNtupleIColumn(4, 10, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID())); 
    G4AnalysisManager::Instance()->FillNtupleIColumn(4, 11, event->GetEventID()); 



    //STARZDC SMD
    //[0-6] : DE FOR SMDV
    //[7-14] : DE FOR SMDH
    //[15-21] : NOP FOR SMDV
    //[22-29] : NOP FOR SMDH
    //30 : RUN NUMBER
    //31 : EVENT NUMBER
    for (int i = 0; i < 7; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleDColumn(5, i, GetDEValue(hc, "SMDVLogical", i)/MeV);
    }
    for (int i = 7; i < 15; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleDColumn(5, i, GetDEValue(hc, "SMDHLogical", i-7)/MeV);
    }
    for (int i = 15; i < 22; i++) {
        G4AnalysisManager::Instance()->FillNtupleIColumn(5, i, GetNOPValue(hc, "SMDVLogical", i-15));
    }
    for (int i = 22; i < 30; i++) 
    {
        G4AnalysisManager::Instance()->FillNtupleIColumn(5, i, GetNOPValue(hc, "SMDHLogical", i-22));
    }
    G4AnalysisManager::Instance()->FillNtupleIColumn(5, 30, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID())); 
    G4AnalysisManager::Instance()->FillNtupleIColumn(5, 31, event->GetEventID()); 


}

void RHICFEventAction::ClearIPInfo()
{
    RHICFManager::GetInstance()->GetNameMap().clear();
    RHICFManager::GetInstance()->GetPXMap().clear();
    RHICFManager::GetInstance()->GetPYMap().clear();
    RHICFManager::GetInstance()->GetPZMap().clear();
    RHICFManager::GetInstance()->GetEnergyMap().clear();
}
