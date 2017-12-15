#ifndef RHICFTrackingAction_h
#define RHICFTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class RHICFEventAction;


class RHICFTrackingAction : public G4UserTrackingAction {

  public:  
    RHICFTrackingAction(RHICFEventAction*);
   ~RHICFTrackingAction();
   
    virtual void  PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);
    void ExtractIPInfo(const G4Track*);
    void StoreIPInfo(const G4Track*, G4int);
    
    
  private:
    RHICFEventAction*        fEvent;
    

};


#endif
