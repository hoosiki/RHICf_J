#include "RHICFSteppingAction.hh"
#include "globals.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Threading.hh"
//Junsang****#include "FileManager.hh"
//Junsang****#include "RHICFAnalysis.hh"

RHICFSteppingAction::RHICFSteppingAction() 
{;}


RHICFSteppingAction::~RHICFSteppingAction()
{ }


void RHICFSteppingAction::UserSteppingAction(const G4Step* step)
{
    G4bool entering = false;
    G4Track* track = step->GetTrack();

    G4String volName; 
    G4String WhereName;
    if (track->GetVolume()) volName =  track->GetVolume()->GetName(); 
    G4cout << volName << G4endl;
    G4String nextVolName;
    if (track->GetNextVolume()) nextVolName =  track->GetNextVolume()->GetName();
    G4cout << nextVolName << G4endl;


}
