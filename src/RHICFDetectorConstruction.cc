#include "RHICFDetectorConstruction.hh"
#include "MagneticField.hh"
#include "RHICFMaterials.hh"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "G4PVReplica.hh"
#include "G4VSolid.hh"
#include "G4TwoVector.hh"
#include "G4ExtrudedSolid.hh"
#include "G4Polyhedra.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4FieldManager.hh"
#include "G4AutoDelete.hh"
#include "G4GenericMessenger.hh"
#include "G4Cons.hh"
#include "G4SDManager.hh"
#include "G4TransportationManager.hh"
#include "G4AutoDelete.hh"
#include "G4VSensitiveDetector.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4Colour.hh"
#include "G4SDParticleFilter.hh"
#include "G4PSFlatSurfaceCurrent.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSTrackLength.hh"
#include "G4PSFlatSurfaceFlux.hh"
#include "G4PSNofSecondary.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4UserLimits.hh"

using namespace CLHEP;
G4ThreadLocal MagneticField* RHICFDetectorConstruction::fMagneticField = 0;
G4ThreadLocal G4FieldManager* RHICFDetectorConstruction::fFieldMgr = 0;


//Kinds of Detector: ZDC(PHENIX), STARZDC, BBC, STARBBC, PIPE


RHICFDetectorConstruction::RHICFDetectorConstruction ( ):G4VUserDetectorConstruction(),  fVisAttributes()
{
    DefineDimension();
}

RHICFDetectorConstruction::~RHICFDetectorConstruction ( )
{
    delete fMaterials;
}

G4VPhysicalVolume* RHICFDetectorConstruction::Construct ( )
{

    /*-*/fMaterials          = RHICFMaterials::GetInstance();

    //Junsang****G4LogicalSkinSurface::CleanSurfaceTable();
    //Junsang****G4LogicalBorderSurface::CleanSurfaceTable();
    //Junsang****G4GeometryManager::GetInstance()-> OpenGeometry();
    //
    /*-*/G4PhysicalVolumeStore::GetInstance()    -> Clean();
    /*-*/G4LogicalVolumeStore::GetInstance()     -> Clean();
    /*-*/G4SolidStore::GetInstance()             -> Clean();
    /*-*/// Option to switch on/off checking of volumes overlaps
    /*-*/checkOverlaps           = false;

    /*-*/// geometries --------------------------------------------------------------
    /*-*/// Define World Volume
    /*-*/fWorldSolid             = new G4Box("WorldSolid", 10*worX*cm, 10*worY*cm, 10*worZ*cm);
    /*-*/fWorldLogical           = new G4LogicalVolume(fWorldSolid, FindMaterial("G4_AIR"), "WorldLogical");
    /*-*/fWorldPhysical          = new G4PVPlacement(0, G4ThreeVector(), fWorldLogical, "WorldPhysical", 0, false, checkOverlaps);
    /*-*/// Setting for color of world volume
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    /*-*/visAttributes           -> SetVisibility(false);
    /*-*/fWorldLogical           -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);


    /*-*/// Define Rotation matrix
    /*-*/SMDVRotation            = new G4RotationMatrix();
    /*-*/SMDHRotation          = new G4RotationMatrix();
    /*-*/SMDH2Rotation          = new G4RotationMatrix();
    /*-*/fSMDRotation            = new G4RotationMatrix();
    /*-*/fSMDRotation            -> rotateY(90*deg);
    /*-*/HODORotation            = new G4RotationMatrix();
    /*-*/HODORotation            -> rotateZ(90*deg);
    /*-*/INVERSERotation         = new G4RotationMatrix();
    /*-*/INVERSERotation         -> rotateY(180*deg);
    /*-*/MagRotation             = new G4RotationMatrix();
    /*-*/MagRotation             -> rotateX(90.*deg);
    /*-*/PipeRotation1           = new G4RotationMatrix();
    /*-*/PipeRotation1           -> rotateY(1.074*deg);
    /*-*/PipeRotation2           = new G4RotationMatrix();
    /*-*/PipeRotation2           -> rotateY(-1.074*deg);
    /*-*/fNonRotation            = new G4RotationMatrix();
    /*-*/fNonRotation            -> rotateX(0*deg);
    /*-*/fQPhi                   = new G4RotationMatrix();
    /*-*/fQPhi                   -> rotateX(360/8*deg);
    /*-*/GAPFRotation            = new G4RotationMatrix();
    /*-*/GAPFRotation            -> rotateX(27*5*deg);
    /*-*/fRotationY180            = new G4RotationMatrix();
    /*-*/fRotationY180            -> rotateY(180*deg);
    /*-*/fRotationZ45            = new G4RotationMatrix();
    /*-*/fRotationZ45            -> rotateZ(-45*deg);
    /*-*/fRotationZ90            = new G4RotationMatrix();
    /*-*/fRotationZ90            -> rotateZ(90*deg);



    // Define 'LOCALPOL-PHENIX ZDC'
    // LOCALPOL is space which contains 'ZDC', 'RCSC', 'FCSC'
    //
    // Structure of 'LOCALPOL'
    // 
    // 'LOCALPOL'-----Nmods of 'ZDC'----26 of 'GAPF's----200 of 'FIBR's
    //          |             |
    //          |             |_27 of 'W_PL's
    //          |             |
    //          |             |-2 of 'I_PL's
    //          |
    //          |
    //          |_'FCSC'
    //          |
    //          |
    //          |_'RCSC'
    //
    //
    //








    //G4ThreeVector a         = G4ThreeVector(towposX[1]-4.5*mm,-towposY[1]-12.5*mm,0);
    //G4ThreeVector a         = G4ThreeVector(towposX[1]-4.5*mm,-towposY[1]-12.5*mm+25*mm,0);
    //G4ThreeVector a         = G4ThreeVector(towposX[1]-4.5*mm,-towposY[1]-12.5*mm+42.5*mm,0);
    //G4ThreeVector a         = G4ThreeVector(towposX[1]-4.5*mm,-towposY[1]-12.5*mm+57.5*mm,0);
    //G4ThreeVector a         = G4ThreeVector(towposX[1]-4.5*mm,-towposY[1]-12.5*mm+72.5*mm,0);

    //Junsang****G4ThreeVector b         = G4ThreeVector(0,-4.05/sqrt(2.)*mm,-1860*cm);
    G4ThreeVector b         = G4ThreeVector(0*cm,-4.05/sqrt(2.)*mm,-90*cm);
    //Junsang****LOCALPOLINSTALL         = LOCALPOL(fWorldPhysical, b, fNonRotation);
    //Junsang****STARZDCINSTALL         = STARZDC(fWorldPhysical, b, fNonRotation);

    ARM1INSTALL              = ARM1(fWorldPhysical, G4ThreeVector(), fNonRotation);

    //BBCINSTALL              = BBC(fWorldPhysical, a, fNonRotation);

    //PIPEINSTALL             = PIPE();


    return                  fWorldPhysical;

}


void RHICFDetectorConstruction::DefineDimension()
{


    /*-*/// Setting for sensitive detectors
    /*-*/// ZDC
    /*-*/SDforWInZDC         = false;
    /*-*/SDforI_PL           = false;
    /*-*/SDforPMMA           = false;
    /*-*/SDforSMD            = false;
    // ARM1
    /*-*/SDforWInARM1        = true;
    /*-*/SDforHolder        = true;
    /*-*/SDforGSOBar         = true;
    /*-*/SDforGSOPlate       = true;
    /*-*/SDforFrame        = true;
    /*-*/SDforPanels         = true;


    /*-*/// Define parameters for geometry
    /*-*/smdHeight           = smdPar[2];
    /*-*/sin                 = std::sqrt(2.0)/2.0;
    /*-*/cos                 = std::sqrt(2.0)/2.0;
    /*-*/tan                 = 1.0;
    /*-*/Nlay                = 26;
    /*-*/iplPar[0]           = 5.0;
    /*-*/iplPar[1]           = 0.025;
    /*-*/iplPar[2]           = 20.0;
    /*-*/wplPar[0]           = 5.0;
    /*-*/wplPar[1]           = 0.213;
    /*-*/wplPar[2]           = 9.35;
    /*-*/gapper[0]           = 5.0;
    /*-*/gapper[1]           = 0.09;
    /*-*/gapper[2]           = 20.0;
    /*-*/fcpPar[0]           = 5.080;
    /*-*/fcpPar[1]           = 12.065;
    /*-*/fcpPar[2]           = 0.3175;
    /*-*/rcPar[0]            = 4.445;
    /*-*/rcPar[1]            = 10.795;
    /*-*/rcPar[2]            = 0.6350;
    /*-*/wcntPar[0]          = 6.0;
    /*-*/wcntPar[1]          = 25.0;
    /*-*/wcntPar[2]          = 70.0;
    /*-*/fibPar[0]           = 0.0;
    /*-*/fibPar[1]           = 0.025;
    /*-*/fibPar[2]           = 20.0;
    /*-*/hodoPar[0]          = 40.0;
    /*-*/hodoPar[1]          = 40.0;
    /*-*/hodoPar[2]          = 1.0;
    /*-*/scinPar[0]          = 4.075;
    /*-*/scinPar[1]          = 40.0;
    /*-*/scinPar[2]          = 0.5;
    /*-*/smdPar[0]           = 5.5;
    /*-*/smdPar[1]           = 0.81;
    /*-*/smdPar[2]           = 9.5;
    /*-*/smdhPar[0]          = 5.25;
    /*-*/smdhPar[1]          = 0.4;
    /*-*/smdhPar[2]          = 1.0;
    /*-*/smdvPar[0]          = 0.75;
    /*-*/smdvPar[1]          = 0.4;
    /*-*/smdvPar[2]          = 8.0;
    /*-*/alPar[0]            = 12.0;
    /*-*/alPar[1]            = 3.0;
    /*-*/alPar[2]            = 40.0;
    /*-*/fePar[0]            = 5.0;
    /*-*/fePar[1]            = 1.0;
    /*-*/fePar[2]            = 40.0;
    /*-*/Lmod                = (2.0*iplPar[1] + (Nlay + 1)*wplPar[1] + Nlay*gapper[1])/sin;
    /*-*/worX                = 50*cm;
    /*-*/worY                = 50*cm;
    /*-*/worZ                = 50*cm;
    /*-*/pipeOutr            = 5*cm;
    /*-*/pipeInr             = 4.5*cm;
    /*-*/pipeLength          = 20*cm;
    /*-*/Nmod                = 3;
    /*-*/zdcPar[0]           = 5.0;
    /*-*/zdcPar[1]           = (gapper[2]*cos + gapper[1]*sin);
    /*-*/zdcPar[2]           = (zdcPar[1]/tan + Lmod);
    /*-*/zdcPar1             = zdcPar[1]*1.2;      
    /*-*/zdcPar2             = zdcPar[2]*1.1;
    /*-*/// Parameters for BBC
    /*-*/kBBAbsorb[0]        = 5.5;
    /*-*/kBBAbsorb[1]        = 14.5;
    /*-*/kBBAbsorb[2]        = 0.01;
    /*-*/kBBAttach[0]        = 0.0;
    /*-*/kBBAttach[1]        = 360;
    /*-*/kBBAttach[2]        = 6.0;
    /*-*/kBBAttach[3]        = 2.0;
    /*-*/kBBAttach[4]        = -0.5;
    /*-*/kBBAttach[5]        = 0.2;
    /*-*/kBBAttach[6]        = 1.4;
    /*-*/kBBAttach[7]        = 0.5;
    /*-*/kBBAttach[8]        = 0.2;
    /*-*/kBBAttach[9]        = 1.4;
    /*-*/kBBBackBD[0]        = 5.5;
    /*-*/kBBBackBD[1]        = 15.0;
    /*-*/kBBBackBD[2]        = 0.5;
    /*-*/kBBBreede[0]        = 0.0;
    /*-*/kBBBreede[1]        = 1.2;
    /*-*/kBBBreede[2]        = 1.95;
    /*-*/kBBCovert           = 0.2;
    /*-*/kBBFrontb[0]        = 5.5;
    /*-*/kBBFrontb[1]        = 15.0;
    /*-*/kBBFrontb[2]        = 0.5;
    /*-*/kBBPMTSiz[0]        = 1.09;
    /*-*/kBBPMTSiz[1]        = 1.29;
    /*-*/kBBPMTSiz[2]        = 2.2;
    /*-*/kBBQuartz[0]        = 0.0;
    /*-*/kBBQuartz[1]        = 360;
    /*-*/kBBQuartz[2]        = 6.0;
    /*-*/kBBQuartz[3]        = 2.0;
    /*-*/kBBQuartz[4]        = -1.5;
    /*-*/kBBQuartz[5]        = 0.0;
    /*-*/kBBQuartz[6]        = 1.27;
    /*-*/kBBQuartz[7]        = 1.5;
    /*-*/kBBQuartz[8]        = 0.0;
    /*-*/kBBQuartz[9]        = 1.27;
    /*-*/kBBShithi           = 0.1;
    /*-*/kBBSpacin           = 0.3;
    /*-*/kBBStruct[0]        = 5.0;
    /*-*/kBBStruct[1]        = 5.5;
    /*-*/kBBStruct[2]        = 12.5;
    /*-*/kBBZPosit[0]        = 144.35;
    /*-*/kBBZPosit[1]        = -144.35;
    /*-*/kBBDetect[0]        = kBBQuartz[0];
    /*-*/kBBDetect[1]        = kBBQuartz[1];
    /*-*/kBBDetect[2]        = kBBQuartz[2];
    /*-*/kBBDetect[3]        = kBBQuartz[3];
    /*-*/kBBDetect[4]        = kBBAttach[4]+kBBQuartz[4]-kBBPMTSiz[2]-kBBBreede[2];
    /*-*/kBBDetect[5]        = kBBQuartz[5];
    /*-*/kBBDetect[6]        = kBBAttach[6];
    /*-*/kBBDetect[7]        = kBBAttach[7]+kBBQuartz[7]+kBBPMTSiz[2]+kBBBreede[2];
    /*-*/kBBDetect[8]        = kBBQuartz[8];
    /*-*/kBBDetect[9]        = kBBAttach[9];
    /*-*/kBBBCover[0]        = kBBFrontb[1];
    /*-*/kBBBCover[1]        = kBBBCover[0]+kBBCovert;
    /*-*/kBBBCover[2]        = kBBStruct[2];
    /*-*/kBBMother[0]        = kBBStruct[0]; 
    /*-*/kBBMother[1]        = kBBBCover[1];
    /*-*/kBBMother[2]        = kBBStruct[2];
    /*-*/kBBShield[0]        = kBBQuartz[0];
    /*-*/kBBShield[1]        = kBBQuartz[1];
    /*-*/kBBShield[2]        = kBBQuartz[2];
    /*-*/kBBShield[3]        = kBBQuartz[3];
    /*-*/kBBShield[4]        = kBBQuartz[4]-kBBPMTSiz[2]-kBBBreede[2];
    /*-*/kBBShield[5]        = kBBAttach[6]-kBBShithi;
    /*-*/kBBShield[6]        = kBBAttach[6];
    /*-*/kBBShield[7]        = kBBQuartz[7]+kBBPMTSiz[2]+kBBBreede[2];
    /*-*/kBBShield[8]        = kBBAttach[9]-kBBShithi;
    /*-*/kBBShield[9]        = kBBAttach[9];
    /*-*/kYSTP               = kBBQuartz[6]+kBBSpacin*0.5;
    /*-*/kXSTP               = kYSTP*std::sqrt(3.0);
    /*-*/kMrow               = (int)(kBBFrontb[1]/kYSTP)+1;
    /*-*/kMcol               = (int)(kBBFrontb[1]/kXSTP)+1;
    /*-*/kRmax               = kBBFrontb[1]-kBBDetect[6]*2./sqrt(3);
    /*-*/kRmin               = kBBFrontb[0]-kBBDetect[6]*2./sqrt(3);
    /*-*/kRRMA               = kRmax*kRmax;
    /*-*/kRRMI               = kRmin*kRmin;
    /*-*/kMaxPMT             = 100;
    /*-*/// Parameters for ARM1
    /*-*/kARM1par[0] = 4.5;
    /*-*/kARM1par[1] = 12.65;
    /*-*/kARM1par[2] = 14;
    /*-*/kNegativeLargeWpar[0] = 4.012/2;
    /*-*/kNegativeLargeWpar[1] = 4.012/2;
    /*-*/kNegativeLargeWpar[2] = 7.2/2;
    /*-*/kNegativeSmallWpar[0] = 2.012/2;
    /*-*/kNegativeSmallWpar[1] = 2.012/2;
    /*-*/kNegativeSmallWpar[2] = 7.2/2;

    /*-*/basez = 116.75;
    /*-*/dz  = 0;
}


void RHICFDetectorConstruction::ConstructSDandField()
{
    //Junsang****G4SDManager::GetSDMpointer() -> SetVerboseLevel(1);
    //Junsang****G4SDParticleFilter* OPFilter = new G4SDParticleFilter("NOP", "opticalphoton");
    //Junsang****
    //Junsang****G4String detName;
    //Junsang****
    //Junsang****/*-*/// Setting for sensitive detector of ZDC
    //Junsang****/*-*/G4String calNameForZDC[9] = {"W_PL_1Logical", "W_PL_2Logical", "W_PL_3Logical", "I_PLLogical", "GAPF_1Logical", "GAPF_2Logical", "GAPF_3Logical", "SMDHLogical", "SMDVLogical"}; 
    //Junsang****/*-*/// Set sensitive detectors for ZDC
    //Junsang****/*-*/for(G4int i=0; i<9; i++)
    //Junsang****/*-*/{
    //Junsang****/*-*/
    //Junsang****/*-*/
    //Junsang****/*-*/
    //Junsang****/*-*/    if(i<3)
    //Junsang****/*-*/    {
    //Junsang****/*-*/        if(SDforWInZDC==true)
    //Junsang****/*-*/        {
    //Junsang****/*-*/            // Sensitive detector for deposit energy
    //Junsang****/*-*/            G4cout << "detector:" << calNameForZDC[i] << G4endl;
    //Junsang****/*-*/            G4MultiFunctionalDetector* SDforDE = new G4MultiFunctionalDetector(calNameForZDC[i]);
    //Junsang****/*-*/
    //Junsang****/*-*/            G4VPrimitiveScorer* PriDEL0;
    //Junsang****/*-*/
    //Junsang****/*-*/            PriDEL0 = new G4PSEnergyDeposit("DE",0);
    //Junsang****/*-*/            SDforDE -> RegisterPrimitive(PriDEL0);
    //Junsang****/*-*/
    //Junsang****/*-*/            SetSensitiveDetector(calNameForZDC[i], SDforDE);
    //Junsang****/*-*/        }
    //Junsang****/*-*/
    //Junsang****/*-*/
    //Junsang****/*-*/
    //Junsang****/*-*/    }
    //Junsang****/*-*/
    //Junsang****/*-*/    if(i==3)
    //Junsang****/*-*/    {
    //Junsang****/*-*/        if(SDforI_PL==true)
    //Junsang****/*-*/        {
    //Junsang****/*-*/            // Sensitive detector for deposit energy
    //Junsang****/*-*/            G4cout << "detector:" << calNameForZDC[i] << G4endl;
    //Junsang****/*-*/            G4MultiFunctionalDetector* SDforDE = new G4MultiFunctionalDetector(calNameForZDC[i]);
    //Junsang****/*-*/
    //Junsang****/*-*/            G4VPrimitiveScorer* PriDEL0;
    //Junsang****/*-*/
    //Junsang****/*-*/            PriDEL0 = new G4PSEnergyDeposit("DE",0);
    //Junsang****/*-*/            SDforDE -> RegisterPrimitive(PriDEL0);
    //Junsang****/*-*/
    //Junsang****/*-*/            SetSensitiveDetector(calNameForZDC[i], SDforDE);
    //Junsang****/*-*/
    //Junsang****/*-*/        }
    //Junsang****/*-*/
    //Junsang****/*-*/    }
    //Junsang****/*-*/
    //Junsang****/*-*/    if(i<7 && i>3)
    //Junsang****/*-*/    {
    //Junsang****/*-*/
    //Junsang****/*-*/            // Sensitive detector for deposit energy and number of optical photon generated
    //Junsang****/*-*/            G4cout << "detector:" << calNameForZDC[i] << G4endl;
    //Junsang****/*-*/            G4MultiFunctionalDetector* SDforDE = new G4MultiFunctionalDetector(calNameForZDC[i]);
    //Junsang****/*-*/
    //Junsang****/*-*/            G4VPrimitiveScorer* PriDEandNoP;
    //Junsang****/*-*/
    //Junsang****/*-*/            PriDEandNoP = new G4PSEnergyDeposit("DE",0);
    //Junsang****/*-*/            SDforDE -> RegisterPrimitive(PriDEandNoP);
    //Junsang****/*-*/            PriDEandNoP = new G4PSNofSecondary("NOP",0);
    //Junsang****/*-*/            PriDEandNoP -> SetFilter(OPFilter);
    //Junsang****/*-*/            SDforDE -> RegisterPrimitive(PriDEandNoP);
    //Junsang****/*-*/
    //Junsang****/*-*/            SetSensitiveDetector(calNameForZDC[i], SDforDE);
    //Junsang****/*-*/    }
    //Junsang****/*-*/
    //Junsang****/*-*/
    //Junsang****/*-*/    if(i>6)
    //Junsang****/*-*/    {
    //Junsang****/*-*/
    //Junsang****/*-*/            // Sensitive detector for deposit energy and number of optical photon generated
    //Junsang****/*-*/            G4cout << "detector:" << calNameForZDC[i] << G4endl;
    //Junsang****/*-*/            G4MultiFunctionalDetector* SDforDE = new G4MultiFunctionalDetector(calNameForZDC[i]);
    //Junsang****/*-*/
    //Junsang****/*-*/            G4VPrimitiveScorer* PriDEandNoP;
    //Junsang****/*-*/
    //Junsang****/*-*/            PriDEandNoP = new G4PSEnergyDeposit("DE",0);
    //Junsang****/*-*/            SDforDE -> RegisterPrimitive(PriDEandNoP);
    //Junsang****/*-*/            PriDEandNoP = new G4PSNofSecondary("NOP",0);
    //Junsang****/*-*/            PriDEandNoP -> SetFilter(OPFilter);
    //Junsang****/*-*/            SDforDE -> RegisterPrimitive(PriDEandNoP);
    //Junsang****/*-*/
    //Junsang****/*-*/            SetSensitiveDetector(calNameForZDC[i], SDforDE);
    //Junsang****/*-*/    }
    //Junsang****/*-*/
    //Junsang****/*-*/
    //Junsang****/*-*/}
    //Junsang****
    //Junsang****/*-*/// Setting for sensitive detector of ARM1
    //Junsang****/*-*/G4String calNameForARM1[18] = {"LargeW_PLLogical", "SmallW_PLLogical", "WHolder_1Logical", "WHolder_2Logical", "GSO_PLHolderLogical", "GSOBarHolderLogical", "AlFrame1Logical", "AlFrame2Logical", "SidePanelLogical", "FrontPanelLogical", "LargeGSO_PLLogical", "SmallGSO_PLLogical", "LightGuideLargeLogical", "LightGuideSmallLogical", "GSORightSmallBarLogical", "GSOLeftSmallBarLogical", "GSORightLargeBarLogical", "GSOLeftLargeBarLogical"};
    //Junsang****/*-*/// Set sensitive detectors for tungsten plate
    //Junsang****/*-*/if(SDforWInARM1==true)
    //Junsang****/*-*/{
    //Junsang****/*-*/   for(G4int i=0; i<2; i++)
    //Junsang****/*-*/   {
    //Junsang****/*-*/        // Sensitive detector for deposit energy
    //Junsang****/*-*/        G4cout << "detector:" << calNameForARM1[i] << G4endl;
    //Junsang****/*-*/        G4MultiFunctionalDetector* SDforDE = new G4MultiFunctionalDetector(calNameForARM1[i]);
    //Junsang****/*-*/
    //Junsang****/*-*/        G4VPrimitiveScorer* PriDEL0;
    //Junsang****/*-*/
    //Junsang****/*-*/        PriDEL0 = new G4PSEnergyDeposit("DE",0);
    //Junsang****/*-*/        SDforDE -> RegisterPrimitive(PriDEL0);
    //Junsang****/*-*/
    //Junsang****/*-*/        SetSensitiveDetector(calNameForARM1[i], SDforDE);
    //Junsang****/*-*/   }
    //Junsang****/*-*/}
    //Junsang****/*-*/// Set sensitive detectors for holders
    //Junsang****/*-*/if(SDforHolder==true)
    //Junsang****/*-*/{
    //Junsang****/*-*/   for(G4int i=2; i<6; i++)
    //Junsang****/*-*/   {
    //Junsang****/*-*/        // Sensitive detector for deposit energy
    //Junsang****/*-*/        G4cout << "detector:" << calNameForARM1[i] << G4endl;
    //Junsang****/*-*/        G4MultiFunctionalDetector* SDforDE = new G4MultiFunctionalDetector(calNameForARM1[i]);
    //Junsang****/*-*/
    //Junsang****/*-*/        G4VPrimitiveScorer* PriDEL0;
    //Junsang****/*-*/
    //Junsang****/*-*/        PriDEL0 = new G4PSEnergyDeposit("DE",0);
    //Junsang****/*-*/        SDforDE -> RegisterPrimitive(PriDEL0);
    //Junsang****/*-*/
    //Junsang****/*-*/        SetSensitiveDetector(calNameForARM1[i], SDforDE);
    //Junsang****/*-*/   }
    //Junsang****/*-*/}
    //Junsang****/*-*/// Set sensitive detectors for frame
    //Junsang****/*-*/if(SDforFrame==true)
    //Junsang****/*-*/{
    //Junsang****/*-*/   for(G4int i=6; i<10; i++)
    //Junsang****/*-*/   {
    //Junsang****/*-*/        // Sensitive detector for deposit energy
    //Junsang****/*-*/        G4cout << "detector:" << calNameForARM1[i] << G4endl;
    //Junsang****/*-*/        G4MultiFunctionalDetector* SDforDE = new G4MultiFunctionalDetector(calNameForARM1[i]);
    //Junsang****/*-*/
    //Junsang****/*-*/        G4VPrimitiveScorer* PriDEL0;
    //Junsang****/*-*/
    //Junsang****/*-*/        PriDEL0 = new G4PSEnergyDeposit("DE",0);
    //Junsang****/*-*/        SDforDE -> RegisterPrimitive(PriDEL0);
    //Junsang****/*-*/
    //Junsang****/*-*/        SetSensitiveDetector(calNameForARM1[i], SDforDE);
    //Junsang****/*-*/   }
    //Junsang****/*-*/}
    //Junsang****/*-*/// Set sensitive detectors for GSO plate
    //Junsang****/*-*/if(SDforGSOPlate==true)
    //Junsang****/*-*/{
    //Junsang****/*-*/   for(G4int i=10; i<12; i++)
    //Junsang****/*-*/   {
    //Junsang****/*-*/            // Sensitive detector for deposit energy and number of optical photon generated
    //Junsang****/*-*/            G4cout << "detector:" << calNameForARM1[i] << G4endl;
    //Junsang****/*-*/            G4MultiFunctionalDetector* SDforDE = new G4MultiFunctionalDetector(calNameForARM1[i]);
    //Junsang****/*-*/
    //Junsang****/*-*/            G4VPrimitiveScorer* PriDEandNoP;
    //Junsang****/*-*/
    //Junsang****/*-*/            PriDEandNoP = new G4PSEnergyDeposit("DE",0);
    //Junsang****/*-*/            SDforDE -> RegisterPrimitive(PriDEandNoP);
    //Junsang****/*-*/            PriDEandNoP = new G4PSNofSecondary("NOP",0);
    //Junsang****/*-*/            PriDEandNoP -> SetFilter(OPFilter);
    //Junsang****/*-*/            SDforDE -> RegisterPrimitive(PriDEandNoP);
    //Junsang****/*-*/
    //Junsang****/*-*/            SetSensitiveDetector(calNameForARM1[i], SDforDE);
    //Junsang****/*-*/   }
    //Junsang****/*-*/}
    //Junsang****/*-*/// Set sensitive detectors for light guide
    //Junsang****/*-*/if(SDforLightGuide==true)
    //Junsang****/*-*/{
    //Junsang****/*-*/   for(G4int i=12; i<14; i++)
    //Junsang****/*-*/   {
    //Junsang****/*-*/            // Sensitive detector for deposit energy and number of optical photon generated
    //Junsang****/*-*/            G4cout << "detector:" << calNameForARM1[i] << G4endl;
    //Junsang****/*-*/            G4MultiFunctionalDetector* SDforDE = new G4MultiFunctionalDetector(calNameForARM1[i]);
    //Junsang****/*-*/
    //Junsang****/*-*/            G4VPrimitiveScorer* PriDEandNoP;
    //Junsang****/*-*/
    //Junsang****/*-*/            PriDEandNoP = new G4PSEnergyDeposit("DE",0);
    //Junsang****/*-*/            SDforDE -> RegisterPrimitive(PriDEandNoP);
    //Junsang****/*-*/            PriDEandNoP = new G4PSNofSecondary("NOP",0);
    //Junsang****/*-*/            PriDEandNoP -> SetFilter(OPFilter);
    //Junsang****/*-*/            SDforDE -> RegisterPrimitive(PriDEandNoP);
    //Junsang****/*-*/
    //Junsang****/*-*/            SetSensitiveDetector(calNameForARM1[i], SDforDE);
    //Junsang****/*-*/   }
    //Junsang****/*-*/}
    //Junsang****/*-*/// Set sensitive detectors for GSO bar
    //Junsang****/*-*/if(SDforGSOBar==true)
    //Junsang****/*-*/{
    //Junsang****/*-*/   for(G4int i=14; i<18; i++)
    //Junsang****/*-*/   {
    //Junsang****/*-*/            // Sensitive detector for deposit energy and number of optical photon generated
    //Junsang****/*-*/            G4cout << "detector:" << calNameForARM1[i] << G4endl;
    //Junsang****/*-*/            G4MultiFunctionalDetector* SDforDE = new G4MultiFunctionalDetector(calNameForARM1[i]);
    //Junsang****/*-*/
    //Junsang****/*-*/            G4VPrimitiveScorer* PriDEandNoP;
    //Junsang****/*-*/
    //Junsang****/*-*/            PriDEandNoP = new G4PSEnergyDeposit("DE",0);
    //Junsang****/*-*/            SDforDE -> RegisterPrimitive(PriDEandNoP);
    //Junsang****/*-*/            PriDEandNoP = new G4PSNofSecondary("NOP",0);
    //Junsang****/*-*/            PriDEandNoP -> SetFilter(OPFilter);
    //Junsang****/*-*/            SDforDE -> RegisterPrimitive(PriDEandNoP);
    //Junsang****/*-*/
    //Junsang****/*-*/            SetSensitiveDetector(calNameForARM1[i], SDforDE);
    //Junsang****/*-*/   }
    //Junsang****/*-*/}


    /*-*/// Set Magnetic Field
    /*-*///
    /*-*/fMagneticField          = new MagneticField();
    /*-*/fFieldMgr               = new G4FieldManager();
    /*-*/fFieldMgr               -> SetDetectorField(fMagneticField);
    /*-*/fFieldMgr               -> CreateChordFinder(fMagneticField);
    /*-*///G4bool forceToAllDaughters = true;
    /*-*///fMagneticLogical -> SetFieldManager(fFieldMgr, forceToAllDaughters);

    /*-*/G4AutoDelete::Register(fMagneticField);
    /*-*/G4AutoDelete::Register(fFieldMgr);


    /*-*/G4SDManager::GetSDMpointer() -> SetVerboseLevel(0); //Related to what level of information of geometry you want to save

}

void RHICFDetectorConstruction::ConstructMaterials ( )
{
}



//Implementation of deleting Materials
//
void RHICFDetectorConstruction::DestructMaterials ( )
{
}


G4Material* RHICFDetectorConstruction::FindMaterial(G4String name)
{
    G4Material* material                = G4Material::GetMaterial(name, true);
    return material;
}

G4VPhysicalVolume* RHICFDetectorConstruction::HODOSCOPE(G4VPhysicalVolume* world_phys, G4RotationMatrix* mat)
{
    /*-*/// Define 'HODO'
    /*-*/fHODOSolid              = new G4Box("HODOSolid", hodoPar[0]*cm, hodoPar[1]*cm, hodoPar[2]*cm);
    /*-*/fHODOLogical            = new G4LogicalVolume(fHODOSolid, FindMaterial("G4_Galactic"), "HODOLogical");
    /*-*/fHODOPhysical           = new G4PVPlacement(mat, G4ThreeVector(0.0, 0.0, 185.0*cm), "HODOPhysical", fHODOLogical, world_phys, 0, false, checkOverlaps);


    /*-*/// Define 'SCIN'
    /*-*/fSCINSolid              = new G4Box("SCINSolid", scinPar[0]*cm, scinPar[1]*cm, scinPar[2]*cm);
    /*-*/fSCINLogical            = new G4LogicalVolume(fSCINSolid, FindMaterial("G4_PLASTIC_SC_VINYLTOLUENE"), "SCINLogical");

    /*-*/// Put 'SCIN' into 'HODO'
    /*-*/for(G4int i=1; i<5; i++)
        /*-*/{
            /*-*/    pos                 = 7.4 + i*8.15 - 4.075;
            /*-*/    new G4PVPlacement(fNonRotation, G4ThreeVector(pos*cm, 0.0, 0.5*cm), fSCINLogical, "SCINPhysical", fHODOLogical, i, false, checkOverlaps);
            /*-*/    //pos = 7.4 + (i+4)*8.15 - 4.075;
            /*-*/    new G4PVPlacement(fNonRotation, G4ThreeVector(-pos*cm, 0.0, 0.5*cm), fSCINLogical, "SCINPhysical", fHODOLogical, i, false, checkOverlaps);
            /*-*/    //pos = 7.4 + (i+8)*8.15 - 4.075;
            /*-*/    new G4PVPlacement(HODORotation, G4ThreeVector(0.0, pos*cm, -0.5*cm), fSCINLogical, "SCINPhysical", fHODOLogical, i, false, checkOverlaps);
            /*-*/    //pos = 7.4 + (i+12)*8.15 - 4.075;
            /*-*/    new G4PVPlacement(HODORotation, G4ThreeVector(0.0, -pos*cm, -0.5*cm), fSCINLogical, "SCINPhysical", fHODOLogical, i, false, checkOverlaps);
            /*-*/
            /*-*/
            /*-*/    visAttributes       = new G4VisAttributes(G4Colour(0.0, 0.0, 0.1));
            /*-*/    visAttributes       -> SetVisibility(false);
            /*-*/    fHODOLogical        -> SetVisAttributes(visAttributes);
            /*-*/    fVisAttributes.push_back(visAttributes);
            /*-*/
            /*-*/    visAttributes       = new G4VisAttributes(G4Colour(0.7, 0.4, 0.1));
            /*-*/    fSCINLogical        -> SetVisAttributes(visAttributes);
            /*-*/    fVisAttributes.push_back(visAttributes);
            /*-*/}

    return fHODOPhysical;
}

G4VPhysicalVolume* RHICFDetectorConstruction::LOCALPOL(G4VPhysicalVolume* world_phys, G4ThreeVector vector, G4RotationMatrix* mat)
{

    /*-*/fLOCALPOLSolid              = new G4Box("LOCALPOLSolid", wcntPar[0]*cm, wcntPar[1]*cm, wcntPar[2]*3.6/5*cm);
    /*-*/fLOCALPOLLogical            = new G4LogicalVolume(fLOCALPOLSolid, FindMaterial("G4_AIR"), "LOCALPOLLogical");
    /*-*/fLOCALPOLPhysical           = new G4PVPlacement(mat, vector, "LOCALPOLPhysical", fLOCALPOLLogical,  world_phys, false, 0, checkOverlaps);


    /*-*///-----------------------------------------------------------------------------------------------------------
    /*-*/// Define SolidVolume
    /*-*/// Define 'ZDC'
    /*-*/fZDCSolid               = new G4Box("ZDCSolid", zdcPar[0]*cm, zdcPar1*cm, zdcPar2*cm);
    /*-*/// Define 'GAPF' : GAPF is gap which has fibers
    /*-*/fGAPFSolid              = new G4Box("GAPFSolid", gapper[0]*cm, fibPar[1]*cm, (gapper[2]+0.5)*cm);
    /*-*/// Define 'Layer between Cladding and Gap'
    /*-*/fFIBRSolid              = new G4Tubs("FIBRSolid", fibPar[0]*cm, fibPar[1]*cm, fibPar[2]/2.*cm, 0, twopi);
    /*-*/// Define 'Iron plate' : I_PL is plate which made of iron
    /*-*/fI_PLSolid              = new G4Box("I_PLSolid", iplPar[0]*cm, iplPar[1]*cm, iplPar[2]*cm);
    /*-*/// Define 'Half of PMMA fiber'
    /*-*/fFIBSolid               = new G4Tubs("FIBSolid", fibPar[0]*cm, fibPar[1]*cm, gapper[2]/2.*cm, 0, twopi);
    /*-*/// Define 'Tengsten plate' : W_PL is plate which made of tungsten
    /*-*/fW_PLSolid              = new G4Box("W_PLSolid", wplPar[0]*cm, wplPar[2]*cm, wplPar[1]*cm);
    /*-*/// Define 'Bar for eliminating optical photon propagating downward'
    /*-*/fBlockerSolid           = new G4Box("BlockerSolid", gapper[0]*cm, fibPar[1]*cm, 0.005*cm);
    /*-*/// Define 'SMD'
    /*-*/fSMDSolid               = new G4Box("SMDSolid", smdPar[0]*cm, smdPar[1]*cm, smdPar[2]*cm);
    /*-*/// Define 'SMDH'
    /*-*/fSMDHSolid     = new G4Box("SMDHSolid", smdhPar[0]*cm, smdhPar[1]*cm, smdhPar[2]*cm);
    /*-*/// Define 'FEPL'
    /*-*/fFEPLSolid              = new G4Box("FEPLSolid", fePar[0]*cm, fePar[1]*cm, fePar[2]);
    /*-*/// Define 'ALPL' 
    /*-*/fALPLSolid              = new G4Box("ALPLSolid",alPar[0]*cm, alPar[1]*cm, alPar[2]*cm);
    /*-*/// Define 'SMDV'
    /*-*/fSMDVSolid              = new G4Box("SMDVSolid", smdvPar[0]*cm, smdvPar[1]*cm, smdvPar[2]*cm);

    /*-*///------------------------------------------------------------------------------------------------------------
    /*-*/// Define LogicalVolume
    /*-*/fZDCLogical             = new G4LogicalVolume(fZDCSolid, FindMaterial("G4_AIR"), "ZDCLogical");
    /*-*/fGAPF_1Logical          = new G4LogicalVolume(fGAPFSolid, FindMaterial("PMMA"), "GAPF_1Logical");
    /*-*/fGAPF_2Logical          = new G4LogicalVolume(fGAPFSolid, FindMaterial("PMMA"), "GAPF_2Logical");
    /*-*/fGAPF_3Logical          = new G4LogicalVolume(fGAPFSolid, FindMaterial("PMMA"), "GAPF_3Logical");
    /*-*/fI_PLLogical            = new G4LogicalVolume(fI_PLSolid, FindMaterial("G4_Fe"), "I_PLLogical");
    /*-*/fW_PL_1Logical          = new G4LogicalVolume(fW_PLSolid, FindMaterial("G4_W"), "W_PL_1Logical");
    /*-*/fW_PL_2Logical          = new G4LogicalVolume(fW_PLSolid, FindMaterial("G4_W"), "W_PL_2Logical");
    /*-*/fW_PL_3Logical          = new G4LogicalVolume(fW_PLSolid, FindMaterial("G4_W"), "W_PL_3Logical");
    /*-*/fSMDLogical             = new G4LogicalVolume(fSMDSolid, FindMaterial("G4_AIR"), "fSMDLogical");
    /*-*/// Horizontal smd bar
    /*-*/fSMDHLogical            = new G4LogicalVolume(fSMDHSolid, FindMaterial("G4_PLASTIC_SC_VINYLTOLUENE"), "SMDHLogical");
    /*-*/fFEPLLogical            = new G4LogicalVolume(fFEPLSolid, FindMaterial("G4_Al"), "FEPLLogical");
    /*-*/fALPLLogical            = new G4LogicalVolume(fALPLSolid, FindMaterial("G4_Fe"), "ALPLLogical");
    /*-*/// Vertical smd bar
    /*-*/fSMDVLogical            = new G4LogicalVolume(fSMDVSolid, FindMaterial("G4_PLASTIC_SC_VINYLTOLUENE"), "SMDVLogical");

    //------------------------------------------------------------------------------------------------------------
    // Define PhysicalVolume


    /*-*/// Put gaps which include fibers into LOCALPOL
    /*-*/if(Nmod>0 && Nmod<4)
        /*-*/{
            /*-*/    for(G4int i=1; i<Nlay + 1; i++)
                /*-*/    {
                    /*-*/        ypos            = 1.85*cos;
                    /*-*/        interval        = (gapper[1]+wplPar[1])*2.0/sin;
                    /*-*/        zpos            = 8 + Lmod - (2.0*iplPar[1]+2.0*wplPar[1]+gapper[1])/sin - interval*(i-1)-1.85*cos;
                    /*-*/        if(Nmod>=1)
                        /*-*/            fGAPF_1Physical = new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]* sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fGAPF_1Logical, "GAPF_1Physical", fLOCALPOLLogical, true, i, checkOverlaps);
                    /*-*/        if(Nmod>=2)
                        /*-*/            fGAPF_2Physical = new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]* sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fGAPF_2Logical, "GAPF_2Physical", fLOCALPOLLogical, true, i, checkOverlaps);
                    /*-*/        if(Nmod>=3)
                        /*-*/            fGAPF_3Physical = new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]* sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fGAPF_3Logical, "GAPF_3Physical", fLOCALPOLLogical, true, i, checkOverlaps);
                    /*-*/    }
            /*-*/}


    /*-*/// Put front and behind iron plate into LOCALPOL
    /*-*/if(Nmod>0 && Nmod<4)
        /*-*/{
            /*-*/    ypos                = 1.85*cos;
            /*-*/    zpos                = 8 + Lmod - iplPar[1]/sin - 1.85*cos;
            /*-*/    if(Nmod>=1)
                /*-*/    {
                    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fI_PLLogical, "I_PLPhysical", fLOCALPOLLogical, false, 0, checkOverlaps);
                    /*-*/    }
            /*-*/    if(Nmod>=2)
                /*-*/    {
                    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm,(Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fI_PLLogical, "I_PLPhysical", fLOCALPOLLogical, false, 0, checkOverlaps);
                    /*-*/    }
            /*-*/    if(Nmod>=3)
                /*-*/    {
                    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fI_PLLogical, "I_PLPhysical", fLOCALPOLLogical, false, 0, checkOverlaps);
                    /*-*/    }
            /*-*/    zpos                = 8 + -Lmod + iplPar[1]/sin -1.85*cos;
            /*-*/    if(Nmod>=1)
                /*-*/    {
                    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fI_PLLogical, "I_PLPhysical", fLOCALPOLLogical, false, 0, checkOverlaps);
                    /*-*/    }
            /*-*/    if(Nmod>=2)
                /*-*/    {
                    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fI_PLLogical, "I_PLPhysical", fLOCALPOLLogical, false, 0, checkOverlaps);
                    /*-*/    }
            /*-*/    if(Nmod>=3)
                /*-*/    {
                    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fI_PLLogical, "I_PLPhysical", fLOCALPOLLogical, false, 0, checkOverlaps);
                    /*-*/    }
            /*-*/}

    /*-*/// Put tungsten plates into LOCALPOL
    /*-*/ypos                     = -zdcPar[1] + wplPar[1]*sin + wplPar[2]*cos +1.85*cos;
    /*-*/interval                 = (gapper[1] + wplPar[1])*2.0/sin;
    /*-*/
    /*-*/
    /*-*/if(Nmod>0 && Nmod<4)
        /*-*/{
            /*-*/
            /*-*/    for(G4int i=1; i<Nlay+2;i++)
                /*-*/    {
                    /*-*/
                    /*-*/        zpos                = 8 + Lmod - ypos/tan - (2.0*iplPar[1] + wplPar[1])/sin - interval*(i-1);
                    /*-*/        if(Nmod>=1)
                        /*-*/            fW_PL_1Physical = new G4PVPlacement(fQPhi, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fW_PL_1Logical, "W_PL_1Physical", fLOCALPOLLogical, true, i, checkOverlaps);
                    /*-*/        if(Nmod>=2)
                        /*-*/            fW_PL_2Physical = new G4PVPlacement(fQPhi, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fW_PL_2Logical, "W_PL_2Physical", fLOCALPOLLogical, true, i, checkOverlaps);
                    /*-*/        if(Nmod>=3)
                        /*-*/            fW_PL_3Physical = new G4PVPlacement(fQPhi, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fW_PL_3Logical, "W_PL_3Physical", fLOCALPOLLogical, true, i, checkOverlaps);
                    /*-*/    }
            /*-*/}

    /*-*/// Put 'FEPL' into 'LOCALPOL'
    /*-*/ypos                    = -7.7;
    /*-*/zpos                    = 8 + Lmod*Nmod + smdPar[1]/sin + Lmod/2 - 40;
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, ypos*cm, zpos*cm), fFEPLLogical, "FEPLPhysical", fLOCALPOLLogical, false, 0, checkOverlaps);

    /*-*/// Put 'ALPL' into 'LOCALPOL'
    /*-*/ypos                    = -11.7;
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, ypos*cm, zpos*cm), fALPLLogical, "ALPLPhysical", fLOCALPOLLogical, false, 0, checkOverlaps);

    /*-*/// Put 'SMDH' into 'SMD'
    /*-*/for(G4int i=1; i<9; i++)
        /*-*/{
            /*-*/    G4int j = i-1;
            /*-*/    zpos                = (4.5-i)*2.0*11.0/10.5;
            /*-*/    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, -0.405*cm, zpos*cm), fSMDHLogical, "SMDHPhysical", fSMDLogical, false, i, checkOverlaps);
            /*-*/}

    /*-*/// Put 'SMDV' into 'SMD'
    /*-*/for(G4int i=1; i<8; i++)
        /*-*/{
            /*-*/    G4int j = i-1;
            /*-*/    xpos                = (i-4)*1.5*11.0/10.5;
            /*-*/    new G4PVPlacement(fNonRotation, G4ThreeVector(xpos*cm, 0.405*cm, 0.*cm), fSMDVLogical, "SMDVPhysical", fSMDLogical, true, i, checkOverlaps);
            /*-*/}

    /*-*/// Put 'SMD' into Localpol
    /*-*/zpos                    = 8 + Lmod*(Nmod-2)+smdPar[1]/sin;
    /*-*/new G4PVPlacement(GAPFRotation, G4ThreeVector(0.0, 0.0, zpos*cm), fSMDLogical, "SMDPhysical", fLOCALPOLLogical, false, 0, checkOverlaps);




    /*-*/// ---------visualization attributes--------------------
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    /*-*/visAttributes           -> SetVisibility(false);
    /*-*/fLOCALPOLLogical        -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 0.8888, 0.0));
    /*-*/visAttributes           -> SetVisibility(false);
    /*-*///-fZDC_1Logical -> SetVisAttributes(visAttributes);
    /*-*/fSMDLogical             -> SetVisAttributes(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));
    /*-*/visAttributes           -> SetVisibility(false);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(2.0, 2.0, 0)); //LighteGray
    /*-*/fW_PL_1Logical          -> SetVisAttributes(visAttributes);
    /*-*/fW_PL_2Logical          -> SetVisAttributes(visAttributes);
    /*-*/fW_PL_3Logical          -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/fSMDHLogical            -> SetVisAttributes(visAttributes);
    /*-*/fGAPF_1Logical          -> SetVisAttributes(visAttributes);
    /*-*/fGAPF_2Logical          -> SetVisAttributes(visAttributes);
    /*-*/fGAPF_3Logical          -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));//Lightgreen
    /*-*/fI_PLLogical            -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 0.0 ,0.0));
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 5.0, 5.0));
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 10.0, 0.0));
    /*-*/fSMDHLogical            -> SetVisAttributes(visAttributes);
    /*-*/fSMDVLogical            -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);

    return                  fLOCALPOLPhysical;
}

G4VPhysicalVolume* RHICFDetectorConstruction::STARZDC(G4VPhysicalVolume* world_phys, G4ThreeVector vector, G4RotationMatrix* mat)
{

    /*-*/fSTARZDCSolid              = new G4Box("STARZDCSolid", wcntPar[0]*cm, wcntPar[1]*cm, wcntPar[2]*3.6/5*cm);
    /*-*/fSTARZDCLogical            = new G4LogicalVolume(fSTARZDCSolid, FindMaterial("G4_AIR"), "STARZDCLogical");
    /*-*/fSTARZDCPhysical           = new G4PVPlacement(mat, vector, "STARZDCPhysical", fSTARZDCLogical,  world_phys, false, 0, checkOverlaps);

    //-----------------------------------------------------------------------------------------------------------
    // Define SolidVolume

    /*-*/// Define 'ZDC'
    /*-*/fZDCSolid               = new G4Box("ZDCSolid", zdcPar[0]*cm, zdcPar1*cm, zdcPar2*cm);
    /*-*/// Define 'GAPF' : GAPF is gap which has fibers
    /*-*/fGAPFSolid              = new G4Box("GAPFSolid", gapper[0]*cm, fibPar[1]*cm, (gapper[2]+0.5)*cm);
    /*-*/// Define 'Layer between Cladding and Gap'
    /*-*/fFIBRSolid              = new G4Tubs("FIBRSolid", fibPar[0]*cm, fibPar[1]*cm, fibPar[2]/2.*cm, 0, twopi);
    /*-*/// Define 'Iron plate' : I_PL is plate which made of iron
    /*-*/fI_PLSolid              = new G4Box("I_PLSolid", iplPar[0]*cm, iplPar[1]*cm, iplPar[2]*cm);
    /*-*/// Define 'Half of PMMA fiber'
    /*-*/fFIBSolid               = new G4Tubs("FIBSolid", fibPar[0]*cm, fibPar[1]*cm, gapper[2]/2.*cm, 0, twopi);
    /*-*/// Define 'Tengsten plate' : W_PL is plate which made of tungsten
    /*-*/fW_PLSolid              = new G4Box("W_PLSolid", wplPar[0]*cm, wplPar[2]*cm, wplPar[1]*cm);
    /*-*/// Define 'Bar for eliminating optical photon propagating downward'
    /*-*/fBlockerSolid           = new G4Box("BlockerSolid", gapper[0]*cm, fibPar[1]*cm, 0.005*cm);
    /*-*/// Define 'SMDVStrip'
    /*-*/std::vector<G4TwoVector> VectorForEdge;
    /*-*/G4TwoVector EdgeOnQuadrant = {3.5*mm,5.0*mm};
    /*-*/VectorForEdge.push_back(EdgeOnQuadrant);
    /*-*/EdgeOnQuadrant = {3.5*mm,-5.0*mm};
    /*-*/VectorForEdge.push_back(EdgeOnQuadrant);
    /*-*/EdgeOnQuadrant = {-3.5*mm,0*mm};
    /*-*/VectorForEdge.push_back(EdgeOnQuadrant);
    /*-*/G4ExtrudedSolid::ZSection Bottom(-9*cm,{0,0},1);
    /*-*/G4ExtrudedSolid::ZSection Top(9*cm,{0,0},1);
    /*-*/std::vector<G4ExtrudedSolid::ZSection> zsections;
    /*-*/zsections.push_back(Bottom);
    /*-*/zsections.push_back(Top);
    /*-*/fSMDVStripSolid          = new G4ExtrudedSolid("SMDStripSolid", VectorForEdge, zsections);
    /*-*/// Define 'SMDHStrip'
    /*-*/G4ExtrudedSolid::ZSection HBottom(-5.5*cm,{0,0},1);
    /*-*/G4ExtrudedSolid::ZSection HTop(5.5*cm,{0,0},1);
    /*-*/std::vector<G4ExtrudedSolid::ZSection> Hzsections;
    /*-*/Hzsections.push_back(HBottom);
    /*-*/Hzsections.push_back(HTop);
    /*-*/fSMDHStripSolid          = new G4ExtrudedSolid("SMDHStripSolid", VectorForEdge, Hzsections);
    /*-*/// Define 'SMD'
    /*-*/fSMDSolid               = new G4Box("SMDSolid", smdPar[0]*cm, smdPar[1]*cm, smdPar[2]*cm);
    /*-*/// Define 'FEPL'
    /*-*/fFEPLSolid              = new G4Box("FEPLSolid", fePar[0]*cm, fePar[1]*cm, fePar[2]);
    /*-*/// Define 'ALPL' 
    /*-*/fALPLSolid              = new G4Box("ALPLSolid",alPar[0]*cm, alPar[1]*cm, alPar[2]*cm);




    /*-*///------------------------------------------------------------------------------------------------------------
    /*-*/// Define LogicalVolume
    /*-*/fZDCLogical             = new G4LogicalVolume(fZDCSolid, FindMaterial("G4_AIR"), "ZDCLogical");
    /*-*/fGAPF_1Logical          = new G4LogicalVolume(fGAPFSolid, FindMaterial("PMMA"), "GAPF_1Logical");
    /*-*/fGAPF_2Logical          = new G4LogicalVolume(fGAPFSolid, FindMaterial("PMMA"), "GAPF_2Logical");
    /*-*/fGAPF_3Logical          = new G4LogicalVolume(fGAPFSolid, FindMaterial("PMMA"), "GAPF_3Logical");
    /*-*/fI_PLLogical            = new G4LogicalVolume(fI_PLSolid, FindMaterial("G4_Fe"), "I_PLLogical");
    /*-*/fW_PL_1Logical          = new G4LogicalVolume(fW_PLSolid, FindMaterial("G4_W"), "W_PL_1Logical");
    /*-*/fW_PL_2Logical          = new G4LogicalVolume(fW_PLSolid, FindMaterial("G4_W"), "W_PL_2Logical");
    /*-*/fW_PL_3Logical          = new G4LogicalVolume(fW_PLSolid, FindMaterial("G4_W"), "W_PL_3Logical");
    /*-*/fSMDLogical             = new G4LogicalVolume(fSMDSolid, FindMaterial("G4_AIR"), "fSMDLogical");
    /*-*/// Horizontal smd bar
    /*-*/fSMDHLogical            = new G4LogicalVolume(fSMDHStripSolid, FindMaterial("G4_PLASTIC_SC_VINYLTOLUENE"), "SMDHLogical");
    /*-*/fFEPLLogical            = new G4LogicalVolume(fFEPLSolid, FindMaterial("G4_Al"), "FEPLLogical");
    /*-*/fALPLLogical            = new G4LogicalVolume(fALPLSolid, FindMaterial("G4_Fe"), "ALPLLogical");
    /*-*/// Vertical smd bar
    /*-*/fSMDVLogical            = new G4LogicalVolume(fSMDVStripSolid, FindMaterial("G4_PLASTIC_SC_VINYLTOLUENE"), "SMDVLogical");



    /*-*///------------------------------------------------------------------------------------------------------------
    /*-*/// Define PhysicalVolume
    /*-*///
    /*-*/// Put gaps which include fibers into STARZDC
    /*-*/if(Nmod>0 && Nmod<4)
        /*-*/{
            /*-*/    for(G4int i=1; i<Nlay + 1; i++)
                /*-*/    {
                    /*-*/        ypos            = 1.85*cos;
                    /*-*/        interval        = (gapper[1]+wplPar[1])*2.0/sin;
                    /*-*/        zpos            = 8 + Lmod - (2.0*iplPar[1]+2.0*wplPar[1]+gapper[1])/sin - interval*(i-1)-1.85*cos;
                    /*-*/        if(Nmod>=1)
                        /*-*/            fGAPF_1Physical = new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]* sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fGAPF_1Logical, "GAPF_1Physical", fSTARZDCLogical, true, i, checkOverlaps);
                    /*-*/        if(Nmod>=2)
                        /*-*/            fGAPF_2Physical = new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]* sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fGAPF_2Logical, "GAPF_2Physical", fSTARZDCLogical, true, i, checkOverlaps);
                    /*-*/        if(Nmod>=3)
                        /*-*/            fGAPF_3Physical = new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]* sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fGAPF_3Logical, "GAPF_3Physical", fSTARZDCLogical, true, i, checkOverlaps);
                    /*-*/    }
            /*-*/}

    /*-*/// Put front and behind iron plate into STARZDC
    /*-*/if(Nmod>0 && Nmod<4)
        /*-*/{
            /*-*/    ypos                = 1.85*cos;
            /*-*/    zpos                = 8 + Lmod - iplPar[1]/sin - 1.85*cos;
            /*-*/    if(Nmod>=1)
                /*-*/    {
                    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fI_PLLogical, "I_PLPhysical", fSTARZDCLogical, false, 0, checkOverlaps);
                    /*-*/    }
            /*-*/    if(Nmod>=2)
                /*-*/    {
                    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm,(Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fI_PLLogical, "I_PLPhysical", fSTARZDCLogical, false, 0, checkOverlaps);
                    /*-*/    }
            /*-*/    if(Nmod>=3)
                /*-*/    {
                    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fI_PLLogical, "I_PLPhysical", fSTARZDCLogical, false, 0, checkOverlaps);
                    /*-*/    }
            /*-*/    zpos                = 8 + -Lmod + iplPar[1]/sin -1.85*cos;
            /*-*/    if(Nmod>=1)
                /*-*/    {
                    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fI_PLLogical, "I_PLPhysical", fSTARZDCLogical, false, 0, checkOverlaps);
                    /*-*/    }
            /*-*/    if(Nmod>=2)
                /*-*/    {
                    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fI_PLLogical, "I_PLPhysical", fSTARZDCLogical, false, 0, checkOverlaps);
                    /*-*/    }
            /*-*/    if(Nmod>=3)
                /*-*/    {
                    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fI_PLLogical, "I_PLPhysical", fSTARZDCLogical, false, 0, checkOverlaps);
                    /*-*/    }
            /*-*/}

    /*-*/// Put tungsten plates into STARZDC
    /*-*///
    /*-*/ypos                     = -zdcPar[1] + wplPar[1]*sin + wplPar[2]*cos +1.85*cos;
    /*-*/interval                 = (gapper[1] + wplPar[1])*2.0/sin;
    /*-*/
    /*-*/
    /*-*/if(Nmod>0 && Nmod<4)
        /*-*/{
            /*-*/
            /*-*/    for(G4int i=1; i<Nlay+2;i++)
                /*-*/    {
                    /*-*/
                    /*-*/        zpos                = 8 + Lmod - ypos/tan - (2.0*iplPar[1] + wplPar[1])/sin - interval*(i-1);
                    /*-*/        if(Nmod>=1)
                        /*-*/            fW_PL_1Physical = new G4PVPlacement(fQPhi, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fW_PL_1Logical, "W_PL_1Physical", fSTARZDCLogical, true, i, checkOverlaps);
                    /*-*/        if(Nmod>=2)
                        /*-*/            fW_PL_2Physical = new G4PVPlacement(fQPhi, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fW_PL_2Logical, "W_PL_2Physical", fSTARZDCLogical, true, i, checkOverlaps);
                    /*-*/        if(Nmod>=3)
                        /*-*/            fW_PL_3Physical = new G4PVPlacement(fQPhi, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fW_PL_3Logical, "W_PL_3Physical", fSTARZDCLogical, true, i, checkOverlaps);
                    /*-*/    }
            /*-*/}

    /*-*/// Put 'FEPL' into 'STARZDC'
    /*-*/ypos                    = -7.7;
    /*-*/zpos                    = 8 + Lmod*Nmod + smdPar[1]/sin + Lmod/2 - 40;
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, ypos*cm, zpos*cm), fFEPLLogical, "FEPLPhysical", fSTARZDCLogical, false, 0, checkOverlaps);

    /*-*/// Put 'ALPL' into 'STARZDC'
    /*-*/ypos                    = -11.7;
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, ypos*cm, zpos*cm), fALPLLogical, "ALPLPhysical", fSTARZDCLogical, false, 0, checkOverlaps);


    /*-*/// Put 'SMDH' into 'SMD'
    /*-*/for(G4int i=1; i<33; i++)
        /*-*/{
            /*-*/    // Odd Number SMDHStrip
            /*-*/    if(i%2 == 1)
                /*-*/    {
                    /*-*/        zpos                = (-8.5+0.5*i);
                    /*-*/        SMDHRotation -> rotateZ(90*deg);
                    /*-*/        SMDHRotation -> rotateX(90*deg);
                    /*-*/        new G4PVPlacement(SMDHRotation, G4ThreeVector(0.0, -0.36*cm, zpos*cm), fSMDHLogical, "SMDHPhysical", fSMDLogical, false, i, checkOverlaps);
                    /*-*/
                    /*-*/    }
            /*-*/
            /*-*/    // Even Number SMDHStrip
            /*-*/    if(i%2 == 0)
                /*-*/    {
                    /*-*/        zpos                = (-8.5+0.5*i);
                    /*-*/        SMDH2Rotation -> rotateZ(-90*deg);
                    /*-*/        SMDH2Rotation -> rotateX(90*deg);
                    /*-*/        new G4PVPlacement(SMDH2Rotation, G4ThreeVector(0.0, -0.36*cm, zpos*cm), fSMDHLogical, "SMDHPhysical", fSMDLogical, false, i, checkOverlaps);
                    /*-*/
                    /*-*/    }
            /*-*/}


    /*-*/// Put 'SMDV' into 'SMD'
    /*-*/for(G4int i=1; i<21; i++)
        /*-*/{
            /*-*/    // Odd number SMDVStrip
            /*-*/    if(i%2 == 1)
                /*-*/    {
                    /*-*/        SMDVRotation            -> rotateZ(-45*deg);
                    /*-*/        xpos                = 0.5*(i-11);
                    /*-*/        new G4PVPlacement(SMDVRotation, G4ThreeVector(xpos*cm, 0.36*cm, 0.*cm), fSMDVLogical, "SMDVPhysical", fSMDLogical, true, i, checkOverlaps);
                    /*-*/    }
            /*-*/
            /*-*/    // Even number SMDVStrip
            /*-*/    if(i%2 == 0)
                /*-*/    {
                    /*-*/        SMDVRotation            -> rotateZ(45*deg);
                    /*-*/        xpos                = 0.5*i-5.5;
                    /*-*/        new G4PVPlacement(SMDVRotation, G4ThreeVector(xpos*cm, 0.36*cm, 0.*cm), fSMDVLogical, "SMDVPhysical", fSMDLogical, true, i, checkOverlaps);
                    /*-*/    }
            /*-*/}

    /*-*/// Put 'SMD' into Localpol
    /*-*/zpos                    = 8 + Lmod*(Nmod-2)+smdPar[1]/sin;
    /*-*/new G4PVPlacement(GAPFRotation, G4ThreeVector(0.0, 0.0, zpos*cm), fSMDLogical, "SMDPhysical", fSTARZDCLogical, false, 0, checkOverlaps);

    /*-*/// ---------visualization attributes--------------------
    /*-*///
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    /*-*/visAttributes           -> SetVisibility(false);
    /*-*/fSTARZDCLogical        -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 0.8888, 0.0));
    /*-*/visAttributes           -> SetVisibility(false);
    /*-*///-fZDC_1Logical -> SetVisAttributes(visAttributes);
    /*-*///Junsang****fSMDLogical             -> SetVisAttributes(visAttributes);
    /*
       fMagneticLogical -> SetVisAttributes(visAttributes);
       fVisAttributes.push_back(visAttributes);
       */
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));
    /*-*/visAttributes           -> SetVisibility(false);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(2.0, 2.0, 0)); //LighteGray
    /*-*/fW_PL_1Logical          -> SetVisAttributes(visAttributes);
    /*-*/fW_PL_2Logical          -> SetVisAttributes(visAttributes);
    /*-*/fW_PL_3Logical          -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.9, 0.9, 0.9)); //Red
    /*-*/fSMDHLogical            -> SetVisAttributes(visAttributes);
    /*-*/fGAPF_1Logical          -> SetVisAttributes(visAttributes);
    /*-*/fGAPF_2Logical          -> SetVisAttributes(visAttributes);
    /*-*/fGAPF_3Logical          -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));//Lightgreen
    /*-*/fI_PLLogical            -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 0.0 ,0.0));
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 5.0, 5.0));
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 10.0, 0.0));
    /*-*/fSMDHLogical            -> SetVisAttributes(visAttributes);
    /*-*/fSMDVLogical            -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);



    return                  fSTARZDCPhysical;


}

///////////////////////////////////////////////////////////////////////////////
G4VPhysicalVolume* RHICFDetectorConstruction::BBC(G4VPhysicalVolume* world_phys, G4ThreeVector vector, G4RotationMatrix* mat)
    ///////////////////////////////////////////////////////////////////////////////
{

    // Define Solid volume
    G4VSolid* fBBCMSolid = new G4Tubs("BBCMSolid", kBBMother[0]*cm, kBBMother[1]*cm, kBBMother[2]*cm, 0, twopi);
    G4LogicalVolume* fBBCMLogical = new G4LogicalVolume(fBBCMSolid, FindMaterial("G4_Galactic"), "BBCMLogical");


    G4VSolid* fBBCESolid = new G4Tubs("BBCESolid", kBBAbsorb[0]*cm, kBBAbsorb[1]*cm, kBBAbsorb[2]*cm, 0, twopi);
    G4LogicalVolume* fBBCELogical = new G4LogicalVolume(fBBCESolid, FindMaterial("G4_Al"), "BBCELogical");


    G4VSolid* fBBCFSolid = new G4Tubs("BBCFSolid", kBBFrontb[0]*cm, kBBFrontb[1]*cm, kBBFrontb[2]*cm, 0, twopi);
    G4LogicalVolume* fBBCFLogical = new G4LogicalVolume(fBBCFSolid, FindMaterial("G4_Al"), "BBCFLogical");

    G4VSolid* fBBCSSolid = new G4Tubs("BBCSSolid", kBBStruct[0]*cm, kBBStruct[1]*cm, kBBStruct[2]*cm, 0, twopi);
    G4LogicalVolume* fBBCSLogical = new G4LogicalVolume(fBBCSSolid, FindMaterial("G4_Al"), "BBCSLogical");


    G4VSolid* fBBCBSolid =  new G4Tubs("BBCBSolid", kBBBackBD[0]*cm, kBBBackBD[1]*cm, kBBBackBD[2]*cm, 0, twopi);
    G4LogicalVolume* fBBCBLogical =  new G4LogicalVolume(fBBCBSolid, FindMaterial("G4_Al"), "BBCBLogical");

    G4VSolid* fBBCCSolid = new G4Tubs("BBCCSolid", kBBBCover[0]*cm, kBBBCover[1]*cm, kBBBCover[2]*cm, 0, twopi);
    G4LogicalVolume* fBBCCLogical = new G4LogicalVolume(fBBCCSolid, FindMaterial("G4_Al"), "BBCCLogical");


    G4double* rmin_array = new G4double[2];
    G4double* rmax_array = new G4double[2];
    G4double* z_array = new G4double[2];

    rmin_array[0] = kBBDetect[5]*cm;
    rmin_array[1] = kBBDetect[8]*cm;

    rmax_array[0] = kBBDetect[6]*cm;
    rmax_array[1] = kBBDetect[9]*cm;

    z_array[0] = kBBDetect[4]*cm;
    z_array[1] = kBBDetect[7]*cm;



    G4VSolid* fBBCDSolid =  new G4Polyhedra("BBCDSolid", kBBDetect[0]*deg, kBBDetect[1]*deg, kBBDetect[2], kBBDetect[3], z_array, rmin_array, rmax_array);
    G4LogicalVolume* fBBCDLogical = new G4LogicalVolume(fBBCDSolid, FindMaterial("G4_Galactic"), "BBCDLogical");

    // Create volumes BBCA, BBCQ, BBCP, BBCR and BBCH for attachement, Quartz, PMT, Breeder and metal shield.
    //


    rmin_array[0] = kBBAttach[5]*cm;
    rmin_array[1] = kBBAttach[8]*cm;

    rmax_array[0] = kBBAttach[6]*cm;
    rmax_array[1] = kBBAttach[9]*cm;

    z_array[0] = kBBAttach[4]*cm;
    z_array[1] = kBBAttach[7]*cm;


    G4VSolid* fBBCASolid = new G4Polyhedra("BBCASolid", kBBAttach[0]*deg, kBBAttach[1]*deg, kBBAttach[2], kBBAttach[3], z_array, rmin_array, rmax_array);
    G4LogicalVolume* fBBCALogical = new G4LogicalVolume(fBBCASolid, FindMaterial("G4_Fe"), "BBCALogical");


    rmin_array[0] = kBBQuartz[5]*cm;
    rmin_array[1] = kBBQuartz[8]*cm;

    rmax_array[0] = kBBQuartz[6]*cm;
    rmax_array[1] = kBBQuartz[9]*cm;

    z_array[0] = kBBQuartz[4]*cm;
    z_array[1] = kBBQuartz[7]*cm;

    G4VSolid* fBBCQSolid = new G4Polyhedra("BBCQSolid", kBBQuartz[0]*deg, kBBQuartz[1]*deg, kBBQuartz[2], kBBQuartz[3], z_array, rmin_array, rmax_array);
    G4LogicalVolume* fBBCQLogical = new G4LogicalVolume(fBBCQSolid, FindMaterial("Quartz"), "BBCQLogical");


    G4VSolid* fBBCPSolid = new G4Tubs("BBCPSolid", kBBPMTSiz[0]*cm, kBBPMTSiz[1]*cm, kBBPMTSiz[2]*cm, 0, twopi);
    fBBCPLogical = new G4LogicalVolume(fBBCPSolid, FindMaterial("Quartz"), "BBCPLogical");

    G4VSolid* fBBCRSolid = new G4Tubs("BBCRSolid", kBBBreede[0]*cm, kBBBreede[1]*cm, kBBBreede[2]*cm, 0, twopi);
    fBBCRLogical = new G4LogicalVolume(fBBCRSolid, FindMaterial("G4_Galactic"), "BBCRLogical");

    rmin_array[0] = kBBShield[5]*cm;
    rmin_array[1] = kBBShield[8]*cm;

    rmax_array[0] = kBBShield[6]*cm;
    rmax_array[1] = kBBShield[9]*cm;

    z_array[0] = kBBShield[4]*cm;
    z_array[1] = kBBShield[7]*cm;

    G4VSolid* fBBCHSolid = new G4Polyhedra("BBCHSolid", kBBShield[0]*deg, kBBShield[1]*deg, kBBShield[2], kBBShield[3], z_array, rmin_array, rmax_array);
    G4LogicalVolume* fBBCHLogical = new G4LogicalVolume(fBBCHSolid, FindMaterial("G4_Fe"), "BBCHLogical");
    // Place the BBCA, BBCQ, BBCP, BBCR and BBCH into BBCD.

    zpos = -(kBBDetect[7]-kBBDetect[4])*0.5+(kBBAttach[7]-kBBAttach[4])*0.5;

    fBBCAPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0,0,zpos*cm), fBBCALogical, "BBCAPhysical", fBBCDLogical, false, 0, checkOverlaps);

    zpos = -(kBBDetect[7]-kBBDetect[4])*0.5+(kBBAttach[7]-kBBAttach[4])+(kBBAttach[7]-kBBAttach[4])*0.5;

    fBBCQPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0,0,zpos*cm), fBBCQLogical, "BBCQPhysical", fBBCDLogical, false, 0, checkOverlaps);

    zpos = -(kBBDetect[7]-kBBDetect[4])*0.5+(kBBAttach[7]-kBBAttach[4])+(kBBAttach[7]-kBBAttach[4])*0.5+kBBPMTSiz[2]*2.0+kBBBreede[2];

    fBBCHPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0,0,zpos*cm), fBBCHLogical, "BBCHPhysical", fBBCDLogical, false, 0, checkOverlaps);

    // Place the Volumes BBCE, BBCF, BBCS, BBCB and BBCC in to BBCM.

    zpos = 0;

    fBBCSPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(), fBBCSLogical, "BBCSPhysical", fBBCMLogical, false, 0, checkOverlaps);

    zpos = -kBBMother[2]+kBBAbsorb[2];

    fBBCEPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0,0,zpos*cm), fBBCELogical, "BBCEPhysical", fBBCMLogical, false, 0, checkOverlaps);

    zpos = -kBBMother[2]+2*kBBAbsorb[2]+kBBFrontb[2];

    fBBCFPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0,0,zpos*cm), fBBCFLogical, "BBCFPhysical", fBBCMLogical, false, 0, checkOverlaps);

    zpos = kBBMother[2]-kBBBackBD[2];

    fBBCBPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0,0,zpos*cm),fBBCBLogical, "BBCBPhysical", fBBCMLogical, false, 0, checkOverlaps);

    zpos = 0;

    fBBCCPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0,0,zpos*cm),fBBCCLogical, "BBCCPhysical", fBBCMLogical, false, 0, checkOverlaps);

    zpos = -kBBMother[2]+2*kBBFrontb[2]+2*kBBAbsorb[2]+(kBBDetect[7]-kBBDetect[4])*0.5;


    kNrow = kMrow;
    kNcol = kMcol;
    kNdet = 0;
    G4double kXps[kMaxPMT];
    G4double kYps[kMaxPMT];
    G4bool kLXYP[kMaxPMT];


    while(kNrow >= -kMrow)
    {
        kLrow = abs(kNrow%2);
        ypos = kNrow*kYSTP;
        kNcol = kMcol;

        while(kNcol >= -kMcol)
        {

            kLcol = abs(kNcol%2);
            if(kLrow == kLcol)
            {
                xpos = kNcol*kXSTP;
                kRrad = xpos*xpos+ypos*ypos;

                if((kRrad <= kRRMA) && (kRrad >= kRRMI))
                {
                    //kNdet = kNdet + 1;


                    if(kNdet < kMaxPMT)
                    {
                        kXps[kNdet] = xpos;
                        kYps[kNdet] = ypos;
                        kLXYP[kNdet] = true;
                    }
                    kNdet = kNdet+1;
                }
            }

            kNcol = kNcol-1;
        }

        kNrow = kNrow-1;
    }

    kNdet=0;

    do
    {
        new G4PVPlacement(fNonRotation, G4ThreeVector(kXps[kNdet]*cm, kYps[kNdet]*cm, zpos*cm), fBBCDLogical, "BBDCPhysical", fBBCMLogical, false, 0, checkOverlaps);
        kNdet = kNdet +1;
    }while(kNdet < 78);

    fBBCMPhysical = new G4PVPlacement(mat, vector, "BBCMPhysical", fBBCMLogical, world_phys, false, 0, checkOverlaps);


    return fBBCMPhysical;

}



G4VPhysicalVolume* RHICFDetectorConstruction::PIPE()
{


    /*-*///----------------------------------------------BEAM PIPE----------------------------------------------------
    /*-*///
    /*-*///
    /*-*///
    /*-*/judgeNS = 1;
    /*-*/ivolu = 0;
    /*-*/iron = 4;
    /*-*/par[0] = 0.0;
    /*-*/par[1] = 0.1;
    /*-*/par[2] = 0.7;
    /*-*/// BEAM PIPE + STEEL SECTIONS
    /*-*///
    /*-*/// Beam pipe: vol. definitions: name shape mat. r_i r_o lz/2 #par
    /*-*///
    /*-*/par[0] = 3.71;
    /*-*/par[1] = 3.81;
    /*-*/par[2] = 600.0;
    /*-*/G4VSolid* BEAMSolid = new G4Tubs("BEAMSolid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* BEAMLogical = new G4LogicalVolume(BEAMSolid, FindMaterial("G4_Be"), "BEAMLogical");
    /*-*/par[0] = 0.0;
    /*-*/par[1] = 3.71;
    /*-*/par[2] = 600.0;
    /*-*/G4VSolid* BEAVSolid = new G4Tubs("BEAVSolid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/BEAVLogical = new G4LogicalVolume(BEAVSolid, FindMaterial("G4_Galactic"), "BEAVLogical");
    /*-*/par[0] = 3.81;
    /*-*/par[1] = 5.85;
    /*-*/par[2] = 2.0;
    /*-*/G4VSolid* FLB1Solid = new G4Tubs("FLB1Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/FLB1Logical = new G4LogicalVolume(FLB1Solid, FindMaterial("G4_Be"), "FLB1Logical");
    /*-*/G4VSolid* FLB2Solid = new G4Tubs("FLB2Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/FLB2Logical = new G4LogicalVolume(FLB2Solid, FindMaterial("G4_Be"), "FLB2Logical");
    /*-*/G4VSolid* FLS1Solid = new G4Tubs("FLS1Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/G4LogicalVolume* FLS1Logical = new G4LogicalVolume(FLS1Solid, FindMaterial("G4_Fe"), "FLS1Logical");
    /*-*/G4VSolid* FLS2Solid = new G4Tubs("FLS2Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/G4LogicalVolume* FLS2Logical = new G4LogicalVolume(FLS2Solid, FindMaterial("G4_Fe"), "FLS2Logical");
    /*-*/par[0] = 3.54;
    /*-*/par[1] = 3.81;
    /*-*/par[2] = 2.0;
    /*-*/G4VSolid* SPL1Solid = new G4Tubs("SPL1Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/G4LogicalVolume* SPL1Logical = new G4LogicalVolume(SPL1Solid, FindMaterial("G4_Fe"), "SPL1VLogical");
    /*-*/G4VSolid* SPL2Solid = new G4Tubs("SPL2Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/SPL2Logical = new G4LogicalVolume(SPL2Solid, FindMaterial("G4_Fe"), "SPL2VLogical");
    /*-*/par[0] = 0.0;
    /*-*/par[1] = 3.54;
    /*-*/par[2] = 2.0;
    /*-*/G4VSolid* SM11Solid = new G4Tubs("SM11Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/G4LogicalVolume* SM11Logical = new G4LogicalVolume(SM11Solid, FindMaterial("G4_Galactic"), "SM11Logical");
    /*-*/G4VSolid* SM12Solid = new G4Tubs("SM12Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/G4LogicalVolume* SM12Logical = new G4LogicalVolume(SM12Solid, FindMaterial("G4_Galactic"), "SM12Logical");
    /*-*/par[0] = 6.35;
    /*-*/par[1] = 8.39;
    /*-*/par[2] = 2.0;
    /*-*/G4VSolid* FLS3Solid = new G4Tubs("FLS3Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/G4LogicalVolume* FLS3Logical = new G4LogicalVolume(FLS3Solid, FindMaterial("G4_Fe"), "FLS3Logical");
    /*-*/G4VSolid* FLS4Solid = new G4Tubs("FLS4Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/FLS4Logical = new G4LogicalVolume(FLS4Solid, FindMaterial("G4_Fe"), "FLS4Logical");
    /*-*/G4VSolid* FLS5Solid = new G4Tubs("FLS5Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/FLS5Logical = new G4LogicalVolume(FLS5Solid, FindMaterial("G4_Fe"), "FLS5Logical");
    /*-*/G4VSolid* FLS6Solid = new G4Tubs("FLS6Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/FLS6Logical = new G4LogicalVolume(FLS6Solid, FindMaterial("G4_Fe"), "FLS6Logical");
    /*-*/par[0] = 6.08;
    /*-*/par[1] = 6.35;
    /*-*/par[2] = 58.5;
    /*-*/G4VSolid* SPU1Solid = new G4Tubs("SPU1Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/SPU1Logical = new G4LogicalVolume(SPU1Solid, FindMaterial("G4_Fe"), "SPU1Logical");
    /*-*/G4VSolid* SPU2Solid = new G4Tubs("SPU2Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/G4LogicalVolume* SPU2Logical = new G4LogicalVolume(SPU2Solid, FindMaterial("G4_Fe"), "SPU2Logical");
    /*-*/par[0] = 0.0;
    /*-*/par[1] = 6.08;
    /*-*/par[2] = 58.5;
    /*-*/G4VSolid* SM21Solid = new G4Tubs("SM21Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/SM21Logical = new G4LogicalVolume(SM21Solid, FindMaterial("G4_Galactic"), "SM21Logical");
    /*-*/G4VSolid* SM22Solid = new G4Tubs("SM22Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/G4LogicalVolume* SM22Logical = new G4LogicalVolume(SM22Solid, FindMaterial("G4_Galactic"), "SM22Logical");
    /*-*/par[0] = 6.35;
    /*-*/par[1] = 7.35;
    /*-*/par[2] = 0.20;
    /*-*/G4VSolid* RB11Solid = new G4Tubs("RB11Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/RB11Logical = new G4LogicalVolume(RB11Solid, FindMaterial("G4_Fe"), "RB11Logical");
    /*-*/G4VSolid* RB12Solid = new G4Tubs("RB12Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/G4LogicalVolume* RB12Logical = new G4LogicalVolume(RB12Solid, FindMaterial("G4_Fe"), "RB12Logical");
    /*-*/G4VSolid* RB13Solid = new G4Tubs("RB13Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/RB13Logical = new G4LogicalVolume(RB13Solid, FindMaterial("G4_Fe"), "RB13Logical");
    /*-*/G4VSolid* RB14Solid = new G4Tubs("RB14Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/RB14Logical = new G4LogicalVolume(RB14Solid, FindMaterial("G4_Fe"), "RB14Logical");
    /*-*/G4VSolid* RB15Solid = new G4Tubs("RB15Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/G4LogicalVolume* RB15Logical = new G4LogicalVolume(RB15Solid, FindMaterial("G4_Fe"), "RB15Logical");
    /*-*/G4VSolid* RB16Solid = new G4Tubs("RB16Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/RB16Logical = new G4LogicalVolume(RB16Solid, FindMaterial("G4_Fe"), "RB16Logical");
    /*-*/G4VSolid* RB17Solid = new G4Tubs("RB17Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/RB17Logical = new G4LogicalVolume(RB17Solid, FindMaterial("G4_Fe"), "RB17Logical");
    /*-*/G4VSolid* RB18Solid = new G4Tubs("RB18Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/G4LogicalVolume* RB18Logical = new G4LogicalVolume(RB18Solid, FindMaterial("G4_Fe"), "RB18Logical");
    /*-*/G4VSolid* RB21Solid = new G4Tubs("RB21Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/RB21Logical = new G4LogicalVolume(RB21Solid, FindMaterial("G4_Fe"), "RB21Logical");
    /*-*/G4VSolid* RB22Solid = new G4Tubs("RB22Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/RB22Logical = new G4LogicalVolume(RB22Solid, FindMaterial("G4_Fe"), "RB22Logical");
    /*-*/G4VSolid* RB23Solid = new G4Tubs("RB23Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/G4LogicalVolume* RB23Logical = new G4LogicalVolume(RB23Solid, FindMaterial("G4_Fe"), "RB23Logical");
    /*-*/G4VSolid* RB24Solid = new G4Tubs("RB24Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/G4LogicalVolume* RB24Logical = new G4LogicalVolume(RB24Solid, FindMaterial("G4_Fe"), "RB24Logical");
    /*-*/G4VSolid* RB25Solid = new G4Tubs("RB25Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/RB25Logical = new G4LogicalVolume(RB25Solid, FindMaterial("G4_Fe"), "RB25Logical");
    /*-*/G4VSolid* RB26Solid = new G4Tubs("RB26Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/G4LogicalVolume* RB26Logical = new G4LogicalVolume(RB26Solid, FindMaterial("G4_Fe"), "RB26Logical");
    /*-*/G4VSolid* RB27Solid = new G4Tubs("RB27Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/RB27Logical = new G4LogicalVolume(RB27Solid, FindMaterial("G4_Fe"), "RB27Logical");
    /*-*/G4VSolid* RB28Solid = new G4Tubs("RB28Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    /*-*/RB28Logical = new G4LogicalVolume(RB28Solid, FindMaterial("G4_Fe"), "RB28Logical");
    /*-*/par[0] = 12.5;
    /*-*/par[1] = 3.54;
    /*-*/par[2] = 3.81;
    /*-*/par[3] = 6.08;
    /*-*/par[4] = 6.35;
    /*-*/G4VSolid* CN11Solid = new G4Cons("CN11Solid", par[1]*cm, par[2]*cm, par[3]*cm, par[4]*cm, par[0]*cm, 0, 360);
    /*-*/CN11Logical = new G4LogicalVolume(CN11Solid, FindMaterial("G4_Galactic"), "CN11Logical");
    /*-*/par[0] = 12.5;
    /*-*/par[1] = 6.08;
    /*-*/par[2] = 6.35;
    /*-*/par[3] = 3.54;
    /*-*/par[4] = 3.81;
    /*-*/G4VSolid* CN12Solid = new G4Cons("CN12Solid", par[1]*cm, par[2]*cm, par[3]*cm, par[4]*cm, par[0]*cm, 0, 360);
    /*-*/CN12Logical = new G4LogicalVolume(CN12Solid, FindMaterial("G4_Galactic"), "CN12Logical");
    /*-*/par[0] = 12.5;
    /*-*/par[1] = 0.0;
    /*-*/par[2] = 3.54;
    /*-*/par[3] = 0.0;
    /*-*/par[4] = 6.08;
    /*-*/G4VSolid* CM11Solid = new G4Cons("CM11Solid", par[1]*cm, par[2]*cm, par[3]*cm, par[4]*cm, par[0]*cm, 0, 360);
    /*-*/CM11Logical = new G4LogicalVolume(CM11Solid, FindMaterial("G4_Galactic"), "CM11Logical");
    /*-*/par[0] = 12.5;
    /*-*/par[1] = 0.0;
    /*-*/par[2] = 6.08;
    /*-*/par[3] = 0.0;
    /*-*/par[4] = 3.54;
    /*-*/G4VSolid* CM12Solid = new G4Cons("CM12Solid", par[1]*cm, par[2]*cm, par[3]*cm, par[4]*cm, par[0]*cm, 0, 360);
    /*-*/CM12Logical = new G4LogicalVolume(CM12Solid, FindMaterial("G4_Galactic"), "CM12Logical");

    /*-*/// Place beam pipe
    /*-*///
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0,0,0*cm), BEAMLogical, "BEAMPhysical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(), BEAVLogical, "BEAVPhysical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 200.0*cm), FLB1Logical, "FLB1Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -200.0*cm), FLB2Logical, "FLB2Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 600.0*cm), FLS1Logical, "FLS1Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -600.0*cm), FLS2Logical, "FLS2Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 602.0*cm), SM11Logical, "SM11Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -602.0*cm), SM12Logical,"SM12Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 602.0*cm), SPL1Logical, "SPL1Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -602.0*cm), SPL2Logical, "SPL2Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 616.50*cm), CM11Logical, "CM11Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -616.50*cm), CM12Logical, "CM12Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 616.50*cm), CN11Logical, "CN11Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -616.50*cm), CN12Logical, "CN12Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 629.0*cm), FLS3Logical, "FLS3Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -629.0*cm), FLS4Logical, "FLS4Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 746.0*cm), FLS5Logical, "FLS5Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -746.0*cm), FLS6Logical, "FLS6Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 687.50*cm), SM21Logical, "SM21Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -687.50*cm), SM22Logical, "SM22Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 687.50*cm), SPU1Logical, "SPU1Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -687.50*cm), SPU2Logical, "SPU2Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 679.80*cm), RB11Logical, "RB11Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 681.95*cm), RB12Logical, "RB12Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 684.10*cm), RB13Logical, "RB13Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 686.25*cm), RB14Logical, "RB14Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 688.40*cm), RB15Logical, "RB15Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 690.55*cm), RB16Logical, "RB16Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 692.70*cm), RB17Logical, "RB17Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 694.85*cm), RB18Logical, "RB18Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -679.80*cm), RB21Logical, "RB21Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -681.95*cm), RB22Logical, "RB22Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -684.10*cm), RB23Logical, "RB23Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -686.25*cm), RB24Logical, "RB24Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -688.40*cm), RB25Logical, "RB25Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -690.55*cm), RB26Logical, "RB26Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -692.70*cm), RB27Logical, "RB27Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -694.85*cm), RB28Logical, "RB28Physical", fWorldLogical, false, 0, checkOverlaps);


    /*-*///---------------------UPSTREAM AREA------------------------
    /*-*/par[0] = 6.08;
    /*-*/par[1] = 6.35;
    /*-*/par[2] = 103.94;
    /*-*/G4VSolid* SP31Solid = new G4Tubs("SP31Solid", par[0]*cm, par[1]*cm, par[2]*cm,0 ,360*deg);
    /*-*/G4LogicalVolume* SP31Logical = new G4LogicalVolume(SP31Solid, FindMaterial("G4_Fe"), "SP31Logical");
    /*-*/G4VSolid* SP32Solid = new G4Tubs("SP32Solid", par[0]*cm, par[1]*cm, par[2]*cm,0 ,360*deg);
    /*-*/G4LogicalVolume* SP32Logical = new G4LogicalVolume(SP32Solid, FindMaterial("G4_Fe"), "SP31Logical");
    /*-*/par[0] = 0.0;
    /*-*/par[1] = 6.08;
    /*-*/par[2] = 103.94;
    /*-*/G4VSolid* SM31Solid = new G4Tubs("SM31Solid", par[0]*cm, par[1]*cm, par[2]*cm,0 ,360*deg);
    /*-*/G4LogicalVolume* SM31Logical = new G4LogicalVolume(SM31Solid, FindMaterial("G4_Galactic"), "SM31Logical");
    /*-*/G4VSolid* SM32Solid = new G4Tubs("SM32Solid", par[0]*cm, par[1]*cm, par[2]*cm,0 ,360*deg);
    /*-*/G4LogicalVolume* SM32Logical = new G4LogicalVolume(SM32Solid, FindMaterial("G4_Galactic"), "SM32Logical");
    /*-*/par[0] = 6.99;
    /*-*/par[1] = 7.14;
    /*-*/par[2] = 210.92;
    /*-*/G4VSolid* SP41Solid = new G4Tubs("SP41Solid", par[0]*cm, par[1]*cm, par[2]*cm,0 ,360*deg);
    /*-*/G4LogicalVolume* SP41Logical = new G4LogicalVolume(SP41Solid, FindMaterial("G4_Fe"), "SP41Logical");
    /*-*/G4VSolid* SP42Solid = new G4Tubs("SP42Solid", par[0]*cm, par[1]*cm, par[2]*cm,0 ,360*deg);
    /*-*/G4LogicalVolume* SP42Logical = new G4LogicalVolume(SP42Solid, FindMaterial("G4_Fe"), "SP42Logical");
    /*-*/par[0] = 0.0;
    /*-*/par[1] = 6.99;
    /*-*/par[2] = 378.89/2.0;
    /*-*/G4VSolid* SM41Solid = new G4Tubs("SM41Solid", par[0], par[1], par[2],0 ,360*deg);
    /*-*/G4LogicalVolume* SM41Logical = new G4LogicalVolume(SM41Solid, FindMaterial("G4_Galactic"), "SM41Logical");
    /*-*/G4VSolid* SM42Solid = new G4Tubs("SM42Solid", par[0], par[1] ,par[2],0 ,360*deg);
    /*-*/G4LogicalVolume* SM42Logical = new G4LogicalVolume(SM42Solid, FindMaterial("G4_Galactic"), "SM42Logical");
    /*-*/par[0] = 0.0;
    /*-*/par[1] = 6.99;
    /*-*/par[2] = 210.92 - (378.89/2.0);
    /*-*/G4VSolid* SM43Solid = new G4Tubs("SM43Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* SM43Logical = new G4LogicalVolume(SM43Solid, FindMaterial("G4_Galactic"), "SM43Logical");
    /*-*/G4VSolid* SM44Solid = new G4Tubs("SM44Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* SM44Logical = new G4LogicalVolume(SM44Solid, FindMaterial("G4_Galactic"), "SM44Logical");
    /*-*/G4VSolid* SM45Solid = new G4Tubs("SM45Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* SM45Logical = new G4LogicalVolume(SM45Solid, FindMaterial("G4_Galactic"), "SM45Logical");
    /*-*/G4VSolid* SM46Solid = new G4Tubs("SM46Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* SM46Logical = new G4LogicalVolume(SM46Solid, FindMaterial("G4_Galactic"), "SM46Logical");
    /*-*/par[0] = 9.53;
    /*-*/par[1] = 10.16;
    /*-*/par[2] = 210.92;
    /*-*/G4VSolid* SP51Solid = new G4Tubs("SP51Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* SP51Logical = new G4LogicalVolume(SP51Solid, FindMaterial("G4_Fe"), "SP51Logical");
    /*-*/G4VSolid* SP52Solid = new G4Tubs("SP52Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* SP52Logical = new G4LogicalVolume(SP52Solid, FindMaterial("G4_Fe"), "SP52Logical");
    /*-*/par[0] = 7.14;
    /*-*/par[1] = 9.53;
    /*-*/par[2] = 210.92;
    /*-*/G4VSolid* SM51Solid = new G4Tubs("SM51Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* SM51Logical = new G4LogicalVolume(SM51Solid, FindMaterial("G4_Galactic"), "SM51Logical");
    /*-*/G4VSolid* SM52Solid = new G4Tubs("SM52Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* SM52Logical = new G4LogicalVolume(SM52Solid, FindMaterial("G4_Galactic"), "SM52Logical");
    /*-*/par[0] = 15.34;
    /*-*/par[1] = 37.00;
    /*-*/par[2] = 210.92;
    /*-*/G4VSolid* SP61Solid = new G4Tubs("SP61Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* SP61Logical = new G4LogicalVolume(SP61Solid, FindMaterial("G4_Fe"), "SP61Logical");
    /*-*/G4VSolid* SP62Solid = new G4Tubs("SP62Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* SP62Logical = new G4LogicalVolume(SP62Solid, FindMaterial("G4_Fe"), "SP62Logical");
    /*-*/par[0] = 10.16;
    /*-*/par[1] = 15.34;
    /*-*/par[2] = 210.92;
    /*-*/G4VSolid* SM61Solid = new G4Tubs("SM61Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* SM61Logical = new G4LogicalVolume(SM61Solid, FindMaterial("G4_Galactic"), "SM61Logical");
    /*-*/G4VSolid* SM62Solid = new G4Tubs("SM62Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* SM62Logical = new G4LogicalVolume(SM62Solid, FindMaterial("G4_Galactic"), "SM62Logical");
    /*-*/par[0] = 20.87;
    /*-*/par[1] = 7.14;
    /*-*/par[2] = 7.77;
    /*-*/par[3] = 14.60;
    /*-*/par[4] = 15.24;
    /*-*/G4VSolid* CN21Solid = new G4Cons("CN21Solid", par[1]*cm, par[2]*cm, par[3]*cm, par[4]*cm, par[0]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* CN21Logical = new G4LogicalVolume(CN21Solid, FindMaterial("G4_Fe"), "CN21Logical");
    /*-*/par[0] = 20.87;
    /*-*/par[1] = 14.60;
    /*-*/par[2] = 15.24;
    /*-*/par[3] = 7.14;
    /*-*/par[4] = 7.77;
    /*-*/G4VSolid* CN22Solid = new G4Cons("CN22Solid", par[1]*cm, par[2]*cm, par[3]*cm, par[4]*cm, par[0]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* CN22Logical = new G4LogicalVolume(CN22Solid, FindMaterial("G4_Fe"), "CN22Logical");
    /*-*/par[0] = 20.87;
    /*-*/par[1] = 0.0;
    /*-*/par[2] = 7.14;
    /*-*/par[3] = 0.0;
    /*-*/par[4] = 14.60;
    /*-*/G4VSolid* CM21Solid = new G4Cons("CM21Solid", par[1]*cm, par[2]*cm, par[3]*cm, par[4]*cm, par[0]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* CM21Logical = new G4LogicalVolume(CM21Solid, FindMaterial("G4_Galactic"), "CM21Logical");
    /*-*/par[0] = 20.87;
    /*-*/par[1] = 0.0;
    /*-*/par[2] = 14.60;
    /*-*/par[3] = 0.0;
    /*-*/par[4] = 7.14;
    /*-*/G4VSolid* CM22Solid = new G4Cons("CM22Solid", par[1]*cm, par[2]*cm, par[3]*cm, par[4]*cm, par[0]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* CM22Logical = new G4LogicalVolume(CM22Solid, FindMaterial("G4_Galactic"), "CM22Logical");
    /*-*/par[0] = 14.60;
    /*-*/par[1] = 15.24;
    /*-*/par[2] = 57.895;
    /*-*/G4VSolid* SP71Solid = new G4Tubs("SP71Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* SP71Logical = new G4LogicalVolume(SP71Solid, FindMaterial("G4_Fe"), "SP71Logical");
    /*-*/G4VSolid* SP72Solid = new G4Tubs("SP72Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* SP72Logical = new G4LogicalVolume(SP72Solid, FindMaterial("G4_Fe"), "SP72Logical");
    /*-*/par[0] = 0.0;
    /*-*/par[1] = 14.60;
    /*-*/par[2] = 57.895;
    /*-*/G4VSolid* SM71Solid = new G4Tubs("SM71Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* SM71Logical = new G4LogicalVolume(SM71Solid, FindMaterial("G4_Galactic"), "SM71Logical");
    /*-*/G4VSolid* SM72Solid = new G4Tubs("SM72Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/G4LogicalVolume* SM72Logical = new G4LogicalVolume(SM72Solid, FindMaterial("G4_Galactic"), "SM72Logical");
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -850.14*cm), SM32Logical, "SM32Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -850.14*cm), SP32Logical, "SP32Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -945.08*cm), SM46Logical, "SM46Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1375.92*cm), SM44Logical, "SM44Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1165.0*cm), SM41Logical, "SM41Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1165.0*cm), SP41Logical, "SP41Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1165.0*cm), SM51Logical, "SM51Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1165.0*cm), SP51Logical, "SP51Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1165.0*cm), SM61Logical, "SM61Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1165.0*cm), SP61Logical, "SP61Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1394.25*cm), CM22Logical, "CM22Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1394.25*cm), CN22Logical, "CN22Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1475.935*cm), SM71Logical, "SM71Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1475.935*cm), SP71Logical, "SP71Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 850.14*cm), SM31Logical, "SM31Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 850.14*cm), SP31Logical, "SP31Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 945.08*cm), SM45Logical, "SM45Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1375.92*cm), SM43Logical, "SM43Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1165.0*cm), SM52Logical, "SM52Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1165.0*cm), SP52Logical, "SP52Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1165.0*cm), SM62Logical, "SM62Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1165.0*cm), SP62Logical, "SP62Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1165.0*cm), SM42Logical, "SM42Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1165.0*cm), SP42Logical, "SP42Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1394.25*cm), CM21Logical, "CM21Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1394.25*cm), CN21Logical, "CN21Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1475.935*cm), SM72Logical, "SM72Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1475.935*cm), SP72Logical, "SP72Physical", fWorldLogical, false, 0, checkOverlaps);

    par[0] = 20.0;
    par[1] = 20.96;
    par[2] = 108.58;

    G4VSolid* SP81Solid = new G4Tubs("SP81Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    SP81Logical = new G4LogicalVolume(SP81Solid, FindMaterial("G4_Fe"), "SP81Logical");

    G4VSolid* SP82Solid = new G4Tubs("SP82Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    SP82Logical = new G4LogicalVolume(SP82Solid, FindMaterial("G4_Fe"), "SP82Logical");

    par[0] = 0.0;
    par[1] = 20.0;
    par[2] = 108.58;

    G4VSolid* SM81Solid = new G4Tubs("SM81Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    SM81Logical = new G4LogicalVolume(SM81Solid, FindMaterial("G4_Galactic"), "SM81Logical");

    G4VSolid* SM82Solid = new G4Tubs("SM82Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    SM82Logical = new G4LogicalVolume(SM82Solid, FindMaterial("G4_Galactic"), "SM82Logical");

    par[0] = 0.0;
    par[1] = 20.96;
    par[2] = 0.47;

    G4VSolid* SP91Solid = new G4Tubs("SP91Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    SP91Logical = new G4LogicalVolume(SP91Solid, FindMaterial("G4_Fe"), "SP91Logical");

    G4VSolid* SP92Solid = new G4Tubs("SP92Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    SP92Logical = new G4LogicalVolume(SP92Solid, FindMaterial("G4_Fe"), "SP92Logical");

    par[0] = 0.0;
    par[1] = 6.35;
    par[2] = 0.47;

    G4VSolid* SHL1Solid = new G4Tubs("SHL1Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    SHL1Logical = new G4LogicalVolume(SHL1Solid, FindMaterial("G4_Galactic"), "SHL1Logical");

    G4VSolid* SHL2Solid = new G4Tubs("SHL2Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    SHL2Logical = new G4LogicalVolume(SHL2Solid, FindMaterial("G4_Galactic"), "SHL2Logical");

    G4VSolid* SHU1Solid = new G4Tubs("SHU1Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    SHU1Logical = new G4LogicalVolume(SHU1Solid, FindMaterial("G4_Galactic"), "SHU1Logical");

    G4VSolid* SHU2Solid = new G4Tubs("SHU2Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    SHU2Logical = new G4LogicalVolume(SHU2Solid, FindMaterial("G4_Galactic"), "SHU2Logical");

    par[0] = 6.07;
    par[1] = 6.35;
    par[2] = 91.5;

    G4VSolid* S101Solid = new G4Tubs("S101Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    S101Logical = new G4LogicalVolume(S101Solid, FindMaterial("G4_Fe"), "S101Logical");

    G4VSolid* S102Solid = new G4Tubs("S102Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    S102Logical = new G4LogicalVolume(S102Solid, FindMaterial("G4_Fe"), "S102Logical");

    G4VSolid* S111Solid = new G4Tubs("S111Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    S111Logical = new G4LogicalVolume(S111Solid, FindMaterial("G4_Fe"), "S111Logical");

    G4VSolid* S112Solid = new G4Tubs("S112Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    S112Logical = new G4LogicalVolume(S112Solid, FindMaterial("G4_Fe"), "S112Logical");

    par[0] = 0.0;
    par[1] = 6.35;
    par[2] = 91.5;

    G4VSolid* M101Solid = new G4Tubs("M101Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    M101Logical = new G4LogicalVolume(M101Solid, FindMaterial("G4_Galactic"), "M101Logical");

    G4VSolid* M102Solid = new G4Tubs("M102Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    M102Logical = new G4LogicalVolume(M102Solid, FindMaterial("G4_Galactic"), "M102Logical");

    G4VSolid* M111Solid = new G4Tubs("M111Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    M111Logical = new G4LogicalVolume(M111Solid, FindMaterial("G4_Galactic"), "M111Logical");

    G4VSolid* M112Solid = new G4Tubs("M112Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    M112Logical = new G4LogicalVolume(M112Solid, FindMaterial("G4_Galactic"), "M112Logical");


    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -1642*cm), SM82Logical, "SM82Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -1642*cm), SP82Logical, "SP82Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 1642*cm), SM81Logical, "SM81Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 1642*cm), SP81Logical, "SP81Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(PipeRotation1, G4ThreeVector(12.82*cm, 0, -1843*cm), M102Logical, "M102Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(PipeRotation1, G4ThreeVector(12.82*cm, 0, -1843*cm), S102Logical, "S102Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(PipeRotation2, G4ThreeVector(-12.82*cm, 0, -1843*cm), M112Logical, "M112Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(PipeRotation2, G4ThreeVector(-12.82*cm, 0, -1843*cm), S112Logical, "S112Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(11.02*cm, 0, 0), SHL1Logical, "SHL1Physical", SP91Logical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(-11.02*cm, 0, 0), SHU1Logical, "SHU1Physical", SP91Logical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(11.02*cm, 0, 0), SHL2Logical, "SHL2Physical", SP92Logical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(-11.02*cm, 0, 0), SHU2Logical, "SHU2Physical", SP92Logical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 1750.99*cm), SP91Logical, "SP91Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -1750.99*cm), SP92Logical, "SP92Physical", fWorldLogical, false, 0, checkOverlaps);





    //---------------------MagneticField---------------
    //
    // Dx Magnet
    //


    G4VSolid* MagneticSolid = new G4Tubs("MagneticSolid", 0., 15.24*cm, 207.92*cm, 0., 360*deg);
    fMagneticLogical = new G4LogicalVolume(MagneticSolid, FindMaterial("G4_Galactic"), "MagneticLogical");

    G4VPhysicalVolume* MagneticPhysical = new G4PVPlacement(fNonRotation , G4ThreeVector(0, 0, -1165.12*cm), fMagneticLogical, "MagneticPhysical", fWorldLogical, false, 0, checkOverlaps);
    new G4PVPlacement( 0, G4ThreeVector(0, 0, 1165.12*cm), fMagneticLogical, "MagneticPhysical", fWorldLogical, false, 0, checkOverlaps);



    return MagneticPhysical;


}


G4VPhysicalVolume* RHICFDetectorConstruction::ARM1(G4VPhysicalVolume* world_phys, G4ThreeVector vector, G4RotationMatrix* mat)
{


    /*-*/// Define ARM1
    /*-*/fARM1Solid          = new G4Box("ARM1Solid", kARM1par[0]*cm, kARM1par[1]*cm, kARM1par[2]*cm); 
    /*-*/fARM1Logical        = new G4LogicalVolume(fARM1Solid, FindMaterial("G4_Galactic"), "ARM1Logical");
    /*-*/fARM1Physical       = new G4PVPlacement(mat, vector, "ARM1Physical", fARM1Logical, world_phys, false, 0, checkOverlaps);

    /* Define Holders-W_PLHolder1,2 GSO_PLHolder, GSOBarHolder */

    /*-*/// Define 'Tunsten Plate Holder1'- First type of tungsten plate holder
    /*-*/std::vector<G4TwoVector> VectorForEdge;
    /*-*/G4TwoVector TwoDVectorForEdge = {-10*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-10*mm,(-(30.1*3/2*sqrt(2)+5)+10)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-30.1/2*sqrt(2)*mm,-(30.1*sqrt(2)+5)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-4.6*mm,-(50.1/2*sqrt(2)-4.6)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-50.1/2*sqrt(2)*mm,0*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-8*mm,(50.1/2*sqrt(2)-8)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-8*mm,((50.1/2+9)*sqrt(2)-8)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {0*mm,(50.1/2+9)*sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {8*mm,((50.1/2+9)*sqrt(2)-8)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {8*mm,(50.1/2*sqrt(2)-8)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {50.1/2*sqrt(2)*mm,0*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {4.6*mm,-(50.1/2*sqrt(2)-4.6)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {30.1/2*sqrt(2)*mm,-(30.1*sqrt(2)+5)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {10*mm,(-(30.1*3/2*sqrt(2)+5)+10)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {10*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/G4ExtrudedSolid::ZSection Bottom(-7.1/2*mm,{0,0},1);
    /*-*/G4ExtrudedSolid::ZSection Top(7.1/2*mm,{0,0},1);
    /*-*/std::vector<G4ExtrudedSolid::ZSection> Hzsections;
    /*-*/Hzsections.push_back(Bottom);
    /*-*/Hzsections.push_back(Top);
    /*-*/fWHolder_1Solid             = new G4ExtrudedSolid("ProtoSolid1", VectorForEdge, Hzsections);
    /*-*/fNegativeLargeWSolid       = new G4Box("NegativeLargeW", kNegativeLargeWpar[0]*cm, kNegativeLargeWpar[1]*cm, kNegativeLargeWpar[2]*cm);
    /*-*/fNegativeSmallWSolid       = new G4Box("NegativeSmallW", kNegativeSmallWpar[0]*cm, kNegativeSmallWpar[1]*cm, kNegativeSmallWpar[2]*cm);
    /*-*/fNegativeTopRoundSolid     = new G4Tubs("NegativeTopRoundSolid", 0*cm, 0.45*cm, 0.72*cm, 0*deg, 360*deg);
    /*-*/fNegativeMiddleRoundSolid  = new G4Tubs("NegativeMiddleRoundSolid", 0.3*cm, 0.45*cm, 0.72*cm, 0*deg, 360*deg);
    /*-*/fNegativeBottomRoundSolid  = new G4Tubs("NegativeBottomRoundSolid", 1*cm, 1.5*cm, 0.71*cm, 0*deg, 360*deg);
    /*-*/fNegativeHoleSolid         = new G4Tubs("NegativeHole", 0*cm, 0.4*cm, 0.72*cm, 0*deg, 360*deg);
    /*-*/fWHolder_1Solid             = new G4SubtractionSolid( "-LargeWSolid", fWHolder_1Solid, fNegativeLargeWSolid, fRotationZ45, G4ThreeVector());
    /*-*/fWHolder_1Solid             = new G4SubtractionSolid( "-SmallWSolid", fWHolder_1Solid, fNegativeSmallWSolid, fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, 0*mm));
    /*--*/fWHolder_1Solid            = new G4SubtractionSolid( "-TopHoleSolid", fWHolder_1Solid, fNegativeHoleSolid, fNonRotation, G4ThreeVector(0*mm, ((50.1/2+9)*sqrt(2)-8-2.65)*mm, 0*mm));
    /*--*/fWHolder_1Solid            = new G4SubtractionSolid( "WHolder_1Solid", fWHolder_1Solid, fNegativeHoleSolid, fNonRotation, G4ThreeVector(0*mm, (-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75+10)*mm, 0*mm));

    /*-*/// Define 'Tunsten Plate Holder2'- Second type of tungsten plate holder
    /*-*/VectorForEdge.clear();  //Delete G2Vecotrs in VectorForEdge
    /*-*/Hzsections.clear(); //Delete ZSections in Hzsections
    /*-*/TwoDVectorForEdge = {-10*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-10*mm,(-(31.05*sqrt(2)+5)-(30.1/2*sqrt(2))+10)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-30.1/2*sqrt(2)*mm,-(31.05*sqrt(2)+5)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-3.22*mm,-30.42*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-48.19/2*sqrt(2)*mm,0*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-8*mm,(48.19/2*sqrt(2)-8)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-8*mm,((48.19/2+9)*sqrt(2)-8)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {0*mm,(48.19/2+9)*sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {8*mm,((48.19/2+9)*sqrt(2)-8)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {8*mm,(48.19/2*sqrt(2)-8)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {48.19/2*sqrt(2)*mm,0*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {3.22*mm,-30.42*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {30.1/2*sqrt(2)*mm,-(31.05*sqrt(2)+5)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {10*mm,(-(31.05*sqrt(2)+5)-(30.1/2*sqrt(2))+10)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {10*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/G4ExtrudedSolid::ZSection Bottom2(-7.1/2*mm,{0,0},1);
    /*-*/G4ExtrudedSolid::ZSection Top2(7.1/2*mm,{0,0},1);
    /*-*/Hzsections.push_back(Bottom2);
    /*-*/Hzsections.push_back(Top2);
    /*-*/fWHolder_2Solid              = new G4ExtrudedSolid("ProtoSolid2", VectorForEdge, Hzsections);
    /*-*/fWHolder_2Solid              = new G4SubtractionSolid( "-SmallWSolid", fWHolder_2Solid, fNegativeSmallWSolid, fRotationZ45, G4ThreeVector(0*mm, -(31.05*sqrt(2)+5)*mm, 0*mm));
    /*-*/fWHolder_2Solid              = new G4SubtractionSolid( "-LargeWSolid", fWHolder_2Solid, fNegativeLargeWSolid, fRotationZ45, G4ThreeVector());
    /*-*/fWHolder_2Solid              = new G4SubtractionSolid( "-TopHoleSolid2", fWHolder_2Solid, fNegativeHoleSolid, fNonRotation, G4ThreeVector(0*mm, ((50.1/2+9)*sqrt(2)-8-2.65)*mm, 0*mm));
    /*-*/fWHolder_2Solid              = new G4SubtractionSolid( "WHolder_2Solid", fWHolder_2Solid, fNegativeHoleSolid, fNonRotation, G4ThreeVector(0*mm, (-(31.05*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75+10)*mm, 0*mm));

    /*-*/// Define 'GSO Plate Holder1'
    /*-*/VectorForEdge.clear();  //Delete G2Vecotrs in VectorForEdge
    /*-*/Hzsections.clear(); //Delete ZSections in Hzsections
    /*-*/TwoDVectorForEdge = {-10*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-10*mm,(-(30.1*3/2*sqrt(2)+5)+10)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-30.1/2*sqrt(2)*mm,-(30.1*sqrt(2)+5)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-4.6*mm,-(50.1/2*sqrt(2)-4.6)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-50.1/2*sqrt(2)*mm,0*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-8*mm,(50.1/2*sqrt(2)-8)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-8*mm,((50.1/2+9)*sqrt(2)-8)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {0*mm,(50.1/2+9)*sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {30.9*mm,35.15*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {30.9*mm,-3.59*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {17.2*mm,-18.6*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {27.06*mm,-28.7*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {27.06*mm,-41.8*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {10*mm,(-(30.1*3/2*sqrt(2)+5)+10)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {10*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/G4ExtrudedSolid::ZSection BottomGSO(-3.2/2*mm,{0,0},1);
    /*-*/G4ExtrudedSolid::ZSection TopGSO(3.2/2*mm,{0,0},1);
    /*-*/Hzsections.push_back(BottomGSO);
    /*-*/Hzsections.push_back(TopGSO);
    /*-*/fGSO_PLHolderSolid             = new G4ExtrudedSolid("GSO_PLHolderProto", VectorForEdge, Hzsections);
    /*-*///// SolidVolume for negative volume of light guide
    /*-*/VectorForEdge.clear();  //Delete G2Vecotrs in VectorForEdge
    /*-*/Hzsections.clear(); //Delete ZSections in Hzsections
    /*-*/TwoDVectorForEdge = {-40.2/sqrt(2)*mm, 0*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {0*mm, 40.2/sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-19.82+40.2/sqrt(2)*mm, 34*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-16.48+40.2/sqrt(2)*mm, 46.72*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {1.17+40.2/sqrt(2)*mm, 37.38*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-1.02+40.2/sqrt(2)*mm, 38.14*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {40.2/sqrt(2)*mm, 0*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {0*mm, -40.2/sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/G4ExtrudedSolid::ZSection BottomNegativeGuide(-0.6*mm,{0,0},1);
    /*-*/G4ExtrudedSolid::ZSection TopNegativeGuide(0.6*mm,{0,0},1);
    /*-*/Hzsections.push_back(BottomNegativeGuide);
    /*-*/Hzsections.push_back(TopNegativeGuide);
    /*-*/fNegativeGuideLargeSolid       = new G4ExtrudedSolid("NegativeGuideLarge", VectorForEdge, Hzsections);
    /*-*///// SolidVolume for negative volume of Small guide
    /*-*/VectorForEdge.clear();  //Delete G2Vecotrs in VectorForEdge
    /*-*/Hzsections.clear(); //Delete ZSections in Hzsections
    /*-*/TwoDVectorForEdge = {-20.2/sqrt(2)*mm, 0*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {0*mm, 20.2/sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-1.58+20.2/sqrt(2)*mm, 19.38*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {+5.84+20.2/sqrt(2)*mm, 28.03*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {+13.6+20.2/sqrt(2)*mm, 19.71*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {+5.31+20.2/sqrt(2)*mm, 11.67*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {+20.2/sqrt(2)*mm, 0*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {0*mm, -20.2/sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/Hzsections.push_back(BottomNegativeGuide);
    /*-*/Hzsections.push_back(TopNegativeGuide);
    /*-*/fNegativeGuideSmallSolid        = new G4ExtrudedSolid("NegativeGuideSmall", VectorForEdge, Hzsections);
    /*-*/fGSO_PLHolderSolid              = new G4SubtractionSolid( "-GSO_PLSmallWSolid", fGSO_PLHolderSolid, fNegativeGuideSmallSolid, fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, 0*mm));
    /*-*/fGSO_PLHolderSolid              = new G4SubtractionSolid( "-GSO_PLLargeWSolid", fGSO_PLHolderSolid, fNegativeGuideLargeSolid, fNonRotation, G4ThreeVector());
    /*-*/fGSO_PLHolderSolid              = new G4SubtractionSolid( "-TopHoleSolid2", fGSO_PLHolderSolid, fNegativeHoleSolid, fNonRotation, G4ThreeVector(0*mm, ((50.1/2+9)*sqrt(2)-8-2.65)*mm, 0*mm));
    /*-*/fGSO_PLHolderSolid              = new G4SubtractionSolid( "GSO_PLHolderSolid", fGSO_PLHolderSolid, fNegativeHoleSolid, fNonRotation, G4ThreeVector(0*mm, (-(31.05*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75+10)*mm, 0*mm));

    /*-*/// Define 'GSO Bar Holder'
    /*-*/VectorForEdge.clear();  //Delete G2Vecotrs in VectorForEdge
    /*-*/Hzsections.clear(); //Delete ZSections in Hzsections
    /*-*/TwoDVectorForEdge = {-10*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-10*mm,(-(30.1*3/2*sqrt(2)+5)+10)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-40.1/2*sqrt(2)*mm,-(30.1*sqrt(2)+5-5*sqrt(2))*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-(4.6+5*sqrt(2)-2.07/2)*mm,-(50.1/2*sqrt(2)-4.6-5*sqrt(2)-2.07/2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-(53.74/2*sqrt(2)-1)*mm,(3.64/sqrt(2)-1+2.07)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-(53.74/2*sqrt(2)-1)*mm,(3.64/sqrt(2)+1+2.07)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-8*mm,(50.1/2*sqrt(2)-8+3.64/sqrt(2)+2.07)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-8*mm,((50.1/2+9)*sqrt(2)-8)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {0*mm,(50.1/2+9)*sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {8*mm,((50.1/2+9)*sqrt(2)-8)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {8*mm,(50.1/2*sqrt(2)-8+3.64/sqrt(2)+2.07)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-(-53.74/2*sqrt(2)+1)*mm,(3.64/sqrt(2)+1+2.07)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-(-53.74/2*sqrt(2)+1)*mm,(3.64/sqrt(2)-1+2.07)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-(-4.6-5*sqrt(2)+2.07/2)*mm,-(50.1/2*sqrt(2)-4.6-5*sqrt(2)-2.07/2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {40.1/2*sqrt(2)*mm,-(30.1*sqrt(2)+5-5*sqrt(2))*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {10*mm,(-(30.1*3/2*sqrt(2)+5)+10)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {10*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/G4ExtrudedSolid::ZSection BottomGSOBarHolder(-3.8/2*mm,{0,0},1);
    /*-*/G4ExtrudedSolid::ZSection TopGSOBarHolder(3.8/2*mm,{0,0},1);
    /*-*/Hzsections.push_back(BottomGSOBarHolder);
    /*-*/Hzsections.push_back(TopGSOBarHolder);
    /*-*/fGSOBarHolderSolid          = new G4ExtrudedSolid("GSOBarSolidProto", VectorForEdge, Hzsections);
    /*-*///// SolidVolume for negative right volume of GSO large bar 
    /*-*/VectorForEdge.clear();  //Delete G2Vecotrs in VectorForEdge
    /*-*/Hzsections.clear(); //Delete ZSections in Hzsections
    /*-*/TwoDVectorForEdge = {-40.35/sqrt(2)*mm,0*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-(40.34/sqrt(2)-53.74*sqrt(2))*mm,53.74*sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {53.74*sqrt(2)*mm,(53.74*sqrt(2)-40.34/sqrt(2))*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {0*mm,-40.34/sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/G4ExtrudedSolid::ZSection BottomNegativeRightLargeGSOBar(-0.4*mm,{0,0},1);
    /*-*/G4ExtrudedSolid::ZSection TopNegativeRightLargeGSOBar(2.0*mm,{0,0},1);
    /*-*/Hzsections.push_back(BottomNegativeRightLargeGSOBar);
    /*-*/Hzsections.push_back(TopNegativeRightLargeGSOBar);
    /*-*/fNegativeGSOBarLargeRightSolid          = new G4ExtrudedSolid("NegativeGSOBarLargeRightSolid", VectorForEdge, Hzsections);
    /*-*///// SolidVolume for negative left volume of GSO large bar 
    /*-*/VectorForEdge.clear();  //Delete G2Vecotrs in VectorForEdge
    /*-*/Hzsections.clear(); //Delete ZSections in Hzsections
    /*-*/TwoDVectorForEdge = {-53.74*sqrt(2)*mm,(53.74*sqrt(2)-40.34/sqrt(2))*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {(40.34/sqrt(2)-53.74*sqrt(2))*mm,53.74*sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {40.35/sqrt(2)*mm,0*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {0*mm,-40.34/sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/G4ExtrudedSolid::ZSection BottomNegativeLeftLargeGSOBar(0.2*mm,{0,0},1);
    /*-*/G4ExtrudedSolid::ZSection TopNegativeLeftLargeGSOBar(2.6*mm,{0,0},1);
    /*-*/Hzsections.push_back(BottomNegativeLeftLargeGSOBar);
    /*-*/Hzsections.push_back(TopNegativeLeftLargeGSOBar);
    /*-*/fNegativeGSOBarLargeLeftSolid          = new G4ExtrudedSolid("NegativeGSOBarLargeLeftSolid", VectorForEdge, Hzsections);
    /*-*///// SolidVolume for negative right volume of GSO small bar 
    /*-*/VectorForEdge.clear();  //Delete G2Vecotrs in VectorForEdge
    /*-*/Hzsections.clear(); //Delete ZSections in Hzsections
    /*-*/TwoDVectorForEdge = {20.1/sqrt(2)*mm,0*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {(20.1/sqrt(2)+40.1*sqrt(2))*mm,40.1*sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-40.1*sqrt(2)*mm,(40.1*sqrt(2)-20.1/sqrt(2))*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {0*mm,-20.1/sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/G4ExtrudedSolid::ZSection BottomNegativeRightSmallGSOBar(-0.4*mm,{0,0},1);
    /*-*/G4ExtrudedSolid::ZSection TopNegativeRightSmallGSOBar(2.0*mm,{0,0},1);
    /*-*/Hzsections.push_back(BottomNegativeRightSmallGSOBar);
    /*-*/Hzsections.push_back(TopNegativeRightSmallGSOBar);
    /*-*/fNegativeGSOBarSmallRightSolid          = new G4ExtrudedSolid("NegativeGSOBarSmallRightSolid", VectorForEdge, Hzsections);
    /*-*///// SolidVolume for negative left volume of GSO small bar 
    /*-*/VectorForEdge.clear();  //Delete G2Vecotrs in VectorForEdge
    /*-*/Hzsections.clear(); //Delete ZSections in Hzsections
    /*-*/TwoDVectorForEdge = {20.1/sqrt(2)*mm,0*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {(20.1/sqrt(2)+40.1*sqrt(2))*mm,40.1*sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {40.1*sqrt(2)*mm,(40.1*sqrt(2)+20.1/sqrt(2))*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-20.1/sqrt(2)*mm,0*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {0*mm,-20.1/sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/G4ExtrudedSolid::ZSection BottomNegativeLeftSmallGSOBar(0.2*mm,{0,0},1);
    /*-*/G4ExtrudedSolid::ZSection TopNegativeLeftSmallGSOBar(2.6*mm,{0,0},1);
    /*-*/Hzsections.push_back(BottomNegativeLeftSmallGSOBar);
    /*-*/Hzsections.push_back(TopNegativeLeftSmallGSOBar);
    /*-*/fNegativeGSOBarSmallLeftSolid         = new G4ExtrudedSolid("NegativeGSOBarSmallLeftSolid", VectorForEdge, Hzsections);
    /*-*/fGSOBarHolderSolid                    = new G4SubtractionSolid("-GSOBarLargeRightSolid", fGSOBarHolderSolid, fNegativeGSOBarLargeRightSolid, fNonRotation, G4ThreeVector());
    /*-*/fGSOBarHolderSolid                    = new G4SubtractionSolid("-GSOBarLargeLeftSolid", fGSOBarHolderSolid, fNegativeGSOBarLargeLeftSolid, fNonRotation, G4ThreeVector());
    /*-*/fGSOBarHolderSolid                    = new G4SubtractionSolid("-GSOBarSmallRightSolid", fGSOBarHolderSolid, fNegativeGSOBarSmallRightSolid, fNonRotation, G4ThreeVector(0*mm,-(30.1*sqrt(2)+5)*mm, 0*mm));
    /*-*/fGSOBarHolderSolid                    = new G4SubtractionSolid("-GSOBarSmallLeftSolid", fGSOBarHolderSolid, fNegativeGSOBarSmallLeftSolid, fNonRotation, G4ThreeVector(0*mm,-(30.1*sqrt(2)+5)*mm, 0*mm));
    /*-*/fGSOBarHolderSolid                    = new G4SubtractionSolid( "-TopHoleGSOBarSolid", fGSOBarHolderSolid, fNegativeHoleSolid, fNonRotation, G4ThreeVector(0*mm, ((50.1/2+9)*sqrt(2)-8-2.65)*mm, 0*mm));
    /*-*/fGSOBarHolderSolid                    = new G4SubtractionSolid( "GSOBarHolderSolid", fGSOBarHolderSolid, fNegativeHoleSolid, fNonRotation, G4ThreeVector(0*mm, (-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75+10)*mm, 0*mm));

    /*-*/// Solid volumes for tungsten plate
    /*-*/fLargeW_PLSolid                       = new G4Box("LargeW_PLSolid", 2.0*cm, 2.0*cm, 0.305*cm);
    /*-*/fSmallW_PLSolid                       = new G4Box("SmallW_PLSolid", 1.0*cm, 1.0*cm, 0.305*cm);

    /*-*/// Solid volumes for GSO pate
    /*-*/fLargeGSO_PLSolid                     = new G4Box("LargeGSO_PLSolid", 2*cm, 2*cm, 0.05*cm); 
    /*-*/fSmallGSO_PLSolid                     = new G4Box("SmallGSO_PLSolid", 1*cm, 1*cm, 0.05*cm); 

    /* Solid volumes for light guide */

    /*-*/// SolidVolume for negative volume of light guide
    /*-*/VectorForEdge.clear();  //Delete G2Vecotrs in VectorForEdge
    /*-*/Hzsections.clear(); //Delete ZSections in Hzsections
    /*-*/TwoDVectorForEdge = {0.01*mm, 40.1/sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-19.82+40.2/sqrt(2)*mm, 33.5*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-1.01+40.2/sqrt(2)*mm, 36.14*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {40.2/sqrt(2)*mm, 0.01*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/G4ExtrudedSolid::ZSection BottomLightGuideLarge(-0.5*mm,{0,0},1);
    /*-*/G4ExtrudedSolid::ZSection TopLightGuideLarge(0.5*mm,{0,0},1);
    /*-*/Hzsections.push_back(BottomLightGuideLarge);
    /*-*/Hzsections.push_back(TopLightGuideLarge);
    /*-*/fLightGuideLargeSolid       = new G4ExtrudedSolid("LightGuideLargeSolid", VectorForEdge, Hzsections);

    /*-*/// SolidVolume for volume of Small guide
    /*-*/VectorForEdge.clear();  //Delete G2Vecotrs in VectorForEdge
    /*-*/Hzsections.clear(); //Delete ZSections in Hzsections
    /*-*/TwoDVectorForEdge = {0.01*mm, 20.1/sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-1.58+20.2/sqrt(2)*mm, 19.37*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {5.30+20.2/sqrt(2)*mm, 11.67*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {20.2/sqrt(2)*mm, 0.01*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/Hzsections.push_back(BottomLightGuideLarge);
    /*-*/Hzsections.push_back(TopLightGuideLarge);
    /*-*/fLightGuideSmallSolid        = new G4ExtrudedSolid("LightGuideSmallSolid", VectorForEdge, Hzsections);

    /*-*/// SolidVolume for GSO bar belt
    /*-*/fGSOSmallBarSolid                 = new G4Box("GSOSmallBarSolid", 10*mm, 0.5*mm, 0.5*mm); 
    /*-*/fGSOLargeBarSolid                 = new G4Box("GSOLargeBarSolid", 20*mm, 0.5*mm, 0.5*mm); 
    /*-*/fGSOSmallBarBeltSolid             = new G4Box("GSOSmallBarBeltSolid", 10*mm, 10*mm, 0.5*mm);
    /*-*/fGSOLargeBarBeltSolid             = new G4Box("GSOLargeBarBeltSolid", 20*mm, 20*mm, 0.5*mm);

    // SolidVolume for aluminum frame
    /*-*/// SolidVolume for part1
    /*-*/VectorForEdge.clear();  //Delete G2Vecotrs in VectorForEdge
    /*-*/Hzsections.clear(); //Delete ZSections in Hzsections
    /*-*/TwoDVectorForEdge = {30*mm, 20.6+29.96*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-30*mm, 20.6+29.96*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-30*mm, -29.5+29.96*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-43*mm, -29.5+29.96*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-43*mm, -37.5+29.96*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-30*mm, -37.5+29.96*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-30*mm, (-(30.1*sqrt(2)+5)-41.43+10.05*sqrt(2)-8.75)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {30*mm, (-(30.1*sqrt(2)+5)-41.43+10.05*sqrt(2)-8.75)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {30*mm, -37.5+29.96*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {43*mm, -37.5+29.96*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {43*mm, -29.5+29.96*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {30*mm, -29.5+29.96*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/G4ExtrudedSolid::ZSection BottomAlFramePart1(-110.73*mm,{0,0},1);
    /*-*/G4ExtrudedSolid::ZSection TopAlFramePart1(-102.53*mm,{0,0},1);
    /*-*/Hzsections.push_back(BottomAlFramePart1);
    /*-*/Hzsections.push_back(TopAlFramePart1);
    /*-*/fAlFramePart1Solid        = new G4ExtrudedSolid("AlFramePart1Solid", VectorForEdge, Hzsections);
    /*-*/// SolidVolume for part2
    /*-*/VectorForEdge.clear();  //Delete G2Vecotrs in VectorForEdge
    /*-*/Hzsections.clear(); //Delete ZSections in Hzsections
    /*-*/TwoDVectorForEdge = {30*mm, 20.6+29.96*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-30*mm, 20.6+29.96*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-30*mm, (-(30.1*sqrt(2)+5)-41.43+10.05*sqrt(2)-8.75)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {30*mm, (-(30.1*sqrt(2)+5)-41.43+10.05*sqrt(2)-8.75)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/G4ExtrudedSolid::ZSection BottomAlFramePart2(120.325*mm,{0,0},1);
    /*-*/G4ExtrudedSolid::ZSection TopAlFramePart2(121.61*mm,{0,0},1);
    /*-*/Hzsections.push_back(BottomAlFramePart2);
    /*-*/Hzsections.push_back(TopAlFramePart2);
    /*-*/fAlFramePart2Solid        = new G4ExtrudedSolid("AlFramePart2Solid", VectorForEdge, Hzsections);
    /*-*/// SolidVolume for part3(pole)
    /*-*/fAlFramePart3Solid         = new G4Tubs("AlFramePart3Solid", 0*mm, 3.99*mm, 275/2*mm, 0*deg, 360*deg);
    /*-*/// SolidVolume for part5(bottom)
    /*-*/VectorForEdge.clear();  //Delete G2Vecotrs in VectorForEdge
    /*-*/Hzsections.clear(); //Delete ZSections in Hzsections
    /*-*/TwoDVectorForEdge = {44.0*mm, (-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.85)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-44.0*mm, (-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.85)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-44.0*mm, (-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75-8)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {44.0*mm, (-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75-8)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/G4ExtrudedSolid::ZSection BottomAlFramePart5(-139.0*mm,{0,0},1);
    /*-*/G4ExtrudedSolid::ZSection TopAlFramePart5(139.0*mm,{0,0},1);
    /*-*/Hzsections.push_back(BottomAlFramePart5);
    /*-*/Hzsections.push_back(TopAlFramePart5);
    /*-*/fAlFramePart5Solid        = new G4ExtrudedSolid("AlFramePart5Solid", VectorForEdge, Hzsections);
    /*-*/// SolidVolume for part6,7,8,9,10
    /*-*/fAlFramePart6Solid         = new G4Box("AlFramePart6Solid", 5.0*mm, 109.9*mm, 5.0*mm);
    /*-*/fAlFramePart7Solid         = new G4Box("AlFramePart7Solid", 20.0*mm, 7.0*mm, 9.0/2*mm);
    /*-*/fAlFramePart8Solid         = new G4Box("AlFramePart8Solid", 40.0*mm, 10.0*mm, 5.0*mm);
    /*-*/fAlFramePart9Solid         = new G4Box("AlFramePart9Solid", 20.0*mm, 7.0*mm, 9.0*mm);
    /*-*/fAlFramePart10Solid         = new G4Box("AlFramePart10Solid", 10.0*mm, 10.0*mm, 9.0/2*mm);

    /*-*/// SolidVolume for Al frame Top part
    /*-*/// Rear & Front 
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2", fAlFramePart1Solid, fAlFramePart2Solid);
    /*-*/// Top pole
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2+Part3", fAlFrame1Solid, fAlFramePart3Solid, fNonRotation, G4ThreeVector(0*mm,((50.1/2+9)*sqrt(2)-8-2.65)*mm, 0*mm));
    /*-*/// Column
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2+Part3+Part6", fAlFrame1Solid, fAlFramePart6Solid, fNonRotation, G4ThreeVector(39*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75+110.0)*mm, 134.0*mm));
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2+Part3+Part6+Part7", fAlFrame1Solid, fAlFramePart6Solid, fNonRotation, G4ThreeVector(-39*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75+110.0)*mm, 134.0*mm));
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2+Part3+Part6+Part7+Part8", fAlFrame1Solid, fAlFramePart6Solid, fNonRotation, G4ThreeVector(39*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75+110.0)*mm, -134.0*mm));
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2+Part3+Part6+Part7+Part8+Part9", fAlFrame1Solid, fAlFramePart6Solid, fNonRotation, G4ThreeVector(-39*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75+110.0)*mm, -134.0*mm));
    /*-*/// Rear parts
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2+Part3+Part6+Part7+Part8+Part9+Part10", fAlFrame1Solid, fAlFramePart8Solid, fNonRotation, G4ThreeVector(0.0*mm,((50.1/2+9)*sqrt(2)-8-2.65)*mm, -134.0*mm));
    /*-*/// Front parts
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2+Part3+Part6+Part7+Part8+Part9+Part10+Part11", fAlFrame1Solid, fAlFramePart8Solid, fNonRotation, G4ThreeVector(0.0*mm,((50.1/2+9)*sqrt(2)-8-2.65)*mm, 134.0*mm));
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2+Part3+Part6+Part7+Part8+Part9+Part10+Part11+Part12", fAlFrame1Solid, fAlFramePart10Solid, fNonRotation, G4ThreeVector(0.0*mm,((50.1/2+9)*sqrt(2)-8-2.65)*mm, 125.5*mm));
    /*-*/fAlFrame1Solid                    = new G4SubtractionSolid( "AlFrameTopSolid", fAlFrame1Solid, fAlFramePart3Solid, fNonRotation, G4ThreeVector(0*mm, (-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75+10)*mm, 0*mm));

    /*-*/// SolidVolume for Al frame Bottom part
    /*-*/fAlFrame2Solid              = new G4UnionSolid("Part1+Part2", fAlFramePart5Solid, fAlFramePart7Solid, fNonRotation, G4ThreeVector(0.0*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.85+7)*mm, -134.5*mm));
    /*-*/fAlFrame2Solid              = new G4UnionSolid("Part1+Part2+Part3", fAlFrame2Solid, fAlFramePart9Solid, fNonRotation, G4ThreeVector(0.0*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.85+7)*mm, 130.0*mm));
    /*-*/// Bottom pole
    /*-*/fAlFrame2Solid              = new G4UnionSolid("Part1+Part2+Part3+Part4", fAlFrame2Solid, fAlFramePart3Solid, fNonRotation, G4ThreeVector(0*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75+10)*mm, 0*mm));

    /*-*/// SolidVolume for Panels
    /*-*/fSidePanelSolid             = new G4Box("SidePanelSolid", 0.5*mm, 235/2*mm, 278/2*mm);
    /*-*/fFrontPanelSolid            = new G4Box("FrontPanelSolid", 44.0*mm, 235/2*mm, 0.5*mm);

    /*-*/// Define logical volume for front/side panels
    /*-*/fSidePanelLogical             = new G4LogicalVolume(fSidePanelSolid, FindMaterial("Duralumin"), "SidePanelLogical");
    /*-*/fFrontPanelLogical            = new G4LogicalVolume(fFrontPanelSolid, FindMaterial("Duralumin"), "FrontPanelLogical");

    /*-*/// Define logical volume for tungsten plate
    /*-*/fLargeW_PLLogical                     = new G4LogicalVolume(fLargeW_PLSolid, FindMaterial("G4_AIR"), "LargeW_PLLogical");
    /*-*/fSmallW_PLLogical                     = new G4LogicalVolume(fSmallW_PLSolid, FindMaterial("G4_AIR"), "SmallW_PLLogical");

    /*-*/// Define logical volumes for GSO plate
    /*-*/fLargeGSO_PLLogical                   = new G4LogicalVolume(fLargeGSO_PLSolid, FindMaterial("GSO"), "LargeGSO_PLLogical");
    /*-*/fSmallGSO_PLLogical                   = new G4LogicalVolume(fSmallGSO_PLSolid, FindMaterial("GSO"), "SmallGSO_PLLogical");

    /*-*/// Define logical volumes for GSO bar belt
    /*-*/fGSORightSmallBarBelt_1Logical                   = new G4LogicalVolume(fGSOSmallBarBeltSolid, FindMaterial("G4_AIR"), "GSORightBarBelt_1Logical");
    /*-*/fGSORightSmallBarBelt_2Logical                   = new G4LogicalVolume(fGSOSmallBarBeltSolid, FindMaterial("G4_AIR"), "GSORightBarBelt_2Logical");
    /*-*/fGSORightSmallBarBelt_3Logical                   = new G4LogicalVolume(fGSOSmallBarBeltSolid, FindMaterial("G4_AIR"), "GSORightBarBelt_3Logical");
    /*-*/fGSORightSmallBarBelt_4Logical                   = new G4LogicalVolume(fGSOSmallBarBeltSolid, FindMaterial("G4_AIR"), "GSORightBarBelt_4Logical");
    /*-*/fGSOLeftSmallBarBelt_1Logical                    = new G4LogicalVolume(fGSOSmallBarBeltSolid, FindMaterial("G4_AIR"), "GSOLeftBarBelt_1Logical");
    /*-*/fGSOLeftSmallBarBelt_2Logical                    = new G4LogicalVolume(fGSOSmallBarBeltSolid, FindMaterial("G4_AIR"), "GSOLeftBarBelt_2Logical");
    /*-*/fGSOLeftSmallBarBelt_3Logical                    = new G4LogicalVolume(fGSOSmallBarBeltSolid, FindMaterial("G4_AIR"), "GSOLeftBarBelt_3Logical");
    /*-*/fGSOLeftSmallBarBelt_4Logical                    = new G4LogicalVolume(fGSOSmallBarBeltSolid, FindMaterial("G4_AIR"), "GSOLeftBarBelt_4Logical");
    /*-*/fGSORightLargeBarBelt_1Logical                   = new G4LogicalVolume(fGSOLargeBarBeltSolid, FindMaterial("G4_AIR"), "GSORightBarBelt_1Logical");
    /*-*/fGSORightLargeBarBelt_2Logical                   = new G4LogicalVolume(fGSOLargeBarBeltSolid, FindMaterial("G4_AIR"), "GSORightBarBelt_2Logical");
    /*-*/fGSORightLargeBarBelt_3Logical                   = new G4LogicalVolume(fGSOLargeBarBeltSolid, FindMaterial("G4_AIR"), "GSORightBarBelt_3Logical");
    /*-*/fGSORightLargeBarBelt_4Logical                   = new G4LogicalVolume(fGSOLargeBarBeltSolid, FindMaterial("G4_AIR"), "GSORightBarBelt_4Logical");
    /*-*/fGSOLeftLargeBarBelt_1Logical                    = new G4LogicalVolume(fGSOLargeBarBeltSolid, FindMaterial("G4_AIR"), "GSOLeftBarBelt_1Logical");
    /*-*/fGSOLeftLargeBarBelt_2Logical                    = new G4LogicalVolume(fGSOLargeBarBeltSolid, FindMaterial("G4_AIR"), "GSOLeftBarBelt_2Logical");
    /*-*/fGSOLeftLargeBarBelt_3Logical                    = new G4LogicalVolume(fGSOLargeBarBeltSolid, FindMaterial("G4_AIR"), "GSOLeftBarBelt_3Logical");
    /*-*/fGSOLeftLargeBarBelt_4Logical                    = new G4LogicalVolume(fGSOLargeBarBeltSolid, FindMaterial("G4_AIR"), "GSOLeftBarBelt_4Logical");
    /*-*/// Define logical volumes for GSO bar
    /*-*/fGSORightSmallBarLogical                           = new G4LogicalVolume(fGSOSmallBarSolid, FindMaterial("GSO"), "GSORightSmallBarLogical");
    /*-*/fGSOLeftSmallBarLogical                            = new G4LogicalVolume(fGSOSmallBarSolid, FindMaterial("GSO"), "GSOLeftSmallBarLogical");
    /*-*/fGSORightLargeBarLogical                           = new G4LogicalVolume(fGSOLargeBarSolid, FindMaterial("GSO"), "GSORightLargeBarLogical");
    /*-*/fGSOLeftLargeBarLogical                            = new G4LogicalVolume(fGSOLargeBarSolid, FindMaterial("GSO"), "GSOLeftLargeBarLogical");

    /*-*/// Put panels into ARM1
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector( 0*mm, ((-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75-8)+235/2)*mm, 139.51*mm), fFrontPanelLogical, "FrontPanelPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector( 0*mm, ((-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75-8)+235/2)*mm, -139.51*mm), fFrontPanelLogical, "FrontPanelPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector( 44.51*mm, ((-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75-8)+235/2)*mm, 0.0*mm), fSidePanelLogical, "SidePanelPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector( -44.51*mm, ((-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75-8)+235/2)*mm, 0.0*mm), fSidePanelLogical, "SidePanelPhysical", fARM1Logical, false, 0, checkOverlaps);

    /*-*///Put GSO bar into GSO bar belt
    /*-*/for(int i=0; i<4; i++)
    /*-*/{
    /*-*/
    /*-*/    // Implement first GSO bars
    /*-*/    if(i==0)
    /*-*/        {
    /*-*/                // Insert GSO bars into GSO bar belt
    /*-*/                for(int j=0; j<20; j++)
    /*-*/                    {
    /*-*/                            ypos = j-9.5;
    /*-*/                            new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, ypos*mm, 0*mm), fGSOLeftSmallBarLogical, "GSOLeftSmallBarLogical", fGSOLeftSmallBarBelt_1Logical, false, j, checkOverlaps);
    /*-*/                            xpos = j-9.5;
    /*-*/                            new G4PVPlacement(fRotationZ90, G4ThreeVector(xpos*mm, 0*mm, 0*mm), fGSORightSmallBarLogical, "GSORightSmallBarLogical", fGSORightSmallBarBelt_1Logical, false, j, checkOverlaps);
    /*-*/                        }
    /*-*/        
    /*-*/                for(int j=0; j<40; j++)
    /*-*/                    {
    /*-*/                
    /*-*/                            ypos = j-19.5;
    /*-*/                            new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, ypos*mm, 0*mm), fGSOLeftLargeBarLogical, "GSOLeftLargeBarLogical", fGSOLeftLargeBarBelt_1Logical, false, j, checkOverlaps);
    /*-*/                            xpos = j-19.5;
    /*-*/                            new G4PVPlacement(fRotationZ90, G4ThreeVector(xpos*mm, 0*mm, 0*mm), fGSORightLargeBarLogical, "GSORightLargeBarLogical", fGSORightLargeBarBelt_1Logical, false, j, checkOverlaps);
    /*-*/                        }
    /*-*/        
    /*-*/            }
    /*-*/
    /*-*/    // Implement second GSO bars
    /*-*/    if(i==1)
    /*-*/        {
    /*-*/                // Insert GSO bars into GSO bar belt
    /*-*/                for(int j=20; j<40; j++)
    /*-*/                    {
    /*-*/                            ypos = j%20-9.5;
    /*-*/                            new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, ypos*mm, 0*mm), fGSOLeftSmallBarLogical, "GSOLeftSmallBarLogical", fGSOLeftSmallBarBelt_2Logical, false, j, checkOverlaps);
    /*-*/                            xpos = j%20-9.5;
    /*-*/                            new G4PVPlacement(fRotationZ90, G4ThreeVector(xpos*mm, 0*mm, 0*mm), fGSORightSmallBarLogical, "GSORightSmallBarLogical", fGSORightSmallBarBelt_2Logical, false, j, checkOverlaps);
    /*-*/                        }
    /*-*/        
    /*-*/                for(int j=40; j<80; j++)
    /*-*/                    {
    /*-*/                
    /*-*/                            ypos = j%40-19.5;
    /*-*/                            new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, ypos*mm, 0*mm), fGSOLeftLargeBarLogical, "GSOLeftLargeBarLogical", fGSOLeftLargeBarBelt_2Logical, false, j, checkOverlaps);
    /*-*/                            xpos = j%40-19.5;
    /*-*/                            new G4PVPlacement(fRotationZ90, G4ThreeVector(xpos*mm, 0*mm, 0*mm), fGSORightLargeBarLogical, "GSORightLargeBarLogical", fGSORightLargeBarBelt_2Logical, false, j, checkOverlaps);
    /*-*/                        }
    /*-*/        
    /*-*/            }
    /*-*/
    /*-*/    // Implement third GSO bars
    /*-*/    if(i==2)
    /*-*/        {
    /*-*/                // Insert GSO bars into GSO bar belt
    /*-*/                for(int j=40; j<60; j++)
    /*-*/                    {
    /*-*/                            ypos = j%20-9.5;
    /*-*/                            new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, ypos*mm, 0*mm), fGSOLeftSmallBarLogical, "GSOLeftSmallBarLogical", fGSOLeftSmallBarBelt_3Logical, false, j, checkOverlaps);
    /*-*/                            xpos = j%20-9.5;
    /*-*/                            new G4PVPlacement(fRotationZ90, G4ThreeVector(xpos*mm, 0*mm, 0*mm), fGSORightSmallBarLogical, "GSORightSmallBarLogical", fGSORightSmallBarBelt_3Logical, false, j, checkOverlaps);
    /*-*/                        }
    /*-*/        
    /*-*/                for(int j=80; j<120; j++)
    /*-*/                    {
    /*-*/                
    /*-*/                            ypos = j%40-19.5;
    /*-*/                            new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, ypos*mm, 0*mm), fGSOLeftLargeBarLogical, "GSOLeftLargeBarLogical", fGSOLeftLargeBarBelt_3Logical, false, j, checkOverlaps);
    /*-*/                            xpos = j%40-19.5;
    /*-*/                            new G4PVPlacement(fRotationZ90, G4ThreeVector(xpos*mm, 0*mm, 0*mm), fGSORightLargeBarLogical, "GSORightLargeBarLogical", fGSORightLargeBarBelt_3Logical, false, j, checkOverlaps);
    /*-*/                        }
    /*-*/        
    /*-*/            }
    /*-*/
    /*-*/    // Implement fourth GSO bars
    /*-*/    if(i==2)
    /*-*/        {
    /*-*/                // Insert GSO bars into GSO bar belt
    /*-*/                for(int j=60; j<80; j++)
    /*-*/                    {
    /*-*/                            ypos = j%20-9.5;
    /*-*/                            new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, ypos*mm, 0*mm), fGSOLeftSmallBarLogical, "GSOLeftSmallBarLogical", fGSOLeftSmallBarBelt_4Logical, false, j, checkOverlaps);
    /*-*/                            xpos = j%20-9.5;
    /*-*/                            new G4PVPlacement(fRotationZ90, G4ThreeVector(xpos*mm, 0*mm, 0*mm), fGSORightSmallBarLogical, "GSORightSmallBarLogical", fGSORightSmallBarBelt_4Logical, false, j, checkOverlaps);
    /*-*/                        }
    /*-*/        
    /*-*/                for(int j=80; j<120; j++)
    /*-*/                    {
    /*-*/                
    /*-*/                            ypos = j%40-19.5;
    /*-*/                            new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, ypos*mm, 0*mm), fGSOLeftLargeBarLogical, "GSOLeftLargeBarLogical", fGSOLeftLargeBarBelt_4Logical, false, j, checkOverlaps);
    /*-*/                            xpos = j%40-19.5;
    /*-*/                            new G4PVPlacement(fRotationZ90, G4ThreeVector(xpos*mm, 0*mm, 0*mm), fGSORightLargeBarLogical, "GSORightLargeBarLogical", fGSORightLargeBarBelt_4Logical, false, j, checkOverlaps);
    /*-*/                        }
    /*-*/        
    /*-*/            }
    /*-*/}

    /*-*///Define logical volumes for tungsten plate holder
    /*-*/fWHolder_1Logical                       = new G4LogicalVolume(fWHolder_1Solid, FindMaterial("G10"), "WHolder_1Logical");
    /*-*/fWHolder_2Logical                       = new G4LogicalVolume(fWHolder_2Solid, FindMaterial("G10"), "WHolder_2Logical");
    //Junsang****/*-*/fWHolderPhysical                       = new G4PVPlacement(fRotationZ45, G4ThreeVector(), fGSOLeftSmallBarBelt_1Logical, "WHolderPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*///Define logical volume  for GSO plate holder
    /*-*/fGSO_PLHolderLogical                = new G4LogicalVolume(fGSO_PLHolderSolid, FindMaterial("Acrylic"), "GSO_PLHolderLogical");

    /*-*///Define logical volume for GSO bar holder
    /*-*/fGSOBarHolderLogical                = new G4LogicalVolume(fGSOBarHolderSolid, FindMaterial("Acrylic"), "GSOBarHolderLogical");

    /*-*///Define logical volimes for light guide
    /*-*/fLightGuideLargeLogical             = new G4LogicalVolume(fLightGuideLargeSolid, FindMaterial("Quartz"), "LightGuideLargeLogical");
    /*-*/fLightGuideSmallLogical             = new G4LogicalVolume(fLightGuideSmallSolid, FindMaterial("Quartz"), "LightGuideSmallLogical");

    /*-*/// Put tungstem plate holder into amr1
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-0+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-10.30+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 1, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-20.60+basez+dz)*mm), fWHolder_2Logical, "WHolder_2Physical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-34.70+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 2, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-45.00+basez+dz)*mm), fWHolder_2Logical, "WHolder_2Physical", fARM1Logical, false, 1, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-59.10+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 3, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-69.40+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 4, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-79.70+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 5, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-90.00+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 6, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-100.30+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 7, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-110.60+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 8, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-120.90+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 9, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-128.00+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 10, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-138.30+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 11, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-145.40+basez+dz)*mm), fWHolder_2Logical, "WHolder_2Physical", fARM1Logical, false, 2, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-159.50+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 12, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-166.60+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 13, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-176.90+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 14, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-184.00+basez+dz)*mm), fWHolder_2Logical, "WHolder_2Physical", fARM1Logical, false, 3, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-198.10+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 15, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-205.20+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 16, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-215.50+basez+dz)*mm), fWHolder_1Logical, "WHolder_1Physical", fARM1Logical, false, 17, checkOverlaps);

    /*-*/// Put GSO bar holder into arm1
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-26.05+basez+dz)*mm), fGSOBarHolderLogical, "GSOBarHolderPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-50.45+basez+dz)*mm), fGSOBarHolderLogical, "GSOBarHolderPhysical", fARM1Logical, false, 1, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-150.85+basez+dz)*mm), fGSOBarHolderLogical, "GSOBarHolderPhysical", fARM1Logical, false, 2, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-189.45+basez+dz)*mm), fGSOBarHolderLogical, "GSOBarHolderPhysical", fARM1Logical, false, 3, checkOverlaps);

    /*-*/// Put GSO plate holder into arm1
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-5.15+basez+dz)*mm), fGSO_PLHolderLogical, "GSO_PLHolderPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-15.45+basez+dz)*mm), fGSO_PLHolderLogical, "GSO_PLHolderPhysical", fARM1Logical, false, 1, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-29.55+basez+dz)*mm), fGSO_PLHolderLogical, "GSO_PLHolderPhysical", fARM1Logical, false, 2, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-39.85+basez+dz)*mm), fGSO_PLHolderLogical, "GSO_PLHolderPhysical", fARM1Logical, false, 3, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-53.95+basez+dz)*mm), fGSO_PLHolderLogical, "GSO_PLHolderPhysical", fARM1Logical, false, 4, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-64.25+basez+dz)*mm), fGSO_PLHolderLogical, "GSO_PLHolderPhysical", fARM1Logical, false, 5, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-74.55+basez+dz)*mm), fGSO_PLHolderLogical, "GSO_PLHolderPhysical", fARM1Logical, false, 6, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-84.85+basez+dz)*mm), fGSO_PLHolderLogical, "GSO_PLHolderPhysical", fARM1Logical, false, 7, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-95.15+basez+dz)*mm), fGSO_PLHolderLogical, "GSO_PLHolderPhysical", fARM1Logical, false, 8, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-105.45+basez+dz)*mm), fGSO_PLHolderLogical, "GSO_PLHolderPhysical", fARM1Logical, false, 9, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-115.75+basez+dz)*mm), fGSO_PLHolderLogical, "GSO_PLHolderPhysical", fARM1Logical, false, 10, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-133.15+basez+dz)*mm), fGSO_PLHolderLogical, "GSO_PLHolderPhysical", fARM1Logical, false, 11, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-154.35+basez+dz)*mm), fGSO_PLHolderLogical, "GSO_PLHolderPhysical", fARM1Logical, false, 12, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-171.75+basez+dz)*mm), fGSO_PLHolderLogical, "GSO_PLHolderPhysical", fARM1Logical, false, 13, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-192.95+basez+dz)*mm), fGSO_PLHolderLogical, "GSO_PLHolderPhysical", fARM1Logical, false, 14, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-210.35+basez+dz)*mm), fGSO_PLHolderLogical, "GSO_PLHolderPhysical", fARM1Logical, false, 15, checkOverlaps);

    /*-*/// Put tungsten plates
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-0+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-10.30+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 1, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-20.60+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-34.70+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 2, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-45.00+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 1, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-59.10+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 3, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-69.40+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 4, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-79.70+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 5, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-90.00+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 6, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-100.30+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 7, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-110.60+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 8, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-120.90+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 9, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-128.00+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 10, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-138.30+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 11, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-145.40+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 2, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-159.50+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 12, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-166.60+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 13, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-176.90+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 14, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-184.00+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 3, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-198.10+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 15, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-205.20+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 16, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-215.50+basez+dz)*mm), fLargeW_PLLogical, "LargeW_PLPhysical", fARM1Logical, false, 17, checkOverlaps);
    /*-*///// Put small tungsten plate into arm1
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-0+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-10.30+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 1, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-20.60+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-34.70+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 2, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-45.00+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 1, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-59.10+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 3, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-69.40+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 4, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-79.70+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 5, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-90.00+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 6, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-100.30+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 7, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-110.60+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 8, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-120.90+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 9, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-128.00+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 10, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-138.30+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 11, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-145.40+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 2, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-159.50+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 12, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-166.60+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 13, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-176.90+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 14, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-184.00+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 3, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-198.10+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 15, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-205.20+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 16, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-215.50+basez+dz)*mm), fSmallW_PLLogical, "SmallW_PLPhysical", fARM1Logical, false, 17, checkOverlaps);


    /*-*/// Put GSO plate into arm1
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-5.15+basez+dz)*mm), fLargeGSO_PLLogical, "LargeGSO_PLPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-15.45+basez+dz)*mm), fLargeGSO_PLLogical, "LargeGSO_PLPhysical", fARM1Logical, false, 1, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-29.55+basez+dz)*mm), fLargeGSO_PLLogical, "LargeGSO_PLPhysical", fARM1Logical, false, 2, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-39.85+basez+dz)*mm), fLargeGSO_PLLogical, "LargeGSO_PLPhysical", fARM1Logical, false, 3, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-53.95+basez+dz)*mm), fLargeGSO_PLLogical, "LargeGSO_PLPhysical", fARM1Logical, false, 4, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-64.25+basez+dz)*mm), fLargeGSO_PLLogical, "LargeGSO_PLPhysical", fARM1Logical, false, 5, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-74.55+basez+dz)*mm), fLargeGSO_PLLogical, "LargeGSO_PLPhysical", fARM1Logical, false, 6, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-84.85+basez+dz)*mm), fLargeGSO_PLLogical, "LargeGSO_PLPhysical", fARM1Logical, false, 7, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-95.15+basez+dz)*mm), fLargeGSO_PLLogical, "LargeGSO_PLPhysical", fARM1Logical, false, 8, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-105.45+basez+dz)*mm), fLargeGSO_PLLogical, "LargeGSO_PLPhysical", fARM1Logical, false, 9, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-115.75+basez+dz)*mm), fLargeGSO_PLLogical, "LargeGSO_PLPhysical", fARM1Logical, false, 10, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-133.15+basez+dz)*mm), fLargeGSO_PLLogical, "LargeGSO_PLPhysical", fARM1Logical, false, 11, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-154.35+basez+dz)*mm), fLargeGSO_PLLogical, "LargeGSO_PLPhysical", fARM1Logical, false, 12, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-171.75+basez+dz)*mm), fLargeGSO_PLLogical, "LargeGSO_PLPhysical", fARM1Logical, false, 13, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-192.95+basez+dz)*mm), fLargeGSO_PLLogical, "LargeGSO_PLPhysical", fARM1Logical, false, 14, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-210.35+basez+dz)*mm), fLargeGSO_PLLogical, "LargeGSO_PLPhysical", fARM1Logical, false, 15, checkOverlaps);
    /*-*/// Small GSO plate
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-5.15+basez+dz)*mm), fSmallGSO_PLLogical, "SmallGSO_PLPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-15.45+basez+dz)*mm), fSmallGSO_PLLogical, "SmallGSO_PLPhysical", fARM1Logical, false, 1, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-29.55+basez+dz)*mm), fSmallGSO_PLLogical, "SmallGSO_PLPhysical", fARM1Logical, false, 2, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-39.85+basez+dz)*mm), fSmallGSO_PLLogical, "SmallGSO_PLPhysical", fARM1Logical, false, 3, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-53.95+basez+dz)*mm), fSmallGSO_PLLogical, "SmallGSO_PLPhysical", fARM1Logical, false, 4, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-64.25+basez+dz)*mm), fSmallGSO_PLLogical, "SmallGSO_PLPhysical", fARM1Logical, false, 5, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-74.55+basez+dz)*mm), fSmallGSO_PLLogical, "SmallGSO_PLPhysical", fARM1Logical, false, 6, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-84.85+basez+dz)*mm), fSmallGSO_PLLogical, "SmallGSO_PLPhysical", fARM1Logical, false, 7, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-95.15+basez+dz)*mm), fSmallGSO_PLLogical, "SmallGSO_PLPhysical", fARM1Logical, false, 8, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-105.45+basez+dz)*mm), fSmallGSO_PLLogical, "SmallGSO_PLPhysical", fARM1Logical, false, 9, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-115.75+basez+dz)*mm), fSmallGSO_PLLogical, "SmallGSO_PLPhysical", fARM1Logical, false, 10, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-133.15+basez+dz)*mm), fSmallGSO_PLLogical, "SmallGSO_PLPhysical", fARM1Logical, false, 11, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-154.35+basez+dz)*mm), fSmallGSO_PLLogical, "SmallGSO_PLPhysical", fARM1Logical, false, 12, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-171.75+basez+dz)*mm), fSmallGSO_PLLogical, "SmallGSO_PLPhysical", fARM1Logical, false, 13, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-192.95+basez+dz)*mm), fSmallGSO_PLLogical, "SmallGSO_PLPhysical", fARM1Logical, false, 14, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-210.35+basez+dz)*mm), fSmallGSO_PLLogical, "SmallGSO_PLPhysical", fARM1Logical, false, 15, checkOverlaps);


    /*-*/// Put Light guide into arm1
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-5.15+basez+dz)*mm), fLightGuideLargeLogical, "LightGuideLargePhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-15.45+basez+dz)*mm), fLightGuideLargeLogical, "LightGuideLargePhysical", fARM1Logical, false, 1, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-29.55+basez+dz)*mm), fLightGuideLargeLogical, "LightGuideLargePhysical", fARM1Logical, false, 2, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-39.85+basez+dz)*mm), fLightGuideLargeLogical, "LightGuideLargePhysical", fARM1Logical, false, 3, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-53.95+basez+dz)*mm), fLightGuideLargeLogical, "LightGuideLargePhysical", fARM1Logical, false, 4, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-64.25+basez+dz)*mm), fLightGuideLargeLogical, "LightGuideLargePhysical", fARM1Logical, false, 5, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-74.55+basez+dz)*mm), fLightGuideLargeLogical, "LightGuideLargePhysical", fARM1Logical, false, 6, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-84.85+basez+dz)*mm), fLightGuideLargeLogical, "LightGuideLargePhysical", fARM1Logical, false, 7, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-95.15+basez+dz)*mm), fLightGuideLargeLogical, "LightGuideLargePhysical", fARM1Logical, false, 8, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-105.45+basez+dz)*mm), fLightGuideLargeLogical, "LightGuideLargePhysical", fARM1Logical, false, 9, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-115.75+basez+dz)*mm), fLightGuideLargeLogical, "LightGuideLargePhysical", fARM1Logical, false, 10, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-133.15+basez+dz)*mm), fLightGuideLargeLogical, "LightGuideLargePhysical", fARM1Logical, false, 11, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-154.35+basez+dz)*mm), fLightGuideLargeLogical, "LightGuideLargePhysical", fARM1Logical, false, 12, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-171.75+basez+dz)*mm), fLightGuideLargeLogical, "LightGuideLargePhysical", fARM1Logical, false, 13, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-192.95+basez+dz)*mm), fLightGuideLargeLogical, "LightGuideLargePhysical", fARM1Logical, false, 14, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, 0*mm, (-210.35+basez+dz)*mm), fLightGuideLargeLogical, "LightGuideLargePhysical", fARM1Logical, false, 15, checkOverlaps);
    /*-*/// Small Light guide
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-5.15+basez+dz)*mm), fLightGuideSmallLogical, "LightGuideSmallPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-15.45+basez+dz)*mm), fLightGuideSmallLogical, "LightGuideSmallPhysical", fARM1Logical, false, 1, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-29.55+basez+dz)*mm), fLightGuideSmallLogical, "LightGuideSmallPhysical", fARM1Logical, false, 2, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-39.85+basez+dz)*mm), fLightGuideSmallLogical, "LightGuideSmallPhysical", fARM1Logical, false, 3, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-53.95+basez+dz)*mm), fLightGuideSmallLogical, "LightGuideSmallPhysical", fARM1Logical, false, 4, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-64.25+basez+dz)*mm), fLightGuideSmallLogical, "LightGuideSmallPhysical", fARM1Logical, false, 5, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-74.55+basez+dz)*mm), fLightGuideSmallLogical, "LightGuideSmallPhysical", fARM1Logical, false, 6, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-84.85+basez+dz)*mm), fLightGuideSmallLogical, "LightGuideSmallPhysical", fARM1Logical, false, 7, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-95.15+basez+dz)*mm), fLightGuideSmallLogical, "LightGuideSmallPhysical", fARM1Logical, false, 8, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-105.45+basez+dz)*mm), fLightGuideSmallLogical, "LightGuideSmallPhysical", fARM1Logical, false, 9, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-115.75+basez+dz)*mm), fLightGuideSmallLogical, "LightGuideSmallPhysical", fARM1Logical, false, 10, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-133.15+basez+dz)*mm), fLightGuideSmallLogical, "LightGuideSmallPhysical", fARM1Logical, false, 11, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-154.35+basez+dz)*mm), fLightGuideSmallLogical, "LightGuideSmallPhysical", fARM1Logical, false, 12, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-171.75+basez+dz)*mm), fLightGuideSmallLogical, "LightGuideSmallPhysical", fARM1Logical, false, 13, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-192.95+basez+dz)*mm), fLightGuideSmallLogical, "LightGuideSmallPhysical", fARM1Logical, false, 14, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-210.35+basez+dz)*mm), fLightGuideSmallLogical, "LightGuideSmallPhysical", fARM1Logical, false, 15, checkOverlaps);

    /*-*/// Put GSO large bar belt into arm1
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-26.05+basez+dz+0.11)*mm), fGSORightLargeBarBelt_1Logical, "GSORightLargeBarBelt_1Physical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-50.45+basez+dz+0.11)*mm), fGSORightLargeBarBelt_2Logical, "GSORightLargeBarBelt_2Physical", fARM1Logical, false, 1, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-150.85+basez+dz+0.11)*mm), fGSORightLargeBarBelt_3Logical, "GSORightLargeBarBelt_3Physical", fARM1Logical, false, 2, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-189.45+basez+dz+0.11)*mm), fGSORightLargeBarBelt_4Logical, "GSORightLargeBarBelt_4Physical", fARM1Logical, false, 3, checkOverlaps);
    // Put GSO small bar bet into arm1
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-26.05+basez+dz+1.12)*mm), fGSOLeftLargeBarBelt_1Logical, "GSOLeftLargeBarBelt_1Physical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-50.45+basez+dz+1.12)*mm), fGSOLeftLargeBarBelt_2Logical, "GSOLeftLargeBarBelt_2Physical", fARM1Logical, false, 1, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-150.85+basez+dz+1.12)*mm), fGSOLeftLargeBarBelt_3Logical, "GSOLeftLargeBarBelt_3Physical", fARM1Logical, false, 2, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, 0*mm, (-189.45+basez+dz+1.12)*mm), fGSOLeftLargeBarBelt_4Logical, "GSOLeftLargeBarBelt_4Physical", fARM1Logical, false, 3, checkOverlaps);


    /*-*/// Put GSO large bar belt into arm1
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-26.05+basez+dz+0.11)*mm), fGSORightSmallBarBelt_1Logical, "GSORightSmallBarBelt_1Physical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-50.45+basez+dz+0.11)*mm), fGSORightSmallBarBelt_2Logical, "GSORightSmallBarBelt_2Physical", fARM1Logical, false, 1, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-150.85+basez+dz+0.11)*mm), fGSORightSmallBarBelt_3Logical, "GSORightSmallBarBelt_3Physical", fARM1Logical, false, 2, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-189.45+basez+dz+0.11)*mm), fGSORightSmallBarBelt_4Logical, "GSORightSmallBarBelt_4Physical", fARM1Logical, false, 3, checkOverlaps);
    /*-*/// Put GSO small bar belt into arm1
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-26.05+basez+dz+1.12)*mm), fGSOLeftSmallBarBelt_1Logical, "GSOLeftSmallBarBelt_1Physical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-50.45+basez+dz+1.12)*mm), fGSOLeftSmallBarBelt_2Logical, "GSOLeftSmallBarBelt_2Physical", fARM1Logical, false, 1, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-150.85+basez+dz+1.12)*mm), fGSOLeftSmallBarBelt_3Logical, "GSOLeftSmallBarBelt_3Physical", fARM1Logical, false, 2, checkOverlaps);
    /*-*/new G4PVPlacement(fRotationZ45, G4ThreeVector(0*mm, -(30.1*sqrt(2)+5)*mm, (-189.45+basez+dz+1.12)*mm), fGSOLeftSmallBarBelt_4Logical, "GSOLeftSmallBarBelt_4Physical", fARM1Logical, false, 3, checkOverlaps);


    /*-*/// Define Logical volume for Al frame
    /*-*/fAlFrame1Logical      =   new G4LogicalVolume(fAlFrame1Solid, FindMaterial("G4_Al"), "AlFrame1Logical");// Top part
    /*-*/fAlFrame2Logical      =   new G4LogicalVolume(fAlFrame2Solid, FindMaterial("G4_Al"), "AlFrame2Logical");// Bottom part

    /*-*/// Put Al frame into arm1
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(), fAlFrame1Logical, "AlFrame1Physical", fARM1Logical, false, 0, checkOverlaps);// Top part
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(), fAlFrame2Logical, "AlFrame2Physical", fARM1Logical, false, 0, checkOverlaps);// Bottom part

    /*-*/// Setting for color
    /*-*///Junsang****visAttributes           = new G4VisAttributes(G4Colour(2.0, 2.0, 0)); //Yellow
    /*-*///Junsang****visAttributes       = new G4VisAttributes(G4Colour(0.7, 0.4, 0.1));// Brown
    /*-*///Junsang****visAttributes           = new G4VisAttributes(G4Colour(0.0, 0.7, 0.0));//Green
    /*-*///Junsang****visAttributes           = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));//Blue
    /*-*///Junsang****visAttributes           = new G4VisAttributes(G4Colour(0.0, 0.0, 0.5));//Dark blue
    /*-*///Junsang****visAttributes           = new G4VisAttributes(G4Colour(0.1, 0.1, 0.1));//Black
    /*-*///Junsang****visAttributes           = new G4VisAttributes(G4Colour(0.2, 0.2, 0.2));//Dark grey
    /*-*///Junsang****visAttributes           = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3));//Dark grey
    /*-*///Junsang****visAttributes           = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));//Light green
    /*-*///Junsang****visAttributes           = new G4VisAttributes(G4Colour(0.0, 0.5, 0.0));//Dark green
    /*-*///Junsang****visAttributes           = new G4VisAttributes(G4Colour(0.0, 10.0, 0.0));//Ligtest green
    /*-*///Junsang****visAttributes           = new G4VisAttributes(G4Colour(10.0, 0.0, 0.0));//Lightest red
    /*-*///Junsang****visAttributes           = new G4VisAttributes(G4Colour(10.0, 0.0, 2.0));//Light magenta
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));//Blue
    /*-*/fSidePanelLogical               -> SetVisAttributes(visAttributes);
    /*-*/fFrontPanelLogical               -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 2.0, 2.0));//Light azure: GSO plate
    /*-*/fLargeGSO_PLLogical         -> SetVisAttributes(visAttributes);
    /*-*/fSmallGSO_PLLogical         -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(2.0, 2.0, 0)); //Yellow: Quartz
    /*-*/fLightGuideLargeLogical     -> SetVisAttributes(visAttributes);
    /*-*/fLightGuideSmallLogical     -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.9, 0.9, 0.9)); //Light grey: Acrylic
    /*-*/fGSO_PLHolderLogical        -> SetVisAttributes(visAttributes);
    /*-*/fGSOBarHolderLogical        -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3));//Dark grey: G10
    /*-*/fWHolder_1Logical           -> SetVisAttributes(visAttributes);
    /*-*/fWHolder_2Logical           -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 0.0, 0.5));//Dark blue: Tungsten
    /*-*/fLargeW_PLLogical           -> SetVisAttributes(visAttributes);
    /*-*/fSmallW_PLLogical           -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));//Light green: GSO bar
    /*-*/fGSOLeftSmallBarLogical     -> SetVisAttributes(visAttributes);
    /*-*/fGSORightSmallBarLogical    -> SetVisAttributes(visAttributes);
    /*-*/fGSOLeftLargeBarLogical     -> SetVisAttributes(visAttributes);
    /*-*/fGSORightLargeBarLogical    -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    /*-*/visAttributes           -> SetVisibility(false);//Transparant: ARM1, GSOBarBelt
    /*-*/fARM1Logical            -> SetVisAttributes(visAttributes);
    /*-*/fGSORightLargeBarBelt_1Logical            -> SetVisAttributes(visAttributes);
    /*-*/fGSORightLargeBarBelt_2Logical            -> SetVisAttributes(visAttributes);
    /*-*/fGSORightLargeBarBelt_3Logical            -> SetVisAttributes(visAttributes);
    /*-*/fGSORightLargeBarBelt_4Logical            -> SetVisAttributes(visAttributes);
    /*-*/fGSOLeftLargeBarBelt_1Logical            -> SetVisAttributes(visAttributes);
    /*-*/fGSOLeftLargeBarBelt_2Logical            -> SetVisAttributes(visAttributes);
    /*-*/fGSOLeftLargeBarBelt_3Logical            -> SetVisAttributes(visAttributes);
    /*-*/fGSOLeftLargeBarBelt_4Logical            -> SetVisAttributes(visAttributes);
    /*-*/fGSORightSmallBarBelt_1Logical            -> SetVisAttributes(visAttributes);
    /*-*/fGSORightSmallBarBelt_2Logical            -> SetVisAttributes(visAttributes);
    /*-*/fGSORightSmallBarBelt_3Logical            -> SetVisAttributes(visAttributes);
    /*-*/fGSORightSmallBarBelt_4Logical            -> SetVisAttributes(visAttributes);
    /*-*/fGSOLeftSmallBarBelt_1Logical            -> SetVisAttributes(visAttributes);
    /*-*/fGSOLeftSmallBarBelt_2Logical            -> SetVisAttributes(visAttributes);
    /*-*/fGSOLeftSmallBarBelt_3Logical            -> SetVisAttributes(visAttributes);
    /*-*/fGSOLeftSmallBarBelt_4Logical            -> SetVisAttributes(visAttributes);
    // Make all parts invisible
    //Junsang****/*-*/fLargeGSO_PLLogical         -> SetVisAttributes(visAttributes);
    //Junsang****/*-*/fSmallGSO_PLLogical         -> SetVisAttributes(visAttributes);
    //Junsang****/*-*/fLightGuideLargeLogical     -> SetVisAttributes(visAttributes);
    //Junsang****/*-*/fLightGuideSmallLogical     -> SetVisAttributes(visAttributes);
    //Junsang****/*-*/fGSO_PLHolderLogical        -> SetVisAttributes(visAttributes);
    //Junsang****/*-*/fGSOBarHolderLogical        -> SetVisAttributes(visAttributes);
    //Junsang****/*-*/fWHolder_1Logical           -> SetVisAttributes(visAttributes);
    //Junsang****/*-*/fWHolder_2Logical           -> SetVisAttributes(visAttributes);
    //Junsang****/*-*/fLargeW_PLLogical           -> SetVisAttributes(visAttributes);
    //Junsang****/*-*/fSmallW_PLLogical           -> SetVisAttributes(visAttributes);
    //Junsang****/*-*/fGSOLeftSmallBarLogical     -> SetVisAttributes(visAttributes);
    //Junsang****/*-*/fGSORightSmallBarLogical    -> SetVisAttributes(visAttributes);
    //Junsang****/*-*/fGSOLeftLargeBarLogical     -> SetVisAttributes(visAttributes);
    //Junsang****/*-*/fGSORightLargeBarLogical    -> SetVisAttributes(visAttributes);
    /*-*/fSidePanelLogical               -> SetVisAttributes(visAttributes);
    /*-*/fFrontPanelLogical               -> SetVisAttributes(visAttributes);
    //
    //
    //
    /*-*/fVisAttributes.push_back(visAttributes);

    /*-*/return fARM1Physical;
}

G4VPhysicalVolume* RHICFDetectorConstruction::STARPIPE(G4VPhysicalVolume* world_phys, G4ThreeVector vector, G4RotationMatrix* mat)
{

    return fSTARPIPEPhysical;
}

