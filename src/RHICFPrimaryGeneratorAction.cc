#include "RHICFPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "HepMCG4AsciiReader.hh"
#include "PythiaInterface.hh"
#include "IPCenterInterface.hh"
#include "RHICFPrimaryGeneratorMessenger.hh"
#include "TestInterface.hh"


RHICFPrimaryGeneratorAction::RHICFPrimaryGeneratorAction()
{
    hepmcAscii =  new HepMCG4AsciiReader();
    pythia  = new PythiaInterface();
    ipunibeam = new IPCenterInterface();
    testunibeam = new TestInterface();
    fGenTypeMap["hepmc"] = hepmcAscii;
    fGenTypeMap["pythia"] = pythia;
    fGenTypeMap["ipuni"] = ipunibeam;
    fGenTypeMap["testuni"] = testunibeam;

    fMessenger = new RHICFPrimaryGeneratorMessenger(this);
}


RHICFPrimaryGeneratorAction::~RHICFPrimaryGeneratorAction()
{
    delete fMessenger;
}


void RHICFPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    if(currentGenerator) currentGenerator-> GeneratePrimaryVertex(event);
    else G4Exception("ExN04PrimaryGeneratorAction::GeneratePrimaries", "PrimaryGeneratorAction001", FatalException, "generator is not instanciated." );

}
