#ifndef RHICFMaterials_h
#define RHICFMaterials_h 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

class RHICFMaterials
{
  public:

    virtual ~RHICFMaterials();
 
    static RHICFMaterials* GetInstance();

    G4Material* GetMaterial(const G4String);
 
  private:

 
    RHICFMaterials();

    void CreateMaterials();

  private:

    static RHICFMaterials* fInstance;

    G4NistManager*      fNistMan;

    G4Material*         fAir;

    G4Material*         fPMMA;
    G4Material*         fPethylene;
    G4Material*         fFPethylene;
    G4Material*         fPolystyrene;
    G4Material*         fSilicone;
    G4Material*         fCoating;
    G4Material*         fFPMMA;
    G4Material*         fPL_SCINTI;
    G4Material*         fQuartz;
    G4Material*         fGSO;
    G4Material*         fGten;
    G4Material*         fAcrylic;


};

#endif /*RHICFMaterials_h*/
