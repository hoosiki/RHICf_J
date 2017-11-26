#include "RHICFRunAction.hh"
#include "RHICFDetectorConstruction.hh"
#include "FileManager.hh"
#include "RHICFManager.hh"
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

    FileManager* fFileManager = FileManager::GetInstance();
    fFileManager->PrepareSavingDirectory();
    fFileManager->SetFileName("LEAKGESTUDYNEUTRONTL");
    G4AnalysisManager::Instance()->SetNtupleMerging(true);
    G4AnalysisManager::Instance()->SetVerboseLevel(0);
    G4AnalysisManager::Instance()->SetFileName(fFileManager->GetPathFortmp()+"/"+fFileManager->GetFileName()+".root");
    NtupleForARM1();

}

RHICFRunAction::~RHICFRunAction()
{
    delete G4AnalysisManager::Instance();
}


void RHICFRunAction::BeginOfRunAction(const G4Run* run)
{
    RHICFManager::GetInstance()->SetVerboseSW(false); //SHOW INFO
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
    //[0-15]: DE FOR TS GSO PLATE, 
    //[16-31]: DE FOR TL GSO PLATE, 
    //[32-47]: NOP FOR TS GSO PLATE, 
    //[48-63]: NOP FOR TL GSO PLATE
    //64 : TOTAL DE ENERGY
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

    G4AnalysisManager::Instance()->CreateNtuple("Center", "ParticleInformation");
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

    G4AnalysisManager::Instance()->CreateNtuple("FrontCounterParticle", "ParticleInformation");
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
}

void RHICFRunAction::NtupleForSTARZDC()
{
    //Junsang****RHICFDetectorConstruction* fConstruction = new RHICFDetectorConstruction();
    //Junsang****
    //Junsang****G4cout << "SDforWInZDC  : " << fConstruction->GetSDforWInZDC() << G4endl;
    //Junsang****G4cout << "SDforI_PL    : " << fConstruction->GetSDforI_PL() << G4endl;
    //Junsang****G4cout << "SDforPMMA    : " << fConstruction->GetSDforPMMA() << G4endl;
    //Junsang****G4cout << "SDforSMD     : " << fConstruction->GetSDforSMD() << G4endl;
    //Junsang****G4cout << "SDforWInARM1 : " << fConstruction->GetSDforWInARM1() << G4endl;
    //Junsang****G4cout << "SDforWHolder : " << fConstruction->GetSDforHolder() << G4endl;
    //Junsang****G4cout << "SDforGSOBar  : " << fConstruction->GetSDforGSOBar() << G4endl;
    //Junsang****G4cout << "SDforGSOPlate: " << fConstruction->GetSDforGSOPlate() << G4endl;
    //Junsang****G4cout << "SDforAlFrame : " << fConstruction->GetSDforFrame() << G4endl;
    //Junsang****G4cout << "SDforPanels  : " << fConstruction->GetSDforPanels() << G4endl;
    //
    //
    //
    //Junsang****// Leaf for SMDH ID:[0-63]
    //Junsang****for(G4int i=1; i<33; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = SMDHCHDE + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleDColumn(tmpstr);
    //Junsang****}
    //Junsang****for(G4int i=1; i<33; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = SMDHCHNOP + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleIColumn(tmpstr);
    //Junsang****}
    //Junsang****// Leaf for SMDV ID:[64-105]
    //Junsang****for(G4int i=1; i<22; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = SMDVCHDE + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleDColumn(tmpstr);
    //Junsang****}
    //Junsang****for(G4int i=1; i<22; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = SMDVCHNOP + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleIColumn(tmpstr);
    //Junsang****}
    //Junsang****// Leaf for ZDC1 ID:[106-157]
    //Junsang****for(G4int i=1; i<27; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC1CHDE + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleDColumn(tmpstr);
    //Junsang****}
    //Junsang****for(G4int i=1; i<27; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC1CHNOP + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleIColumn(tmpstr);
    //Junsang****}
    //Junsang****// Leaf for ZDC2 ID:[158-209]
    //Junsang****for(G4int i=1; i<27; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC2CHDE + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleDColumn(tmpstr);
    //Junsang****}
    //Junsang****for(G4int i=1; i<27; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC2CHNOP + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleIColumn(tmpstr);
    //Junsang****}
    //Junsang****// Leaf for ZDC3 ID:[210-261]
    //Junsang****for(G4int i=1; i<27; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC3CHDE + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleDColumn(tmpstr);
    //Junsang****}
    //Junsang****for(G4int i=1; i<27; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC3CHNOP + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleIColumn(tmpstr);
    //Junsang****}
    //Junsang****// Leaf for ZDC1W ID:[262-288]
    //Junsang****for(G4int i=1; i<28; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC1WCHDE + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleDColumn(tmpstr);
    //Junsang****}
    //Junsang****// Leaf for ZDC2W ID:[289-315]
    //Junsang****for(G4int i=1; i<28; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC2WCHDE + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleDColumn(tmpstr);
    //Junsang****}
    //Junsang****// Leaf for ZDC3W ID:[316-342]
    //Junsang****for(G4int i=1; i<28; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC3WCHDE + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleDColumn(tmpstr);
    //Junsang****}
    //Junsang****
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleDColumn(ZDCTDE); // ID:343
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleIColumn(ZDCTNOP);// ID:344
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleDColumn(I_PLTDE);// ID:345
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleDColumn(W_PLTDE);// ID:346
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleDColumn(SMDHTDE);// ID:347
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleIColumn(SMDHTNOP);// ID:348
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleDColumn(SMDVTDE);// ID:349
    //Junsang****G4AnalysisManager::Instance()->CreateNtupleIColumn(SMDVTNOP);// ID:350
}
