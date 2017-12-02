#include "RHICFRunActionMessenger.hh"

#include "RHICFRunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "FileManager.hh"

RHICFRunActionMessenger::RHICFRunActionMessenger(RHICFRunAction* run):G4UImessenger(),fRun(run), fRunDir(0), fFactoryCmd(0)
{
    fRunDir = new G4UIdirectory("/RHICfRunAction/");
    fRunDir->SetGuidance("run control");

    fFactoryCmd = new G4UIcmdWithAString("/RHICfRunAction/setFileName",this);
    fFactoryCmd->SetGuidance("set name for the root file");
}


RHICFRunActionMessenger::~RHICFRunActionMessenger()
{
    delete fRunDir;
    delete fFactoryCmd;
}


void RHICFRunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
    if (command == fFactoryCmd)
    { fRun->SetRootFileName(newValue);}
}
