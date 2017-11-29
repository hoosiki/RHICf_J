#ifndef RHICFEventAction_h
#define RHICFEventAction_h 1


#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4THitsMap.hh"
#include <vector>
#include "RHICFPrimaryGeneratorAction.hh"
#include "G4HCofThisEvent.hh"

class RHICFRunAction;

class RHICFEventAction : public G4UserEventAction
{
    public:

        RHICFEventAction();
        virtual ~RHICFEventAction();
        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event*);
        G4double GetDEValue(G4HCofThisEvent*, G4String, int );
        G4int GetNOPValue(G4HCofThisEvent*, G4String, int );
        void    ExtractDEValueForARM1(G4HCofThisEvent*, const G4Event*);
        void    ExtractNOPValueForARM1(G4HCofThisEvent*);
        void    ExtractValueForSTARZDC(G4HCofThisEvent*, const G4Event*);

    private:

        G4int fPrintModulo;
};


#endif
