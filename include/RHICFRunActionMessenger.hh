#ifndef RHICFRunActionMessenger_h
#define RHICFRunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class RHICFRunAction;
class G4UIdirectory;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RHICFRunActionMessenger: public G4UImessenger
{
    public:
        RHICFRunActionMessenger(RHICFRunAction*);
        virtual ~RHICFRunActionMessenger();

        virtual void SetNewValue(G4UIcommand*, G4String);

    private:
        RHICFRunAction*            fRun;
        G4UIdirectory*        fRunDir;
        G4UIcmdWithAString*   fFactoryCmd;
};
#endif
