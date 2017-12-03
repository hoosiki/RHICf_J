#include "G4UIcommand.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"
#include "G4UIparameter.hh"
#include "RHICFPrimaryGeneratorMessenger.hh"
#include "RHICFPrimaryGeneratorAction.hh"

RHICFPrimaryGeneratorMessenger::RHICFPrimaryGeneratorMessenger (RHICFPrimaryGeneratorAction* genaction) : primaryAction(genaction)
{
    fDir = new G4UIdirectory("/RHICfGenerator/");
    fDir-> SetGuidance("RHICF generator control commands.");

    fSelect= new G4UIcmdWithAString("/RHICfGenerator/select", this);
    fSelect-> SetGuidance("select generator type");
    fSelect-> SetParameterName("generator_type", false, false);
    fSelect-> SetCandidates("hepmc pythia ipuni testuni");
    fSelect-> SetDefaultValue("ipuni");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RHICFPrimaryGeneratorMessenger::~RHICFPrimaryGeneratorMessenger()
{
    delete fSelect;
    delete fDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RHICFPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
    if ( command == fSelect ) 
    {
        primaryAction-> SetGenerator(newValues);
        G4cout << "current generator type: " << primaryAction-> GetGeneratorName() << G4endl;
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String RHICFPrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand* command)
{
    G4String cv, st;
    if ( command == fSelect ) 
    {
        cv = primaryAction-> GetGeneratorName();
    }
    return cv;
}
