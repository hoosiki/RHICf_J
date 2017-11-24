#ifndef RHICFEventAction_h
#define RHICFEventAction_h 1


#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4THitsMap.hh"
#include <vector>
#include "B5PrimaryGeneratorAction.hh"
#include "G4HCofThisEvent.hh"

/// Event action
class RHICFRunAction;

class RHICFEventAction : public G4UserEventAction
{
public:
    RHICFEventAction(B5PrimaryGeneratorAction*);
    virtual ~RHICFEventAction();
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    G4double GetDEValue(G4HCofThisEvent*, G4String, int );
    G4int GetNOPValue(G4HCofThisEvent*, G4String, int );
    void    ExtractDEValue(G4HCofThisEvent*, const G4Event*);
    void    ExtractNOPValue(G4HCofThisEvent*, const G4Event*);

    
private:


    std::vector<G4THitsMap<G4double>*> fHCE;

    B5PrimaryGeneratorAction* fB5Primary;

    // Number for Sensitive detector
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
    G4int NbLargeW_PL;
    G4int NbSmallW_PL;
    G4int NbW_1Holder;
    G4int NbW_2Holder;
    G4int NbLargeGSO_PL;
    G4int NbSmallGSO_PL;
    G4int NbLargeLightGuide;
    G4int NbSmallLightGuide;
    G4int NbGSO_PLHolder;
    G4int NbGSOBarHolder;
    G4int NbGSOSmallRightBar;
    G4int NbGSOLargeRightBar;
    G4int NbGSOSmallLeftBar;
    G4int NbGSOLargeLeftBar;
    G4int NbAlFrame;
    G4int NbSidePanels;
    G4int NbFrontPanels;
    
    




    G4int fPrintModulo;
};


#endif
