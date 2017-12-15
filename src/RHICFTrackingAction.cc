#include "RHICFTrackingAction.hh"

#include "RHICFEventAction.hh"

#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4RunManager.hh"
#include "g4root.hh"
#include "RHICFManager.hh"
#include "FileManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"


RHICFTrackingAction::RHICFTrackingAction(RHICFEventAction* EA) :G4UserTrackingAction(), fEvent(EA) 
{
}

RHICFTrackingAction::~RHICFTrackingAction()
{
}

void RHICFTrackingAction::PreUserTrackingAction(const G4Track* track)
{
    G4int tmpID = track->GetTrackID();
    if (tmpID<=RHICFManager::GetInstance()->GetParticleNumber()) 
    {
        StoreIPInfo(track, tmpID);
        ExtractIPInfo(track);
    }
}


void RHICFTrackingAction::PostUserTrackingAction(const G4Track* track)
{
}

void RHICFTrackingAction::ExtractIPInfo(const G4Track* track)
{
    G4AnalysisManager::Instance()->FillNtupleDColumn(7, 0, track->GetVertexPosition().x()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(7, 1, track->GetVertexPosition().y()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(7, 2, track->GetVertexPosition().z()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(7, 3, track->GetMomentum().x()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(7, 4, track->GetMomentum().y()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(7, 5, track->GetMomentum().z()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(7, 6, track->GetTotalEnergy()/GeV);
    G4AnalysisManager::Instance()->FillNtupleSColumn(7, 7, track->GetParticleDefinition()->GetParticleName());
    G4AnalysisManager::Instance()->FillNtupleIColumn(7, 8, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID()));
    G4AnalysisManager::Instance()->FillNtupleIColumn(7, 9, G4RunManager::GetRunManager()-> GetCurrentEvent()->GetEventID());
    G4AnalysisManager::Instance()->AddNtupleRow(7);     
}

void RHICFTrackingAction::StoreIPInfo(const G4Track* track, G4int trackid)
{
    RHICFManager::GetInstance()->MapName[trackid] = track->GetParticleDefinition()->GetParticleName();
    RHICFManager::GetInstance()->MapPX[trackid] = track->GetMomentum().x();
    RHICFManager::GetInstance()->MapPY[trackid] =  track->GetMomentum().y();
    RHICFManager::GetInstance()->MapPZ[trackid] =  track->GetMomentum().z();
    RHICFManager::GetInstance()->MapE[trackid] = track->GetTotalEnergy();
}
