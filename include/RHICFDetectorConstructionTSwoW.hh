#ifndef RHICFDetectorConstructionTSwoW_h
#define RHICFDetectorConstructionTSwoW_1 1

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
class RHICFDetectorConstructionTSwoW: public G4VUserDetectorConstruction
{

    public:

        //static G4ThreadLocal G4bool fConstructedSDandField;
        RHICFDetectorConstructionTSwoW();
        virtual ~RHICFDetectorConstructionTSwoW();
        virtual G4VPhysicalVolume* Construct();
        G4bool GetSDforWInZDC(){ return SDforWInZDC; };
        G4bool GetSDforI_PL(){ return SDforI_PL; };
        G4bool GetSDforPMMA(){ return SDforPMMA; };
        G4bool GetSDforSMD(){ return SDforSMD; };    
        G4bool GetSDforWInARM1(){ return SDforWInARM1; };
        G4bool GetSDforHolder(){ return SDforHolder; };
        G4bool GetSDforGSOBar(){ return SDforGSOBar; };
        G4bool GetSDforGSOPlate(){ return SDforGSOPlate; };
        G4bool GetSDforFrame(){ return SDforFrame; };
        G4bool GetSDforPanels(){ return SDforPanels; };



        virtual void ConstructSDandField();

        G4Material* FindMaterial(G4String);
    private:

        void DefineDimension();
        void ConstructMaterials();
        void DestructMaterials();
        void PHENIXPIPE();
        void SetMagneticField(G4LogicalVolume*);
        void SetSDForFrontCounter();
        void SetSDForARM1();
        void SetSDForSTARZDC();
        void SetDEScorer(G4String);
        void SetDENOPScorer(G4String);
        
        std::vector<G4VisAttributes*>fVisAttributes;


        G4VisAttributes*            visAttributes;
                                    
        void          PHENIXZDCINSTALL(G4VPhysicalVolume*, G4ThreeVector,  G4RotationMatrix*);
        void          BBCINSTALL(G4VPhysicalVolume*, G4ThreeVector, G4RotationMatrix*);
        void          ARM1INSTALL(G4VPhysicalVolume*, G4ThreeVector, G4RotationMatrix*);
        void          STARPIPEINSTALL(G4double, G4double);
        void          STARZDCINSTALL(G4VPhysicalVolume*, G4ThreeVector, G4RotationMatrix*);
                                    
        static G4ThreadLocal        MagneticField* fMagneticField;
        static G4ThreadLocal        G4FieldManager* fFieldMgr;
          
        G4OpticalSurface*           fOpsurface;
        RHICFMaterials*             fMaterials;
        G4VSolid*                   fWorldSolid;
        G4VSolid*                   fPHENIXZDCSolid;
        G4VSolid*                   fZDCSolid;
        G4VSolid*                   fSTARZDCSolid;
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
        G4VSolid*                   fSMDVStripSolid;
        G4VSolid*                   fSMDHStripSolid;
        G4VSolid*                   fARM1Solid;//SolidVolumes for ARM1
        G4VSolid*                   fWHolder_1Solid;
        G4VSolid*                   fWHolder_2Solid;
        G4VSolid*                   fNegativeSmallWSolid;
        G4VSolid*                   fNegativeLargeWSolid;
        G4VSolid*                   fNegativeTopRoundSolid;
        G4VSolid*                   fNegativeMiddleRoundSolid;
        G4VSolid*                   fNegativeBottomRoundSolid;
        G4VSolid*                   fNegativeHoleSolid;
        G4VSolid*                   fLargeW_PLSolid;
        G4VSolid*                   fSmallW_PLSolid;
        G4VSolid*                   fGSO_PLHolderSolid;     
        G4VSolid*                   fNegativeGuideLargeSolid;
        G4VSolid*                   fNegativeGuideSmallSolid;
        G4VSolid*                   fLargeGSO_PLSolid;
        G4VSolid*                   fSmallGSO_PLSolid;
        G4VSolid*                   fGSOBarHolderSolid;
        G4VSolid*                   fNegativeGSOBarLargeRightSolid;
        G4VSolid*                   fNegativeGSOBarLargeLeftSolid;
        G4VSolid*                   fNegativeGSOBarSmallRightSolid;
        G4VSolid*                   fNegativeGSOBarSmallLeftSolid;
        G4VSolid*                   fLightGuideLargeSolid;
        G4VSolid*                   fLightGuideSmallSolid;
        G4VSolid*                   fGSOSmallBarSolid;
        G4VSolid*                   fGSOLargeBarSolid;
        G4VSolid*                   fGSOSmallBarBeltSolid;
        G4VSolid*                   fGSOLargeBarBeltSolid;
        G4VSolid*                   fAlFramePart1Solid;
        G4VSolid*                   fAlFramePart2Solid;
        G4VSolid*                   fAlFramePart3Solid;
        G4VSolid*                   fAlFramePart5Solid;
        G4VSolid*                   fAlFramePart6Solid;
        G4VSolid*                   fAlFramePart7Solid;
        G4VSolid*                   fAlFramePart8Solid;
        G4VSolid*                   fAlFramePart9Solid;
        G4VSolid*                   fAlFramePart10Solid;
        G4VSolid*                   fAlFrameSolid;
        G4VSolid*                   fAlFrame1Solid;
        G4VSolid*                   fAlFrame2Solid;
        G4VSolid*                   fSidePanelSolid;
        G4VSolid*                   fFrontPanelSolid;




        G4LogicalVolume*            fSMDLogical;//Logical volumes for ZDC
        G4LogicalVolume*            fZDCLogical;
        G4LogicalVolume*            fPHENIXZDCLogical;
        G4LogicalVolume*            fSTARZDCLogical;
        G4LogicalVolume*            fWorldLogical;
        G4LogicalVolume*            fGAPF_1Logical;
        G4LogicalVolume*            fGAPF_2Logical;
        G4LogicalVolume*            fGAPF_3Logical;
        G4LogicalVolume*            fGAPF1Logical;
        G4LogicalVolume*            fGAPF2Logical;
        // SMDV
        G4LogicalVolume*            fSMDVLogical;
        G4LogicalVolume*            fMagneticLogical;
        G4LogicalVolume*            fW_PL_1Logical;
        G4LogicalVolume*            fW_PL_2Logical;
        G4LogicalVolume*            fW_PL_3Logical;
        G4LogicalVolume*            fBlockerLogical;
        G4LogicalVolume*            fI_PLLogical;
        G4LogicalVolume*            fFEPLLogical;
        G4LogicalVolume*            fALPLLogical;
        // SMDH
        G4LogicalVolume*            fSMDHLogical;
        G4LogicalVolume*            RB14Logical;//Logical volumes for pipe
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
        G4LogicalVolume*            fARM1Logical;//Logical volumes for ARM1
        G4LogicalVolume*            fWHolder_1Logical;
        G4LogicalVolume*            fWHolder_2Logical;
        G4LogicalVolume*            fNegativeLargeWLogical;
        G4LogicalVolume*            fNegativeSmallWLogical;
        G4LogicalVolume*            fLargeW_PLLogical;
        G4LogicalVolume*            fSmallW_PLLogical;
        G4LogicalVolume*            fLargeGSO_PLLogical;
        G4LogicalVolume*            fSmallGSO_PLLogical;
        G4LogicalVolume*            fGSORightSmallBarBelt_1Logical;
        G4LogicalVolume*            fGSORightSmallBarBelt_2Logical;
        G4LogicalVolume*            fGSORightSmallBarBelt_3Logical;
        G4LogicalVolume*            fGSORightSmallBarBelt_4Logical;
        G4LogicalVolume*            fGSORightLargeBarBelt_1Logical;
        G4LogicalVolume*            fGSORightLargeBarBelt_2Logical;
        G4LogicalVolume*            fGSORightLargeBarBelt_3Logical;
        G4LogicalVolume*            fGSORightLargeBarBelt_4Logical;
        G4LogicalVolume*            fGSOLeftSmallBarBelt_1Logical;
        G4LogicalVolume*            fGSOLeftSmallBarBelt_2Logical;
        G4LogicalVolume*            fGSOLeftSmallBarBelt_3Logical;
        G4LogicalVolume*            fGSOLeftSmallBarBelt_4Logical;
        G4LogicalVolume*            fGSOLeftLargeBarBelt_1Logical;
        G4LogicalVolume*            fGSOLeftLargeBarBelt_2Logical;
        G4LogicalVolume*            fGSOLeftLargeBarBelt_3Logical;
        G4LogicalVolume*            fGSOLeftLargeBarBelt_4Logical;
        G4LogicalVolume*            fGSORightSmallBarLogical;
        G4LogicalVolume*            fGSOLeftSmallBarLogical;
        G4LogicalVolume*            fGSORightLargeBarLogical;
        G4LogicalVolume*            fGSOLeftLargeBarLogical;
        G4LogicalVolume*            fGSOLeftSmallBarBeltLogical;
        G4LogicalVolume*            fGSO_PLHolderLogical;
        G4LogicalVolume*            fGSOBarHolderLogical;
        G4LogicalVolume*            fLightGuideLargeLogical;
        G4LogicalVolume*            fLightGuideSmallLogical;
        G4LogicalVolume*            fAlFramePart1Logical;
        G4LogicalVolume*            fAlFramePart2Logical;
        G4LogicalVolume*            fAlFrame1Logical;
        G4LogicalVolume*            fAlFrame2Logical;
        G4LogicalVolume*            fSidePanelLogical;
        G4LogicalVolume*            fFrontPanelLogical;
        G4LogicalVolume*            f55InchPipeVacuumLogical;


        G4VPhysicalVolume*          fBlockerPhysical;
        G4VPhysicalVolume*          fFIBRPhysical;
        G4VPhysicalVolume*          fWorldPhysical;
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
        G4VPhysicalVolume*          fPHENIXZDCPhysical;
        G4VPhysicalVolume*          fSTARZDCPhysical;
        G4VPhysicalVolume*          fWHolderPhysical;
        G4VPhysicalVolume*          fARM1Physical;

        G4MaterialPropertiesTable* fBlack;






        // Define RotationMatrix
        G4RotationMatrix*           PipeRotation1;
        G4RotationMatrix*           PipeRotation2;
        G4RotationMatrix*           INVERSERotation;
        G4RotationMatrix*           GAPFRotation;
        G4RotationMatrix*           MagRotation;
        G4RotationMatrix*           fNonRotation;
        G4RotationMatrix*           fQPhi;
        G4RotationMatrix*           fSMDRotation;
        G4RotationMatrix*           SMDVRotation;
        G4RotationMatrix*           SMDHRotation;
        G4RotationMatrix*           SMDH2Rotation;
        G4RotationMatrix*           fRotationZ45;
        G4RotationMatrix*           fRotationZ90;
        G4RotationMatrix*           fRotationY180;

        G4double                    worX, worY, worZ;
        G4double                    interval, zpos, ypos;
        G4double                    putFx, pos;
        G4double                    pipeInr, pipeOutr, pipeLength;
        G4double                    Lmod;
        G4double                    basez;
        G4double                    dz;


        G4double                    par[5];
        G4double                    zdcPar[3];
                                    
        G4double                    iplPar[3];
        G4double                    wplPar[3];
        G4double                    gapper[3];
        G4double                    fcpPar[3];
        G4double                    rcPar[3];
        G4double                    wcntPar[3];
        G4double                    fibPar[3];
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
        G4bool                      checkOverlaps;

        // BOOL VARIABLES FOR SETTING SENSITIVE DETECTOR
        G4bool                      SDforWInZDC;
        G4bool                      SDforI_PL;
        G4bool                      SDforPMMA;    
        G4bool                      SDforSMD;     
        G4bool                      SDforWInARM1; 
        G4bool                      SDforHolder; 
        G4bool                      SDforGSOBar;  
        G4bool                      SDforGSOPlate;
        G4bool                      SDforLightGuide;
        G4bool                      SDforFrame;
        G4bool                      SDforPanels;

        G4double                    Nlay,Nmod;









        // PARAMETER FOR ARM1


        G4double                    kARM1par[3];
        G4double                    kNegativeLargeWpar[3];
        G4double                    kNegativeSmallWpar[3];



        

};

#endif
