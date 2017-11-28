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
        void ExtractFCInfo(const G4Step* step);
};

#endif
