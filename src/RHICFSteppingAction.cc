#include "RHICFSteppingAction.hh"
#include "globals.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Threading.hh"
//Junsang****#include "FileManager.hh"
//Junsang****#include "RHICFAnalysis.hh"

RHICFSteppingAction::RHICFSteppingAction() 
{;}


RHICFSteppingAction::~RHICFSteppingAction()
{ }


void RHICFSteppingAction::UserSteppingAction(const G4Step* step)
{
    //Junsang****G4bool entering = false;
    //Junsang****G4Track* track = step->GetTrack();
//Junsang****
    //Junsang****if(G4PhysicalVolumeStore::GetInstance()->GetVolume("ARM1Physical",false))
    //Junsang****{
    //Junsang****G4String volName; 
    //Junsang****G4String WhereName;
    //Junsang****if (track->GetVolume()) volName =  track->GetVolume()->GetName(); 
    //Junsang****G4cout << volName << G4endl;
    //Junsang****G4String nextVolName;
    //Junsang****if (track->GetNextVolume()) nextVolName =  track->GetNextVolume()->GetName();
    //Junsang****G4cout << nextVolName << G4endl;
    //Junsang****}


}
