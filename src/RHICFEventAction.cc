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
RHICFEventAction::RHICFEventAction(B5PrimaryGeneratorAction* B5G): G4UserEventAction(), fPMT_1NP(-1), fPMT_2NP(-1), fPMT_3NP(-1), fPMT_1DE(-1), fPMT_2DE(-1), fPMT_3DE(-1), fW_1(-1), fW_2(-1), fW_3(-1), fSMDH(-1), fSMDV(-1), fI_PL(-1), fFIBRID(-1), fSMDHN(-1), fSMDVN(-1), fB5Primary(B5G)
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


    if(fSMDHN == -1)
    {
        G4SDManager* fSDManager = G4SDManager::GetSDMpointer();

        fSMDHN = fSDManager -> GetCollectionID("SMDH/DE0");
        fSMDVN = fSDManager -> GetCollectionID("SMDH/DE1");
    }


}     


///////////////////////////////////////////////////////////////////////////////
void RHICFEventAction::EndOfEventAction(const G4Event* event)
///////////////////////////////////////////////////////////////////////////////
{
   


    G4HCofThisEvent* fHCE = event -> GetHCofThisEvent();

    G4THitsMap<G4double>* fEventMap_1 = (G4THitsMap<G4double>*)(fHCE -> GetHC(fSMDHN));
    G4THitsMap<G4double>* fEventMap_2 = (G4THitsMap<G4double>*)(fHCE -> GetHC(fSMDVN));


    std::map<G4int, G4double*>::iterator itr;
    std::map<G4int, G4double*>::iterator itr1;
    G4cout << "Size: " << fEventMap_2->GetSize() << G4endl;
    G4cout << "entries: " << fEventMap_2->entries() << G4endl;

    //Junsang****itr1 = fEventMap_2->GetMap()->begin();
    //Junsang****G4double dd = *(itr1->second);
    //Junsang****G4cout << "total energy: " << dd/GeV << G4endl;
    
    for(itr = fEventMap_1->GetMap()->begin(); itr != fEventMap_1->GetMap()->end(); itr++)
    {

        G4int copyNb = (itr->first);
        G4double edep = *(itr->second);

        G4cout << "\n smdh" << copyNb << ": " << edep/GeV << "GeV";


    }
    G4cout << "Size: " << fEventMap_1->GetSize() << G4endl;
    G4cout << "entries: " << fEventMap_1->entries() << G4endl;
    


    G4double* kDepSMDH0 = (*fEventMap_1)[0];
    G4double* kDepSMDH1 = (*fEventMap_1)[1];
    G4double* kDepSMDH2 = (*fEventMap_1)[2];
    G4double* kDepSMDH3 = (*fEventMap_1)[3];
    G4double* kDepSMDH4 = (*fEventMap_1)[4];
    G4double* kDepSMDH5 = (*fEventMap_1)[5];
    G4double* kDepSMDH6 = (*fEventMap_1)[6];

    if(!kDepSMDH0) kDepSMDH0 = new G4double(0.0);
    if(!kDepSMDH1) kDepSMDH1 = new G4double(0.0);
    if(!kDepSMDH2) kDepSMDH2 = new G4double(0.0);
    if(!kDepSMDH3) kDepSMDH3 = new G4double(0.0);
    if(!kDepSMDH4) kDepSMDH4 = new G4double(0.0);
    if(!kDepSMDH5) kDepSMDH5 = new G4double(0.0);

    cout << "0: " << *kDepSMDH0 << endl;
    cout << "1: " << *kDepSMDH1 << endl;
    cout << "2: " << *kDepSMDH2 << endl;
    cout << "3: " << *kDepSMDH3 << endl;
    cout << "4: " << *kDepSMDH4 << endl;
    cout << "5: " << *kDepSMDH5 << endl;


}


