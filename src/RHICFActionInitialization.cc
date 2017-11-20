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
    //Junsang****RHICFEventAction* eventAction = 0;
    //Junsang****SetUserAction(new RHICFRunAction(eventAction));
}

void RHICFActionInitialization::Build() const
{


    //ExN04PrimaryGeneratorAction* Primary = new ExN04PrimaryGeneratorAction;
    B5PrimaryGeneratorAction* Primary = new B5PrimaryGeneratorAction;

    //SetUserAction(new ExN04PrimaryGeneratorAction);
    SetUserAction(Primary);

    
    //Junsang****RHICFEventAction* eventAction = new RHICFEventAction(Primary);
    //Junsang****SetUserAction(eventAction);
//Junsang****
    //Junsang****RHICFRunAction* runAction = new RHICFRunAction(eventAction);
    //Junsang****SetUserAction(runAction);
    SetUserAction(new RHICFSteppingAction);



}


