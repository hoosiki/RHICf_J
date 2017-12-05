#include "RHICFPythia.hh"
#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"
#include "HepMC/IO_GenEvent.h"
#include <iostream>
#include <string>
#include "Seeder.hh"

using namespace std;

bool RHICFPythia::RHICFPythiaFlag = false;

RHICFPythia* RHICFPythia::fRHICFPythia = NULL;

RHICFPythia::RHICFPythia() 
{

    Seeder* fSeeder = new Seeder;
    //Junsang****pythia.readString(("Main:timesAllowErrors = "+sevent.str()).c_str());
    //Junsang****pythia.readString("SoftQCD:elastic = on");
    //Junsang****pythia.readString("SoftQCD:singleDiffractive = off");
    //Junsang****pythia.readString("SoftQCD:doubleDiffractive = off");
    //Junsang****pythia.readString("SoftQCD:centralDiffractive = off");
    //Junsang****pythia.readString("SoftQCD:nonDiffractive = off");
    //Junsang****pythia.readString("SoftQCD:inelastic = off");
    /// inelastic
    fPythia.readString("Main:timesAllowErrors = 10");
    fPythia.readString("SoftQCD:all = on");
    fPythia.readString("SoftQCD:elastic = off");
    fPythia.readString("SoftQCD:singleDiffractive = on");
    fPythia.readString("SoftQCD:doubleDiffractive = on");
    fPythia.readString("SoftQCD:centralDiffractive = on");
    fPythia.readString("SoftQCD:nonDiffractive = on");
    fPythia.readString("SoftQCD:inelastic = on");
    fPythia.readString("Init:showChangedSettings = on");
    fPythia.readString("Init:showChangedParticleData = off");
    fPythia.readString("Next:numberShowInfo = 0");
    fPythia.readString("Next:numberShowProcess = 0");
    fPythia.readString("Next:numberShowEvent = 0");
    fPythia.readString("Beams:frameType = 1");
    fPythia.readString("Beams:idA = 2212");
    fPythia.readString("Beams:idB = 2212");
    fPythia.readString("Beams:eCM = 510.");
    fPythia.readString("Diffraction:PomFlux = 5");
    fPythia.readString("MultipartonInteractions:pT0Ref = 1.");
    fPythia.readString("MultipartonInteractions:ecmRef = 100.");
    fPythia.readString("MultipartonInteractions:ecmPow = 0.16");
    fPythia.readString("MultipartonInteractions:pTmin = 0.01");
    fPythia.readString("MultipartonInteractions:bProfile = 3");
    fPythia.readString("MultipartonInteractions:coreRadius = 0.4");
    fPythia.readString("MultipartonInteractions:coreFraction = 0.5");
    fPythia.readString("MultipartonInteractions:expPow = 1.");
    fPythia.readString("ParticleDecays:limitTau0 = on");
    fPythia.readString("ParticleDecays:tau0Max = 0");
    fPythia.readString("Check:abortIfVeto = off");
    fPythia.readString("Main:numberOfEvents = 100000");

    fPythia.readString("Random:setSeed = on");
    string tmpstring = "Random:seed = " + to_string(fSeeder->GetSeedForPythia());
    std::cout << "Pythia seed: " << tmpstring << std::endl;
    fPythia.readString(tmpstring); 
    fPythia.init();

}

RHICFPythia::~RHICFPythia()
{
    RHICFPythiaFlag = false;
}

RHICFPythia* RHICFPythia::GetInstance()
{
    if(!fRHICFPythia)
    {
        fRHICFPythia = new RHICFPythia();
        RHICFPythiaFlag = true;
    }
    return fRHICFPythia;


}


