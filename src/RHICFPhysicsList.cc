#include "RHICFPhysicsList.hh"
#include "RHICFPhysicsListMessenger.hh"

#include "RHICFExtraPhysics.hh"
#include "RHICFOpticalPhysics.hh"

#include "G4LossTableManager.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

//#include "G4PhysListFactory.hh"
#include "FTFP_BERT.hh"
#include "QGSP_BERT_HP.hh"
#include "QGSP_BERT.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "RHICFStepMax.hh"

#include "G4ProcessTable.hh"

#include "G4PionDecayMakeSpin.hh"
#include "G4DecayWithSpin.hh"

#include "G4DecayTable.hh"
#include "G4MuonDecayChannelWithSpin.hh"
#include "G4MuonRadiativeDecayChannelWithSpin.hh"

#include "G4RadioactiveDecayPhysics.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RHICFPhysicsList::RHICFPhysicsList(G4String physName) : G4VModularPhysicsList()
{
    G4LossTableManager::Instance();

    defaultCutValue  = 1.*mm;
    fCutForGamma     = defaultCutValue;
    fCutForElectron  = defaultCutValue;
    fCutForPositron  = defaultCutValue;

//    G4PhysListFactory factory;
    G4VModularPhysicsList* phys = NULL;
    if (physName == "QGSP_BERT") {
       phys = new QGSP_BERT;
    } else {
       phys = new FTFP_BERT;
    }
//    if (factory.IsReferencePhysList(physName)) {
//       phys = factory.GetReferencePhysList(physName);
//       if(!phys)G4Exception("RHICFPhysicsList::RHICFPhysicsList","InvalidSetup",
//                            FatalException,"PhysicsList does not exist");
       fMessenger = new RHICFPhysicsListMessenger(this);
//    }

    for (G4int i = 0; ; ++i) {
       G4VPhysicsConstructor* elem =
                  const_cast<G4VPhysicsConstructor*> (phys->GetPhysics(i));
       if (elem == NULL) break;
       G4cout << "RegisterPhysics: " << elem->GetPhysicsName() << G4endl;
       RegisterPhysics(elem);
    }

    fAbsorptionOn = false;
    
    //This looks complex, but it is not:
    //Get from base-class the pointer of the phsyicsVector
    //to be used. Remember: G4VModularPhysicsList is now a split class.
    //Why G4VModularPhysicsList::RegisterPhysics method is not used instead?
    //If possible we can remove this...
    fPhysicsVector =
                GetSubInstanceManager().offset[GetInstanceID()].physicsVector;
    
    fPhysicsVector->push_back(new RHICFExtraPhysics());
    //Junsang****fPhysicsVector->push_back(fOpticalPhysics = new RHICFOpticalPhysics(fAbsorptionOn));

    //Junsang****fPhysicsVector->push_back(new G4RadioactiveDecayPhysics());

    fStepMaxProcess = new RHICFStepMax();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RHICFPhysicsList::~RHICFPhysicsList()
{
    delete fMessenger;

    delete fStepMaxProcess;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RHICFPhysicsList::ClearPhysics()
{
    for (G4PhysConstVector::iterator p  = fPhysicsVector->begin();
                                     p != fPhysicsVector->end(); ++p) {
        delete (*p);
    }
    fPhysicsVector->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RHICFPhysicsList::ConstructParticle()
{
    G4VModularPhysicsList::ConstructParticle();

    G4DecayTable* MuonPlusDecayTable = new G4DecayTable();
    MuonPlusDecayTable -> Insert(new
                           G4MuonDecayChannelWithSpin("mu+",0.986));
    MuonPlusDecayTable -> Insert(new
                           G4MuonRadiativeDecayChannelWithSpin("mu+",0.014));
    G4MuonPlus::MuonPlusDefinition() -> SetDecayTable(MuonPlusDecayTable);

    G4DecayTable* MuonMinusDecayTable = new G4DecayTable();
    MuonMinusDecayTable -> Insert(new
                            G4MuonDecayChannelWithSpin("mu-",0.986));
    MuonMinusDecayTable -> Insert(new
                            G4MuonRadiativeDecayChannelWithSpin("mu-",0.014));
    G4MuonMinus::MuonMinusDefinition() -> SetDecayTable(MuonMinusDecayTable);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RHICFPhysicsList::ConstructProcess()
{
    G4VModularPhysicsList::ConstructProcess();

    SetVerbose(0);

    G4DecayWithSpin* decayWithSpin = new G4DecayWithSpin();

    G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();

    G4VProcess* decay;
    decay = processTable->FindProcess("Decay",G4MuonPlus::MuonPlus());

    G4ProcessManager* pManager;
    pManager = G4MuonPlus::MuonPlus()->GetProcessManager();

    if (pManager) {
      if (decay) pManager->RemoveProcess(decay);
      pManager->AddProcess(decayWithSpin);
      // set ordering for PostStepDoIt and AtRestDoIt
      pManager ->SetProcessOrdering(decayWithSpin, idxPostStep);
      pManager ->SetProcessOrdering(decayWithSpin, idxAtRest);
    }

    decay = processTable->FindProcess("Decay",G4MuonMinus::MuonMinus());

    pManager = G4MuonMinus::MuonMinus()->GetProcessManager();

    if (pManager) {
      if (decay) pManager->RemoveProcess(decay);
      pManager->AddProcess(decayWithSpin);
      // set ordering for PostStepDoIt and AtRestDoIt
      pManager ->SetProcessOrdering(decayWithSpin, idxPostStep);
      pManager ->SetProcessOrdering(decayWithSpin, idxAtRest);
    }

    G4PionDecayMakeSpin* poldecay = new G4PionDecayMakeSpin();

    decay = processTable->FindProcess("Decay",G4PionPlus::PionPlus());

    pManager = G4PionPlus::PionPlus()->GetProcessManager();

    if (pManager) {
      if (decay) pManager->RemoveProcess(decay);
      pManager->AddProcess(poldecay);
      // set ordering for PostStepDoIt and AtRestDoIt
      pManager ->SetProcessOrdering(poldecay, idxPostStep);
      pManager ->SetProcessOrdering(poldecay, idxAtRest);
    }

    decay = processTable->FindProcess("Decay",G4PionMinus::PionMinus());

    pManager = G4PionMinus::PionMinus()->GetProcessManager();

    if (pManager) {
      if (decay) pManager->RemoveProcess(decay);
      pManager->AddProcess(poldecay);
      // set ordering for PostStepDoIt and AtRestDoIt
      pManager ->SetProcessOrdering(poldecay, idxPostStep);
      pManager ->SetProcessOrdering(poldecay, idxAtRest);
    }

    AddStepMax();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RHICFPhysicsList::RemoveFromPhysicsList(const G4String& name)
{
    G4bool success = false;
    for (G4PhysConstVector::iterator p  = fPhysicsVector->begin();
                                     p != fPhysicsVector->end(); ++p) {
        G4VPhysicsConstructor* e = (*p);
        if (e->GetPhysicsName() == name) {
           fPhysicsVector->erase(p);
           success = true;
           break;
        }
    }
    if (!success) {
       G4ExceptionDescription message;
       message << "PhysicsList::RemoveFromEMPhysicsList "<< name << "not found";
       G4Exception("example RHICFPhysicsList::RemoveFromPhysicsList()",
       "ExamRHICFPhysicsList01",FatalException,message);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RHICFPhysicsList::SetAbsorption(G4bool toggle)
{
       fAbsorptionOn = toggle;
       RemoveFromPhysicsList("Optical");
       fPhysicsVector->
                    push_back(fOpticalPhysics = new RHICFOpticalPhysics(toggle));
       fOpticalPhysics->ConstructProcess();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RHICFPhysicsList::SetCuts()
{
    if (verboseLevel >0) {
        G4cout << "RHICFPhysicsList::SetCuts:";
        G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length")
               << G4endl;
    }

    // set cut values for gamma at first and for e- second and next for e+,
    // because some processes for e+/e- need cut values for gamma
    SetCutValue(fCutForGamma, "gamma");
    SetCutValue(fCutForElectron, "e-");
    SetCutValue(fCutForPositron, "e+");

    if (verboseLevel>0) DumpCutValuesTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RHICFPhysicsList::SetCutForGamma(G4double cut)
{
    fCutForGamma = cut;
    SetParticleCuts(fCutForGamma, G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RHICFPhysicsList::SetCutForElectron(G4double cut)
{
    fCutForElectron = cut;
    SetParticleCuts(fCutForElectron, G4Electron::Electron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RHICFPhysicsList::SetCutForPositron(G4double cut)
{
    fCutForPositron = cut;
    SetParticleCuts(fCutForPositron, G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RHICFPhysicsList::SetStepMax(G4double step)
{
  fStepMaxProcess->SetStepMax(step);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RHICFStepMax* RHICFPhysicsList::GetStepMaxProcess()
{
  return fStepMaxProcess;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RHICFPhysicsList::AddStepMax()
{
  // Step limitation seen as a process

  theParticleIterator->reset();
  while ((*theParticleIterator)()){
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();

      if (fStepMaxProcess->IsApplicable(*particle) && !particle->IsShortLived())
      {
         if (pmanager) pmanager ->AddDiscreteProcess(fStepMaxProcess);
      }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RHICFPhysicsList::SetNbOfPhotonsCerenkov(G4int maxNumber)
{
   fOpticalPhysics->SetNbOfPhotonsCerenkov(maxNumber);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RHICFPhysicsList::SetVerbose(G4int verbose)
{
   //Junsang****fOpticalPhysics->GetCerenkovProcess()->SetVerboseLevel(verbose);
   //Junsang****fOpticalPhysics->GetScintillationProcess()->SetVerboseLevel(verbose);
   //Junsang****fOpticalPhysics->GetAbsorptionProcess()->SetVerboseLevel(verbose);
   //Junsang****fOpticalPhysics->GetRayleighScatteringProcess()->SetVerboseLevel(verbose);
   //Junsang****fOpticalPhysics->GetMieHGScatteringProcess()->SetVerboseLevel(verbose);
   //Junsang****fOpticalPhysics->GetBoundaryProcess()->SetVerboseLevel(verbose);
}
