
#ifndef RHICF_PRIMARY_GENERATOR_ACTION_H
#define RHICF_PRIMARY_GENERATOR_ACTION_H

#include <map>
#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;
class G4VPrimaryGenerator;
class RHICFPrimaryGeneratorMessenger;

class RHICFPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction 
{
    public:

        RHICFPrimaryGeneratorAction();
        ~RHICFPrimaryGeneratorAction();

        virtual void GeneratePrimaries(G4Event* anEvent);

        void SetGenerator(G4VPrimaryGenerator* gen);
        void SetGenerator(G4String genname);

        G4VPrimaryGenerator* GetGenerator() const;
        G4String GetGeneratorName() const;

    private:

        //Junsang****G4VPrimaryGenerator* fParticleGun;
        G4VPrimaryGenerator* hepmcAscii;
        G4VPrimaryGenerator* pythia;
        G4VPrimaryGenerator* ipunibeam;
        G4VPrimaryGenerator* testunibeam;

        G4VPrimaryGenerator* currentGenerator;
        G4String currentGeneratorName;
        std::map<G4String, G4VPrimaryGenerator*> fGenTypeMap;

        RHICFPrimaryGeneratorMessenger* fMessenger;

};

inline void RHICFPrimaryGeneratorAction::SetGenerator(G4VPrimaryGenerator* gen)
{
    currentGenerator = gen;
}

inline void RHICFPrimaryGeneratorAction::SetGenerator(G4String genname)
{
    std::map<G4String, G4VPrimaryGenerator*>::iterator pos = fGenTypeMap.find(genname);
    if ( pos != fGenTypeMap.end() ) {
        currentGenerator = pos->second;
        currentGeneratorName = genname;
    }
}

inline G4VPrimaryGenerator* RHICFPrimaryGeneratorAction::GetGenerator() const
{
    return currentGenerator;
}

inline G4String RHICFPrimaryGeneratorAction::GetGeneratorName() const
{
    return currentGeneratorName;
}

#endif
