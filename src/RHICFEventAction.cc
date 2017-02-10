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
#include "Randomize.hh"
#include <fstream>
#include <cstdlib>
#include <vector>
using namespace std;
///////////////////////////////////////////////////////////////////////////////







///////////////////////////////////////////////////////////////////////////////
RHICFEventAction::RHICFEventAction(B5PrimaryGeneratorAction* B5G): G4UserEventAction(), NbW_1(-1), NbW_2(-1), NbW_3(-1), NbSMDH(-1), NbSMDV(-1), NbI_PL(-1), fB5Primary(B5G)
///////////////////////////////////////////////////////////////////////////////
{

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




    if(NbSMDH == -1)
    {

        G4SDManager* fSDManager = G4SDManager::GetSDMpointer();
        NbW_1          = fSDManager -> GetCollectionID("W_PL_1Logical/DE");
        NbW_2          = fSDManager -> GetCollectionID("W_PL_2Logical/DE");
        NbW_3          = fSDManager -> GetCollectionID("W_PL_3Logical/DE");
        NbSMDH         = fSDManager -> GetCollectionID("SMDHLogical/DE");
        NbSMDV         = fSDManager -> GetCollectionID("SMDVLogical/DE");
        NbI_PL         = fSDManager -> GetCollectionID("I_PLLogical/DE");
        NbGAP_1        = fSDManager -> GetCollectionID("GAPF_1Logical/DE");
        NbGAP_2        = fSDManager -> GetCollectionID("GAPF_2Logical/DE");
        NbGAP_3        = fSDManager -> GetCollectionID("GAPF_3Logical/DE");
    }


}     


///////////////////////////////////////////////////////////////////////////////
void RHICFEventAction::EndOfEventAction(const G4Event* event)
///////////////////////////////////////////////////////////////////////////////
{
   
    G4double TDE_ZDC=0;
    G4double TDE_W_1=0;
    G4double TDE_W_2=0;
    G4double TDE_W_3=0;
    G4double TDE_W=0;
    G4double TDE_SMDH=0;
    G4double TDE_SMDV=0;
    G4double TDE_I_PL=0;
    G4double TDE_GAP_1=0;
    G4double TDE_GAP_2=0;
    G4double TDE_GAP_3=0;


    G4HCofThisEvent* fHCE = event -> GetHCofThisEvent();


    //#ofMapW_1,2,3 [1-27]
    G4THitsMap<G4double>* fEvMapForW_1          = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbW_1));
    G4THitsMap<G4double>* fEvMapForW_2          = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbW_2));
    G4THitsMap<G4double>* fEvMapForW_3          = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbW_3));
    //#ofMapSMDH [1-8]
    G4THitsMap<G4double>* fEvMapForSMDH         = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbSMDH));
    //#ofMapSMDV [1-7]
    G4THitsMap<G4double>* fEvMapForSMDV         = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbSMDV));
    //#ofMapI_PL [0]
    G4THitsMap<G4double>* fEvMapForI_PL         = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbI_PL));
    //#ofMapGaP_1,2,3 [1-26]
    G4THitsMap<G4double>* fEvMapForGAP_1        = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbGAP_1));
    G4THitsMap<G4double>* fEvMapForGAP_2        = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbGAP_2));
    G4THitsMap<G4double>* fEvMapForGAP_3        = (G4THitsMap<G4double>*)(fHCE -> GetHC(NbGAP_3));


    // Deposit energy in I_PL
    G4double* kDepI_PL = (*fEvMapForI_PL)[0];
    if(!kDepI_PL) kDepI_PL = new G4double(0.0);
    TDE_I_PL = *kDepI_PL;


    // Deposit energy in SMDH-vertical
    G4double* kDepSMDH[8];

    for(G4int i=0; i<8; i++)
    {
        kDepSMDH[i] = (*fEvMapForSMDH)[i+1];
        if(!kDepSMDH[i]) kDepSMDH[i] = new G4double(0.0);

        G4cout << i+1 <<  "SMDH: " << *kDepSMDH[i] << G4endl;
        TDE_SMDH += *kDepSMDH[i];
    }


    // Deposit energy in SMDV-vertical
    G4double* kDepSMDV[7];

    for(G4int i=0; i<7; i++)
    {
        kDepSMDV[i] = (*fEvMapForSMDV)[i+1];
        if(!kDepSMDV[i]) kDepSMDV[i] = new G4double(0.0);

        G4cout << i+1 <<  "SMDV: " << *kDepSMDV[i] << G4endl;
        TDE_SMDV += *kDepSMDV[i];
    }

    // Deposit energy in GAP_1
    G4double* kDepGAP_1[27];

    for(G4int i=0; i<27; i++)
    {
        kDepGAP_1[i] = (*fEvMapForGAP_1)[i+1];
        if(!kDepGAP_1[i]) kDepGAP_1[i] = new G4double(0.0);

        G4cout << i+1 <<  "GAP_1: " << *kDepGAP_1[i] << G4endl;
        TDE_GAP_1 += *kDepGAP_1[i];

    }

    // Deposit energy in GAP_2
    G4double* kDepGAP_2[27];

    for(G4int i=0; i<27; i++)
    {
        kDepGAP_2[i] = (*fEvMapForGAP_2)[i+1];
        if(!kDepGAP_2[i]) kDepGAP_2[i] = new G4double(0.0);

        G4cout << i+1 <<  "GAP_2: " << *kDepGAP_2[i] << G4endl;
        TDE_GAP_2 += *kDepGAP_2[i];

    }

    // Deposit energy in GAP_3
    G4double* kDepGAP_3[27];

    for(G4int i=0; i<27; i++)
    {
        kDepGAP_3[i] = (*fEvMapForGAP_3)[i+1];
        if(!kDepGAP_3[i]) kDepGAP_3[i] = new G4double(0.0);

        G4cout << i+1 <<  "GAP_3: " << *kDepGAP_3[i] << G4endl;
        TDE_GAP_3 += *kDepGAP_3[i];

    }

    // Deposit energy in W_1
    G4double* kDepW_1[26];

    for(G4int i=0; i<26; i++)
    {
        kDepW_1[i] = (*fEvMapForW_1)[i+1];
        if(!kDepW_1[i]) kDepW_1[i] = new G4double(0.0);

        G4cout << i+1 <<  "W_1: " << *kDepW_1[i] << G4endl;
        TDE_W_1 += *kDepW_1[i];

    }

    // Deposit energy in W_2
    G4double* kDepW_2[26];

    for(G4int i=0; i<26; i++)
    {
        kDepW_2[i] = (*fEvMapForW_2)[i+1];
        if(!kDepW_2[i]) kDepW_2[i] = new G4double(0.0);

        G4cout << i+1 <<  "W_2: " << *kDepW_2[i] << G4endl;
        TDE_W_2 += *kDepW_2[i];

    }

    // Deposit energy in W_3
    G4double* kDepW_3[26];

    for(G4int i=0; i<26; i++)
    {
        kDepW_3[i] = (*fEvMapForW_3)[i+1];
        if(!kDepW_3[i]) kDepW_3[i] = new G4double(0.0);

        G4cout << i+1 <<  "W_3: " << *kDepW_3[i] << G4endl;
        TDE_W_3 += *kDepW_3[i];

        
    }

    TDE_ZDC = TDE_W_1+ TDE_W_2+ TDE_W_3+ TDE_SMDH+ TDE_SMDV+ TDE_I_PL+ TDE_GAP_1+ TDE_GAP_2+ TDE_GAP_3;
    TDE_W = TDE_W_1+ TDE_W_2+ TDE_W_3;
    G4cout << "TDE_ZDC: " << TDE_ZDC/GeV << G4endl;
    G4cout << "TDE_W_1: " << TDE_W_1/GeV << G4endl;
    G4cout << "TDE_W_2: " << TDE_W_2/GeV << G4endl;
    G4cout << "TDE_W_3: " << TDE_W_3/GeV << G4endl;
    G4cout << "TDE_W: "   << TDE_W << G4endl;
    G4cout << "TDE_SMDH: " << TDE_SMDH/MeV << G4endl;
    G4cout << "TDE_SMDV: " << TDE_SMDV/MeV << G4endl;
    G4cout << "TDE_I_PL: " << TDE_I_PL << G4endl;
    G4cout << "TDE_GAP_1: " << TDE_GAP_1/MeV << G4endl;
    G4cout << "TDE_GAP_2: " << TDE_GAP_2/MeV << G4endl;
    G4cout << "TDE_GAP_3: " << TDE_GAP_3/MeV << G4endl;
              



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


}

