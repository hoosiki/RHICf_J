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




}     


///////////////////////////////////////////////////////////////////////////////
void RHICFEventAction::EndOfEventAction(const G4Event* event)
///////////////////////////////////////////////////////////////////////////////
{
   







}


