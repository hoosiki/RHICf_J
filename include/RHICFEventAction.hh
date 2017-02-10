#ifndef RHICFEventAction_h
#define RHICFEventAction_h 1


#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4THitsMap.hh"
#include <vector>
#include "B5PrimaryGeneratorAction.hh"

/// Event action
class RHICFRunAction;

class RHICFEventAction : public G4UserEventAction
{
public:
    RHICFEventAction(B5PrimaryGeneratorAction*);
    virtual ~RHICFEventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    
private:


    std::vector<G4THitsMap<G4double>*> fHCE;

    B5PrimaryGeneratorAction* fB5Primary;

    G4int fW_1;
    G4int fW_2;
    G4int fW_3;
    G4int fSMDH;
    G4int fSMDV;
    G4int fI_PL;

    G4int fPrintModulo;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
