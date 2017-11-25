#ifndef RHICFManager_h
#define RHICFManager_h 1


#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include <cstdlib>
#include <iostream>


//SINGLETON FOR STORING PARAMETER WHILE PROGRAM RUNNING

class RHICFManager {
    
    private:

        RHICFManager();
        G4double ARM1X, ARM1Y, ARM1Z;
        static RHICFManager* fRHICFManager;

    public:
        
        static RHICFManager* GetInstance();
        virtual ~RHICFManager();
        static bool RHICFManagerFlag;
        void SetARM1X(G4double);
        void SetARM1Y(G4double);
        void SetARM1Z(G4double);
        void SetVerboseSW(G4bool);
        void ShowDInfo(G4String, G4double);
        void ShowIInfo(G4String, G4int);
        void ShowSInfo(G4String, G4String);
        G4double GetARM1X(){ return ARM1X;};
        G4double GetARM1Y(){ return ARM1Y;};
        G4double GetARM1Z(){ return ARM1Z;};
        G4bool trigger;
};



#endif
