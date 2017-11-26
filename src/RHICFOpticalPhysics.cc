#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"

#include "RHICFOpticalPhysics.hh"

RHICFOpticalPhysics::RHICFOpticalPhysics(G4bool toggle)
    : G4VPhysicsConstructor("Optical")
{
  fRHICFProcess              = NULL;
  fScintProcess              = NULL;
  fCerenkovProcess           = NULL;
  fBoundaryProcess           = NULL;
  fAbsorptionProcess         = NULL;
  fRayleighScattering        = NULL;
  fMieHGScatteringProcess    = NULL;

  fAbsorptionOn              = toggle;
}

RHICFOpticalPhysics::~RHICFOpticalPhysics() { }

#include "G4OpticalPhoton.hh"

void RHICFOpticalPhysics::ConstructParticle()
{
  G4OpticalPhoton::OpticalPhotonDefinition();
}

#include "G4ProcessManager.hh"

void RHICFOpticalPhysics::ConstructProcess()
{
    G4cout << "RHICFOpticalPhysics:: Add Optical Physics Processes"
           << G4endl;

  fRHICFProcess = new G4OpWLS();

  fScintProcess = new G4Scintillation();
  fScintProcess->SetScintillationYieldFactor(1.);
  fScintProcess->SetTrackSecondariesFirst(true);

  fCerenkovProcess = new G4Cerenkov();
  fCerenkovProcess->SetMaxNumPhotonsPerStep(300);
  fCerenkovProcess->SetTrackSecondariesFirst(true);

  fAbsorptionProcess      = new G4OpAbsorption();
  fRayleighScattering     = new G4OpRayleigh();
  //Junsang****fMieHGScatteringProcess = new G4OpMieHG();
  //Junsang****fBoundaryProcess        = new G4OpBoundaryProcess();

  G4ProcessManager* pManager =
                G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

  if (!pManager) {
     std::ostringstream o;
     o << "Optical Photon without a Process Manager";
     G4Exception("RHICFOpticalPhysics::ConstructProcess()","",
                  FatalException,o.str().c_str());
  }

  if (fAbsorptionOn) pManager->AddDiscreteProcess(fAbsorptionProcess);

  pManager->AddDiscreteProcess(fRayleighScattering);
  //Junsang****pManager->AddDiscreteProcess(fMieHGScatteringProcess);
//Junsang****
  //Junsang****pManager->AddDiscreteProcess(fBoundaryProcess);

  fRHICFProcess->UseTimeProfile("delta");
  //fRHICFProcess->UseTimeProfile("exponential");

  pManager->AddDiscreteProcess(fRHICFProcess);

  fScintProcess->SetScintillationYieldFactor(0.2); // LIGHT YIELD OF GSO IS 0.2
  fScintProcess->SetScintillationExcitationRatio(1.0);
  fScintProcess->SetTrackSecondariesFirst(true);

  // Use Birks Correction in the Scintillation process

  G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
  fScintProcess->AddSaturation(emSaturation);

  auto aParticleIterator=GetParticleIterator();
  aParticleIterator->reset();
  while ( (*aParticleIterator)() ){

    G4ParticleDefinition* particle = aParticleIterator->value();
    G4String particleName = particle->GetParticleName();

    pManager = particle->GetProcessManager();
    if (!pManager) {
       std::ostringstream o;
       o << "Particle " << particleName << "without a Process Manager";
       G4Exception("RHICFOpticalPhysics::ConstructProcess()","",
                    FatalException,o.str().c_str());
    }

    if(fCerenkovProcess->IsApplicable(*particle)){
      pManager->AddProcess(fCerenkovProcess);
      pManager->SetProcessOrdering(fCerenkovProcess,idxPostStep);
    }
    //Junsang****if(fScintProcess->IsApplicable(*particle)){ //SCINTILLATOR ON/OFF
      //Junsang****pManager->AddProcess(fScintProcess);
      //Junsang****pManager->SetProcessOrderingToLast(fScintProcess,idxAtRest);
      //Junsang****pManager->SetProcessOrderingToLast(fScintProcess,idxPostStep);
    //Junsang****}

  }
}

void RHICFOpticalPhysics::SetNbOfPhotonsCerenkov(G4int maxNumber)
{
  fCerenkovProcess->SetMaxNumPhotonsPerStep(maxNumber);
}
