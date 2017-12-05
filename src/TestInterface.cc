#include "TestInterface.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4MTHepRandom.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "RHICFManager.hh"
#include "G4SystemOfUnits.hh"
#include "TMath.h"
#include "Randomize.hh"


TestInterface::TestInterface()
: fParticleGun(0), fMessenger(0), fElectron(0), fNeutron(0), fPion(0), fProton(0), fSigmaAngle(0.*deg), fSigmaRange(1.*mm), fX(0), fY(0), fZ(0), fRandomizePrimary(false)
{
    fParticleGun  = new G4ParticleGun();
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    fElectron = particleTable->FindParticle(particleName="e-");
    fNeutron = particleTable->FindParticle(particleName="neutron");
    fPion = particleTable->FindParticle(particleName="pi0");
    fNeutron = particleTable->FindParticle(particleName="neutron");
    fProton = particleTable->FindParticle(particleName="proton");
    
    // default particle kinematics
    fParticleGun->SetParticlePosition(G4ThreeVector(0.*cm,50*cm,50.*cm));
    //Junsang****fParticleGun->SetParticleDefinition(fElectron);
    fParticleGun->SetParticleDefinition(fNeutron);
    
    DefineCommands();
}


TestInterface::~TestInterface()
{
    delete fParticleGun;
    delete fMessenger;
}


void TestInterface::GeneratePrimaryVertex(G4Event* event)
{
    G4ParticleDefinition* particle;
    particle = fParticleGun->GetParticleDefinition();
    
    //Junsang****G4double pp = fMomentum + (G4UniformRand()-0.5)*fSigmaMomentum;
    fParticleGun->SetParticleEnergy(fEnergy*GeV);
    
   
    //Junsang****G4double angle = (G4UniformRand()-0.5)*fSigmaAngle;
    //Junsang****G4ThreeVector position = G4ThreeVector((fX+fSigmaRange*(G4UniformRand()-0.5))*mm, (fY+fSigmaRange*(G4UniformRand()-0.5)+4.05/sqrt(2.))*mm, (fZ+500)*mm);
    G4ThreeVector position = G4ThreeVector(0.*mm, 0.*mm, 0.*mm);

    fParticleGun->SetParticlePosition(position);
    //Junsang****fParticleGun->SetParticleMomentumDirection(G4ThreeVector(std::sin(angle),0.,std::cos(angle)));
    //Junsang****if (G4UniformRand()<0.5) // TL CENTER DIRECTION
    //Junsang****{
        //Junsang****G4double length = sqrt((RHICFManager::GetInstance()->GetARM1Y()-14.15)*(RHICFManager::GetInstance()->GetARM1Y()-14.15)+(RHICFManager::GetInstance()->GetARM1Y())*(RHICFManager::GetInstance()->GetARM1Y()));
        //Junsang****fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,(RHICFManager::GetInstance()->GetARM1Y())/length,(RHICFManager::GetInstance()->GetARM1Y()-14.15)/length));
        //Junsang****
    //Junsang****}else // TS CENTER DIRECTION
    //Junsang****{
        //Junsang****G4double length = sqrt((RHICFManager::GetInstance()->GetARM1Y()-14.15)*(RHICFManager::GetInstance()->GetARM1Y()-14.15)+(RHICFManager::GetInstance()->GetARM1Y()-4.75)*(RHICFManager::GetInstance()->GetARM1Y()-4.75));
        //Junsang****fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,(RHICFManager::GetInstance()->GetARM1Y()-4.75)/length,(RHICFManager::GetInstance()->GetARM1Y()-14.15)/length));
    //Junsang****}

    //CONE MODE


    if (G4UniformRand()<0.8) 
    {
        G4double length = sqrt((RHICFManager::GetInstance()->GetARM1Z()-14.15)*(RHICFManager::GetInstance()->GetARM1Z()-14.15)+(RHICFManager::GetInstance()->GetARM1Y())*(RHICFManager::GetInstance()->GetARM1Y()));
        G4double A = 2.2*sqrt(2)/length;
        G4double phi = 2*TMath::Pi()*G4UniformRand();
        G4double theta = atan(A)*G4UniformRand();
        //Junsang****G4double p = G4UniformRand(); 
        //Junsang****fParticleGun->SetParticleMomentumDirection(G4ThreeVector(TMath::Cos(phi)*p*A,TMath::Sin(phi)*p*A, sqrt(1-p*p*A*A)).rotate(G4ThreeVector(1., 0., 0.),-TMath::ATan((RHICFManager::GetInstance()->GetARM1Y())/(RHICFManager::GetInstance()->GetARM1Z()-14.15))*rad));
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(cos(phi)*sin(theta), sin(theta)*sin(phi), cos(theta)).rotate(G4ThreeVector(1., 0., 0.),-TMath::ATan((RHICFManager::GetInstance()->GetARM1Y())/(RHICFManager::GetInstance()->GetARM1Z()-14.15))*rad));
        
    }else
    {
        G4double length = sqrt((RHICFManager::GetInstance()->GetARM1Z()-14.15)*(RHICFManager::GetInstance()->GetARM1Z()-14.15)+(RHICFManager::GetInstance()->GetARM1Y()-(30.1*sqrt(2)+5+24)/10.)*(RHICFManager::GetInstance()->GetARM1Y()-(30.1*sqrt(2)+5+24)/10.));
        G4double A = 1.1*sqrt(2)/length;
        G4double phi = 2*TMath::Pi()*G4UniformRand();
        G4double theta = atan(A)*G4UniformRand();
        //Junsang****G4double p = G4UniformRand(); 
        //Junsang****fParticleGun->SetParticleMomentumDirection(G4ThreeVector(TMath::Cos(phi)*p*A,TMath::Sin(phi)*p*A, sqrt(1-p*p*A*A)).rotate(G4ThreeVector(1., 0., 0.),-TMath::ATan((RHICFManager::GetInstance()->GetARM1Y()-4.74)/(RHICFManager::GetInstance()->GetARM1Z()-14.15))*rad));
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(cos(phi)*sin(theta), sin(theta)*sin(phi), cos(theta)).rotate(G4ThreeVector(1., 0., 0.),-TMath::ATan((RHICFManager::GetInstance()->GetARM1Y()-4.74)/(RHICFManager::GetInstance()->GetARM1Z()-14.15))*rad));
    }

    fParticleGun->GeneratePrimaryVertex(event);
}


void TestInterface::DefineCommands()
{
    // Define /RHICF/generator command directory using generic messenger class
    fMessenger = new G4GenericMessenger(this, "/TestUnibeam/", "Primary generator control");
              
    // momentum command
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
