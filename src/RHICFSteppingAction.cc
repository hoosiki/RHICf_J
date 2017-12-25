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
    G4Track* ftrack = step->GetTrack();
    G4StepPoint* fStepPoint = step -> GetPreStepPoint();
    G4StepPoint* fStepPostPoint = step -> GetPostStepPoint();
    G4String prePhysical;
    G4String postPhysical;
    G4String nextVolName;
    if (ftrack->GetNextVolume()) nextVolName =  ftrack->GetNextVolume()->GetName();

    if(IfGoingThrough("GhostCirclePhysical",step))
    {
        ExtractGhostCircleInfo(step);
    }

    if(IfGoingThrough("GhostCenterSmallPhysical",step))
    {
        //Junsang****G4cout << "GhostSmall" << G4endl;
        ExtractGhostInfo(step);
        G4int tmpID = step->GetTrack()->GetParentID();
        if((tmpID<=RHICFManager::GetInstance()->GetParticleNumber()) && (ftrack->GetParticleDefinition()->GetParticleName()=="gamma") && (RHICFManager::GetInstance()->GetNameMap()[tmpID] == "pi0"))
        {
            ExtractPionInfo(step,0);
        }
        if((tmpID<=RHICFManager::GetInstance()->GetParticleNumber()) && (RHICFManager::GetInstance()->GetNameMap()[ftrack->GetTrackID()]=="neutron"))
        {
            ExtractNeutronInfo(step);
        }
    }
    if(IfGoingThrough("GhostCenterLargePhysical", step))
    {
        //Junsang****G4cout << "GhostLarge" << G4endl;

        ExtractGhostInfo(step);
        G4int tmpID = step->GetTrack()->GetParentID();
        if((tmpID<=RHICFManager::GetInstance()->GetParticleNumber()) && (ftrack->GetParticleDefinition()->GetParticleName()=="gamma") && (RHICFManager::GetInstance()->GetNameMap()[tmpID] == "pi0"))
        {
            ExtractPionInfo(step,1);
        }
        if((tmpID<=RHICFManager::GetInstance()->GetParticleNumber()) && (RHICFManager::GetInstance()->GetNameMap()[ftrack->GetTrackID()]=="neutron"))
        {
            ExtractNeutronInfo(step);
        }
    }

    if(IfGoingThrough("1stSmallW_PLPhysical",step))
    {
        ExtractWInfo(step);
        G4int tmpID = step->GetTrack()->GetParentID();
        if((tmpID<=RHICFManager::GetInstance()->GetParticleNumber()) && (ftrack->GetParticleDefinition()->GetParticleName()=="gamma") && (RHICFManager::GetInstance()->GetNameMap()[tmpID] == "pi0"))
        {
            ExtractPionSignal(step,0);
        }
        if((tmpID<=RHICFManager::GetInstance()->GetParticleNumber()) && (RHICFManager::GetInstance()->GetNameMap()[ftrack->GetTrackID()]=="neutron"))
        {
            ExtractNeutronSignal(step);
        }
    }
    if(IfGoingThrough("1stLargeW_PLPhysical", step))
    {
        ExtractWInfo(step);
        G4int tmpID = step->GetTrack()->GetParentID();
        if((tmpID<=RHICFManager::GetInstance()->GetParticleNumber()) && (ftrack->GetParticleDefinition()->GetParticleName()=="gamma") && (RHICFManager::GetInstance()->GetNameMap()[tmpID] == "pi0"))
        {
            ExtractPionSignal(step,1);
        }
        if((tmpID<=RHICFManager::GetInstance()->GetParticleNumber()) && (RHICFManager::GetInstance()->GetNameMap()[ftrack->GetTrackID()]=="neutron"))
        {
            ExtractNeutronSignal(step);
        }
    }



    if(!G4PhysicalVolumeStore::GetInstance()->GetVolume("GhostZDCPhysical",false)) // IF WANT TO USE THIS INFO, CHANGE FROM "!G4PhysicalVolume..." TO "G4PhyscalVolume..."
    {
        if(IfGoingThrough("GhostZDCPhysical",step)) ExtractGhostZDCInfo(step);
    }


}

void RHICFSteppingAction::ExtractGhostInfo(const G4Step* step)//EXTRACT INFO FROM STEPS INFRONT OF GHOSTCENTER AND FILL INFO INTO TREE
{
    G4Track* track = step->GetTrack();
    G4StepPoint* fStepPoint = step -> GetPostStepPoint();
    G4double tmpx = (fStepPoint->GetPosition().x())*(RHICFManager::GetInstance()->GetARM1Z()-120.3/10.)/(fStepPoint->GetPosition().z()/cm)/mm;
    G4double tmpy = (fStepPoint->GetPosition().y())*(RHICFManager::GetInstance()->GetARM1Z()-120.3/10.)/(fStepPoint->GetPosition().z()/cm)/mm;
    //Junsang****G4double tmpxx = (fStepPoint->GetPosition().x());
    //Junsang****G4cout << "X: " << tmpxx << G4endl;
    //Junsang****G4double tmpyy = (fStepPoint->GetPosition().y());
    //Junsang****G4cout << "Y: " << tmpy << G4endl;
    //Junsang****G4double tmpz = (fStepPoint->GetPosition().z());
    //Junsang****G4cout << "Z: " << tmpz << G4endl;
    G4AnalysisManager::Instance()->FillNtupleDColumn(8, 0, tmpx);
    G4AnalysisManager::Instance()->FillNtupleDColumn(8, 1, tmpy);
    G4AnalysisManager::Instance()->FillNtupleDColumn(8, 2, fStepPoint->GetMomentum().x()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(8, 3, fStepPoint->GetMomentum().y()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(8, 4, fStepPoint->GetMomentum().z()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(8, 5, track->GetTotalEnergy()/GeV);
    G4AnalysisManager::Instance()->FillNtupleSColumn(8, 6, track->GetParticleDefinition()->GetParticleName());

    if (track->GetParentID()<=RHICFManager::GetInstance()->GetParticleNumber()) 
    {
        if(0==track->GetParentID())
        {
            G4AnalysisManager::Instance()->FillNtupleSColumn(8, 7, track->GetParticleDefinition()->GetParticleName());
            G4AnalysisManager::Instance()->FillNtupleDColumn(8, 8, track->GetTotalEnergy()/GeV);
        }else
        {
            G4AnalysisManager::Instance()->FillNtupleSColumn(8, 7, RHICFManager::GetInstance()->GetNameMap()[track->GetParentID()]);
            G4AnalysisManager::Instance()->FillNtupleDColumn(8, 8, RHICFManager::GetInstance()->GetEnergyMap()[track->GetParentID()]/GeV);
        }

    }else
    {
            G4AnalysisManager::Instance()->FillNtupleSColumn(8, 7, "NA");
            G4AnalysisManager::Instance()->FillNtupleDColumn(8, 8, 0.);
    }

    G4AnalysisManager::Instance()->FillNtupleIColumn(8, 9, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID()));
    G4AnalysisManager::Instance()->FillNtupleIColumn(8, 10, G4RunManager::GetRunManager()-> GetCurrentEvent()->GetEventID());
    G4AnalysisManager::Instance()->AddNtupleRow(8);
}

void RHICFSteppingAction::ExtractGhostCircleInfo(const G4Step* step)//EXTRACT INFO FROM STEPS INFRONT OF GHOSTCENTER AND FILL INFO INTO TREE
{
    G4Track* track = step->GetTrack();
    G4StepPoint* fStepPoint = step -> GetPostStepPoint();
    G4double tmpx = (fStepPoint->GetPosition().x())*(RHICFManager::GetInstance()->GetARM1Z()-120.3/10.)/(fStepPoint->GetPosition().z()/cm)/mm;
    G4double tmpy = ((fStepPoint->GetPosition().y())*(RHICFManager::GetInstance()->GetARM1Z()-120.3/10.)/(fStepPoint->GetPosition().z()/cm))/mm;
    //Junsang****G4double tmpxx = (fStepPoint->GetPosition().x());
    //Junsang****G4cout << "X: " << tmpxx << G4endl;
    //Junsang****G4double tmpyy = (fStepPoint->GetPosition().y());
    //Junsang****G4cout << "Y: " << tmpy << G4endl;
    //Junsang****G4double tmpz = (fStepPoint->GetPosition().z());
    //Junsang****G4cout << "Z: " << tmpz << G4endl;
    G4AnalysisManager::Instance()->FillNtupleDColumn(9, 0, tmpx);
    G4AnalysisManager::Instance()->FillNtupleDColumn(9, 1, tmpy);
    G4AnalysisManager::Instance()->FillNtupleDColumn(9, 2, fStepPoint->GetMomentum().x()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(9, 3, fStepPoint->GetMomentum().y()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(9, 4, fStepPoint->GetMomentum().z()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(9, 5, track->GetTotalEnergy()/GeV);
    G4AnalysisManager::Instance()->FillNtupleSColumn(9, 6, track->GetParticleDefinition()->GetParticleName());

    if (track->GetParentID()<=RHICFManager::GetInstance()->GetParticleNumber()) 
    {
        if(0==track->GetParentID())
        {
            G4AnalysisManager::Instance()->FillNtupleSColumn(9, 7, track->GetParticleDefinition()->GetParticleName());
            G4AnalysisManager::Instance()->FillNtupleDColumn(9, 8, track->GetTotalEnergy()/GeV);
        }else
        {
            G4AnalysisManager::Instance()->FillNtupleSColumn(9, 7, RHICFManager::GetInstance()->GetNameMap()[track->GetParentID()]);
            G4AnalysisManager::Instance()->FillNtupleDColumn(9, 8, RHICFManager::GetInstance()->GetEnergyMap()[track->GetParentID()]/GeV);
        }

    }else
    {
            G4AnalysisManager::Instance()->FillNtupleSColumn(9, 7, "NA");
            G4AnalysisManager::Instance()->FillNtupleDColumn(9, 8, 0.);
    }


    G4AnalysisManager::Instance()->FillNtupleIColumn(9, 9, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID()));
    G4AnalysisManager::Instance()->FillNtupleIColumn(9, 10, G4RunManager::GetRunManager()-> GetCurrentEvent()->GetEventID());
    G4AnalysisManager::Instance()->AddNtupleRow(9);
}

void RHICFSteppingAction::ExtractWInfo(const G4Step* step)
{
    G4Track* track = step->GetTrack();
    G4StepPoint* fStepPoint = step -> GetPostStepPoint();
    G4AnalysisManager::Instance()->FillNtupleDColumn(3, 0, fStepPoint->GetPosition().x()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(3, 1, fStepPoint->GetPosition().y()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(3, 2, fStepPoint->GetPosition().z()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(3, 3, fStepPoint->GetMomentum().x()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(3, 4, fStepPoint->GetMomentum().y()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(3, 5, fStepPoint->GetMomentum().z()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(3, 6, track->GetTotalEnergy()/GeV);
    G4AnalysisManager::Instance()->FillNtupleSColumn(3, 7, track->GetParticleDefinition()->GetParticleName());
    if (track->GetParentID()<=RHICFManager::GetInstance()->GetParticleNumber()) 
    {
        if(0==track->GetParentID())
        {
            G4AnalysisManager::Instance()->FillNtupleSColumn(3, 8, track->GetParticleDefinition()->GetParticleName());
            G4AnalysisManager::Instance()->FillNtupleDColumn(3, 9, track->GetTotalEnergy()/GeV);
        }else
        {
            G4AnalysisManager::Instance()->FillNtupleSColumn(3, 8, RHICFManager::GetInstance()->GetNameMap()[track->GetParentID()]);
            G4AnalysisManager::Instance()->FillNtupleDColumn(3, 9, RHICFManager::GetInstance()->GetEnergyMap()[track->GetParentID()]/GeV);
        }

    }else
    {
            G4AnalysisManager::Instance()->FillNtupleSColumn(3, 8, "NA");
            G4AnalysisManager::Instance()->FillNtupleDColumn(3, 9, 0.);
    }
    G4AnalysisManager::Instance()->FillNtupleIColumn(3, 10, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID()));
    G4AnalysisManager::Instance()->FillNtupleIColumn(3, 11, G4RunManager::GetRunManager()-> GetCurrentEvent()->GetEventID());
    G4AnalysisManager::Instance()->AddNtupleRow(3);
}

void RHICFSteppingAction::ExtractGhostZDCInfo(const G4Step* step)
{
    G4Track* track = step->GetTrack();
    G4StepPoint* fStepPoint = step -> GetPreStepPoint();
    G4AnalysisManager::Instance()->FillNtupleDColumn(6, 0, fStepPoint->GetPosition().x()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(6, 1, fStepPoint->GetPosition().y()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(6, 2, fStepPoint->GetPosition().z()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(6, 3, fStepPoint->GetMomentum().x()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(6, 4, fStepPoint->GetMomentum().y()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(6, 5, fStepPoint->GetMomentum().z()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(6, 6, track->GetTotalEnergy()/GeV);
    G4AnalysisManager::Instance()->FillNtupleSColumn(6, 7, track->GetParticleDefinition()->GetParticleName());
    if (track->GetParentID()<=RHICFManager::GetInstance()->GetParticleNumber()) 
    {
        if(0==track->GetParentID())
        {
            G4AnalysisManager::Instance()->FillNtupleSColumn(6, 8, track->GetParticleDefinition()->GetParticleName());
            G4AnalysisManager::Instance()->FillNtupleDColumn(6, 9, track->GetTotalEnergy()/GeV);
        }else
        {
            G4AnalysisManager::Instance()->FillNtupleSColumn(6, 8, RHICFManager::GetInstance()->GetNameMap()[track->GetParentID()]);
            G4AnalysisManager::Instance()->FillNtupleDColumn(6, 9, RHICFManager::GetInstance()->GetEnergyMap()[track->GetParentID()]/GeV);
        }

    }else
    {
            G4AnalysisManager::Instance()->FillNtupleSColumn(6, 8, "NA");
            G4AnalysisManager::Instance()->FillNtupleDColumn(6, 9, 0.);
    }
    G4AnalysisManager::Instance()->FillNtupleIColumn(6, 10, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID()));
    G4AnalysisManager::Instance()->FillNtupleIColumn(6, 11, G4RunManager::GetRunManager()-> GetCurrentEvent()->GetEventID());
    G4AnalysisManager::Instance()->AddNtupleRow(6);
}

void RHICFSteppingAction::ExtractPionInfo(const G4Step* step, int tmpint)
{
    G4Track* track = step->GetTrack();
    G4StepPoint* fStepPoint = step -> GetPostStepPoint();
    G4double tmpx = (fStepPoint->GetPosition().x())*(RHICFManager::GetInstance()->GetARM1Z()-120.3/10.)/(fStepPoint->GetPosition().z()/cm)/mm;
    G4double tmpy = (fStepPoint->GetPosition().y())*(RHICFManager::GetInstance()->GetARM1Z()-120.3/10.)/(fStepPoint->GetPosition().z()/cm)/mm;
    G4AnalysisManager::Instance()->FillNtupleDColumn(10, 0, tmpx);
    G4AnalysisManager::Instance()->FillNtupleDColumn(10, 1, tmpy);
    G4AnalysisManager::Instance()->FillNtupleDColumn(10, 2, fStepPoint->GetMomentum().x()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(10, 3, fStepPoint->GetMomentum().y()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(10, 4, fStepPoint->GetMomentum().z()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(10, 5, track->GetTotalEnergy()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(10, 6, RHICFManager::GetInstance()->GetPXMap()[track->GetParentID()]/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(10, 7, RHICFManager::GetInstance()->GetPYMap()[track->GetParentID()]/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(10, 8, RHICFManager::GetInstance()->GetPZMap()[track->GetParentID()]/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(10, 9, RHICFManager::GetInstance()->GetEnergyMap()[track->GetParentID()]/GeV);
    G4AnalysisManager::Instance()->FillNtupleIColumn(10, 10, tmpint);
    G4AnalysisManager::Instance()->FillNtupleIColumn(10, 11, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID()));
    G4AnalysisManager::Instance()->FillNtupleIColumn(10, 12, G4RunManager::GetRunManager()-> GetCurrentEvent()->GetEventID());
    G4AnalysisManager::Instance()->FillNtupleIColumn(10, 13, track->GetTrackID());
    G4AnalysisManager::Instance()->AddNtupleRow(10);
}

void RHICFSteppingAction::ExtractNeutronInfo(const G4Step* step)
{
    G4Track* track = step->GetTrack();
    G4StepPoint* fStepPoint = step -> GetPostStepPoint();
    G4double tmpx = (fStepPoint->GetPosition().x())*(RHICFManager::GetInstance()->GetARM1Z()-120.3/10.)/(fStepPoint->GetPosition().z()/cm)/mm;
    G4double tmpy = (fStepPoint->GetPosition().y())*(RHICFManager::GetInstance()->GetARM1Z()-120.3/10.)/(fStepPoint->GetPosition().z()/cm)/mm;
    G4AnalysisManager::Instance()->FillNtupleDColumn(11, 0, tmpx);
    G4AnalysisManager::Instance()->FillNtupleDColumn(11, 1, tmpy);
    G4AnalysisManager::Instance()->FillNtupleDColumn(11, 2, fStepPoint->GetMomentum().x()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(11, 3, fStepPoint->GetMomentum().y()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(11, 4, fStepPoint->GetMomentum().z()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(11, 5, track->GetTotalEnergy()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(11, 6, RHICFManager::GetInstance()->GetPXMap()[track->GetTrackID()]/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(11, 7, RHICFManager::GetInstance()->GetPYMap()[track->GetTrackID()]/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(11, 8, RHICFManager::GetInstance()->GetPZMap()[track->GetTrackID()]/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(11, 9, RHICFManager::GetInstance()->GetEnergyMap()[track->GetTrackID()]/GeV);
    G4AnalysisManager::Instance()->FillNtupleIColumn(11, 10, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID()));
    G4AnalysisManager::Instance()->FillNtupleIColumn(11, 11, G4RunManager::GetRunManager()-> GetCurrentEvent()->GetEventID());
    G4AnalysisManager::Instance()->FillNtupleIColumn(11, 12, track->GetTrackID());
    G4AnalysisManager::Instance()->AddNtupleRow(11);
}

void RHICFSteppingAction::ExtractPionSignal(const G4Step* step, int tmpint)
{
    G4Track* track = step->GetTrack();
    G4StepPoint* fStepPoint = step -> GetPostStepPoint();
    G4AnalysisManager::Instance()->FillNtupleDColumn(12, 0, fStepPoint->GetPosition().x()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(12, 1, fStepPoint->GetPosition().y()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(12, 2, fStepPoint->GetMomentum().x()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(12, 3, fStepPoint->GetMomentum().y()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(12, 4, fStepPoint->GetMomentum().z()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(12, 5, track->GetTotalEnergy()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(12, 6, track->GetVertexPosition().x()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(12, 7, track->GetVertexPosition().y()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(12, 8, track->GetVertexPosition().z()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(12, 9,  RHICFManager::GetInstance()->GetPXMap()[track->GetParentID()]/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(12, 10, RHICFManager::GetInstance()->GetPYMap()[track->GetParentID()]/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(12, 11, RHICFManager::GetInstance()->GetPZMap()[track->GetParentID()]/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(12, 12, RHICFManager::GetInstance()->GetEnergyMap()[track->GetParentID()]/GeV);
    G4AnalysisManager::Instance()->FillNtupleIColumn(12, 13, tmpint);
    G4AnalysisManager::Instance()->FillNtupleIColumn(12, 14, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID()));
    G4AnalysisManager::Instance()->FillNtupleIColumn(12, 15, G4RunManager::GetRunManager()-> GetCurrentEvent()->GetEventID());
    G4AnalysisManager::Instance()->FillNtupleIColumn(12, 16, track->GetTrackID());
    G4AnalysisManager::Instance()->AddNtupleRow(12);
}

void RHICFSteppingAction::ExtractNeutronSignal(const G4Step* step)
{
    G4Track* track = step->GetTrack();
    G4StepPoint* fStepPoint = step -> GetPostStepPoint();
    G4AnalysisManager::Instance()->FillNtupleDColumn(13, 0, fStepPoint->GetPosition().x()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(13, 1, fStepPoint->GetPosition().y()/mm);
    G4AnalysisManager::Instance()->FillNtupleDColumn(13, 2, fStepPoint->GetMomentum().x()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(13, 3, fStepPoint->GetMomentum().y()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(13, 4, fStepPoint->GetMomentum().z()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(13, 5, track->GetTotalEnergy()/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(13, 6, RHICFManager::GetInstance()->GetPXMap()[track->GetTrackID()]/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(13, 7, RHICFManager::GetInstance()->GetPYMap()[track->GetTrackID()]/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(13, 8, RHICFManager::GetInstance()->GetPZMap()[track->GetTrackID()]/GeV);
    G4AnalysisManager::Instance()->FillNtupleDColumn(13, 9, RHICFManager::GetInstance()->GetEnergyMap()[track->GetTrackID()]/GeV);
    G4AnalysisManager::Instance()->FillNtupleIColumn(13, 10, stoi(FileManager::GetInstance()->GetTime()+FileManager::GetInstance()->GetPID()));
    G4AnalysisManager::Instance()->FillNtupleIColumn(13, 11, G4RunManager::GetRunManager()-> GetCurrentEvent()->GetEventID());
    G4AnalysisManager::Instance()->AddNtupleRow(13);     
}                                                        

bool RHICFSteppingAction::IfGoingThrough(G4String physicalname, const G4Step* step)
{
    if(G4PhysicalVolumeStore::GetInstance()->GetVolume(physicalname,false))
    {
        if (step->GetTrack()->GetNextVolume())
        {
            if(step->GetTrack()->GetNextVolume()->GetName()==physicalname)
            {
                if(step->GetPostStepPoint()->GetPhysicalVolume())
                {
                    G4String prePhysical = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
                    G4String postPhysical = step->GetPostStepPoint()->GetPhysicalVolume()->GetName();
                    if (prePhysical!=physicalname && postPhysical==physicalname) 
                    {
                        if (step->GetPreStepPoint()->GetMomentum().z()>0) 
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}
