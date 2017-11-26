#include "RHICFPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "RHICFManager.hh"
#include "G4SystemOfUnits.hh"
#include "TMath.h"
#include "Randomize.hh"


RHICFPrimaryGeneratorAction::RHICFPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),     
  fParticleGun(0), fMessenger(0), 
  fElectron(0), fNeutron(0), fPion(0), fProton(0),
  fMomentum(250.*GeV),
  fSigmaMomentum(0.*MeV),
  fSigmaAngle(0.*deg),
  fSigmaRange(1.*mm),
  fX(0), fY(0), fZ(0),
  fRandomizePrimary(false)
{
    G4int n_particle = 1;
    fParticleGun  = new G4ParticleGun(n_particle);
    
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
    
    //G4cout << "***********************" << (fParticleGun -> GetMomentum())/GeV << "*****************"<< G4endl;
    // define commands for this class
    DefineCommands();
}


RHICFPrimaryGeneratorAction::~RHICFPrimaryGeneratorAction()
{
    delete fParticleGun;
    delete fMessenger;
}


void RHICFPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    G4ParticleDefinition* particle;
   

    if (fRandomizePrimary)
    {
        G4int i = (int)(5.*G4UniformRand());
        switch(i)
        {
            case 0:
                particle = fElectron;
                break;
            case 1:
                particle = fNeutron;
                break;
            case 2:
                particle = fPion;
                break;
            case 3:
                particle = fProton;
                break;
            default:
                particle = fProton;
                break;
        }
        fParticleGun->SetParticleDefinition(particle);
    }
    else
    {
        particle = fParticleGun->GetParticleDefinition();
    }
    
    //Junsang****G4double pp = fMomentum + (G4UniformRand()-0.5)*fSigmaMomentum;
    G4double pp = fMomentum;
    G4double mass = particle->GetPDGMass();
    G4double Ekin = std::sqrt(pp*pp+mass*mass)-mass;
    fParticleGun->SetParticleEnergy(Ekin);
    
   
    //Junsang****G4double angle = (G4UniformRand()-0.5)*fSigmaAngle;
    //Junsang****G4ThreeVector position = G4ThreeVector((fX+fSigmaRange*(G4UniformRand()-0.5))*mm, (fY+fSigmaRange*(G4UniformRand()-0.5)+4.05/sqrt(2.))*mm, (fZ+500)*mm);
    G4ThreeVector position = G4ThreeVector(0.*mm, 0.*mm, 0.*mm);

    fParticleGun->SetParticlePosition(position);
    //Junsang****fParticleGun->SetParticleMomentumDirection(G4ThreeVector(std::sin(angle),0.,std::cos(angle)));
    //Junsang****fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
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
        G4double length = sqrt((RHICFManager::GetInstance()->GetARM1Z()-14.15)*(RHICFManager::GetInstance()->GetARM1Z()-14.15)+(RHICFManager::GetInstance()->GetARM1Y()-4.75)*(RHICFManager::GetInstance()->GetARM1Y()-4.75));
        G4double A = 1.1*sqrt(2)/length;
        G4double phi = 2*TMath::Pi()*G4UniformRand();
        G4double theta = atan(A)*G4UniformRand();
        //Junsang****G4double p = G4UniformRand(); 
        //Junsang****fParticleGun->SetParticleMomentumDirection(G4ThreeVector(TMath::Cos(phi)*p*A,TMath::Sin(phi)*p*A, sqrt(1-p*p*A*A)).rotate(G4ThreeVector(1., 0., 0.),-TMath::ATan((RHICFManager::GetInstance()->GetARM1Y()-4.74)/(RHICFManager::GetInstance()->GetARM1Z()-14.15))*rad));
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(cos(phi)*sin(theta), sin(theta)*sin(phi), cos(theta)).rotate(G4ThreeVector(1., 0., 0.),-TMath::ATan((RHICFManager::GetInstance()->GetARM1Y()-4.74)/(RHICFManager::GetInstance()->GetARM1Z()-14.15))*rad));
    }

    fParticleGun->GeneratePrimaryVertex(event);
}


void RHICFPrimaryGeneratorAction::DefineCommands()
{
    // Define /RHICF/generator command directory using generic messenger class
    fMessenger 
      = new G4GenericMessenger(this, 
                               "/RHICF/generator/", 
                               "Primary generator control");
              
    // momentum command
    G4GenericMessenger::Command& momentumCmd
      = fMessenger->DeclarePropertyWithUnit("momentum", "GeV", fMomentum, 
                                    "Mean momentum of primaries.");
    momentumCmd.SetParameterName("p", true);
    momentumCmd.SetRange("p>=0.");                                
    momentumCmd.SetDefaultValue("1.");
    // ok
    //momentumCmd.SetParameterName("p", true);
    //momentumCmd.SetRange("p>=0.");                                
    
    // sigmaMomentum command
    G4GenericMessenger::Command& sigmaMomentumCmd
      = fMessenger->DeclarePropertyWithUnit("sigmaMomentum",
          "MeV", fSigmaMomentum, "Sigma momentum of primaries.");
    sigmaMomentumCmd.SetParameterName("sp", true);
    sigmaMomentumCmd.SetRange("sp>=0.");                                
    sigmaMomentumCmd.SetDefaultValue("50.");

    // sigmaAngle command
    G4GenericMessenger::Command& sigmaAngleCmd
      = fMessenger->DeclarePropertyWithUnit("sigmaAngle", "deg", fSigmaAngle, 
                                    "Sigma angle divergence of primaries.");
    sigmaAngleCmd.SetParameterName("t", true);
    sigmaAngleCmd.SetRange("t>=0.");                                
    sigmaAngleCmd.SetDefaultValue("2.");

    // sigmarange command
    G4GenericMessenger::Command& sigmaRangeCmd
      = fMessenger->DeclarePropertyWithUnit("sigmaRange", "mm", fSigmaRange, 
                                    "Sigma range divergence of primaries.");
    sigmaAngleCmd.SetParameterName("st", true);
    sigmaAngleCmd.SetRange("st>=0.");                                
    sigmaAngleCmd.SetDefaultValue("1.");

    // X command
    G4GenericMessenger::Command& XCmd
      = fMessenger->DeclarePropertyWithUnit("X", "mm", fX, 
                                    "X of position");
    sigmaAngleCmd.SetParameterName("X", true);
    sigmaAngleCmd.SetDefaultValue("0.");

    // Y command
    G4GenericMessenger::Command& YCmd
      = fMessenger->DeclarePropertyWithUnit("Y", "mm", fY, 
                                    "Y of position");
    sigmaAngleCmd.SetParameterName("Y", true);
    sigmaAngleCmd.SetDefaultValue("0.");

    // Z command
    G4GenericMessenger::Command& ZCmd
      = fMessenger->DeclarePropertyWithUnit("Z", "mm", fZ, 
                                    "Z of position");
    sigmaAngleCmd.SetParameterName("Z", true);
    sigmaAngleCmd.SetDefaultValue("0.");


    // randomizePrimary command
    G4GenericMessenger::Command& randomCmd
      = fMessenger->DeclareProperty("randomizePrimary", fRandomizePrimary);
    G4String guidance
       = "Boolean flag for randomizing primary particle types.\n";   
    guidance
       += "In case this flag is false, you can select the primary particle\n";
    guidance += "  with /gun/particle command.";                               
    randomCmd.SetGuidance(guidance);
    randomCmd.SetParameterName("flg", true);
    randomCmd.SetDefaultValue("true");
}

