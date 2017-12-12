#include "IPCenterInterface.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "RHICFManager.hh"
#include "G4SystemOfUnits.hh"
#include "TRandom.h"
#include "TMath.h"
#include "Randomize.hh"


IPCenterInterface::IPCenterInterface()
:Position("TL"), Tower("Large"), Shape("Diamond"), ParticleName("neutron"), fEnergy(100.*GeV), fMessenger(0), fSigmaAngle(0.*deg), fSigmaRange(0.), fX(0.), fY(0.), fRandomizePrimary(false)
{
    DefineCommands();
}


IPCenterInterface::~IPCenterInterface()
{
    delete fMessenger;
}


void IPCenterInterface::GeneratePrimaryVertex(G4Event* event)
{
    G4int PDGID = G4ParticleTable::GetParticleTable()->FindParticle(ParticleName)->GetPDGEncoding();

    G4PrimaryVertex* fVertex = new G4PrimaryVertex();
    fVertex-> SetPosition(0.*mm, 0.*mm, 0.*mm);
    G4PrimaryParticle* fPrimaryParticle = new G4PrimaryParticle();
    fPrimaryParticle-> SetPDGcode(PDGID);
    fPrimaryParticle-> SetTotalEnergy(fEnergy);

    G4double tmpx,tmpy;

    if (Shape=="Diamond") 
    {
        tmpx = gRandom->Uniform(-fSigmaRange,fSigmaRange);
        tmpy = gRandom->Uniform(-fSigmaRange,fSigmaRange);
        G4ThreeVector tmpdirection = G4ThreeVector( tmpx, tmpy, 0.).rotate(G4ThreeVector(0., 0., 1.), 45*deg);
        
    }else if(Shape=="Circle")
    {
        gRandom->Circle(tmpx, tmpy, fSigmaRange);
    }



    if (Position=="TOP") 
    {
        if (Tower=="Small") 
        {
            
            fPrimaryParticle->SetMomentumDirection(UnitVector(G4ThreeVector(tmpx, tmpy+(RHICFManager::GetInstance()->GetARM1Y()*10.),(RHICFManager::GetInstance()->GetARM1Z()*10.)-120.3)));
        }else
        {
        fPrimaryParticle->SetMomentumDirection(UnitVector(G4ThreeVector(tmpx, tmpy+(RHICFManager::GetInstance()->GetARM1Y()*10.)+47.4, (RHICFManager::GetInstance()->GetARM1Z()*10.)-120.3)));
        }
    }
    if (Position=="TS") 
    {
        if (Tower=="Small") 
        {
            fPrimaryParticle->SetMomentumDirection(UnitVector(G4ThreeVector(tmpx, tmpy, (RHICFManager::GetInstance()->GetARM1Z()*10.)-120.3)));
        }else
        {
            fPrimaryParticle->SetMomentumDirection(UnitVector(G4ThreeVector(tmpx, tmpy+47.4, (RHICFManager::GetInstance()->GetARM1Z()*10.)-120.3)));
        }
    }
    if (Position=="Manual") 
    {
            fPrimaryParticle->SetMomentumDirection(UnitVector(G4ThreeVector(tmpx+fX, tmpy+fY, (RHICFManager::GetInstance()->GetARM1Z()*10.)-120.3)));
    }
    
    fVertex-> SetPrimary(fPrimaryParticle);
    event-> AddPrimaryVertex(fVertex);
}


void IPCenterInterface::DefineCommands()
{
    // Define /RHICF/generator command directory using generic messenger class
    fMessenger = new G4GenericMessenger(this, "/IPUnibeam/", "Primary generator control");

    // pdgid command
    G4GenericMessenger::Command& pdgidCmd = fMessenger->DeclarePropertyWithUnit("particle", "G4String", ParticleName, "particle of primaries.");
    G4String guidance = "Setting for Primary particle having specific energy thrown from IP.\n";   
    guidance += "eg: e-, neutron, pi0, pi+, pi-, etc..\n";
    guidance += "eg: /IPUnibeam/particle neutron";                               
    pdgidCmd.SetGuidance(guidance);
    pdgidCmd.SetParameterName("particle", true);
    pdgidCmd.SetDefaultValue("neutron");

    // position command
    G4GenericMessenger::Command& positionidCmd = fMessenger->DeclarePropertyWithUnit("position", "G4String", Position, "RHICF position");
    guidance = "Setting for Position of RHICf\n";   
    guidance += "eg: TL, TS, TOP, Manual\n";
    guidance += "Manual is set beam center manually with X,Y value\n";
    guidance += "eg: /IPUnibeam/position TL";                               
    positionidCmd.SetGuidance(guidance);
    positionidCmd.SetParameterName("position", true);
    positionidCmd.SetDefaultValue("TL");
    
    // tower command
    G4GenericMessenger::Command& toweridCmd = fMessenger->DeclarePropertyWithUnit("tower", "G4String", Tower, "RHICF tower");
    guidance = "Beam center tower\n";   
    guidance += "eg: Large, Small\n";
    guidance += "eg: /IPUnibeam/tower Large";                               
    toweridCmd.SetGuidance(guidance);
    toweridCmd.SetParameterName("tower", true);
    toweridCmd.SetDefaultValue("Large");
    
    // shape command
    G4GenericMessenger::Command& shapeidCmd = fMessenger->DeclarePropertyWithUnit("shape", "G4String", Shape, "Beam shape");
    guidance = "Beam shape\n";   
    guidance += "eg: Diamond, Circle\n";
    guidance += "eg: /IPUnibeam/shape Diamond";                               
    shapeidCmd.SetGuidance(guidance);
    shapeidCmd.SetParameterName("shape", true);
    shapeidCmd.SetDefaultValue("Diamond");
              
    // energy command
    G4GenericMessenger::Command& energyCmd = fMessenger->DeclarePropertyWithUnit("energy", "GeV", fEnergy, "Mean energy of primaries.");
    guidance = "Setting for Primary particle having specific energy thrown from IP.\n";   
    guidance += "eg: /IPUnibeam/energy 100 GeV";                               
    energyCmd.SetGuidance(guidance);
    energyCmd.SetParameterName("E", true);
    energyCmd.SetRange("E>=0.");                                
    energyCmd.SetDefaultValue("100.");
    
    // sigmaAngle command
    G4GenericMessenger::Command& sigmaAngleCmd = fMessenger->DeclarePropertyWithUnit("sigmaAngle", "deg", fSigmaAngle, "Sigma angle divergence of primaries.");
    sigmaAngleCmd.SetParameterName("t", true);
    sigmaAngleCmd.SetRange("t>=0.");                                
    sigmaAngleCmd.SetDefaultValue("2.");

    // sigmarange command
    G4GenericMessenger::Command& sigmaRangeCmd = fMessenger->DeclarePropertyWithUnit("sigmaRange", "mm", fSigmaRange, "Sigma range divergence of primaries.");
    sigmaAngleCmd.SetParameterName("st", true);
    sigmaAngleCmd.SetRange("st>=0.");                                
    sigmaAngleCmd.SetDefaultValue("1.");

    // X command
    G4GenericMessenger::Command& XCmd = fMessenger->DeclarePropertyWithUnit("X", "mm", fX, "X of position");
    sigmaAngleCmd.SetParameterName("X", true);
    sigmaAngleCmd.SetDefaultValue("0.");

    // Y command
    G4GenericMessenger::Command& YCmd = fMessenger->DeclarePropertyWithUnit("Y", "mm", fY, "Y of position");
    sigmaAngleCmd.SetParameterName("Y", true);
    sigmaAngleCmd.SetDefaultValue("0.");


    // randomizePrimary command
    G4GenericMessenger::Command& randomCmd = fMessenger->DeclareProperty("randomizePrimary", fRandomizePrimary);
    guidance = "Boolean flag for randomizing primary particle types.\n";   
    guidance += "In case this flag is false, you can select the primary particle\n";
    guidance += "  with /gun/particle command.";                               
    randomCmd.SetGuidance(guidance);
    randomCmd.SetParameterName("flg", true);
    randomCmd.SetDefaultValue("true");
}


G4ThreeVector IPCenterInterface::UnitVector(G4ThreeVector tmpvec)
{
    G4double length = sqrt(tmpvec.x()*tmpvec.x()+ tmpvec.y()*tmpvec.y()+tmpvec.z()*tmpvec.z());
    G4ThreeVector vec = G4ThreeVector(tmpvec.x()/length, tmpvec.y()/length, tmpvec.z()/length); 
    return vec;
}
