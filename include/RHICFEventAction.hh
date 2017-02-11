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

    G4int NbW_1;
    G4int NbW_2;
    G4int NbW_3;
    G4int NbSMDH;
    G4int NbSMDV;
    G4int NbI_PL;
    G4int NbGAP_1;
    G4int NbGAP_2;
    G4int NbGAP_3;
    G4int NbNOPGAP_1;
    G4int NbNOPGAP_2;
    G4int NbNOPGAP_3;
    G4int NbNOPSMDH;
    G4int NbNOPSMDV;



    G4int fPrintModulo;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
