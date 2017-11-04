#ifndef RHICFRunAction_h
#define RHICFRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class RHICFEventAction;
class G4Run;
class HistoManager;
/// Run action class

class RHICFRunAction : public G4UserRunAction
{
    public:
        RHICFRunAction(RHICFEventAction* eventAction);
        virtual ~RHICFRunAction();

        virtual void BeginOfRunAction(const G4Run*);
        virtual void   EndOfRunAction(const G4Run*);


    private:

        RHICFEventAction* fEventAction;





};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

