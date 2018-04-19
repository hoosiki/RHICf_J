#include "TestInterface.hh"

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
#include "RHICFManager.hh"


TestInterface::TestInterface() :Tower("Large"), Shape("Square"), ParticleName("neutron"), fEnergy(100.*GeV), fMessenger(0), fSigmaAngle(0.*deg), fSigmaRange(0.), fX(0.), fY(0.), fRandomizePrimary(false)
{
    DefineCommands();
}


TestInterface::~TestInterface()
{
    delete fMessenger;
}


void TestInterface::GeneratePrimaryVertex(G4Event* event)
{
    G4int tmpevent = 0;
    G4PrimaryVertex* fVertex = new G4PrimaryVertex();
    G4PrimaryParticle* fPrimaryParticle = new G4PrimaryParticle();
    fPrimaryParticle->SetMomentumDirection(G4ThreeVector(0., 0., 1.));
    G4int PDGID = G4ParticleTable::GetParticleTable()->FindParticle(ParticleName)->GetPDGEncoding();
    G4double tmpx,tmpy,tmptheta,tmpr;
    G4ThreeVector tmpdirection;
    gRandom->SetSeed(0);
    if (Shape=="Rectangular") 
    {
        tmpx = gRandom->Uniform(-50,50);
        tmpy = gRandom->Uniform(-fSigmaRange,fSigmaRange);
    }else if(Shape=="Square")
    {
        tmpx = gRandom->Uniform(-fSigmaRange,fSigmaRange);
        tmpy = gRandom->Uniform(-fSigmaRange,fSigmaRange);
    }else if(Shape=="Circle")
    {
        tmptheta = gRandom->Uniform(0.,180.);
        tmpr = gRandom->Uniform(-fSigmaRange,fSigmaRange);
        tmpdirection = G4ThreeVector( tmpr, 0., 0.).rotate(G4ThreeVector(0., 0., 1.), tmptheta*deg);
        tmpx = tmpdirection.x();
        tmpy = tmpdirection.y();
    }else if(Shape=="Diamond")
    {
        tmpx = gRandom->Uniform(-fSigmaRange,fSigmaRange);
        tmpy = gRandom->Uniform(-fSigmaRange,fSigmaRange);
        G4ThreeVector tmpdirection = G4ThreeVector( tmpx, tmpy, 0.).rotate(G4ThreeVector(0., 0., 1.), 45*deg);
        tmpx = tmpdirection.x();
        tmpy = tmpdirection.y();
    }

    fVertex-> SetPosition((tmpx+fX)*mm, (tmpy+fY)*mm, 0.*mm);
    fPrimaryParticle-> SetPDGcode(PDGID);
    fPrimaryParticle-> SetTotalEnergy(fEnergy);
    tmpevent++;
    fVertex-> SetPrimary(fPrimaryParticle);
    event-> AddPrimaryVertex(fVertex);
    RHICFManager::GetInstance()->SetParticleNumber(tmpevent);
}


void TestInterface::DefineCommands()
{
    // Define /RHICF/generator command directory using generic messenger class
    fMessenger = new G4GenericMessenger(this, "/Testbeam/", "Primary generator control");

    // pdgid command
    G4GenericMessenger::Command& pdgidCmd = fMessenger->DeclarePropertyWithUnit("particle", "G4String", ParticleName, "particle of primaries.");
    G4String guidance = "Setting for Primary particle having specific energy thrown from IP.\n";   
    guidance += "eg: e-, neutron, pi0, pi+, pi-, etc..\n";
    guidance += "eg: /Testbeam/particle neutron";                               
    pdgidCmd.SetGuidance(guidance);
    //Junsang****pdgidCmd.SetParameterName("particle", true);
    pdgidCmd.SetDefaultValue("neutron");
    
    // tower command
    G4GenericMessenger::Command& toweridCmd = fMessenger->DeclarePropertyWithUnit("tower", "G4String", Tower, "RHICF tower");
    guidance = "Beam center tower\n";   
    guidance += "eg: Large, Small\n";
    guidance += "eg: /Testbeam/tower Large";                               
    toweridCmd.SetGuidance(guidance);
    toweridCmd.SetParameterName("particle", true);
    toweridCmd.SetDefaultValue("Large");
              
    // shape command
    G4GenericMessenger::Command& shapeidCmd = fMessenger->DeclarePropertyWithUnit("shape", "G4String", Shape, "Beam shape");
    guidance = "Beam shape\n";   
    guidance += "eg: Rectangular, Sqaure, Diamond, Circle\n";
    guidance += "eg: /Testbeam/shape Diamond";                               
    shapeidCmd.SetGuidance(guidance);
    shapeidCmd.SetParameterName("shape", true);
    shapeidCmd.SetDefaultValue("Diamond");

    // energy command
    G4GenericMessenger::Command& energyCmd = fMessenger->DeclarePropertyWithUnit("energy", "GeV", fEnergy, "Mean energy of primaries.");
    guidance = "Setting for Primary particle having specific energy thrown from IP.\n";   
    guidance += "eg: /Testbeam/energy 100 GeV";                               
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

G4ThreeVector TestInterface::UnitVector(G4ThreeVector tmpvec)
{
    G4double length = sqrt(tmpvec.x()*tmpvec.x()+ tmpvec.y()*tmpvec.y()+tmpvec.z()*tmpvec.z());
    G4ThreeVector vec = G4ThreeVector(tmpvec.x()/length, tmpvec.y()/length, tmpvec.z()/length); 
    return vec;
}
