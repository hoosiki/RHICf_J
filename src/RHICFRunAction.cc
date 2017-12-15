#include "RHICFRunAction.hh"
#include "RHICFDetectorConstruction.hh"
#include "FileManager.hh"
#include "Seeder.hh"
////////////////////////////////////////////////////////////////////////////////
#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "g4root.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "G4String.hh"
#include <ctime>
#include <unistd.h>
#include <string>
#include <cstdlib>
////////////////////////////////////////////////////////////////////////////////





RHICFRunAction::RHICFRunAction(RHICFEventAction* eventAction): G4UserRunAction(), fEventAction(eventAction), fRHICFRunActionMessenger(0)
{
    fRHICFRunActionMessenger = new RHICFRunActionMessenger(this);
    FileManager::GetInstance()->PrepareSavingDirectory();
    G4AnalysisManager::Instance()->SetNtupleMerging(true);
    G4AnalysisManager::Instance()->SetVerboseLevel(0);

}

RHICFRunAction::~RHICFRunAction()
{
    delete G4AnalysisManager::Instance();
    delete fRHICFRunActionMessenger;
}


void RHICFRunAction::BeginOfRunAction(const G4Run* run)
{
    Seeder* fSeeder = new Seeder;
    G4int tmpint = fSeeder->GetSeedForG4();
    G4Random::setTheSeed(tmpint);
    G4AnalysisManager::Instance()->SetFileName(FileManager::GetInstance()->GetPathFortmp()+"/"+FileManager::GetInstance()->GetFileName()+".root");
    NtupleForARM1();
    NtupleForSTARZDC();
    NtupleForIP();
    G4AnalysisManager::Instance()->OpenFile();
}



void RHICFRunAction::EndOfRunAction(const G4Run* run)
{
    G4AnalysisManager::Instance()->Write();
    G4AnalysisManager::Instance()->CloseFile();
}

void RHICFRunAction::SetRootFileName(G4String name)
{
    FileManager::GetInstance()->SetFileName(name);
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


    G4AnalysisManager::Instance()->CreateNtuple("WPlate", "ParticleInformation");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(3, "POSITIONX[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(3, "POSITIONY[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(3, "POSITIONZ[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(3, "MOMENTUMX[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(3, "MOMENTUMY[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(3, "MOMENTUMZ[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(3, "ENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleSColumn(3, "NAME");
    G4AnalysisManager::Instance()->CreateNtupleSColumn(3, "ORIGINALPARTICLE");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(3, "OPENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(3, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(3, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(3);

}

void RHICFRunAction::NtupleForSTARZDC()
{
    G4AnalysisManager::Instance()->CreateNtuple("ZDCPMMA", "PMMALAYER");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(4, "1stPMMADE[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(4, "2ndPMMADE[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(4, "3rdPMMADE[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(4, "1stPMMANOP");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(4, "2ndPMMANOP");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(4, "3rdPMMANOP");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(4, "1stZDCDE[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(4, "2ndZDCDE[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(4, "3rdZDCDE[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(4, "TotalEnergy");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(4, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(4, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(4);

    G4AnalysisManager::Instance()->CreateNtuple("ZDCSMD", "SMD");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "SMDVDE1[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "SMDVDE2[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "SMDVDE3[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "SMDVDE4[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "SMDVDE5[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "SMDVDE6[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "SMDVDE7[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "SMDHDE1[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "SMDHDE2[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "SMDHDE3[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "SMDHDE4[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "SMDHDE5[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "SMDHDE6[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "SMDHDE7[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(5, "SMDHDE8[MeV]");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "SMDVNOP1");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "SMDVNOP2");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "SMDVNOP3");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "SMDVNOP4");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "SMDVNOP5");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "SMDVNOP6");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "SMDVNOP7");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "SMDHNOP1");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "SMDHNOP2");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "SMDHNOP3");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "SMDHNOP4");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "SMDHNOP5");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "SMDHNOP6");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "SMDHNOP7");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "SMDHNOP8");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(5, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(5);

    G4AnalysisManager::Instance()->CreateNtuple("ZDCGhost", "Particle"); // INFO OF PARTICLES GOTHROUGH ZDCGHOST
    G4AnalysisManager::Instance()->CreateNtupleDColumn(6, "POSITIONX[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(6, "POSITIONY[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(6, "MOMENTUMX[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(6, "MOMENTUMY[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(6, "MOMENTUMZ[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(6, "ENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleSColumn(6, "NAME");
    G4AnalysisManager::Instance()->CreateNtupleSColumn(6, "ORIGINALPARTICLE");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(6, "OPENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(6, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(6, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(6);
}

void RHICFRunAction::NtupleForIP()
{
    G4AnalysisManager::Instance()->CreateNtuple("IP", "ParticleInformation");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "POSITIONX[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "POSITIONY[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "POSITIONZ[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "MOMENTUMX[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "MOMENTUMY[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "MOMENTUMZ[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(7, "ENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleSColumn(7, "NAME");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(7, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(7);

    G4AnalysisManager::Instance()->CreateNtuple("Ghost", "ParticleInformation");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(8, "POSITIONX[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(8, "POSITIONY[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(8, "MOMENTUMX[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(8, "MOMENTUMY[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(8, "MOMENTUMZ[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(8, "ENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleSColumn(8, "NAME");
    G4AnalysisManager::Instance()->CreateNtupleSColumn(8, "ORIGINALPARTICLE");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(8, "OPENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(8, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(8, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(8);

    G4AnalysisManager::Instance()->CreateNtuple("Circle", "ParticleInformation");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(9, "POSITIONX[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(9, "POSITIONY[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(9, "MOMENTUMX[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(9, "MOMENTUMY[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(9, "MOMENTUMZ[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(9, "ENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleSColumn(9, "NAME");
    G4AnalysisManager::Instance()->CreateNtupleSColumn(9, "ORIGINALPARTICLE");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(9, "OPENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(9, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(9, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(9);

    G4AnalysisManager::Instance()->CreateNtuple("GhostWhenPion", "ParticleInformation");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(10, "POSITIONX[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(10, "POSITIONY[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(10, "MOMENTUMX[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(10, "MOMENTUMY[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(10, "MOMENTUMZ[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(10, "ENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(10, "PIONMOMENTUMX");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(10, "PIONMOMENTUMY");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(10, "PIONMOMENTUMZ");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(10, "PIONENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(10, "TOWERID");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(10, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(10, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(10);

    G4AnalysisManager::Instance()->CreateNtuple("GhostWhenNeutron", "ParticleInformation");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(11, "POSITIONX[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(11, "POSITIONY[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(11, "MOMENTUMX[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(11, "MOMENTUMY[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(11, "MOMENTUMZ[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(11, "ENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(11, "NEUTRONMOMENTUMX");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(11, "NEUTRONMOMENTUMY");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(11, "NEUTRONMOMENTUMZ");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(11, "NEUTRONENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(11, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(11, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(11);

    G4AnalysisManager::Instance()->CreateNtuple("PionSignal", "ParticleInformation");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(12, "POSITIONX[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(12, "POSITIONY[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(12, "MOMENTUMX[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(12, "MOMENTUMY[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(12, "MOMENTUMZ[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(12, "ENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(12, "VPOSITIONX[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(12, "VPOSITIONY[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(12, "VPOSITIONZ[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(12, "PIONMOMENTUMX");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(12, "PIONMOMENTUMY");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(12, "PIONMOMENTUMZ");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(12, "PIONENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(12, "TOWERID");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(12, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(12, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(12);

    G4AnalysisManager::Instance()->CreateNtuple("NeutronSignal", "ParticleInformation");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(13, "POSITIONX[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(13, "POSITIONY[mm]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(13, "MOMENTUMX[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(13, "MOMENTUMY[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(13, "MOMENTUMZ[GeV/c]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(13, "ENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(13, "NEUTRONMOMENTUMX");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(13, "NEUTRONMOMENTUMY");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(13, "NEUTRONMOMENTUMZ");
    G4AnalysisManager::Instance()->CreateNtupleDColumn(13, "NEUTRONENERGY[GeV]");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(13, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(13, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(13);

    G4AnalysisManager::Instance()->CreateNtuple("SPINInfo", "Spin pattern");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(14, "SpinPattern");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(14, "RunNumber");
    G4AnalysisManager::Instance()->CreateNtupleIColumn(14, "EventNumber");
    G4AnalysisManager::Instance()->FinishNtuple(14);
}
