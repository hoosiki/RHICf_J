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
    G4cout << "Run action generated" << G4endl;
    FileManager* fFileManager = FileManager::GetInstance();
    auto fAnalysisManager = G4AnalysisManager::Instance();
    fFileManager->PrepareSavingDirectory();
    fFileManager->SetFileName("LEAKGESTUDYNEUTRONTL10GeV");
    fAnalysisManager->SetNtupleMerging(true);
    fAnalysisManager->SetVerboseLevel(0);
    fAnalysisManager->SetFileName(fFileManager->GetPathFortmp()+"/"+fFileManager->GetFileName()+".root");
    NtupleForARM1();

}

RHICFRunAction::~RHICFRunAction()
{
    delete G4AnalysisManager::Instance();
}


void RHICFRunAction::BeginOfRunAction(const G4Run* run)
{
    G4cout << "Run Begin" << G4endl;
    auto fAnalysisManager = G4AnalysisManager::Instance();
    RHICFManager::GetInstance()->SetVerboseSW(false); //SHOW INFO
    Seeder* fSeeder = new Seeder();
    G4long tmpseed = (long)fSeeder->GetSeedForG4();
    G4Random::setTheSeeds(&tmpseed);
    fAnalysisManager->OpenFile();
}



void RHICFRunAction::EndOfRunAction(const G4Run* run)
{

    G4cout << "Run ended" << G4endl;
    auto fAnalysisManager = G4AnalysisManager::Instance();
    fAnalysisManager->Write();
    fAnalysisManager->CloseFile();
}


void RHICFRunAction::NtupleForARM1()
{
    auto fAnalysisManager = G4AnalysisManager::Instance();
    fAnalysisManager->CreateNtuple("ARM1PL", "GSOPL");
    //[0-15]: DE FOR TS GSO PLATE, 
    //[16-31]: DE FOR TL GSO PLATE, 
    //[32-47]: NOP FOR TS GSO PLATE, 
    //[48-63]: NOP FOR TL GSO PLATE
    //64 : TOTAL DE ENERGY
    //65 : RUN NUMBER
    //66 : EVENT NUMBER
    for (int i = 0; i < 16; i++) 
    {
        fAnalysisManager->CreateNtupleDColumn(0,"TSGSOPLDE"+std::to_string(i));
    }
    for (int i = 0; i < 16; i++) 
    {
        fAnalysisManager->CreateNtupleDColumn(0,"TLGSOPLDE"+std::to_string(i));
    }
    for (int i = 0; i < 16; i++) 
    {
        fAnalysisManager->CreateNtupleIColumn(0,"TSGSOPLNOP"+std::to_string(i));
    }
    for (int i = 0; i < 16; i++) 
    {
        fAnalysisManager->CreateNtupleIColumn(0,"TLGSOPLNOP"+std::to_string(i));
    }
    fAnalysisManager->CreateNtupleDColumn(0, "TotalEnergy");
    fAnalysisManager->CreateNtupleIColumn(0,"RunNumber");
    fAnalysisManager->CreateNtupleIColumn(0,"EventNumber");
    fAnalysisManager->FinishNtuple(0);

    fAnalysisManager->CreateNtuple("ARM1BAR", "GSOBAR");
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
            fAnalysisManager->CreateNtupleDColumn(1, "TSGSOBLDE"+std::to_string(i)+std::to_string(j));
        }
    }
    for (int i = 0; i < 4; i++) 
    {
        for(int j=0; j<40; j++)
        {
            fAnalysisManager->CreateNtupleDColumn(1, "TLGSOBLDE"+std::to_string(i)+std::to_string(j));
        }
    }
    for (int i = 0; i < 4; i++) 
    {
        
        for(int j=0; j<20; j++)
        {
            fAnalysisManager->CreateNtupleDColumn(1, "TSGSOBRDE"+std::to_string(i)+std::to_string(j));
        }
    }
    for (int i = 0; i < 4; i++) 
    {
        for(int j=0; j<40; j++)
        {
            fAnalysisManager->CreateNtupleDColumn(1, "TLGSOBRDE"+std::to_string(i)+std::to_string(j));
        }
    }
    for (int i = 0; i < 4; i++) 
    {
        
        for(int j=0; j<20; j++)
        {
            fAnalysisManager->CreateNtupleIColumn(1, "TSGSOBLNOP"+std::to_string(i)+std::to_string(j));
        }
    }
    for (int i = 0; i < 4; i++) 
    {
        for(int j=0; j<40; j++)
        {
            fAnalysisManager->CreateNtupleIColumn(1, "TLGSOBLNOP"+std::to_string(i)+std::to_string(j));
        }
    }
    for (int i = 0; i < 4; i++) 
    {
        
        for(int j=0; j<20; j++)
        {
            fAnalysisManager->CreateNtupleIColumn(1, "TSGSOBRNOP"+std::to_string(i)+std::to_string(j));
        }
    }
    for (int i = 0; i < 4; i++) 
    {
        for(int j=0; j<40; j++)
        {
            fAnalysisManager->CreateNtupleIColumn(1, "TLGSOBRNOP"+std::to_string(i)+std::to_string(j));
        }
    }
    fAnalysisManager->CreateNtupleIColumn(1,"RunNumber");
    fAnalysisManager->CreateNtupleIColumn(1,"EventNumber");
    fAnalysisManager->FinishNtuple(1);

    fAnalysisManager->CreateNtuple("FrontCounterInfo", "DENOP");
    fAnalysisManager->CreateNtupleDColumn(2, "TSFCDE");
    fAnalysisManager->CreateNtupleDColumn(2, "TLFCDE");
    fAnalysisManager->CreateNtupleIColumn(2, "TSFCNOP");
    fAnalysisManager->CreateNtupleIColumn(2, "TLFCNOP");
    fAnalysisManager->CreateNtupleIColumn(2, "RunNumber");
    fAnalysisManager->CreateNtupleIColumn(2, "EventNumber");

    fAnalysisManager->FinishNtuple(2);

    fAnalysisManager->CreateNtuple("Center", "ParticleInformation");
    fAnalysisManager->CreateNtupleDColumn(3, "POSITIONX[mm]");
    fAnalysisManager->CreateNtupleDColumn(3, "POSITIONY[mm]");
    fAnalysisManager->CreateNtupleDColumn(3, "MOMENTUMX[GeV/c]");
    fAnalysisManager->CreateNtupleDColumn(3, "MOMENTUMY[GeV/c]");
    fAnalysisManager->CreateNtupleDColumn(3, "MOMENTUMZ[GeV/c]");
    fAnalysisManager->CreateNtupleDColumn(3, "KINETICENERGY[GeV]");
    fAnalysisManager->CreateNtupleIColumn(3, "PID[PDG]");
    fAnalysisManager->CreateNtupleSColumn(3, "TYPE");
    fAnalysisManager->CreateNtupleIColumn(3, "RunNumber");
    fAnalysisManager->CreateNtupleIColumn(3, "EventNumber");
    fAnalysisManager->FinishNtuple(3);

    fAnalysisManager->CreateNtuple("FrontCounterParticle", "ParticleInformation");
    fAnalysisManager->CreateNtupleDColumn(4, "POSITIONX[mm]");
    fAnalysisManager->CreateNtupleDColumn(4, "POSITIONY[mm]");
    fAnalysisManager->CreateNtupleDColumn(4, "MOMENTUMX[GeV/c]");
    fAnalysisManager->CreateNtupleDColumn(4, "MOMENTUMY[GeV/c]");
    fAnalysisManager->CreateNtupleDColumn(4, "MOMENTUMZ[GeV/c]");
    fAnalysisManager->CreateNtupleDColumn(4, "KINETICENERGY[GeV]");
    fAnalysisManager->CreateNtupleIColumn(4, "PID[PDG]");
    fAnalysisManager->CreateNtupleSColumn(4, "TYPE");
    fAnalysisManager->CreateNtupleIColumn(4, "RunNumber");
    fAnalysisManager->CreateNtupleIColumn(4, "EventNumber");
    fAnalysisManager->FinishNtuple(4);
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
    //Junsang****fAnalysisManager->CreateNtupleDColumn(tmpstr);
    //Junsang****}
    //Junsang****for(G4int i=1; i<33; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = SMDHCHNOP + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****fAnalysisManager->CreateNtupleIColumn(tmpstr);
    //Junsang****}
    //Junsang****// Leaf for SMDV ID:[64-105]
    //Junsang****for(G4int i=1; i<22; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = SMDVCHDE + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****fAnalysisManager->CreateNtupleDColumn(tmpstr);
    //Junsang****}
    //Junsang****for(G4int i=1; i<22; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = SMDVCHNOP + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****fAnalysisManager->CreateNtupleIColumn(tmpstr);
    //Junsang****}
    //Junsang****// Leaf for ZDC1 ID:[106-157]
    //Junsang****for(G4int i=1; i<27; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC1CHDE + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****fAnalysisManager->CreateNtupleDColumn(tmpstr);
    //Junsang****}
    //Junsang****for(G4int i=1; i<27; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC1CHNOP + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****fAnalysisManager->CreateNtupleIColumn(tmpstr);
    //Junsang****}
    //Junsang****// Leaf for ZDC2 ID:[158-209]
    //Junsang****for(G4int i=1; i<27; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC2CHDE + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****fAnalysisManager->CreateNtupleDColumn(tmpstr);
    //Junsang****}
    //Junsang****for(G4int i=1; i<27; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC2CHNOP + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****fAnalysisManager->CreateNtupleIColumn(tmpstr);
    //Junsang****}
    //Junsang****// Leaf for ZDC3 ID:[210-261]
    //Junsang****for(G4int i=1; i<27; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC3CHDE + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****fAnalysisManager->CreateNtupleDColumn(tmpstr);
    //Junsang****}
    //Junsang****for(G4int i=1; i<27; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC3CHNOP + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****fAnalysisManager->CreateNtupleIColumn(tmpstr);
    //Junsang****}
    //Junsang****// Leaf for ZDC1W ID:[262-288]
    //Junsang****for(G4int i=1; i<28; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC1WCHDE + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****fAnalysisManager->CreateNtupleDColumn(tmpstr);
    //Junsang****}
    //Junsang****// Leaf for ZDC2W ID:[289-315]
    //Junsang****for(G4int i=1; i<28; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC2WCHDE + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****fAnalysisManager->CreateNtupleDColumn(tmpstr);
    //Junsang****}
    //Junsang****// Leaf for ZDC3W ID:[316-342]
    //Junsang****for(G4int i=1; i<28; i++)
    //Junsang****{
    //Junsang****
    //Junsang****G4String tmpstr = ZDC3WCHDE + std::to_string(i);
    //Junsang****G4cout << "tmpstr: " << tmpstr << G4endl;
    //Junsang****fAnalysisManager->CreateNtupleDColumn(tmpstr);
    //Junsang****}
    //Junsang****
    //Junsang****fAnalysisManager->CreateNtupleDColumn(ZDCTDE); // ID:343
    //Junsang****fAnalysisManager->CreateNtupleIColumn(ZDCTNOP);// ID:344
    //Junsang****fAnalysisManager->CreateNtupleDColumn(I_PLTDE);// ID:345
    //Junsang****fAnalysisManager->CreateNtupleDColumn(W_PLTDE);// ID:346
    //Junsang****fAnalysisManager->CreateNtupleDColumn(SMDHTDE);// ID:347
    //Junsang****fAnalysisManager->CreateNtupleIColumn(SMDHTNOP);// ID:348
    //Junsang****fAnalysisManager->CreateNtupleDColumn(SMDVTDE);// ID:349
    //Junsang****fAnalysisManager->CreateNtupleIColumn(SMDVTNOP);// ID:350
}
