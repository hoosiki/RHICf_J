#ifndef RHICFPythia_h
#define RHICFPythia_h 1


#include <cstdlib>
#include <iostream>
#include <string>

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"
#include "HepMC/IO_GenEvent.h"
using namespace std;

//SINGLETON FOR STORING PARAMETER WHILE PROGRAM RUNNING

class RHICFPythia {
    
    private:

        RHICFPythia();
        static RHICFPythia* fRHICFPythia;

    public:
        
        Pythia8::Pythia fPythia;
        static RHICFPythia* GetInstance();
        virtual ~RHICFPythia();
        static bool RHICFPythiaFlag;
};



#endif
