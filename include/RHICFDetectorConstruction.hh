#ifndef RHICFDetectorConstruction_h
#define RHICFDetectorConstruction_1 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4RotationMatrix.hh"
#include <vector>
#include "G4FieldManager.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"

class RHICFMaterials;
class MagneticField;
class G4VSensitiveDetector;
class G4VPhysicalVolume;
class G4VisAttributes;
class G4VReadOutGeometry;
class G4OpticalSurface;
class RHICFDetectorConstruction: public G4VUserDetectorConstruction
{

    public:

        //static G4ThreadLocal G4bool fConstructedSDandField;
        RHICFDetectorConstruction();
        virtual ~RHICFDetectorConstruction();
        virtual G4VPhysicalVolume* Construct();

        virtual void ConstructSDandField();

        G4Material* FindMaterial(G4String);
    private:

        void DefineDimension();
        void ConstructMaterials();
        void DestructMaterials();
        std::vector<G4VisAttributes*>fVisAttributes;


        G4VisAttributes*            visAttributes;
                                    
        G4VPhysicalVolume*          HODOSCOPE(G4VPhysicalVolume*, G4RotationMatrix*);
        G4VPhysicalVolume*          WCNT(G4VPhysicalVolume*, G4ThreeVector,  G4RotationMatrix*);
        G4VPhysicalVolume*          BBC(G4VPhysicalVolume*, G4ThreeVector, G4RotationMatrix*);
        G4VPhysicalVolume*          PIPE();
        G4VPhysicalVolume*          ARM2(G4VPhysicalVolume*, G4ThreeVector, G4RotationMatrix*);
                                    
        static G4ThreadLocal        MagneticField* fMagneticField;
        static G4ThreadLocal        G4FieldManager* fFieldMgr;
          
        G4OpticalSurface*           fOpsurface;
        RHICFMaterials*             fMaterials;
        G4VSolid*                   fWorldSolid;
        G4VSolid*                   fHODOSolid;
        G4VSolid*                   fSCINSolid;
        G4VSolid*                   fWCNTSolid;
        G4VSolid*                   fZDCSolid;
        G4VSolid*                   fGAPFSolid;
        G4VSolid*                   fGAPF1Solid;
        G4VSolid*                   fGAPF2Solid;
        G4VSolid*                   fFIBSolid;
        G4VSolid*                   fFIBRSolid;
        G4VSolid*                   fBlockerSolid;
        G4VSolid*                   fW_PLSolid;
        G4VSolid*                   fI_PLSolid;
        G4VSolid*                   fFEPLSolid;
        G4VSolid*                   fALPLSolid;
        G4VSolid*                   fSMDSolid;
        G4VSolid*                   fSMDHSolid;
        G4VSolid*                   fSMDVSolid;
        G4LogicalVolume*            fSMDLogical;
        G4LogicalVolume*            fZDCLogical;
        G4LogicalVolume*            fWCNTLogical;
        G4LogicalVolume*            fSCINLogical;
        G4LogicalVolume*            fWorldLogical;
        G4LogicalVolume*            fHODOLogical;
        G4LogicalVolume*            fGAPF_1Logical;
        G4LogicalVolume*            fGAPF_2Logical;
        G4LogicalVolume*            fGAPF_3Logical;
        G4LogicalVolume*            fGAPF1Logical;
        G4LogicalVolume*            fGAPF2Logical;
        // SMDV
        G4LogicalVolume*            fSMDVLogical;
        G4LogicalVolume*            fFIBRLogical;
        G4LogicalVolume*            fMagneticLogical;
        G4LogicalVolume*            fW_PL_1Logical;
        G4LogicalVolume*            fW_PL_2Logical;
        G4LogicalVolume*            fW_PL_3Logical;
        G4LogicalVolume*            fFIBLogical;
        G4LogicalVolume*            fBBCRLogical;
        G4LogicalVolume*            fBBCPLogical;
        G4LogicalVolume*            fBlockerLogical;
        G4LogicalVolume*            fI_PLLogical;
        G4LogicalVolume*            fFEPLLogical;
        G4LogicalVolume*            fALPLLogical;
        // SMDH
        G4LogicalVolume*            fSMDHLogical;
        G4LogicalVolume*            RB14Logical;
        G4LogicalVolume*            FLS4Logical;
        G4LogicalVolume*            CM11Logical;
        G4LogicalVolume*            CM12Logical;
        G4LogicalVolume*            FLB2Logical;
        G4LogicalVolume*            SM21Logical;
        G4LogicalVolume*            RB16Logical;
        G4LogicalVolume*            SPU1Logical;
        G4LogicalVolume*            RB27Logical;
        G4LogicalVolume*            BEAVLogical;
        G4LogicalVolume*            FLS5Logical;
        G4LogicalVolume*            RB13Logical;
        G4LogicalVolume*            CN12Logical;
        G4LogicalVolume*            RB17Logical;
        G4LogicalVolume*            SPL2Logical;
        G4LogicalVolume*            FLB1Logical;
        G4LogicalVolume*            RB22Logical;
        G4LogicalVolume*            RB25Logical;
        G4LogicalVolume*            RB28Logical;
        G4LogicalVolume*            FLS6Logical;
        G4LogicalVolume*            RB21Logical;
        G4LogicalVolume*            CN11Logical;
        G4LogicalVolume*            RB11Logical;
        G4LogicalVolume*            S101Logical;
        G4LogicalVolume*            M101Logical;
        G4LogicalVolume*            S112Logical;
        G4LogicalVolume*            M102Logical;
        G4LogicalVolume*            M111Logical;
        G4LogicalVolume*            M112Logical;
        G4LogicalVolume*            S111Logical;
        G4LogicalVolume*            S102Logical;
        G4LogicalVolume*            SHU1Logical;
        G4LogicalVolume*            SHL2Logical;
        G4LogicalVolume*            SHL1Logical;
        G4LogicalVolume*            SP92Logical;
        G4LogicalVolume*            SP91Logical;
        G4LogicalVolume*            SHU2Logical;
        G4LogicalVolume*            SM82Logical;
        G4LogicalVolume*            SM81Logical;
        G4LogicalVolume*            SP82Logical;
        G4LogicalVolume*            SP81Logical;

        G4VPhysicalVolume*          fBlockerPhysical;
        G4VPhysicalVolume*          fFIBRPhysical;
        G4VPhysicalVolume*          fWorldPhysical;
        G4VPhysicalVolume*          fHODOPhysical;
        G4VPhysicalVolume*          fGAPF_1Physical;
        G4VPhysicalVolume*          fGAPF_2Physical;
        G4VPhysicalVolume*          fGAPF_3Physical;
        G4VPhysicalVolume*          fGAPF1_1Physical;
        G4VPhysicalVolume*          fGAPF1_2Physical;
        G4VPhysicalVolume*          fGAPF1_3Physical;
        G4VPhysicalVolume*          fGAPF2Physical;
        G4VPhysicalVolume*          fW_PL_1Physical;
        G4VPhysicalVolume*          fW_PL_2Physical;
        G4VPhysicalVolume*          fW_PL_3Physical;
        G4VPhysicalVolume*          fFIBPhysical;
        G4VPhysicalVolume*          fWCNTPhysical;
        G4VPhysicalVolume*          fBBCMPhysical;
        G4VPhysicalVolume*          HODOINSTALL;
        G4VPhysicalVolume*          LOCALPOLINSTALL;
        G4VPhysicalVolume*          PIPEINSTALL;
        G4VPhysicalVolume*          BBCINSTALL;
        G4VPhysicalVolume*          ARM2INSTALL;
        G4VPhysicalVolume*          fBBCCPhysical;
        G4VPhysicalVolume*          fBBCBPhysical;
        G4VPhysicalVolume*          fBBCFPhysical;
        G4VPhysicalVolume*          fBBCEPhysical;
        G4VPhysicalVolume*          fBBCSPhysical;
        G4VPhysicalVolume*          fBBCHPhysical;
        G4VPhysicalVolume*          fBBCQPhysical;
        G4VPhysicalVolume*          fBBCAPhysical;

        G4MaterialPropertiesTable* fBlack;






        // Define RotationMatrix
        //
        G4RotationMatrix*           PipeRotation1;
        G4RotationMatrix*           PipeRotation2;
        G4RotationMatrix*           INVERSERotation;
        G4RotationMatrix*           HODORotation;
        G4RotationMatrix*           GAPFRotation;
        G4RotationMatrix*           MagRotation;
        G4RotationMatrix*           fNonRotation;
        G4RotationMatrix*           fQPhi;
        G4RotationMatrix*           fSMDRotation;


        G4int iron; 

        G4double                    worX, worY, worZ;
        G4double                    interval, zpos, ypos;
        G4double                    putFx, pos;
        G4double                    pipeInr, pipeOutr, pipeLength;
        G4double                    Lmod;
        G4double                    ivolu, judgeNS;


        G4double                    par[5];
        G4double                    zdcPar[3];
                                    
        G4double                    iplPar[3];
        G4double                    wplPar[3];
        G4double                    gapper[3];
        G4double                    fcpPar[3];
        G4double                    rcPar[3];
        G4double                    wcntPar[3];
        G4double                    fibPar[3];
        G4double                    hodoPar[3];
        G4double                    scinPar[3];
        G4double                    smdPar[3];
        G4double                    smdHeight;
        G4double                    smdhPar[3];
        G4double                    smdvPar[3];
        G4double                    alPar[3];
        G4double                    fePar[3];
        G4double                    sin, cos, tan;
        G4double                    zdcPar1, zdcPar2;
        G4double                    xpos;

//      Parameter for BBC


        G4double                    kBBAbsorb[3];
        G4double                    kBBAttach[10];
        G4double                    kBBBackBD[3];
        G4double                    kBBBreede[3];
        G4double                    kBBCovert;
        G4double                    kBBFrontb[3];
        G4double                    kBBPMTSiz[3];
        G4double                    kBBQuartz[10];
        G4double                    kBBShithi;
        G4double                    kBBSpacin;
        G4double                    kBBStruct[3];
        G4double                    kBBZPosit[2];
                                    
        G4double                    kBBBCover[3];
        G4double                    kBBDetect[10];
        G4double                    kBBMother[3];
        G4double                    kBBShield[10];
                                    
        G4double                    kXSTP, kYSTP, kRrad, kRRMA, kRRMI, kRmax, kRmin;
        G4int                       kMrow, kMcol, kNrow, kNcol, kNdet, kMaxPMT, kLrow, kLcol;

        G4bool                      checkOverlaps;


        G4double                    Nlay,Nmod;


        // Parameter for ARM2
        //

        G4int                       lcur_num;
        G4int                       wcur_num;
        G4int                       noflayer;
        G4int                       nofssdset;
        G4double                    TL;
        G4double                    TS;
        G4double                    SSDX;
        G4double                    SSDY;
        G4double                    gsoThickness;
        G4double                    acrylThickness;
        G4double                    ssdsiThickness;
        G4double                    ssdalThickness;
        G4double                    ssdacrylThickness;
        G4double                    firstThickness;
        G4double                    Arm2SizeX;
        G4double                    Arm2SizeY;
        G4double                    firstSizeY;
        G4double                    gapThickness;
        G4double                    layerThickness;
        G4double                    ssdThickness;
        G4double                    ssdxyThickness;
        G4double                    Arm2Thickness;
        G4double                    place;




        

};
#endif
