#ifndef RHICFStepMax_h
#define RHICFStepMax_h 1

#include "globals.hh"

#include "G4Step.hh"
#include "G4VDiscreteProcess.hh"
#include "G4ParticleDefinition.hh"

class RHICFStepMax : public G4VDiscreteProcess
{
    public:

        RHICFStepMax(const G4String& processName = "UserStepMax");
        RHICFStepMax(RHICFStepMax &);

        virtual ~RHICFStepMax();

        virtual G4bool IsApplicable(const G4ParticleDefinition&);

        void SetStepMax(G4double);

        G4double GetStepMax() {return fMaxChargedStep;};

        virtual G4double PostStepGetPhysicalInteractionLength(const G4Track& track, G4double previousStepSize, G4ForceCondition* condition);

        virtual G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);

    protected:

        G4double GetMeanFreePath(const G4Track&, G4double, G4ForceCondition*);

    private:

        // hide assignment operator as private
        RHICFStepMax & operator=(const RHICFStepMax &right);
        RHICFStepMax(const RHICFStepMax&);

    private:

        G4double fMaxChargedStep;

};

#endif
