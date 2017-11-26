#include "RHICFSteppingAction.hh"
#include "globals.hh"
#include "G4Track.hh"
#include "RHICFManager.hh"
#include "G4StepPoint.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Threading.hh"
#include "FileManager.hh"
#include "g4root.hh"
RHICFSteppingAction::RHICFSteppingAction() 
{
}


RHICFSteppingAction::~RHICFSteppingAction()
{
}

void RHICFSteppingAction::UserSteppingAction(const G4Step* step)
{
    track = step->GetTrack();
    fStepPoint = step -> GetPostStepPoint();

    G4String nextVolName;
    if (track->GetNextVolume()) nextVolName =  track->GetNextVolume()->GetName();
    if(G4PhysicalVolumeStore::GetInstance()->GetVolume("GhostCenterSmallPhysical",false))
    {
        if (nextVolName=="GhostCenterSmallPhysical" || nextVolName=="GhostCenterLargePhysical") 
        {
            if (fStepPoint->GetMomentum().z()>0) 
            {
                ExtractGhostInfo();
            }
        }
    }
    fStepPoint = step -> GetPostStepPoint();
    if(G4PhysicalVolumeStore::GetInstance()->GetVolume("FrontCounterSmallPhysical",false))
    {
        if (nextVolName=="FrontCounterSmallPhysical" || nextVolName=="FrontCounterLargePhysical") 
        {
            if (fStepPoint->GetMomentum().z()>0) 
            {
                ExtractFCInfo();
            }
        }
    }


}

void RHICFSteppingAction::ExtractGhostInfo()//EXTRACT INFO FROM STEPS INFRONT OF GHOSTCENTER AND FILL INFO INTO TREE
{

    G4double tmpx = (fStepPoint->GetPosition().x())*(RHICFManager::GetInstance()->GetARM1Z()-14.15)/(fStepPoint->GetPosition().z()/cm)/mm;
    RHICFManager::GetInstance()-> ShowDInfo("X", tmpx);
    G4double tmpy = (fStepPoint->GetPosition().y())*(RHICFManager::GetInstance()->GetARM1Z()-14.15)/(fStepPoint->GetPosition().z()/cm)/mm;
    RHICFManager::GetInstance()-> ShowDInfo("Y", tmpy);
    G4AnalysisManager::Instance()->FillNtupleDColumn(3, 0, tmpx);
    G4AnalysisManager::Instance()->FillNtupleDColumn(3, 1, tmpy);
    G4AnalysisManager::Instance()->FillNtupleDColumn(3, 2, fStepPoint->GetMomentum().x()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(3, 3, fStepPoint->GetMomentum().y()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(3, 4, fStepPoint->GetMomentum().z()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(3, 5, track->GetKineticEnergy()/GeV);
    G4AnalysisManager::Instance()->FillNtupleIColumn(3, 6, track->GetParticleDefinition()->GetPDGEncoding());
    G4AnalysisManager::Instance()->FillNtupleSColumn(3, 7, track->GetParticleDefinition()->GetParticleType());
    G4AnalysisManager::Instance()->FillNtupleIColumn(3, 8, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID()));
    G4AnalysisManager::Instance()->FillNtupleIColumn(3, 9, G4RunManager::GetRunManager()-> GetCurrentEvent()->GetEventID());
    G4AnalysisManager::Instance()->AddNtupleRow(3);
}

void RHICFSteppingAction::ExtractFCInfo()
{
    G4AnalysisManager::Instance()->FillNtupleDColumn(4, 0, fStepPoint->GetPosition().x()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(4, 1, fStepPoint->GetPosition().y()/mm);
    //Junsang****//Junsang****RHICFManager::GetInstance()->ShowDInfo("z: ", fStepPoint->GetPosition().z()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(4, 2, fStepPoint->GetMomentum().x()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(4, 3, fStepPoint->GetMomentum().y()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(4, 4, fStepPoint->GetMomentum().z()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(4, 5, track->GetKineticEnergy()/GeV);
    G4AnalysisManager::Instance()->FillNtupleIColumn(4, 6, track->GetParticleDefinition()->GetPDGEncoding());
    G4AnalysisManager::Instance()->FillNtupleSColumn(4, 7, track->GetParticleDefinition()->GetParticleType());
    G4AnalysisManager::Instance()->FillNtupleIColumn(4, 8, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID()));
    G4AnalysisManager::Instance()->FillNtupleIColumn(4, 9, G4RunManager::GetRunManager()-> GetCurrentEvent()->GetEventID());
    G4AnalysisManager::Instance()->AddNtupleRow(4);
}
