//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: RHICFStepMax.cc 69561 2013-05-08 12:25:56Z gcosmo $
//
/// \file optical/RHICF/src/RHICFStepMax.cc
/// \brief Implementation of the RHICFStepMax class
//
//
#include "G4Track.hh"
#include "G4VParticleChange.hh"

#include "RHICFStepMax.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RHICFStepMax::RHICFStepMax(const G4String& aName)
  : G4VDiscreteProcess(aName), fMaxChargedStep(DBL_MAX)
{
   if (verboseLevel>0) {
     G4cout << GetProcessName() << " is created "<< G4endl;
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RHICFStepMax::~RHICFStepMax() { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RHICFStepMax::RHICFStepMax(RHICFStepMax& right) : G4VDiscreteProcess(right) 
{

    fMaxChargedStep = DBL_MAX;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool RHICFStepMax::IsApplicable(const G4ParticleDefinition& particle)
{
  return (particle.GetPDGCharge() != 0.);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RHICFStepMax::SetStepMax(G4double step) { fMaxChargedStep = step ; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double RHICFStepMax::PostStepGetPhysicalInteractionLength(
                                              const G4Track&,
                                              G4double,
                                              G4ForceCondition* condition)
{
  // condition is set to "Not Forced"
  *condition = NotForced;

  G4double ProposedStep = DBL_MAX;

  if ( fMaxChargedStep > 0.) ProposedStep = fMaxChargedStep;

   return ProposedStep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VParticleChange* RHICFStepMax::PostStepDoIt(const G4Track& aTrack,
                                         const G4Step&         )
{
   // do nothing
   aParticleChange.Initialize(aTrack);
   return &aParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double RHICFStepMax::GetMeanFreePath(const G4Track&,G4double,G4ForceCondition*)
{
  return 0.;
}
