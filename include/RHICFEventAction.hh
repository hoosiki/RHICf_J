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

        RHICFEventAction(RHICFPrimaryGeneratorAction*);
        virtual ~RHICFEventAction();
        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event*);
        G4double GetDEValue(G4HCofThisEvent*, G4String, int );
        G4int GetNOPValue(G4HCofThisEvent*, G4String, int );
        void    ExtractDEValue(G4HCofThisEvent*, const G4Event*);
        void    ExtractNOPValue(G4HCofThisEvent*);

    private:

        RHICFPrimaryGeneratorAction* fRHICFPrimary;
        G4int fPrintModulo;
};


#endif
