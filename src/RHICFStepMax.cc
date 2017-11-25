#include "G4Track.hh"
#include "G4VParticleChange.hh"

#include "RHICFStepMax.hh"


RHICFStepMax::RHICFStepMax(const G4String& aName)
  : G4VDiscreteProcess(aName), fMaxChargedStep(DBL_MAX)
{
   if (verboseLevel>0) {
     G4cout << GetProcessName() << " is created "<< G4endl;
   }
}


RHICFStepMax::~RHICFStepMax() { }


RHICFStepMax::RHICFStepMax(RHICFStepMax& right) : G4VDiscreteProcess(right) 
{

    fMaxChargedStep = DBL_MAX;

}


G4bool RHICFStepMax::IsApplicable(const G4ParticleDefinition& particle)
{
  return (particle.GetPDGCharge() != 0.);
}


void RHICFStepMax::SetStepMax(G4double step) { fMaxChargedStep = step ; }


G4double RHICFStepMax::PostStepGetPhysicalInteractionLength(
                                              const G4Track&,
                                              G4double,
                                              G4ForceCondition* condition)
{
  // condition is set to "Not Forced"
  *condition = NotForced;

  G4double ProposedStep = DBL_MAX;

  if ( fMaxChargedStep > 0.) ProposedStep = fMaxChargedStep;

   return ProposedStep;
}


G4VParticleChange* RHICFStepMax::PostStepDoIt(const G4Track& aTrack,
                                         const G4Step&         )
{
   // do nothing
   aParticleChange.Initialize(aTrack);
   return &aParticleChange;
}


G4double RHICFStepMax::GetMeanFreePath(const G4Track&,G4double,G4ForceCondition*)
{
  return 0.;
}
