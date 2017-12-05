#include "IPCenterInterface.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4ParticleTable.hh"
#include "G4MTHepRandom.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "RHICFManager.hh"
#include "G4SystemOfUnits.hh"
#include "TMath.h"
#include "Randomize.hh"


IPCenterInterface::IPCenterInterface()
: ParticleName("neutron"), fEnergy(100.), fMessenger(0), fSigmaAngle(0.*deg), fSigmaRange(1.*mm), fX(0.), fY(0.), fZ(0.), fRandomizePrimary(false)
{
    DefineCommands();
}


IPCenterInterface::~IPCenterInterface()
{
    delete fMessenger;
}


void IPCenterInterface::GeneratePrimaryVertex(G4Event* event)
{
    //Junsang****G4ParticleDefinition* tmpparticle = G4ParticleTable::GetParticleTable()->FindParticle(ParticleName);
    
    G4int PDGID = G4ParticleTable::GetParticleTable()->FindParticle(ParticleName)->GetPDGEncoding();
    //Junsang****G4cout << "PDGID: " << PDGID << G4endl;
    //Junsang****G4int PDGID = tmpparticle->GetPDGEncoding();

    //CONE MODE

    // SET PARTICLE INFO
    G4PrimaryVertex* fVertex = new G4PrimaryVertex();
    fVertex-> SetPosition(fX*mm, fY*mm, fZ*mm);
    G4PrimaryParticle* fPrimaryParticle = new G4PrimaryParticle();
    fPrimaryParticle-> SetTotalEnergy(fEnergy);
    fPrimaryParticle-> SetPDGcode(PDGID);


    if (G4UniformRand()<0.8) 
    {
        G4double length = sqrt((RHICFManager::GetInstance()->GetARM1Z()-14.15)*(RHICFManager::GetInstance()->GetARM1Z()-14.15)+(RHICFManager::GetInstance()->GetARM1Y())*(RHICFManager::GetInstance()->GetARM1Y()));
        G4double A = 2.2*sqrt(2)/length;
        G4double phi = 2*TMath::Pi()*G4UniformRand();
        G4double theta = atan(A)*G4UniformRand();
        //Junsang****G4double p = G4UniformRand(); 
        //Junsang****fParticleGun->SetParticleMomentumDirection(G4ThreeVector(TMath::Cos(phi)*p*A,TMath::Sin(phi)*p*A, sqrt(1-p*p*A*A)).rotate(G4ThreeVector(1., 0., 0.),-TMath::ATan((RHICFManager::GetInstance()->GetARM1Y())/(RHICFManager::GetInstance()->GetARM1Z()-14.15))*rad));
        fPrimaryParticle-> SetMomentumDirection(G4ThreeVector(cos(phi)*sin(theta), sin(theta)*sin(phi), cos(theta)).rotate(G4ThreeVector(1., 0., 0.),-TMath::ATan((RHICFManager::GetInstance()->GetARM1Y())/(RHICFManager::GetInstance()->GetARM1Z()-14.15))*rad));
        
    }else
    {
        G4double length = sqrt((RHICFManager::GetInstance()->GetARM1Z()-14.15)*(RHICFManager::GetInstance()->GetARM1Z()-14.15)+(RHICFManager::GetInstance()->GetARM1Y()-(30.1*sqrt(2)+5+24)/10.)*(RHICFManager::GetInstance()->GetARM1Y()-(30.1*sqrt(2)+5+24)/10.));
        G4double A = 1.1*sqrt(2)/length;
        G4double phi = 2*TMath::Pi()*G4UniformRand();
        G4double theta = atan(A)*G4UniformRand();
        //Junsang****G4double p = G4UniformRand(); 
        //Junsang****fParticleGun->SetParticleMomentumDirection(G4ThreeVector(TMath::Cos(phi)*p*A,TMath::Sin(phi)*p*A, sqrt(1-p*p*A*A)).rotate(G4ThreeVector(1., 0., 0.),-TMath::ATan((RHICFManager::GetInstance()->GetARM1Y()-4.74)/(RHICFManager::GetInstance()->GetARM1Z()-14.15))*rad));
        fPrimaryParticle-> SetMomentumDirection(G4ThreeVector(cos(phi)*sin(theta), sin(theta)*sin(phi), cos(theta)).rotate(G4ThreeVector(1., 0., 0.),-TMath::ATan((RHICFManager::GetInstance()->GetARM1Y()-4.74)/(RHICFManager::GetInstance()->GetARM1Z()-14.15))*rad));
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
    pdgidCmd.SetParameterName("particle", true);
    //Junsang****pdgidCmd.SetRange("E>=0.");                                
    pdgidCmd.SetDefaultValue("neutron");
              
    // energy command
    G4GenericMessenger::Command& energyCmd = fMessenger->DeclarePropertyWithUnit("energy", "GeV", fEnergy, "Mean energy of primaries.");
    energyCmd.SetParameterName("E", true);
    energyCmd.SetRange("E>=0.");                                
    energyCmd.SetDefaultValue("100.");
    
    // sigmaMomentum command
    G4GenericMessenger::Command& sigmaMomentumCmd = fMessenger->DeclarePropertyWithUnit("sigmaMomentum", "MeV", fSigmaMomentum, "Sigma momentum of primaries.");
    sigmaMomentumCmd.SetParameterName("sp", true);
    sigmaMomentumCmd.SetRange("sp>=0.");                                
    sigmaMomentumCmd.SetDefaultValue("50.");

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

    // Z command
    G4GenericMessenger::Command& ZCmd = fMessenger->DeclarePropertyWithUnit("Z", "mm", fZ, "Z of position");
    sigmaAngleCmd.SetParameterName("Z", true);
    sigmaAngleCmd.SetDefaultValue("0.");


    // randomizePrimary command
    G4GenericMessenger::Command& randomCmd = fMessenger->DeclareProperty("randomizePrimary", fRandomizePrimary);
    G4String guidance = "Boolean flag for randomizing primary particle types.\n";   
    guidance += "In case this flag is false, you can select the primary particle\n";
    guidance += "  with /gun/particle command.";                               
    randomCmd.SetGuidance(guidance);
    randomCmd.SetParameterName("flg", true);
    randomCmd.SetDefaultValue("true");
}

