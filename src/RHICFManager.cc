#include "RHICFManager.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>
#include <map>
#include <string>

//SINGLETON FOR STORING PARAMETER WHILE PROGRAM RUNNING

bool RHICFManager::RHICFManagerFlag = false;

RHICFManager* RHICFManager::fRHICFManager = NULL;

RHICFManager::RHICFManager() 
{
    ARM1X = 0.;
    ARM1Y = 0.;
    ARM1Z = 0.;
    trigger = false;
}

RHICFManager::~RHICFManager()
{
    RHICFManagerFlag = false;
}

void RHICFManager::SetARM1X(G4double x)
{
    ARM1X = x;
}

void RHICFManager::SetARM1Y(G4double y)
{
    ARM1Y = y;
}

void RHICFManager::SetARM1Z(G4double z)
{
    ARM1Z = z;
}

void RHICFManager::SetMomentumX(G4double x)
{
    MomentumX = x;
}

void RHICFManager::SetMomentumY(G4double y)
{
    MomentumY = y;
}

void RHICFManager::SetMomentumZ(G4double z)
{
    MomentumZ = z;
}

void RHICFManager::SetVerboseSW(G4bool tr)
{
    trigger = tr;
}

void RHICFManager::ShowDInfo(G4String s, G4double d)
{
    if (trigger==true) 
    {
        G4cout << s+" : " << d << G4endl;
    }
}

void RHICFManager::ShowIInfo(G4String s, G4int i)
{
    if (trigger==true) 
    {
        G4cout << s+" : " << i << G4endl;
    }
}

void RHICFManager::ShowSInfo(G4String s, G4String ss)
{
    if (trigger==true) 
    {
        G4cout << s+" : " << ss << G4endl;
    }
}

RHICFManager* RHICFManager::GetInstance()
{
    if(!fRHICFManager)
    {
        fRHICFManager = new RHICFManager();
        RHICFManagerFlag = true;
    }
    return fRHICFManager;


}

void RHICFManager::SetParticleName(G4String string)
{
    fParticleName = string;
}

void RHICFManager::SetParticleNumber(G4int num)
{
    fParticleNum = num;
}

