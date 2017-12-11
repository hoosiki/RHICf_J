#ifndef TestInterface_h
#define TestInterface_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4VPrimaryGenerator.hh"
#include "globals.hh"
#include "G4ParticleGun.hh"

class G4GenericMessenger;
class G4Event;

/// Primary generator
///
/// A single particle is generated.
/// User can select 
/// - the initial momentum and angle
/// - the momentum and angle spreads
/// - random selection of a particle type from proton, kaon+, pi+, muon+, e+ 


class TestInterface : public G4ParticleGun
{
    public:

        TestInterface();
        virtual ~TestInterface();

        void SetParticleName(G4String val) { ParticleName = val; }
        G4String GetParticleName() const { return ParticleName; }

        void SetPosition(G4String val) { Position = val; }
        G4String GetPosition() const { return Position; }

        void SetShape(G4String val) { Shape = val; }
        G4String GetShape() const { return Shape; }

        void SetTower(G4String val) { Tower = val; }
        G4String GetTower() const { return Tower; }
    
        void SetEnergy(G4double val) { fEnergy = val; }
        G4double GetEnergy() const { return fEnergy; }

        void SetSigmaAngle(G4double val) { fSigmaAngle = val; }
        G4double GetSigmaAngle() const { return fSigmaAngle; }

        void SetRandomize(G4bool val) { fRandomizePrimary = val; }
        G4bool GetRandomize() const { return fRandomizePrimary; }

        void SetSigmaRange(G4double val) { fSigmaRange = val; }
        G4double GetSigmaRange() const { return fSigmaRange; }

        void SetX(G4double val) { fX = val; }
        G4double GetX() const { return fX; }

        void SetY(G4double val) { fY = val; }
        G4double GetY() const { return fY; }

        G4ThreeVector UnitVector(G4ThreeVector);

    private:
        void DefineCommands();
        void GeneratePrimaryVertex(G4Event* event);

        G4GenericMessenger* fMessenger;
        G4String ParticleName;
        G4String Position;
        G4String Shape;
        G4String Tower;
        G4double fX;
        G4double fY;
        G4double fEnergy;
        G4double fSigmaAngle;
        G4double fSigmaRange;
        G4bool fRandomizePrimary;


};

#endif
