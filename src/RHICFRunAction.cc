#include "RHICFRunAction.hh"
#include "RHICFDetectorConstruction.hh"
#include "FileManager.hh"
#include "Seeder.hh"
////////////////////////////////////////////////////////////////////////////////
#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "g4root.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4String.hh"
#include <ctime>
#include <unistd.h>
#include <string>
#include <cstdlib>
////////////////////////////////////////////////////////////////////////////////





RHICFRunAction::RHICFRunAction(RHICFEventAction* eventAction): G4UserRunAction(), fEventAction(eventAction)
{
    FileManager::GetInstance()->PrepareSavingDirectory();
    FileManager::GetInstance()->SetFileName("LEAKGESTUDYNEUTRONTL250GeV");
    G4AnalysisManager::Instance()->SetNtupleMerging(true);
    G4AnalysisManager::Instance()->SetVerboseLevel(0);
    G4AnalysisManager::Instance()->SetFileName(FileManager::GetInstance()->GetPathFortmp()+"/"+FileManager::GetInstance()->GetFileName()+".root");
    //Junsang****G4AnalysisManager::Instance()->SetFileName("test.root");
    NtupleForARM1();
    NtupleForSTARZDC();

}

RHICFRunAction::~RHICFRunAction()
{
    delete G4AnalysisManager::Instance();
}


void RHICFRunAction::BeginOfRunAction(const G4Run* run)
{
    Seeder* fSeeder = new Seeder;
    G4long tmpseed = (long)fSeeder->GetSeedForG4();
    G4Random::setTheSeeds(&tmpseed);
    G4AnalysisManager::Instance()->OpenFile();
}



void RHICFRunAction::EndOfRunAction(const G4Run* run)
{
    G4AnalysisManager::Instance()->Write();
    G4AnalysisManager::Instance()->CloseFile();
}


void RHICFRunAction::NtupleForARM1()
{
    G4AnalysisManager::Instance()->CreateNtuple("ARM1PL", "GSOPL");
    //[0-15]: DE FOR TS GSO PLATE 
    //[16-31]: DE FOR TL GSO PLATE 
    //[32-47]: NOP FOR TS GSO PLATE 
    //[48-63]: NOP FOR TL GSO PLATE
    //64 : TOTAL DE ENERGY[GeV]
    //65 : RUN NUMBER
    //66 : EVENT NUMBER
    for (int i = 0; i < 16; i++) 
    {
        G4AnalysisManager::Instance()->CreateNtupleDColumn(0,"TSGSOPLDE"+std::to_string(i));
    }
    for (int i = 0; i < 16; i++) 
    {
        G4AnalysisManager::Instance()->CreateNtupleDColumn(0,"TLGSOPLDE"+std::to_string(i));
    }
    for (int i = 0; i < 16; i++) 
    {
        G4AnalysisManager::Instance()->CreateNtupleIColumn(0,"TSGSOPLNOP"+std::to_string(i));
    }
    for (int i = 0; i < 16; i++) 
    {
        G4AnalysisManager::Instance()->CreateNtupleIColumn(0,"TLGSOPLNOP"+std::to_string(i));
    }
    G4AnalysisManager::Instance()->CreateNtupleDColumn(0, "TotalEnergy");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(0,"RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(0,"EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(0);

    G4AnalysisManager::Instance()->CreateNtuple("ARM1BAR", "GSOBAR");
    //[0-79] : DE FOR TS GSO BAR LEFT DIRECTION * 4 LAYERS
    //[80-239]: DE FOR TL GSO BAR LEFT DIRECTION * 4 LAYERS
    //[240-319]: DE FOR TS GSO BAR RIGHT DIRECTION * 4 LAYERS
    //[320-479]: DE FOR TL GSO BAR RIGHT DIRECTION * 4 LAYERS
    //480 : RUN NUMBER
    //481 : EVENT NUMBER
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<20; j++)
        {
            G4AnalysisManager::Instance()->CreateNtupleDColumn(1, "TSGSOBLDE"+std::to_string(i)+std::to_string(j));
        }
    }
    for (int i = 0; i < 4; i++) 
    {
        for(int j=0; j<40; j++)
        {
            G4AnalysisManager::Instance()->CreateNtupleDColumn(1, "TLGSOBLDE"+std::to_string(i)+std::to_string(j));
        }
    }
    for (int i = 0; i < 4; i++) 
    {
        
        for(int j=0; j<20; j++)
        {
            G4AnalysisManager::Instance()->CreateNtupleDColumn(1, "TSGSOBRDE"+std::to_string(i)+std::to_string(j));
        }
    }
    for (int i = 0; i < 4; i++) 
    {
        for(int j=0; j<40; j++)
        {
            G4AnalysisManager::Instance()->CreateNtupleDColumn(1, "TLGSOBRDE"+std::to_string(i)+std::to_string(j));
        }
    }
    for (int i = 0; i < 4; i++) 
    {
        
        for(int j=0; j<20; j++)
        {
            G4AnalysisManager::Instance()->CreateNtupleIColumn(1, "TSGSOBLNOP"+std::to_string(i)+std::to_string(j));
        }
    }
    for (int i = 0; i < 4; i++) 
    {
        for(int j=0; j<40; j++)
        {
            G4AnalysisManager::Instance()->CreateNtupleIColumn(1, "TLGSOBLNOP"+std::to_string(i)+std::to_string(j));
        }
    }
    for (int i = 0; i < 4; i++) 
    {
        
        for(int j=0; j<20; j++)
        {
            G4AnalysisManager::Instance()->CreateNtupleIColumn(1, "TSGSOBRNOP"+std::to_string(i)+std::to_string(j));
        }
    }
    for (int i = 0; i < 4; i++) 
    {
        for(int j=0; j<40; j++)
        {
            G4AnalysisManager::Instance()->CreateNtupleIColumn(1, "TLGSOBRNOP"+std::to_string(i)+std::to_string(j));
        }
    }
    G4AnalysisManager::Instance()->CreateNtupleIColumn(1,"RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(1,"EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(1);

    G4AnalysisManager::Instance()->CreateNtuple("FrontCounterInfo", "DENOP");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(2, "TSFCDE");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(2, "TLFCDE");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(2, "TSFCNOP");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(2, "TLFCNOP");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(2, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(2, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(2);

    G4AnalysisManager::Instance()->CreateNtuple("CenterGhost", "ParticleInformation");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(3, "POSITIONX[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(3, "POSITIONY[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(3, "MOMENTUMX[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(3, "MOMENTUMY[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(3, "MOMENTUMZ[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(3, "KINETICENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(3, "PID[PDG]");
    G4AnalysisManager::Instance()->CreateNtupleSColumn(3, "TYPE");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(3, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(3, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(3);

    G4AnalysisManager::Instance()->CreateNtuple("CenterCircle", "ParticleInformation");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(4, "POSITIONX[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(4, "POSITIONY[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(4, "MOMENTUMX[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(4, "MOMENTUMY[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(4, "MOMENTUMZ[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(4, "KINETICENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(4, "PID[PDG]");
    G4AnalysisManager::Instance()->CreateNtupleSColumn(4, "TYPE");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(4, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(4, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(4);

    G4AnalysisManager::Instance()->CreateNtuple("FrontCounterParticle", "ParticleInformation");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "POSITIONX[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "POSITIONY[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "MOMENTUMX[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "MOMENTUMY[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "MOMENTUMZ[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "KINETICENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "PID[PDG]");
    G4AnalysisManager::Instance()->CreateNtupleSColumn(5, "TYPE");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(5);
}

void RHICFRunAction::NtupleForSTARZDC()
{
    G4AnalysisManager::Instance()->CreateNtuple("ZDCPMMA", "PMMALAYER");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(6, "1stPMMADE[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(6, "2ndPMMADE[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(6, "3rdPMMADE[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(6, "1stPMMANOP");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(6, "2ndPMMANOP");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(6, "3rdPMMANOP");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(6, "1stZDCDE[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(6, "2ndZDCDE[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(6, "3rdZDCDE[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(6, "TotalEnergy");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(6, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(6, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(6);

    G4AnalysisManager::Instance()->CreateNtuple("ZDCSMD", "SMD");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "SMDVDE1[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "SMDVDE2[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "SMDVDE3[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "SMDVDE4[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "SMDVDE5[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "SMDVDE6[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "SMDVDE7[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "SMDHDE1[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "SMDHDE2[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "SMDHDE3[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "SMDHDE4[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "SMDHDE5[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "SMDHDE6[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "SMDHDE7[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "SMDHDE8[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "SMDVNOP1");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "SMDVNOP2");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "SMDVNOP3");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "SMDVNOP4");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "SMDVNOP5");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "SMDVNOP6");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "SMDVNOP7");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "SMDHNOP1");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "SMDHNOP2");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "SMDHNOP3");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "SMDHNOP4");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "SMDHNOP5");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "SMDHNOP6");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "SMDHNOP7");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "SMDHNOP8");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(7);

    G4AnalysisManager::Instance()->CreateNtuple("ZDCGhost", "Particle"); // INFO OF PARTICLES GOTHROUGH ZDCGHOST
    G4AnalysisManager::Instance()->CreateNtupleDColumn(8, "POSITIONX[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(8, "POSITIONY[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(8, "MOMENTUMX[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(8, "MOMENTUMY[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(8, "MOMENTUMZ[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(8, "KINETICENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(8, "PID[PDG]");
    G4AnalysisManager::Instance()->CreateNtupleSColumn(8, "TYPE");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(8, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(8, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(8);
}
