#ifndef RHICFRunAction_h
#define RHICFRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "RHICFRunActionMessenger.hh"

class RHICFEventAction;
class G4Run;
class HistoManager;

class RHICFRunAction : public G4UserRunAction
{
    public:
        void NtupleForARM1();
        void NtupleForSTARZDC();
        void NtupleForIP();
        RHICFRunAction(RHICFEventAction* eventAction);
        virtual ~RHICFRunAction();
        virtual void BeginOfRunAction(const G4Run*);
        virtual void   EndOfRunAction(const G4Run*);
        void SetRootFileName(G4String);
    private:
        RHICFEventAction* fEventAction;
        RHICFRunActionMessenger* fRHICFRunActionMessenger;

};
#endif

