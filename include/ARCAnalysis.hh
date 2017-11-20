#ifndef G4PROCESSTESTANALYSIS_HH
#define G4PROCESSTESTANALYSIS_HH

#include "globals.hh"
#include "G4ios.hh"


#include "g4root.hh"

class G4Track;

class ARCAnalysis
{
public:

  ~ARCAnalysis();

  void book(G4bool isMaster);
  
  void finish(G4bool isMaster);
  
  void analyseStepping(const G4Track& track, G4bool entering, G4String WhereName);

  static ARCAnalysis* getInstance();

  void Update(G4double energy,G4int threadID);

private:

  ARCAnalysis();

  static ARCAnalysis* instance;

  // Quantities for the ntuple
  G4double eKin;
  G4double x;
  G4double y;
  G4double z;
  G4double dirX;
  G4double dirY;
  G4double dirZ;

  G4String asciiFileName;
  G4String histFileName;
  
  std::ofstream *asciiFile;

  //global counters: log separately for each thread (or sequential)
  std::map<G4int,G4int> *nEnteringTracks;
  std::map<G4int,G4double> *totEnteringEnergy;

};

#endif 
