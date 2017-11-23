#include "RHICFActionInitialization.hh"
#include "B5PrimaryGeneratorAction.hh"
#include "RHICFEventAction.hh"
#include "RHICFRunAction.hh"
#include "RHICFSteppingAction.hh"
#include "ExN04PrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"

RHICFActionInitialization::RHICFActionInitialization(): G4VUserActionInitialization()
{
    ;
}

RHICFActionInitialization::~RHICFActionInitialization()
{
    ;
}

void RHICFActionInitialization::BuildForMaster() const
{
    RHICFEventAction* eventAction = 0;
    SetUserAction(new RHICFRunAction(eventAction));
}

void RHICFActionInitialization::Build() const
{


    //ExN04PrimaryGeneratorAction* Primary = new ExN04PrimaryGeneratorAction;
    B5PrimaryGeneratorAction* Primary = new B5PrimaryGeneratorAction;

    //SetUserAction(new ExN04PrimaryGeneratorAction);
    SetUserAction(Primary);

    
    RHICFEventAction* eventAction = new RHICFEventAction(Primary);
    SetUserAction(eventAction);
    RHICFRunAction* runAction = new RHICFRunAction(eventAction);
    SetUserAction(runAction);
    //Junsang****SetUserAction(new RHICFSteppingAction);



}


