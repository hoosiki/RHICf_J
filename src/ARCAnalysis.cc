#include <fstream>
#include <iomanip>
#include "G4AutoLock.hh"
#include "ARCAnalysis.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4ios.hh"
#include "G4SteppingManager.hh"
#include "G4ThreeVector.hh"
#include "G4RunManager.hh"
#include <ctime>
#include <unistd.h>

ARCAnalysis* ARCAnalysis::instance = 0;

namespace { 
  //Mutex to acquire access to singleton instance check/creation
  G4Mutex instanceMutex = G4MUTEX_INITIALIZER;
  //Mutex to acquire accss to histograms creation/access
  //It is also used to control all operations related to histos 
  //File writing and check analysis
  G4Mutex dataManipulationMutex = G4MUTEX_INITIALIZER;
}

ARCAnalysis::ARCAnalysis() : 
  asciiFile(0),nEnteringTracks(0), totEnteringEnergy(0)
{
    G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");

    long seeds[2];

    time_t systime = time(NULL) + getpid();
    seeds[0] = (long) systime;
    seeds[1] = (long) (systime*G4UniformRand());

    G4Random::setTheSeeds(seeds);

//    std::cout << "+++++++" << seeds[0]+seeds[1] << std::endl;
    std::ostringstream temp;
    

    temp << seeds[0];

  histFileName = "test"+temp.str();


  asciiFileName="xraytel.out";
  asciiFile = new std::ofstream(asciiFileName);

  if(asciiFile->is_open()) 
    (*asciiFile) << "Energy (keV)  x (mm)    y (mm)    z (mm)" << G4endl << G4endl;  
}

ARCAnalysis::~ARCAnalysis()
{
  if (asciiFile)
    delete asciiFile;
  if (nEnteringTracks)
    delete nEnteringTracks;
  if (totEnteringEnergy)
    delete totEnteringEnergy;
}


ARCAnalysis* ARCAnalysis::getInstance()
{
  G4AutoLock l(&instanceMutex);
  if (instance == 0) 
    instance = new ARCAnalysis;
  return instance;
}


void ARCAnalysis::book(G4bool isMaster)
{
  G4AutoLock l(&dataManipulationMutex);

  //reset counters: do be done only once, by the master
  if (isMaster)
    {
      if (nEnteringTracks)    
	{
	  delete nEnteringTracks;
	  nEnteringTracks = 0;
	}
      nEnteringTracks = new std::map<G4int,G4int>;
  
      if (totEnteringEnergy)
	{
	  delete totEnteringEnergy;
	  totEnteringEnergy = 0;
	}
      totEnteringEnergy = new std::map<G4int,G4double>;
    }

  // Get/create analysis manager
  G4AnalysisManager* man = G4AnalysisManager::Instance();

  // Open an output file: it is done in master and threads. The 
  // printout is done only by the master, for tidyness
  if (isMaster)
    G4cout << "Opening output file " << histFileName << " ... ";
  man->OpenFile(histFileName);
  man->SetFirstHistoId(1);
  if (isMaster)
    G4cout << " done" << G4endl;

  // Book 1D histograms
  man->CreateH1("h1","Energy, all /keV",  100,0.,100.);
  man->CreateH1("h2","Energy, entering detector /keV", 500,0.,500.);

  // Book 2D histograms (notice: the numbering is independent)
  man->CreateH2("d1","y-z, all /mm", 100,-60000.,60000.,100,-60000.,60000.); 
  man->CreateH2("d2","y-z, entering detector /mm", 200,-60000.,60000.,200,-60000.,60000.);
  
  // Book ntuples
  man->CreateNtuple("AIR", "Track ntuple"); //0
  man->CreateNtupleDColumn("energy");
  man->CreateNtupleDColumn("x");
  man->CreateNtupleDColumn("y");
  man->CreateNtupleDColumn("z");
  man->CreateNtupleDColumn("dirx");
  man->CreateNtupleDColumn("diry");
  man->CreateNtupleDColumn("dirz");

  man->CreateNtuple("21Floor", "Track ntuple"); //1
  man->CreateNtupleDColumn("21Fx");
  man->CreateNtupleDColumn("21Fy");
  man->CreateNtupleDColumn("21Fz");
  man->CreateNtupleDColumn("21F_dirx");
  man->CreateNtupleDColumn("21F_diry");
  man->CreateNtupleDColumn("21F_dirz");


  man->CreateNtuple("22Floor", "Track ntuple"); //2
  man->CreateNtupleDColumn("22Fx");
  man->CreateNtupleDColumn("22Fy");
  man->CreateNtupleDColumn("22Fz");
  man->CreateNtupleDColumn("22F_dirx");
  man->CreateNtupleDColumn("22F_diry");
  man->CreateNtupleDColumn("22F_dirz");

  man->CreateNtuple("21Bed1", "Track ntuple"); //3
  man->CreateNtupleDColumn("21Bed1x");
  man->CreateNtupleDColumn("21Bed1y");
  man->CreateNtupleDColumn("21Bed1z");
  man->CreateNtupleDColumn("21Bed1_dirx");
  man->CreateNtupleDColumn("21Bed1_diry");
  man->CreateNtupleDColumn("21Bed1_dirz");

  man->CreateNtuple("21Bed2", "Track ntuple"); //4
  man->CreateNtupleDColumn("21Bed2x");
  man->CreateNtupleDColumn("21Bed2y");
  man->CreateNtupleDColumn("21Bed2z");
  man->CreateNtupleDColumn("21Bed2_dirx");
  man->CreateNtupleDColumn("21Bed2_diry");
  man->CreateNtupleDColumn("21Bed2_dirz");

  man->CreateNtuple("21Bed3", "Track ntuple"); //5
  man->CreateNtupleDColumn("21Bed3x");
  man->CreateNtupleDColumn("21Bed3y");
  man->CreateNtupleDColumn("21Bed3z");
  man->CreateNtupleDColumn("21Bed3_dirx");
  man->CreateNtupleDColumn("21Bed3_diry");
  man->CreateNtupleDColumn("21Bed3_dirz");

  man->CreateNtuple("21Bed4", "Track ntuple"); //6
  man->CreateNtupleDColumn("21Bed4x");
  man->CreateNtupleDColumn("21Bed4y");
  man->CreateNtupleDColumn("21Bed4z");
  man->CreateNtupleDColumn("21Bed4_dirx");
  man->CreateNtupleDColumn("21Bed4_diry");
  man->CreateNtupleDColumn("21Bed4_dirz");

  man->CreateNtuple("21Living", "Track ntuple"); //7
  man->CreateNtupleDColumn("21Livingx");
  man->CreateNtupleDColumn("21Livingy");
  man->CreateNtupleDColumn("21Livingz");
  man->CreateNtupleDColumn("21Living_dirx");
  man->CreateNtupleDColumn("21Living_diry");
  man->CreateNtupleDColumn("21Living_dirz");

  man->CreateNtuple("22Bed1", "Track ntuple"); //8
  man->CreateNtupleDColumn("22Bed1x");
  man->CreateNtupleDColumn("22Bed1y");
  man->CreateNtupleDColumn("22Bed1z");
  man->CreateNtupleDColumn("22Bed1_dirx");
  man->CreateNtupleDColumn("22Bed1_diry");
  man->CreateNtupleDColumn("22Bed1_dirz");

  man->CreateNtuple("22Bed2", "Track ntuple"); //9
  man->CreateNtupleDColumn("22Bed2x");
  man->CreateNtupleDColumn("22Bed2y");
  man->CreateNtupleDColumn("22Bed2z");
  man->CreateNtupleDColumn("22Bed2_dirx");
  man->CreateNtupleDColumn("22Bed2_diry");
  man->CreateNtupleDColumn("22Bed2_dirz");

  man->CreateNtuple("22Bed3", "Track ntuple"); //10
  man->CreateNtupleDColumn("22Bed3x");
  man->CreateNtupleDColumn("22Bed3y");
  man->CreateNtupleDColumn("22Bed3z");
  man->CreateNtupleDColumn("22Bed3_dirx");
  man->CreateNtupleDColumn("22Bed3_diry");
  man->CreateNtupleDColumn("22Bed3_dirz");

  man->CreateNtuple("22Bed4", "Track ntuple"); //11
  man->CreateNtupleDColumn("22Bed4x");
  man->CreateNtupleDColumn("22Bed4y");
  man->CreateNtupleDColumn("22Bed4z");
  man->CreateNtupleDColumn("22Bed4_dirx");
  man->CreateNtupleDColumn("22Bed4_diry");
  man->CreateNtupleDColumn("22Bed4_dirz");

  man->CreateNtuple("22Living", "Track ntuple"); //12
  man->CreateNtupleDColumn("22Livingx");
  man->CreateNtupleDColumn("22Livingy");
  man->CreateNtupleDColumn("22Livingz");
  man->CreateNtupleDColumn("22Living_dirx");
  man->CreateNtupleDColumn("22Living_diry");
  man->CreateNtupleDColumn("22Living_dirz");

  man->CreateNtuple("23Floor", "Track ntuple"); //13
  man->CreateNtupleDColumn("23Fx");
  man->CreateNtupleDColumn("23Fy");
  man->CreateNtupleDColumn("23Fz");
  man->CreateNtupleDColumn("23F_dirx");
  man->CreateNtupleDColumn("23F_diry");
  man->CreateNtupleDColumn("23F_dirz");


  man->CreateNtuple("30Floor", "Track ntuple"); //14
  man->CreateNtupleDColumn("30Fx");
  man->CreateNtupleDColumn("30Fy");
  man->CreateNtupleDColumn("30Fz");
  man->CreateNtupleDColumn("30F_dirx");
  man->CreateNtupleDColumn("30F_diry");
  man->CreateNtupleDColumn("30F_dirz");

  man->CreateNtuple("23Bed1", "Track ntuple"); //15
  man->CreateNtupleDColumn("23Bed1x");
  man->CreateNtupleDColumn("23Bed1y");
  man->CreateNtupleDColumn("23Bed1z");
  man->CreateNtupleDColumn("23Bed1_dirx");
  man->CreateNtupleDColumn("23Bed1_diry");
  man->CreateNtupleDColumn("23Bed1_dirz");

  man->CreateNtuple("23Bed2", "Track ntuple"); //16
  man->CreateNtupleDColumn("23Bed2x");
  man->CreateNtupleDColumn("23Bed2y");
  man->CreateNtupleDColumn("23Bed2z");
  man->CreateNtupleDColumn("23Bed2_dirx");
  man->CreateNtupleDColumn("23Bed2_diry");
  man->CreateNtupleDColumn("23Bed2_dirz");

  man->CreateNtuple("23Bed3", "Track ntuple"); //17
  man->CreateNtupleDColumn("23Bed3x");
  man->CreateNtupleDColumn("23Bed3y");
  man->CreateNtupleDColumn("23Bed3z");
  man->CreateNtupleDColumn("23Bed3_dirx");
  man->CreateNtupleDColumn("23Bed3_diry");
  man->CreateNtupleDColumn("23Bed3_dirz");

  man->CreateNtuple("23Bed4", "Track ntuple"); //18
  man->CreateNtupleDColumn("23Bed4x");
  man->CreateNtupleDColumn("23Bed4y");
  man->CreateNtupleDColumn("23Bed4z");
  man->CreateNtupleDColumn("23Bed4_dirx");
  man->CreateNtupleDColumn("23Bed4_diry");
  man->CreateNtupleDColumn("23Bed4_dirz");

  man->CreateNtuple("23Living", "Track ntuple"); //19
  man->CreateNtupleDColumn("23Livingx");
  man->CreateNtupleDColumn("23Livingy");
  man->CreateNtupleDColumn("23Livingz");
  man->CreateNtupleDColumn("23Living_dirx");
  man->CreateNtupleDColumn("23Living_diry");
  man->CreateNtupleDColumn("23Living_dirz");

  man->CreateNtuple("30Bed1", "Track ntuple"); //20
  man->CreateNtupleDColumn("30Bed1x");
  man->CreateNtupleDColumn("30Bed1y");
  man->CreateNtupleDColumn("30Bed1z");
  man->CreateNtupleDColumn("30Bed1_dirx");
  man->CreateNtupleDColumn("30Bed1_diry");
  man->CreateNtupleDColumn("30Bed1_dirz");

  man->CreateNtuple("30Bed2", "Track ntuple"); //21
  man->CreateNtupleDColumn("30Bed2x");
  man->CreateNtupleDColumn("30Bed2y");
  man->CreateNtupleDColumn("30Bed2z");
  man->CreateNtupleDColumn("30Bed2_dirx");
  man->CreateNtupleDColumn("30Bed2_diry");
  man->CreateNtupleDColumn("30Bed2_dirz");

  man->CreateNtuple("30Bed3", "Track ntuple"); //22
  man->CreateNtupleDColumn("30Bed3x");
  man->CreateNtupleDColumn("30Bed3y");
  man->CreateNtupleDColumn("30Bed3z");
  man->CreateNtupleDColumn("30Bed3_dirx");
  man->CreateNtupleDColumn("30Bed3_diry");
  man->CreateNtupleDColumn("30Bed3_dirz");

  man->CreateNtuple("30Bed4", "Track ntuple"); //23
  man->CreateNtupleDColumn("30Bed4x");
  man->CreateNtupleDColumn("30Bed4y");
  man->CreateNtupleDColumn("30Bed4z");
  man->CreateNtupleDColumn("30Bed4_dirx");
  man->CreateNtupleDColumn("30Bed4_diry");
  man->CreateNtupleDColumn("30Bed4_dirz");

  man->CreateNtuple("30Living", "Track ntuple"); //24
  man->CreateNtupleDColumn("30Livingx");
  man->CreateNtupleDColumn("30Livingy");
  man->CreateNtupleDColumn("30Livingz");
  man->CreateNtupleDColumn("30Living_dirx");
  man->CreateNtupleDColumn("30Living_diry");
  man->CreateNtupleDColumn("30Living_dirz");

  man->CreateNtuple("25Floor", "Track ntuple"); //25
  man->CreateNtupleDColumn("25Fx");
  man->CreateNtupleDColumn("25Fy");
  man->CreateNtupleDColumn("25Fz");
  man->CreateNtupleDColumn("25F_dirx");
  man->CreateNtupleDColumn("25F_diry");
  man->CreateNtupleDColumn("25F_dirz");


  man->CreateNtuple("26Floor", "Track ntuple"); //26
  man->CreateNtupleDColumn("26Fx");
  man->CreateNtupleDColumn("26Fy");
  man->CreateNtupleDColumn("26Fz");
  man->CreateNtupleDColumn("26F_dirx");
  man->CreateNtupleDColumn("26F_diry");
  man->CreateNtupleDColumn("26F_dirz");

  man->CreateNtuple("25Bed1", "Track ntuple"); //27
  man->CreateNtupleDColumn("25Bed1x");
  man->CreateNtupleDColumn("25Bed1y");
  man->CreateNtupleDColumn("25Bed1z");
  man->CreateNtupleDColumn("25Bed1_dirx");
  man->CreateNtupleDColumn("25Bed1_diry");
  man->CreateNtupleDColumn("25Bed1_dirz");

  man->CreateNtuple("25Bed2", "Track ntuple"); //28
  man->CreateNtupleDColumn("25Bed2x");
  man->CreateNtupleDColumn("25Bed2y");
  man->CreateNtupleDColumn("25Bed2z");
  man->CreateNtupleDColumn("25Bed2_dirx");
  man->CreateNtupleDColumn("25Bed2_diry");
  man->CreateNtupleDColumn("25Bed2_dirz");

  man->CreateNtuple("25Bed3", "Track ntuple"); //29
  man->CreateNtupleDColumn("25Bed3x");
  man->CreateNtupleDColumn("25Bed3y");
  man->CreateNtupleDColumn("25Bed3z");
  man->CreateNtupleDColumn("25Bed3_dirx");
  man->CreateNtupleDColumn("25Bed3_diry");
  man->CreateNtupleDColumn("25Bed3_dirz");

  man->CreateNtuple("25Bed4", "Track ntuple"); //30
  man->CreateNtupleDColumn("25Bed4x");
  man->CreateNtupleDColumn("25Bed4y");
  man->CreateNtupleDColumn("25Bed4z");
  man->CreateNtupleDColumn("25Bed4_dirx");
  man->CreateNtupleDColumn("25Bed4_diry");
  man->CreateNtupleDColumn("25Bed4_dirz");

  man->CreateNtuple("25Living", "Track ntuple"); //31
  man->CreateNtupleDColumn("25Livingx");
  man->CreateNtupleDColumn("25Livingy");
  man->CreateNtupleDColumn("25Livingz");
  man->CreateNtupleDColumn("25Living_dirx");
  man->CreateNtupleDColumn("25Living_diry");
  man->CreateNtupleDColumn("25Living_dirz");

  man->CreateNtuple("26Bed1", "Track ntuple"); //32
  man->CreateNtupleDColumn("26Bed1x");
  man->CreateNtupleDColumn("26Bed1y");
  man->CreateNtupleDColumn("26Bed1z");
  man->CreateNtupleDColumn("26Bed1_dirx");
  man->CreateNtupleDColumn("26Bed1_diry");
  man->CreateNtupleDColumn("26Bed1_dirz");

  man->CreateNtuple("26Bed2", "Track ntuple"); //33
  man->CreateNtupleDColumn("26Bed2x");
  man->CreateNtupleDColumn("26Bed2y");
  man->CreateNtupleDColumn("26Bed2z");
  man->CreateNtupleDColumn("26Bed2_dirx");
  man->CreateNtupleDColumn("26Bed2_diry");
  man->CreateNtupleDColumn("26Bed2_dirz");

  man->CreateNtuple("26Bed3", "Track ntuple"); //34
  man->CreateNtupleDColumn("26Bed3x");
  man->CreateNtupleDColumn("26Bed3y");
  man->CreateNtupleDColumn("26Bed3z");
  man->CreateNtupleDColumn("26Bed3_dirx");
  man->CreateNtupleDColumn("26Bed3_diry");
  man->CreateNtupleDColumn("26Bed3_dirz");

  man->CreateNtuple("26Bed4", "Track ntuple"); //35
  man->CreateNtupleDColumn("26Bed4x");
  man->CreateNtupleDColumn("26Bed4y");
  man->CreateNtupleDColumn("26Bed4z");
  man->CreateNtupleDColumn("26Bed4_dirx");
  man->CreateNtupleDColumn("26Bed4_diry");
  man->CreateNtupleDColumn("26Bed4_dirz");

  man->CreateNtuple("26Living", "Track ntuple"); //36
  man->CreateNtupleDColumn("26Livingx");
  man->CreateNtupleDColumn("26Livingy");
  man->CreateNtupleDColumn("26Livingz");
  man->CreateNtupleDColumn("26Living_dirx");
  man->CreateNtupleDColumn("26Living_diry");
  man->CreateNtupleDColumn("26Living_dirz");

  man->CreateNtuple("27Floor", "Track ntuple"); //37
  man->CreateNtupleDColumn("27Fx");
  man->CreateNtupleDColumn("27Fy");
  man->CreateNtupleDColumn("27Fz");
  man->CreateNtupleDColumn("27F_dirx");
  man->CreateNtupleDColumn("27F_diry");
  man->CreateNtupleDColumn("27F_dirz");


  man->CreateNtuple("28Floor", "Track ntuple"); //38
  man->CreateNtupleDColumn("28Fx");
  man->CreateNtupleDColumn("28Fy");
  man->CreateNtupleDColumn("28Fz");
  man->CreateNtupleDColumn("28F_dirx");
  man->CreateNtupleDColumn("28F_diry");
  man->CreateNtupleDColumn("28F_dirz");

  man->CreateNtuple("27Bed1", "Track ntuple"); //39
  man->CreateNtupleDColumn("27Bed1x");
  man->CreateNtupleDColumn("27Bed1y");
  man->CreateNtupleDColumn("27Bed1z");
  man->CreateNtupleDColumn("27Bed1_dirx");
  man->CreateNtupleDColumn("27Bed1_diry");
  man->CreateNtupleDColumn("27Bed1_dirz");

  man->CreateNtuple("27Bed2", "Track ntuple"); //40
  man->CreateNtupleDColumn("27Bed2x");
  man->CreateNtupleDColumn("27Bed2y");
  man->CreateNtupleDColumn("27Bed2z");
  man->CreateNtupleDColumn("27Bed2_dirx");
  man->CreateNtupleDColumn("27Bed2_diry");
  man->CreateNtupleDColumn("27Bed2_dirz");

  man->CreateNtuple("27Bed3", "Track ntuple"); //41
  man->CreateNtupleDColumn("27Bed3x");
  man->CreateNtupleDColumn("27Bed3y");
  man->CreateNtupleDColumn("27Bed3z");
  man->CreateNtupleDColumn("27Bed3_dirx");
  man->CreateNtupleDColumn("27Bed3_diry");
  man->CreateNtupleDColumn("27Bed3_dirz");

  man->CreateNtuple("27Living", "Track ntuple"); //42
  man->CreateNtupleDColumn("27Livingx");
  man->CreateNtupleDColumn("27Livingy");
  man->CreateNtupleDColumn("27Livingz");
  man->CreateNtupleDColumn("27Living_dirx");
  man->CreateNtupleDColumn("27Living_diry");
  man->CreateNtupleDColumn("27Living_dirz");

  man->CreateNtuple("28Bed1", "Track ntuple"); //43
  man->CreateNtupleDColumn("28Bed1x");
  man->CreateNtupleDColumn("28Bed1y");
  man->CreateNtupleDColumn("28Bed1z");
  man->CreateNtupleDColumn("28Bed1_dirx");
  man->CreateNtupleDColumn("28Bed1_diry");
  man->CreateNtupleDColumn("28Bed1_dirz");

  man->CreateNtuple("28Bed2", "Track ntuple"); //44
  man->CreateNtupleDColumn("28Bed2x");
  man->CreateNtupleDColumn("28Bed2y");
  man->CreateNtupleDColumn("28Bed2z");
  man->CreateNtupleDColumn("28Bed2_dirx");
  man->CreateNtupleDColumn("28Bed2_diry");
  man->CreateNtupleDColumn("28Bed2_dirz");

  man->CreateNtuple("28Bed3", "Track ntuple"); //45
  man->CreateNtupleDColumn("28Bed3x");
  man->CreateNtupleDColumn("28Bed3y");
  man->CreateNtupleDColumn("28Bed3z");
  man->CreateNtupleDColumn("28Bed3_dirx");
  man->CreateNtupleDColumn("28Bed3_diry");
  man->CreateNtupleDColumn("28Bed3_dirz");

  man->CreateNtuple("28Living", "Track ntuple"); //46
  man->CreateNtupleDColumn("28Livingx");
  man->CreateNtupleDColumn("28Livingy");
  man->CreateNtupleDColumn("28Livingz");
  man->CreateNtupleDColumn("28Living_dirx");
  man->CreateNtupleDColumn("28Living_diry");
  man->CreateNtupleDColumn("28Living_dirz");

  man->CreateNtuple("29Bed1", "Track ntuple"); //47
  man->CreateNtupleDColumn("29Bed1x");
  man->CreateNtupleDColumn("29Bed1y");
  man->CreateNtupleDColumn("29Bed1z");
  man->CreateNtupleDColumn("29Bed1_dirx");
  man->CreateNtupleDColumn("29Bed1_diry");
  man->CreateNtupleDColumn("29Bed1_dirz");

  man->CreateNtuple("29Bed2", "Track ntuple"); //48
  man->CreateNtupleDColumn("29Bed2x");
  man->CreateNtupleDColumn("29Bed2y");
  man->CreateNtupleDColumn("29Bed2z");
  man->CreateNtupleDColumn("29Bed2_dirx");
  man->CreateNtupleDColumn("29Bed2_diry");
  man->CreateNtupleDColumn("29Bed2_dirz");

  man->CreateNtuple("29Bed3", "Track ntuple"); //49
  man->CreateNtupleDColumn("29Bed3x");
  man->CreateNtupleDColumn("29Bed3y");
  man->CreateNtupleDColumn("29Bed3z");
  man->CreateNtupleDColumn("29Bed3_dirx");
  man->CreateNtupleDColumn("29Bed3_diry");
  man->CreateNtupleDColumn("29Bed3_dirz");

  man->CreateNtuple("29Living", "Track ntuple"); //50
  man->CreateNtupleDColumn("29Livingx");
  man->CreateNtupleDColumn("29Livingy");
  man->CreateNtupleDColumn("29Livingz");
  man->CreateNtupleDColumn("29Living_dirx");
  man->CreateNtupleDColumn("29Living_diry");
  man->CreateNtupleDColumn("29Living_dirz");

  man->CreateNtuple("29Floor", "Track ntuple"); //51
  man->CreateNtupleDColumn("29Fx");
  man->CreateNtupleDColumn("29Fy");
  man->CreateNtupleDColumn("29Fz");
  man->CreateNtupleDColumn("29F_dirx");
  man->CreateNtupleDColumn("29F_diry");
  man->CreateNtupleDColumn("29F_dirz");

  man->FinishNtuple();

}


void ARCAnalysis::finish(G4bool isMaster)
{
  G4AutoLock l(&dataManipulationMutex);
  // Save histograms
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();
  // Complete clean-up
  delete G4AnalysisManager::Instance();

  if (!isMaster)
    return;

  //only master performs these operations
  asciiFile->close();
 
  //Sequential run: output results!
  if (nEnteringTracks->count(-2))
    {
      G4cout << "End of Run summary (sequential)" << G4endl << G4endl;
      G4cout << "Total Entering Detector : " << nEnteringTracks->find(-2)->second  
	     << G4endl;
      G4cout << "Total Entering Detector Energy : " 
	     << (totEnteringEnergy->find(-2)->second)/MeV  
	     << " MeV"
	     << G4endl;
      return;
    }
  

  //MT run: sum results
 

  //MT build, but sequential run  
  if (nEnteringTracks->count(-1))
    {
      G4cout << "End of Run summary (sequential with MT build)" << G4endl << G4endl;
      G4cout << "Total Entering Detector : " << nEnteringTracks->find(-1)->second  
	     << G4endl;
      G4cout << "Total Entering Detector Energy : " 
	     << (totEnteringEnergy->find(-1)->second)/MeV  
	     << " MeV"
	     << G4endl;
      G4cout << "##########################################" << G4endl;
      return;
    }

  G4bool loopAgain = true;
  G4int totEntries = 0;
  G4double totEnergy = 0.;

  G4cout << "##########################################" << G4endl;
  for (size_t i=0; loopAgain; i++)
    {
      //ok, this thread was found
      if (nEnteringTracks->count(i))
	{
	  G4cout << "End of Run summary (thread= " << i << ")" << G4endl;
	  G4int part = nEnteringTracks->find(i)->second;
	  G4cout << "Total Entering Detector : " << part << G4endl;
	  G4double ene = totEnteringEnergy->find(i)->second;
	  G4cout << "Total Entering Detector Energy : " 
		 << ene/MeV  
		 << " MeV" << G4endl;
	  totEntries += part;
	  totEnergy += ene;
	  G4cout << "##########################################" << G4endl;
	}
      else
	loopAgain = false;
    }
  //Report global numbers
  if (totEntries)
    {
      G4cout << "End of Run summary (MT) global" << G4endl << G4endl;
      G4cout << "Total Entering Detector : " << totEntries << G4endl;
      G4cout << "Total Entering Detector Energy : " 
	     << totEnergy/MeV  
	     << " MeV" << G4endl;
      G4cout << "##########################################" << G4endl;
    }
}

void ARCAnalysis::analyseStepping(const G4Track& track, G4bool entering, G4String WhereName)
{
  G4AutoLock l(&dataManipulationMutex);
  eKin = track.GetKineticEnergy()/eV;
  G4ThreeVector pos = track.GetPosition()/cm;
  y = pos.y();
  z = pos.z();
  x = pos.x();
  G4ThreeVector dir = track.GetMomentumDirection();
  dirX = dir.x();
  dirY = dir.y();
  dirZ = dir.z();
  G4int tupleNum = 0;

  // Fill histograms
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->FillH1(1,eKin);
  man->FillH2(1,x,y);
/*
      man->FillNtupleDColumn(0,0,eKin);
      man->FillNtupleDColumn(0,1,x);
      man->FillNtupleDColumn(0,2,y);
      man->FillNtupleDColumn(0,3,z);
      man->FillNtupleDColumn(0,4,dirX);
      man->FillNtupleDColumn(0,5,dirY);
      man->FillNtupleDColumn(0,6,dirZ);
      man->AddNtupleRow(0); 
*/

      //21 Floor

      if(WhereName == "B21Bed1FloorPhysical")
      {
          tupleNum = 1;
      }
      if(WhereName == "B21Bed2FloorPhysical")
      {
          tupleNum = 1;
      }
      if(WhereName == "B21Bed3FloorPhysical")
      {
          tupleNum = 1;
      }
      if(WhereName == "B21Bed4FloorPhysical")
      {
          tupleNum = 1;
      }
      if(WhereName == "B21LivingFloorPhysical")
      {
          tupleNum = 1;
      }

      //22 Floor
      if(WhereName == "B22Bed1FloorPhysical")
      {
          tupleNum = 2;
      }
      if(WhereName == "B22Bed2FloorPhysical")
      {
          tupleNum = 2;
      }
      if(WhereName == "B22Bed3FloorPhysical")
      {
          tupleNum = 2;
      }
      if(WhereName == "B22Bed4FloorPhysical")
      {
          tupleNum = 2;
      }
      if(WhereName == "B22LivingFloorPhysical")
      {
          tupleNum = 2;
      }

      //23 Floor
      if(WhereName == "B23Bed1FloorPhysical")
      {
          tupleNum = 13;
      }
      if(WhereName == "B23Bed2FloorPhysical")
      {
          tupleNum = 13;
      }
      if(WhereName == "B23Bed3FloorPhysical")
      {
          tupleNum = 13;
      }
      if(WhereName == "B23Bed4FloorPhysical")
      {
          tupleNum = 13;
      }
      if(WhereName == "B23LivingFloorPhysical")
      {
          tupleNum = 13;
      }

      //25 Floor
      if(WhereName == "B25Bed1FloorPhysical")
      {
          tupleNum = 25;
      }
      if(WhereName == "B25Bed2FloorPhysical")
      {
          tupleNum = 25;
      }
      if(WhereName == "B25Bed3FloorPhysical")
      {
          tupleNum = 25;
      }
      if(WhereName == "B25Bed4FloorPhysical")
      {
          tupleNum = 25;
      }
      if(WhereName == "B25LivingFloorPhysical")
      {
          tupleNum = 25;
      }


      //26 Floor
      if(WhereName == "B26Bed1FloorPhysical")
      {
          tupleNum = 26;
      }
      if(WhereName == "B26Bed2FloorPhysical")
      {
          tupleNum = 26;
      }
      if(WhereName == "B26Bed3FloorPhysical")
      {
          tupleNum = 26;
      }
      if(WhereName == "B26Bed4FloorPhysical")
      {
          tupleNum = 26;
      }
      if(WhereName == "B26LivingFloorPhysical")
      {
          tupleNum = 26;
      }


      //27 Floor
      if(WhereName == "B27Bed1FloorPhysical")
      {
          tupleNum = 37;
      }
      if(WhereName == "B27Bed2FloorPhysical")
      {
          tupleNum = 37;
      }
      if(WhereName == "B27Bed3FloorPhysical")
      {
          tupleNum = 37;
      }
      if(WhereName == "B27Bed4FloorPhysical")
      {
          tupleNum = 37;
      }
      if(WhereName == "B27LivingFloorPhysical")
      {
          tupleNum = 37;
      }


      //28 Floor
      if(WhereName == "B28Bed1FloorPhysical")
      {
          tupleNum = 38;
      }
      if(WhereName == "B28Bed2FloorPhysical")
      {
          tupleNum = 38;
      }
      if(WhereName == "B28Bed3FloorPhysical")
      {
          tupleNum = 38;
      }
      if(WhereName == "B28Bed4FloorPhysical")
      {
          tupleNum = 38;
      }
      if(WhereName == "B28LivingFloorPhysical")
      {
          tupleNum = 38;
      }

      
      //29 Floor
      if(WhereName == "B29Bed1FloorPhysical")
      {
          tupleNum = 51;
      }
      if(WhereName == "B29Bed2FloorPhysical")
      {
          tupleNum = 51;
      }
      if(WhereName == "B29Bed3FloorPhysical")
      {
          tupleNum = 51;
      }
      if(WhereName == "B29Bed4FloorPhysical")
      {
          tupleNum = 51;
      }
      if(WhereName == "B29LivingFloorPhysical")
      {
          tupleNum = 51;
      }

      //30 Floor
      if(WhereName == "B30Bed1FloorPhysical")
      {
          tupleNum = 14;
      }
      if(WhereName == "B30Bed2FloorPhysical")
      {
          tupleNum = 14;
      }
      if(WhereName == "B30Bed3FloorPhysical")
      {
          tupleNum = 14;
      }
      if(WhereName == "B30Bed4FloorPhysical")
      {
          tupleNum = 14;
      }
      if(WhereName == "B30LivingFloorPhysical")
      {
          tupleNum = 14;
      }





      
      //21 Room
      if(WhereName == "B21Bed1RoomPhysical")
      {
          tupleNum = 3;
      }
      if(WhereName == "B21Bed2RoomPhysical")
      {
          tupleNum = 4;
      }
      if(WhereName == "B21Bed3RoomPhysical")
      {
          tupleNum = 5;
      }
      if(WhereName == "B21Bed4RoomPhysical")
      {
          tupleNum = 6;
      }
      if(WhereName == "B21LivingRoomPhysical")
      {
          tupleNum = 7;
      }


      if(WhereName == "B22Bed1RoomPhysical")
      {
          tupleNum = 8;
      }
      if(WhereName == "B22Bed2RoomPhysical")
      {
          tupleNum = 9;
      }
      if(WhereName == "B22Bed3RoomPhysical")
      {
          tupleNum = 10;
      }
      if(WhereName == "B22Bed4RoomPhysical")
      {
          tupleNum = 11;
      }
      if(WhereName == "B22LivingRoomPhysical")
      {
          tupleNum = 12;
      }

      //22 Room

      if(WhereName == "B22Bed1FloorPhysical")
      {
          tupleNum = 2;
      }
      if(WhereName == "B22Bed2FloorPhysical")
      {
          tupleNum = 2;
      }
      if(WhereName == "B22Bed3FloorPhysical")
      {
          tupleNum = 2;
      }
      if(WhereName == "B22Bed4FloorPhysical")
      {
          tupleNum = 2;
      }
      if(WhereName == "B22LivingFloorPhysical")
      {
          tupleNum = 2;
      }

      if(WhereName == "B22Bed1RoomPhysical")
      {
          tupleNum = 8;
      }
      if(WhereName == "B22Bed2RoomPhysical")
      {
          tupleNum = 9;
      }
      if(WhereName == "B22Bed3RoomPhysical")
      {
          tupleNum = 10;
      }
      if(WhereName == "B22Bed4RoomPhysical")
      {
          tupleNum = 11;
      }
      if(WhereName == "B22LivingRoomPhysical")
      {
          tupleNum = 12;
      }

      //23 Room

      if(WhereName == "B23Bed1RoomPhysical")
      {
          tupleNum = 15;
      }
      if(WhereName == "B23Bed2RoomPhysical")
      {
          tupleNum = 16;
      }
      if(WhereName == "B23Bed3RoomPhysical")
      {
          tupleNum = 17;
      }
      if(WhereName == "B23Bed4RoomPhysical")
      {
          tupleNum = 18;
      }
      if(WhereName == "B23LivingRoomPhysical")
      {
          tupleNum = 19;
      }

      //25 Room

      if(WhereName == "B25Bed1RoomPhysical")
      {
          tupleNum = 27;
      }
      if(WhereName == "B25Bed2RoomPhysical")
      {
          tupleNum = 28;
      }
      if(WhereName == "B25Bed3RoomPhysical")
      {
          tupleNum = 29;
      }
      if(WhereName == "B25Bed4RoomPhysical")
      {
          tupleNum = 30;
      }
      if(WhereName == "B25LivingRoomPhysical")
      {
          tupleNum = 31;
      }

      //26 Room

      if(WhereName == "B26Bed1RoomPhysical")
      {
          tupleNum = 32;
      }
      if(WhereName == "B26Bed2RoomPhysical")
      {
          tupleNum = 33;
      }
      if(WhereName == "B26Bed3RoomPhysical")
      {
          tupleNum = 34;
      }
      if(WhereName == "B26Bed4RoomPhysical")
      {
          tupleNum = 35;
      }
      if(WhereName == "B26LivingRoomPhysical")
      {
          tupleNum = 36;
      }

      //27 Room

      if(WhereName == "B27Bed1RoomPhysical")
      {
          tupleNum = 39;
      }
      if(WhereName == "B27Bed2RoomPhysical")
      {
          tupleNum = 40;
      }
      if(WhereName == "B27Bed3RoomPhysical")
      {
          tupleNum = 41;
      }
      if(WhereName == "B27LivingRoomPhysical")
      {
          tupleNum = 42;
      }

      //28 Room

      if(WhereName == "B28Bed1RoomPhysical")
      {
          tupleNum = 43;
      }
      if(WhereName == "B28Bed2RoomPhysical")
      {
          tupleNum = 44;
      }
      if(WhereName == "B28Bed3RoomPhysical")
      {
          tupleNum = 45;
      }
      if(WhereName == "B28LivingRoomPhysical")
      {
          tupleNum = 46;
      }

      //29 Room

      if(WhereName == "B29Bed1RoomPhysical")
      {
          tupleNum = 47;
      }
      if(WhereName == "B29Bed2RoomPhysical")
      {
          tupleNum = 48;
      }
      if(WhereName == "B29Bed3RoomPhysical")
      {
          tupleNum = 49;
      }
      if(WhereName == "B29LivingRoomPhysical")
      {
          tupleNum = 50;
      }

      //30 Room

      if(WhereName == "B30Bed1RoomPhysical")
      {
          tupleNum = 20;
      }
      if(WhereName == "B30Bed2RoomPhysical")
      {
          tupleNum = 21;
      }
      if(WhereName == "B30Bed3RoomPhysical")
      {
          tupleNum = 22;
      }
      if(WhereName == "B30Bed4RoomPhysical")
      {
          tupleNum = 23;
      }
      if(WhereName == "B30LivingRoomPhysical")
      {
          tupleNum = 24;
      }























  // Fill histograms and ntuple, tracks entering the detector
  if (entering ) 
  {



  
      // Fill and plot histograms

      man->FillNtupleDColumn(tupleNum,0,x);
      man->FillNtupleDColumn(tupleNum,1,y);
      man->FillNtupleDColumn(tupleNum,2,z);
      man->FillNtupleDColumn(tupleNum,3,dirX);
      man->FillNtupleDColumn(tupleNum,4,dirY);
      man->FillNtupleDColumn(tupleNum,5,dirZ);

      man->AddNtupleRow(tupleNum); 
  }


  // Write to file
  if (entering) {
    if(asciiFile->is_open()) {
      (*asciiFile) << std::setiosflags(std::ios::fixed)
		   << std::setprecision(3)
		   << std::setiosflags(std::ios::right)
		   << std::setw(10);
      (*asciiFile) << eKin;
      (*asciiFile) << std::setiosflags(std::ios::fixed)
		   << std::setprecision(3)
		   << std::setiosflags(std::ios::right)
		   << std::setw(10);
      (*asciiFile) << x;
      (*asciiFile) << std::setiosflags(std::ios::fixed)
		   << std::setprecision(3)
		   << std::setiosflags(std::ios::right)
		   << std::setw(10);
      (*asciiFile) << y;
      (*asciiFile) << std::setiosflags(std::ios::fixed)
		   << std::setprecision(3)
		   << std::setiosflags(std::ios::right)
		   << std::setw(10);
      (*asciiFile) << z
		   << G4endl;
    }
  }
}

void ARCAnalysis::Update(G4double energy,G4int threadID)
{
  G4AutoLock l(&dataManipulationMutex);
  //It already exists: increase the counter
  if (nEnteringTracks->count(threadID))
    {
      (nEnteringTracks->find(threadID)->second)++;     
    }
  else //enter a new one
    {
      G4int tracks = 1;
      nEnteringTracks->insert(std::make_pair(threadID,tracks));
    }

  //It already exists: increase the counter
  if (totEnteringEnergy->count(threadID))
    (totEnteringEnergy->find(threadID)->second) += energy;
  else //enter a new one    
    totEnteringEnergy->insert(std::make_pair(threadID,energy));
    
  return;
}

