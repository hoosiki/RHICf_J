// Edited by Junsang Park. 2014.12.10
#include "RHICFEventAction.hh"
#include "RHICFRunAction.hh"
#include "B5PrimaryGeneratorAction.hh"
///////////////////////////////////////////////////////////////////////////////
#include "G4Event.hh"
#include "g4root.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
//Junsang****#include "G4Analysis.hh"
#include "Randomize.hh"
#include <fstream>
#include <cstdlib>
#include <vector>
///////////////////////////////////////////////////////////////////////////////






///////////////////////////////////////////////////////////////////////////////
RHICFEventAction::RHICFEventAction(B5PrimaryGeneratorAction* B5G): G4UserEventAction(), fPMT_1NP(-1), fPMT_2NP(-1), fPMT_3NP(-1), fPMT_1DE(-1), fPMT_2DE(-1), fPMT_3DE(-1), fW_1(-1), fW_2(-1), fW_3(-1), fSMDH(-1), fSMDV(-1), fI_PL(-1), fFIBRID(-1), fSMDHN(-1), fSMDVN(-1), fB5Primary(B5G)
///////////////////////////////////////////////////////////////////////////////
{
    //G4RunManager::GetRunManager() -> SetPrintProgress(1);
}


///////////////////////////////////////////////////////////////////////////////
RHICFEventAction::~RHICFEventAction()
///////////////////////////////////////////////////////////////////////////////
{
}



///////////////////////////////////////////////////////////////////////////////
void RHICFEventAction::BeginOfEventAction(const G4Event*)
///////////////////////////////////////////////////////////////////////////////
{


    if (fPMT_1NP==-1) 
    {


    G4SDManager* fSDManager = G4SDManager::GetSDMpointer();



    fPMT_1NP = fSDManager -> GetCollectionID("PMT_1/NOp");
    fPMT_1DE = fSDManager -> GetCollectionID("PMT_1/DE");
    fW_1 = fSDManager -> GetCollectionID("W_PL_1/DE");
    fPMT_2NP = fSDManager -> GetCollectionID("PMT_2/NOp");
    fPMT_2DE = fSDManager -> GetCollectionID("PMT_2/DE");
    fW_2 = fSDManager -> GetCollectionID("W_PL_2/DE");
    fPMT_3NP = fSDManager -> GetCollectionID("PMT_3/NOp");
    fPMT_3DE = fSDManager -> GetCollectionID("PMT_3/DE");
    fW_3 = fSDManager -> GetCollectionID("W_PL_3/DE");
    fSMDH = fSDManager -> GetCollectionID("SMDH/DE");
    fSMDV = fSDManager -> GetCollectionID("SMDV/DE");
    fI_PL = fSDManager -> GetCollectionID("I_PL/DE");
    fFIBRID = fSDManager -> GetCollectionID("FIBR/DE");
    fSMDHN = fSDManager -> GetCollectionID("SMDHPMTCore/NOp");
    fSMDVN = fSDManager -> GetCollectionID("SMDVPMTCore/NOp");
      

    }


}     


///////////////////////////////////////////////////////////////////////////////
void RHICFEventAction::EndOfEventAction(const G4Event* event)
///////////////////////////////////////////////////////////////////////////////
{
   



    G4HCofThisEvent* fHCE = event -> GetHCofThisEvent();

    G4THitsMap<G4double>* fEventMap_1 = (G4THitsMap<G4double>*)(fHCE -> GetHC(fPMT_1NP));

    G4THitsMap<G4double>* fEnergyMap_1 = (G4THitsMap<G4double>*)(fHCE -> GetHC(fPMT_1DE));

    G4THitsMap<G4double>* fDetEZDCMap_1 = (G4THitsMap<G4double>*)(fHCE -> GetHC(fW_1));

    G4THitsMap<G4double>* fEventMap_2 = (G4THitsMap<G4double>*)(fHCE -> GetHC(fPMT_2NP));

    G4THitsMap<G4double>* fEnergyMap_2 = (G4THitsMap<G4double>*)(fHCE -> GetHC(fPMT_2DE));

    G4THitsMap<G4double>* fDetEZDCMap_2 = (G4THitsMap<G4double>*)(fHCE -> GetHC(fW_2));

    G4THitsMap<G4double>* fEventMap_3 = (G4THitsMap<G4double>*)(fHCE -> GetHC(fPMT_3NP));

    G4THitsMap<G4double>* fEnergyMap_3 = (G4THitsMap<G4double>*)(fHCE -> GetHC(fPMT_3DE));

    G4THitsMap<G4double>* fDetEZDCMap_3 = (G4THitsMap<G4double>*)(fHCE -> GetHC(fW_3));


    G4THitsMap<G4double>* fDetESMDVMap = (G4THitsMap<G4double>*)(fHCE -> GetHC(fSMDV));
    G4THitsMap<G4double>* fDetESMDHMap = (G4THitsMap<G4double>*)(fHCE -> GetHC(fSMDH));

    G4THitsMap<G4double>* fSMDHNMap = (G4THitsMap<G4double>*)(fHCE -> GetHC(fSMDVN));
    G4THitsMap<G4double>* fSMDVNMap = (G4THitsMap<G4double>*)(fHCE -> GetHC(fSMDHN));

    G4THitsMap<G4double>* fDetEI_PLMap = (G4THitsMap<G4double>*)(fHCE -> GetHC(fI_PL));

    G4THitsMap<G4double>* fDetFIBRMap = (G4THitsMap<G4double>*)(fHCE -> GetHC(fFIBRID));

    G4double* kNumOfOp_1 = (*fEventMap_1)[0];
    G4double* kDepE_1 = (*fEnergyMap_1)[0];
    G4double* kDepEW_1 = (*fDetEZDCMap_1)[0];
    G4double* kNumOfOp_2 = (*fEventMap_2)[0];
    G4double* kDepE_2 = (*fEnergyMap_2)[0];
    G4double* kDepEW_2 = (*fDetEZDCMap_2)[0];
    G4double* kNumOfOp_3 = (*fEventMap_3)[0];
    G4double* kDepE_3 = (*fEnergyMap_3)[0];
    G4double* kDepEW_3 = (*fDetEZDCMap_3)[0];

    G4double* kDepSMDH = NULL; 
    G4double* kDepSMDV = (*fDetESMDVMap)[1];
    G4double* kDepI_PL = (*fDetEI_PLMap)[0];

    G4double* kDepFIBR = (*fDetFIBRMap)[0];

    std::vector<G4double*> kNumOfSMDH;
    std::vector<G4double*> kNumOfSMDV;
    std::vector<G4double> kXPosition;
    std::vector<G4double> kYPosition;

    if(!kDepSMDH) kDepSMDH = new G4double(0.0);
    if(!kDepSMDV) kDepSMDV = new G4double(0.0);
    for(G4int i=1; i<9; i++)
    {
        kDepSMDH = (*fDetESMDHMap)[i];
        if(!kDepSMDH) kDepSMDH = new G4double(0.0);
        G4cout << i << "+++" << *kDepSMDH << G4endl;
    }

    //Junsang****for(G4int i=1; i<8; i++)
    //Junsang****{
        //Junsang****G4cout << i << "+++" << *(*fDetESMDVMap)[i] << G4endl;
    //Junsang****}
  
    G4double Temp14 = 0;
    G4double Temp15 = 0;
    G4double TempSumXN = 0;
    G4double TempSumYN = 0;

    for(G4int j=0; j<7; j++)
    {
        kNumOfSMDH.push_back((*fSMDHNMap)[j]);
        if(!kNumOfSMDH.at(j))  kNumOfSMDH.at(j) = new G4double(0.0);
        kXPosition.push_back((j-3)*1.5*11.0/10.5);
        Temp14 = Temp14 + (kXPosition.at(j)*(*kNumOfSMDH.at(j))*120);
        TempSumXN = + TempSumXN + (*kNumOfSMDH.at(j))*120;

    }

    for(G4int k=0; k<8; k++)
    {
        kNumOfSMDV.push_back((*fSMDVNMap)[k]);
        if(!kNumOfSMDV.at(k))  kNumOfSMDV.at(k) = new G4double(0.0);
        kYPosition.push_back((-3+k)*2.0*11.0/10.5-11.0/10.5);
        Temp15 = Temp15 + (kYPosition.at(k)*(*kNumOfSMDV.at(k))*480);
        TempSumYN = + TempSumYN + (*kNumOfSMDV.at(k))*480;
    }



    if(!kNumOfOp_1) kNumOfOp_1 = new G4double(0.0);
    if(!kDepE_1) kDepE_1 = new G4double(0.0);
    if(!kDepEW_1) kDepEW_1 = new G4double(0.0);
    if(!kNumOfOp_2) kNumOfOp_2 = new G4double(0.0);
    if(!kDepE_2) kDepE_2 = new G4double(0.0);
    if(!kDepEW_2) kDepEW_2 = new G4double(0.0);
    if(!kNumOfOp_3) kNumOfOp_3 = new G4double(0.0);
    if(!kDepE_3) kDepE_3 = new G4double(0.0);
    if(!kDepEW_3) kDepEW_3 = new G4double(0.0);
    if(!kDepSMDH) kDepSMDH = new G4double(0.0);
    if(!kDepSMDV) kDepSMDV = new G4double(0.0);
    if(!kDepI_PL) kDepI_PL = new G4double(0.0);
    if(!kDepFIBR) kDepFIBR = new G4double(0.0);

    //if(!kDepSMDHN) kDepSMDHN = new G4double(0.0);


    G4double kTemp1 = 180*(*kNumOfOp_1);
    G4double kTemp2 = (*kDepE_1);
    G4double kTemp3 = (*kDepEW_1);


    G4double kTemp4 = 180*(*kNumOfOp_2);
    G4double kTemp5 = (*kDepE_2);
    G4double kTemp6 = (*kDepEW_2);

    G4double kTemp7 = 180*(*kNumOfOp_3);
    G4double kTemp8 = (*kDepE_3);
    G4double kTemp9 = (*kDepEW_3);

    G4double kTemp10 = (*kDepSMDH);
    G4double kTemp11 = (*kDepSMDV);

    G4double kTemp12 = (*kDepI_PL);

    G4double kTemp13 = (*kDepFIBR);

    G4double PositionX = Temp14/TempSumXN;
    G4double PositionY = Temp15/TempSumYN;


    G4cout << "X: " << PositionX << G4endl;
    G4cout << "Y: " << PositionY << G4endl;
    G4cout << "TX: " << Temp14 << G4endl;
    G4cout << "TY: " << Temp15 << G4endl;

    

   

    G4int ParticleEnergy = 10;//(fB5Primary -> GetMomentum())/GeV;
    G4int X = 10;//(fB5Primary -> GetX());
    G4int Y = 10;//(fB5Primary -> GetY());
    G4String fileName;
    G4String nameE;//std::string x;
    G4String nameAdd1;
    G4String nameAdd2;
    std::ostringstream temp;

    std::ostringstream tempx;
    std::ostringstream tempy;

    tempx << X;
    tempy << Y;


    temp << ParticleEnergy;
    nameE = temp.str();
    nameAdd1 = tempx.str();
    nameAdd2 = tempy.str();
    //std::string x = std::to_string(ParticleEnergy);


// Get analysis manager
//
    G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();
    
    fAnalysisManager->FillNtupleIColumn(0, kTemp1);
    fAnalysisManager->FillNtupleIColumn(1, kTemp4);
    fAnalysisManager->FillNtupleIColumn(2, kTemp7);
    fAnalysisManager->FillNtupleIColumn(3, kTemp1+kTemp4+kTemp7);


    fAnalysisManager->FillNtupleDColumn(4, kTemp3/GeV);
    fAnalysisManager->FillNtupleDColumn(5, kTemp6/GeV);
    fAnalysisManager->FillNtupleDColumn(6, kTemp9/GeV);
    fAnalysisManager->FillNtupleDColumn(6, kTemp3/GeV+kTemp6/GeV+kTemp9/GeV);

    fAnalysisManager->FillNtupleDColumn(7, PositionX);
    fAnalysisManager->FillNtupleDColumn(8, PositionY);




    fAnalysisManager->AddNtupleRow();




}


