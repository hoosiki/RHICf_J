#ifndef RHICFSteppingAction_h
#define RHICFSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class G4Step;
class RHICFRunAction;
class G4Track;
class G4StepPoint;

class RHICFSteppingAction : public G4UserSteppingAction
{
    public:

        RHICFSteppingAction();
        virtual ~RHICFSteppingAction();
        virtual void UserSteppingAction(const G4Step* step);
        void ExtractGhostInfo(const G4Step* step);
        void ExtractGhostCircleInfo(const G4Step* step);
        void ExtractWInfo(const G4Step* step);
        void ExtractGhostZDCInfo(const G4Step* step);
        void ExtractNeutronSignal(const G4Step* step);
        void ExtractPionSignal(const G4Step* step, int tmpint);
        void ExtractNeutronInfo(const G4Step* step);
        void ExtractPionInfo(const G4Step* step, int tmpint);
        bool IfGoingThrough(G4String, const G4Step*);
};

#endif
