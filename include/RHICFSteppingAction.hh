#ifndef RHICFSteppingAction_h
#define RHICFSteppingAction_h 1

#include "G4UserSteppingAction.hh"

class G4Step;
class RHICFRunAction;

class RHICFSteppingAction : public G4UserSteppingAction
{
public:

  RHICFSteppingAction();

  virtual ~RHICFSteppingAction();

  virtual void UserSteppingAction(const G4Step* step);
  

};

#endif
