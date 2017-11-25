#ifndef RHICFSteppingAction_h
#define RHICFSteppingAction_h 1

#include "G4UserSteppingAction.hh"

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
  
  void ExtractGhostInfo();
  void ExtractFCInfo();
private:

  G4Track* track;
  G4StepPoint* fStepPoint; 
};

#endif
