#include "ExN04PrimaryGeneratorAction.hh"
#include "ExN04PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "HepMCG4AsciiReader.hh"
#include "HepMCG4PythiaInterface.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ExN04PrimaryGeneratorAction::ExN04PrimaryGeneratorAction():G4VUserPrimaryGeneratorAction()
{
  // default generator is particle gun.
  currentGenerator = particleGun= new G4ParticleGun();
  currentGeneratorName = "particleGun";
  hepmcAscii = new HepMCG4AsciiReader();
#ifdef G4LIB_USE_PYTHIA
  pythiaGen = new HepMCG4PythiaInterface();
#else
  pythiaGen = 0;
#endif
  gentypeMap["particleGun"] = particleGun;
  gentypeMap["hepmcAscii"] = hepmcAscii;
  gentypeMap["pythia"] = pythiaGen;

  messenger= new ExN04PrimaryGeneratorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ExN04PrimaryGeneratorAction::~ExN04PrimaryGeneratorAction()
{
  delete messenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ExN04PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(currentGenerator)
    currentGenerator-> GeneratePrimaryVertex(anEvent);
  else
    G4Exception("ExN04PrimaryGeneratorAction::GeneratePrimaries",
                "PrimaryGeneratorAction001", FatalException,
                "generator is not instanciated." );
}
