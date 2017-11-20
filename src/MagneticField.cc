#include "MagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField::MagneticField()
//: G4MagneticField(), fMessenger(0), fBy(7.6*tesla)
//: G4MagneticField(), fMessenger(0), fBy(1.7578*tesla)// momentum = 100 GeV
//: G4MagneticField(), fMessenger(0), fBy(2.*tesla)// momentum = 150 GeV
//: G4MagneticField(), fMessenger(0), fBy(3.*tesla)// momentum = 200 GeV
: G4MagneticField(), fMessenger(0), fBy(-4.255*tesla)// momentum = 250 GeV
{
    // define commands for this class
    DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField::~MagneticField()
{ 
    delete fMessenger; 
}

void MagneticField::GetFieldValue(const G4double [4],double *bField) const
{
    bField[0] = 0.;
    bField[1] = fBy;
    bField[2] = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MagneticField::DefineCommands()
{
    // Define /B5/field command directory using generic messenger class
    fMessenger = new G4GenericMessenger(this, 
                                        "/RHICf/field/", 
                                        "Field control");

    // fieldValue command 
    G4GenericMessenger::Command& valueCmd
      = fMessenger->DeclareMethodWithUnit("value","tesla",
                                  &MagneticField::SetField, 
                                  "Set field strength.");
    valueCmd.SetParameterName("field", true);
    valueCmd.SetDefaultValue("1.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
