// main01.cc is a part of the PYTHIA event generator.
// Copyright (C) 2017 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program. It fits on one slide in a talk.
// It studies the charged multiplicity distribution at the LHC.

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"
#include "HepMC/IO_GenEvent.h"
using namespace Pythia8;
int main() {
  // Generator. Process selection. LHC initialization. Histogram.
  Pythia pythia;
  
  //Junsang****pythia.readString(("Main:timesAllowErrors = "+sevent.str()).c_str());
  //Junsang****pythia.readString("SoftQCD:elastic = on");
  //Junsang****pythia.readString("SoftQCD:singleDiffractive = off");
  //Junsang****pythia.readString("SoftQCD:doubleDiffractive = off");
  //Junsang****pythia.readString("SoftQCD:centralDiffractive = off");
  //Junsang****pythia.readString("SoftQCD:nonDiffractive = off");
  //Junsang****pythia.readString("SoftQCD:inelastic = off");
  /// inelastic
  pythia.readString("Main:timesAllowErrors = 10");
  pythia.readString("SoftQCD:all = on");
  pythia.readString("SoftQCD:elastic = off");
  pythia.readString("SoftQCD:singleDiffractive = on");
  pythia.readString("SoftQCD:doubleDiffractive = on");
  pythia.readString("SoftQCD:centralDiffractive = on");
  pythia.readString("SoftQCD:nonDiffractive = on");
  pythia.readString("SoftQCD:inelastic = on");

  pythia.readString("Main:numberOfEvents = 1");//+sevent.str()).c_str());
  pythia.readString("Init:showChangedSettings = on");
  pythia.readString("Init:showChangedParticleData = off");
  pythia.readString("Next:numberCount = 10");
  pythia.readString("Next:numberShowInfo = 0");
  pythia.readString("Next:numberShowProcess = 0");
  pythia.readString("Next:numberShowEvent = 0");
  pythia.readString("Beams:frameType = 3");
  pythia.readString("Beams:idA = 2212");
  pythia.readString("Beams:idB = 2212");
  pythia.readString("Beams:pzA = 255.");
  pythia.readString("Beams:pzB = -255.");
  pythia.readString("Diffraction:PomFlux = 5");
  pythia.readString("MultipartonInteractions:pT0Ref = 1.");
  pythia.readString("MultipartonInteractions:ecmRef = 100.");
  pythia.readString("MultipartonInteractions:ecmPow = 0.16");
  pythia.readString("MultipartonInteractions:pTmin = 0.01");
  pythia.readString("MultipartonInteractions:bProfile = 3");
  pythia.readString("MultipartonInteractions:coreRadius = 0.4");
  pythia.readString("MultipartonInteractions:coreFraction = 0.5");
  pythia.readString("MultipartonInteractions:expPow = 1.");
  pythia.readString("ParticleDecays:limitTau0 = on");
  pythia.readString("ParticleDecays:tau0Max = 0");
  pythia.readString("Check:abortIfVeto = off");
  pythia.readString("Random:setSeed = on");

	pythia.readString("Random:seed = 10"); 

	int nEvent=pythia.mode("Main:numberOfEvents");
	int nAbort=pythia.mode("Main:timesAllowErrors");

	//Junsang****std::ofstream output(ftmp.c_str());
	std::ofstream output("outputtmp1.txt");
	HepMC::IO_GenEvent ascii_io(output);

    pythia.init();

	for(int iEvent=0; iEvent<nEvent; ++iEvent) {
	  if(!pythia.next()) {
	    if(pythia.info.atEndOfFile()) {
	      cout << " Aborted since reached end of Les Houches Event File" << endl;
	      break;
	    }
	    //Junsang****if(++iAbort < nAbort) continue;
	    //Junsang****cout << " Event generation aborted prematurely, owing to error!" << endl;
	    //Junsang****break;
	  }
	  HepMC::GenEvent* hepmcevt=new HepMC::GenEvent();
	  HepMC::Pythia8ToHepMC ToHepMC;
	  ToHepMC.fill_next_event(pythia, hepmcevt, 1);
	  ascii_io << hepmcevt;
	  //      ascii_io.print();
	  delete hepmcevt;
	}
  //Junsang****pythia.stat();
  return 0;
}
