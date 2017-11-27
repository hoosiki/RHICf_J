#ifndef RHICFActionInitialization_h
#define RHICFActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "globals.hh"

class RHICFActionInitialization : public G4VUserActionInitialization
{
  public:
    RHICFActionInitialization();
    virtual ~RHICFActionInitialization();
    virtual void BuildForMaster() const;
    virtual void Build() const;

};

#endif

    
