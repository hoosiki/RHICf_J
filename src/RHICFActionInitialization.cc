#include "RHICFActionInitialization.hh"
#include "RHICFPrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "RHICFEventAction.hh"
#include "RHICFRunAction.hh"
#include "RHICFSteppingAction.hh"
#include "ExN04PrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "RHICFManager.hh"

RHICFActionInitialization::RHICFActionInitialization(): G4VUserActionInitialization()
{
}

RHICFActionInitialization::~RHICFActionInitialization()
{
}

void RHICFActionInitialization::BuildForMaster() const
{
    RHICFEventAction* eventAction = new RHICFEventAction;
    SetUserAction(new RHICFRunAction(eventAction));
}

void RHICFActionInitialization::Build() const
{
    //SetUserAction(new ExN04PrimaryGeneratorAction);
    SetUserAction(new RHICFPrimaryGeneratorAction);
    auto eventAction = new RHICFEventAction();
    SetUserAction(eventAction);
    RHICFRunAction* runAction = new RHICFRunAction(eventAction);
    SetUserAction(runAction);
    SetUserAction(new RHICFSteppingAction());
}


