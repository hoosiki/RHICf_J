//Kinds of Detector: ZDC(PHENIX), STARZDC, BBC, STARBBC, PHENIXPIPE, STARPIPE
#include "RHICFDetectorConstruction.hh"
#include "RHICFManager.hh"
#include "MagneticField.hh"
#include "RHICFMaterials.hh"
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
#include "G4Tubs.hh"
#include "G4Trd.hh"
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

    /*-*/// DEFINE WORLD VOLUME
    /*-*/fWorldSolid             = new G4Box("WorldSolid", 50*cm, 80*cm, 2000*cm);
    /*-*/fWorldLogical           = new G4LogicalVolume(fWorldSolid, FindMaterial("G4_AIR"), "WorldLogical");
    /*-*/fWorldPhysical          = new G4PVPlacement(0, G4ThreeVector(), fWorldLogical, "WorldPhysical", 0, false, checkOverlaps);
    /*-*/// SETTING FOR COLOR OF WORLD VOLUME
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    /*-*/visAttributes           -> SetVisibility(false);
    /*-*/fWorldLogical           -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);

    /*-*/// DEFINE ROTATION MATRIX
    /*-*/SMDVRotation            = new G4RotationMatrix();
    /*-*/SMDHRotation          = new G4RotationMatrix();
    /*-*/SMDH2Rotation          = new G4RotationMatrix();
    /*-*/fSMDRotation            = new G4RotationMatrix();
    /*-*/fSMDRotation            -> rotateY(90*deg);
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

    /*-*///SET GEOMETRY
    //Junsang****PHENIXPIPE();
    //Junsang****/*-*/STARZDCINSTALL(fWorldPhysical, G4ThreeVector(0.*cm, 0.*cm, 50*cm), fRotationY180);
    RHICFManager::GetInstance()->SetARM1Z(1787);
    //Junsang****RHICFManager::GetInstance()->SetARM1Y(7.16);//TOP CENTER
    //Junsang****RHICFManager::GetInstance()->SetARM1Y(4.76);//TS CENTER
    RHICFManager::GetInstance()->SetARM1Y(0.);//TS CENTER
    //Junsang****RHICFManager::GetInstance()->SetARM1Z(50.);
    
    /*-*/STARPIPEINSTALL(RHICFManager::GetInstance()->GetARM1Y(), (RHICFManager::GetInstance()->GetARM1Z()-14.15));
    /*-*/ARM1INSTALL(fWorldPhysical, G4ThreeVector(0.*cm, RHICFManager::GetInstance()->GetARM1Y()*cm, RHICFManager::GetInstance()->GetARM1Z()*cm), fRotationY180);
    //Junsang****/*-*/STARZDCINSTALL(fWorldPhysical, G4ThreeVector(0.*cm, 0.*cm, 1867.59*cm), fRotationY180);
    /*-*/STARZDCINSTALL(fWorldPhysical, G4ThreeVector(0.*cm, 0.*cm, 1854.59*cm), fRotationY180);


    
    return                  fWorldPhysical;

}

void RHICFDetectorConstruction::ConstructSDandField()
{
    G4SDManager::GetSDMpointer() -> SetVerboseLevel(0);
    if(G4PhysicalVolumeStore::GetInstance()->GetVolume("FrontCounterSmallPhysical",false))
    {
        SetSDForFrontCounter();
    }
    if(G4PhysicalVolumeStore::GetInstance()->GetVolume("ARM1Physical",false))
    {
        SetSDForARM1();
    }
    if(G4PhysicalVolumeStore::GetInstance()->GetVolume("STARZDCPhysical",false))
    {
        SetSDForSTARZDC();
    }
    if(G4PhysicalVolumeStore::GetInstance()->GetVolume("55InchPipeVacuumPhysical", false))
    {
        SetMagneticField(f55InchPipeVacuumLogical);
    }

}

void RHICFDetectorConstruction::DefineDimension()
{
    /*-*/// DEFINE PARAMETERS FOR GEOMETRY
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
    /*-*/// Parameters for ARM1
    /*-*/kARM1par[0] = 4.5;
    /*-*/kARM1par[1] = 12.65;
    /*-*/kARM1par[2] = 17;
    /*-*/kNegativeLargeWpar[0] = 4.012/2;
    /*-*/kNegativeLargeWpar[1] = 4.012/2;
    /*-*/kNegativeLargeWpar[2] = 7.2/2;
    /*-*/kNegativeSmallWpar[0] = 2.012/2;
    /*-*/kNegativeSmallWpar[1] = 2.012/2;
    /*-*/kNegativeSmallWpar[2] = 7.2/2;
    

    /*-*/basez = 116.75;
    /*-*/dz  = 0;
}


void RHICFDetectorConstruction::SetMagneticField(G4LogicalVolume* logical )
{
    /*-*/// Set Magnetic Field
    /*-*/fMagneticField          = new MagneticField();
    /*-*/fFieldMgr               = new G4FieldManager();
    /*-*/fFieldMgr               -> SetDetectorField(fMagneticField);
    /*-*/fFieldMgr               -> CreateChordFinder(fMagneticField);
    /*-*/G4bool forceToAllDaughters = true;
    /*-*///fMagneticLogical -> SetFieldManager(fFieldMgr, forceToAllDaughters);//PHENIXPIPE
    //Junsang****f55InchPipeVacuumLogical-> SetFieldManager(fFieldMgr, forceToAllDaughters);//STARPIPE
    logical -> SetFieldManager(fFieldMgr, forceToAllDaughters);//STARPIPE

    /*-*/G4AutoDelete::Register(fMagneticField);
    /*-*/G4AutoDelete::Register(fFieldMgr);


}

void RHICFDetectorConstruction::ConstructMaterials ( )
{
}

void RHICFDetectorConstruction::DestructMaterials ( )
{
}

G4Material* RHICFDetectorConstruction::FindMaterial(G4String name)
{
    G4Material* material                = G4Material::GetMaterial(name, true);
    return material;
}

// Define 'PHENIXZDC-PHENIX ZDC'
// PHENIXZDC is space which contains 'ZDC', 'RCSC', 'FCSC'
//
// Structure of 'PHENIXZDC'
// 
// 'PHENIXZDC'-----Nmods of 'ZDC'----26 of 'GAPF's----200 of 'FIBR's
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
void RHICFDetectorConstruction::PHENIXZDCINSTALL(G4VPhysicalVolume* world_phys, G4ThreeVector vector, G4RotationMatrix* mat)
{

    /*-*/fPHENIXZDCSolid              = new G4Box("PHENIXZDCSolid", wcntPar[0]*cm, wcntPar[1]*cm, wcntPar[2]*3.6/5*cm);
    /*-*/fPHENIXZDCLogical            = new G4LogicalVolume(fPHENIXZDCSolid, FindMaterial("G4_AIR"), "PHENIXZDCLogical");
    /*-*/fPHENIXZDCPhysical           = new G4PVPlacement(mat, vector, "PHENIXZDCPhysical", fPHENIXZDCLogical,  world_phys, false, 0, checkOverlaps);


    /*-*///-----------------------------------------------------------------------------------------------------------
    /*-*/// DEFINE SOLIDVOLUME
    /*-*/// DEFINE 'ZDC'
    /*-*/fZDCSolid               = new G4Box("ZDCSolid", zdcPar[0]*cm, zdcPar1*cm, zdcPar2*cm);
    /*-*/// DEFINE 'GAPF' : GAPF IS GAP WHICH HAS FIBERS
    /*-*/fGAPFSolid              = new G4Box("GAPFSolid", gapper[0]*cm, fibPar[1]*cm, (gapper[2]+0.5)*cm);
    /*-*/// DEFINE 'LAYER BETWEEN CLADDING AND GAP'
    /*-*/fFIBRSolid              = new G4Tubs("FIBRSolid", fibPar[0]*cm, fibPar[1]*cm, fibPar[2]/2.*cm, 0, twopi);
    /*-*/// DEFINE 'IRON PLATE' : I_PL IS PLATE WHICH MADE OF IRON
    /*-*/fI_PLSolid              = new G4Box("I_PLSolid", iplPar[0]*cm, iplPar[1]*cm, iplPar[2]*cm);
    /*-*/// DEFINE 'HALF OF PMMA FIBER'
    /*-*/fFIBSolid               = new G4Tubs("FIBSolid", fibPar[0]*cm, fibPar[1]*cm, gapper[2]/2.*cm, 0, twopi);
    /*-*/// DEFINE 'TENGSTEN PLATE' : W_PL IS PLATE WHICH MADE OF TUNGSTEN
    /*-*/fW_PLSolid              = new G4Box("W_PLSolid", wplPar[0]*cm, wplPar[2]*cm, wplPar[1]*cm);
    /*-*/// DEFINE 'BAR FOR ELIMINATING OPTICAL PHOTON PROPAGATING DOWNWARD'
    /*-*/fBlockerSolid           = new G4Box("BlockerSolid", gapper[0]*cm, fibPar[1]*cm, 0.005*cm);
    /*-*/// DEFINE 'SMD'
    /*-*/fSMDSolid               = new G4Box("SMDSolid", smdPar[0]*cm, smdPar[1]*cm, smdPar[2]*cm);
    /*-*/// DEFINE 'SMDH'
    /*-*/fSMDHSolid     = new G4Box("SMDHSolid", smdhPar[0]*cm, smdhPar[1]*cm, smdhPar[2]*cm);
    /*-*/// DEFINE 'FEPL'
    /*-*/fFEPLSolid              = new G4Box("FEPLSolid", fePar[0]*cm, fePar[1]*cm, fePar[2]);
    /*-*/// DEFINE 'ALPL' 
    /*-*/fALPLSolid              = new G4Box("ALPLSolid",alPar[0]*cm, alPar[1]*cm, alPar[2]*cm);
    /*-*/// DEFINE 'SMDV'
    /*-*/fSMDVSolid              = new G4Box("SMDVSolid", smdvPar[0]*cm, smdvPar[1]*cm, smdvPar[2]*cm);

    /*-*///------------------------------------------------------------------------------------------------------------
    /*-*/// DEFINE LOGICALVOLUME
    /*-*/fZDCLogical             = new G4LogicalVolume(fZDCSolid, FindMaterial("G4_AIR"), "ZDCLogical");
    /*-*/fGAPF_1Logical          = new G4LogicalVolume(fGAPFSolid, FindMaterial("PMMA"), "GAPF_1Logical");
    /*-*/fGAPF_2Logical          = new G4LogicalVolume(fGAPFSolid, FindMaterial("PMMA"), "GAPF_2Logical");
    /*-*/fGAPF_3Logical          = new G4LogicalVolume(fGAPFSolid, FindMaterial("PMMA"), "GAPF_3Logical");
    /*-*/fI_PLLogical            = new G4LogicalVolume(fI_PLSolid, FindMaterial("G4_Fe"), "I_PLLogical");
    /*-*/fW_PL_1Logical          = new G4LogicalVolume(fW_PLSolid, FindMaterial("G4_W"), "W_PL_1Logical");
    /*-*/fW_PL_2Logical          = new G4LogicalVolume(fW_PLSolid, FindMaterial("G4_W"), "W_PL_2Logical");
    /*-*/fW_PL_3Logical          = new G4LogicalVolume(fW_PLSolid, FindMaterial("G4_W"), "W_PL_3Logical");
    /*-*/fSMDLogical             = new G4LogicalVolume(fSMDSolid, FindMaterial("G4_AIR"), "fSMDLogical");
    /*-*/// HORIZONTAL SMD BAR
    /*-*/fSMDHLogical            = new G4LogicalVolume(fSMDHSolid, FindMaterial("G4_PLASTIC_SC_VINYLTOLUENE"), "SMDHLogical");
    /*-*/fFEPLLogical            = new G4LogicalVolume(fFEPLSolid, FindMaterial("G4_Al"), "FEPLLogical");
    /*-*/fALPLLogical            = new G4LogicalVolume(fALPLSolid, FindMaterial("G4_Fe"), "ALPLLogical");
    /*-*/// VERTICAL SMD BAR
    /*-*/fSMDVLogical            = new G4LogicalVolume(fSMDVSolid, FindMaterial("G4_PLASTIC_SC_VINYLTOLUENE"), "SMDVLogical");

    //------------------------------------------------------------------------------------------------------------
    // DEFINE PHYSICALVOLUME
    /*-*/// Put gaps which include fibers into PHENIXZDC
    /*-*/if(Nmod>0 && Nmod<4)
    /*-*/{
    /*-*/    for(G4int i=1; i<Nlay + 1; i++)
    /*-*/    {
    /*-*/        ypos            = 1.85*cos;
    /*-*/        interval        = (gapper[1]+wplPar[1])*2.0/sin;
    /*-*/        zpos            = 8 + Lmod - (2.0*iplPar[1]+2.0*wplPar[1]+gapper[1])/sin - interval*(i-1)-1.85*cos;
    /*-*/        if(Nmod>=1)
    /*-*/            fGAPF_1Physical = new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]* sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fGAPF_1Logical, "GAPF_1Physical", fPHENIXZDCLogical, true, i, checkOverlaps);
    /*-*/        if(Nmod>=2)
    /*-*/            fGAPF_2Physical = new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]* sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fGAPF_2Logical, "GAPF_2Physical", fPHENIXZDCLogical, true, i, checkOverlaps);
    /*-*/        if(Nmod>=3)
    /*-*/            fGAPF_3Physical = new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]* sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fGAPF_3Logical, "GAPF_3Physical", fPHENIXZDCLogical, true, i, checkOverlaps);
    /*-*/    }
    /*-*/}


    /*-*/// PUT FRONT AND BEHIND IRON PLATE INTO PHENIXZDC
    /*-*/if(Nmod>0 && Nmod<4)
    /*-*/{
    /*-*/    ypos                = 1.85*cos;
    /*-*/    zpos                = 8 + Lmod - iplPar[1]/sin - 1.85*cos;
    /*-*/    if(Nmod>=1)
    /*-*/    {
    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fI_PLLogical, "I_PLPhysical", fPHENIXZDCLogical, false, 0, checkOverlaps);
    /*-*/    }
    /*-*/    if(Nmod>=2)
    /*-*/    {
    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm,(Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fI_PLLogical, "I_PLPhysical", fPHENIXZDCLogical, false, 0, checkOverlaps);
    /*-*/    }
    /*-*/    if(Nmod>=3)
    /*-*/    {
    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fI_PLLogical, "I_PLPhysical", fPHENIXZDCLogical, false, 0, checkOverlaps);
    /*-*/    }
    /*-*/    zpos                = 8 + -Lmod + iplPar[1]/sin -1.85*cos;
    /*-*/    if(Nmod>=1)
    /*-*/    {
    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fI_PLLogical, "I_PLPhysical", fPHENIXZDCLogical, false, 0, checkOverlaps);
    /*-*/    }
    /*-*/    if(Nmod>=2)
    /*-*/    {
    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fI_PLLogical, "I_PLPhysical", fPHENIXZDCLogical, false, 0, checkOverlaps);
    /*-*/    }
    /*-*/    if(Nmod>=3)
    /*-*/    {
    /*-*/        new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fI_PLLogical, "I_PLPhysical", fPHENIXZDCLogical, false, 0, checkOverlaps);
    /*-*/    }
    /*-*/}

    /*-*/// PUT TUNGSTEN PLATES INTO PHENIXZDC
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
    /*-*/            fW_PL_1Physical = new G4PVPlacement(fQPhi, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fW_PL_1Logical, "W_PL_1Physical", fPHENIXZDCLogical, true, i, checkOverlaps);
    /*-*/        if(Nmod>=2)
    /*-*/            fW_PL_2Physical = new G4PVPlacement(fQPhi, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fW_PL_2Logical, "W_PL_2Physical", fPHENIXZDCLogical, true, i, checkOverlaps);
    /*-*/        if(Nmod>=3)
    /*-*/            fW_PL_3Physical = new G4PVPlacement(fQPhi, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fW_PL_3Logical, "W_PL_3Physical", fPHENIXZDCLogical, true, i, checkOverlaps);
    /*-*/    }
    /*-*/}

    /*-*/// PUT 'FEPL' INTO 'PHENIXZDC'
    /*-*/ypos                    = -7.7;
    /*-*/zpos                    = 8 + Lmod*Nmod + smdPar[1]/sin + Lmod/2 - 40;
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, ypos*cm, zpos*cm), fFEPLLogical, "FEPLPhysical", fPHENIXZDCLogical, false, 0, checkOverlaps);

    /*-*/// PUT 'ALPL' INTO 'PHENIXZDC'
    /*-*/ypos                    = -11.7;
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, ypos*cm, zpos*cm), fALPLLogical, "ALPLPhysical", fPHENIXZDCLogical, false, 0, checkOverlaps);

    /*-*/// PUT 'SMDH' INTO 'SMD'
    /*-*/for(G4int i=1; i<9; i++)
    /*-*/{
    /*-*/    G4int j = i-1;
    /*-*/    zpos                = (4.5-i)*2.0*11.0/10.5;
    /*-*/    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, -0.405*cm, zpos*cm), fSMDHLogical, "SMDHPhysical", fSMDLogical, false, i, checkOverlaps);
    /*-*/}

    /*-*/// PUT 'SMDV' INTO 'SMD'
    /*-*/for(G4int i=1; i<8; i++)
    /*-*/{
    /*-*/    G4int j = i-1;
    /*-*/    xpos                = (i-4)*1.5*11.0/10.5;
    /*-*/    new G4PVPlacement(fNonRotation, G4ThreeVector(xpos*cm, 0.405*cm, 0.*cm), fSMDVLogical, "SMDVPhysical", fSMDLogical, true, i, checkOverlaps);
    /*-*/}

    /*-*/// PUT 'SMD' INTO LOCALPOL
    /*-*/zpos                    = 8 + Lmod*(Nmod-2)+smdPar[1]/sin;
    /*-*/new G4PVPlacement(GAPFRotation, G4ThreeVector(0.0, 0.0, zpos*cm), fSMDLogical, "SMDPhysical", fPHENIXZDCLogical, false, 0, checkOverlaps);

    /*-*/// ---------VISUALIZATION ATTRIBUTES--------------------
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    /*-*/visAttributes           -> SetVisibility(false);
    /*-*/fPHENIXZDCLogical        -> SetVisAttributes(visAttributes);
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
}

void RHICFDetectorConstruction::STARZDCINSTALL(G4VPhysicalVolume* world_phys, G4ThreeVector vector, G4RotationMatrix* mat)
{
    /*-*/fSTARZDCSolid              = new G4Box("STARZDCSolid", wcntPar[0]*cm, wcntPar[1]*cm, (wcntPar[2]*3.6/5+0.1)*cm);
    /*-*/fSTARZDCLogical            = new G4LogicalVolume(fSTARZDCSolid, FindMaterial("G4_AIR"), "STARZDCLogical");
    /*-*/fSTARZDCPhysical           = new G4PVPlacement(mat, vector, "STARZDCPhysical", fSTARZDCLogical,  world_phys, false, 0, checkOverlaps);

    //-----------------------------------------------------------------------------------------------------------
    // DEFINE SOLIDVOLUME
    /*-*/// DEFINE 'ZDC'
    /*-*/fZDCSolid               = new G4Box("ZDCSolid", zdcPar[0]*cm, zdcPar1*cm, zdcPar2*cm);
    /*-*/// DEFINE 'GAPF' : GAPF IS GAP WHICH HAS FIBERS
    /*-*/fGAPFSolid              = new G4Box("GAPFSolid", gapper[0]*cm, fibPar[1]*cm, (gapper[2]+0.5)*cm);
    /*-*/// DEFINE 'LAYER BETWEEN CLADDING AND GAP'
    /*-*/fFIBRSolid              = new G4Tubs("FIBRSolid", fibPar[0]*cm, fibPar[1]*cm, fibPar[2]/2.*cm, 0, twopi);
    /*-*/// DEFINE 'IRON PLATE' : I_PL IS PLATE WHICH MADE OF IRON
    /*-*/fI_PLSolid              = new G4Box("I_PLSolid", iplPar[0]*cm, iplPar[1]*cm, iplPar[2]*cm);
    /*-*/// DEFINE 'HALF OF PMMA FIBER'
    /*-*/fFIBSolid               = new G4Tubs("FIBSolid", fibPar[0]*cm, fibPar[1]*cm, gapper[2]/2.*cm, 0, twopi);
    auto fGhostZDCSolid = new G4Box("GhostZDCSolid", wplPar[0]*cm, wplPar[2]/sqrt(2)*cm, 0.0005*cm);
    auto fGhostZDCLogical = new G4LogicalVolume(fGhostZDCSolid, FindMaterial("G4_AIR"), "GhostZDCLogical");
    /*-*/// DEFINE 'TENGSTEN PLATE' : W_PL IS PLATE WHICH MADE OF TUNGSTEN
    /*-*/fW_PLSolid              = new G4Box("W_PLSolid", wplPar[0]*cm, wplPar[2]*cm, wplPar[1]*cm);
    /*-*/// DEFINE 'BAR FOR ELIMINATING OPTICAL PHOTON PROPAGATING DOWNWARD'
    /*-*/fBlockerSolid           = new G4Box("BlockerSolid", gapper[0]*cm, fibPar[1]*cm, 0.005*cm);
    /*-*/// DEFINE 'SMDVSTRIP'
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
    /*-*/// DEFINE 'SMDHSTRIP'
    /*-*/G4ExtrudedSolid::ZSection HBottom(-5.5*cm,{0,0},1);
    /*-*/G4ExtrudedSolid::ZSection HTop(5.5*cm,{0,0},1);
    /*-*/std::vector<G4ExtrudedSolid::ZSection> Hzsections;
    /*-*/Hzsections.push_back(HBottom);
    /*-*/Hzsections.push_back(HTop);
    /*-*/fSMDHStripSolid          = new G4ExtrudedSolid("SMDHStripSolid", VectorForEdge, Hzsections);
    /*-*/// DEFINE 'SMD'
    /*-*/fSMDSolid               = new G4Box("SMDSolid", smdPar[0]*cm, smdPar[1]*cm, smdPar[2]*cm);
    /*-*/// DEFINE 'FEPL'
    /*-*/fFEPLSolid              = new G4Box("FEPLSolid", fePar[0]*cm, fePar[1]*cm, fePar[2]);
    /*-*/// DEFINE 'ALPL' 
    /*-*/fALPLSolid              = new G4Box("ALPLSolid",alPar[0]*cm, alPar[1]*cm, alPar[2]*cm);

    /*-*///------------------------------------------------------------------------------------------------------------
    /*-*/// DEFINE LOGICALVOLUME
    /*-*/fZDCLogical             = new G4LogicalVolume(fZDCSolid, FindMaterial("G4_AIR"), "ZDCLogical");
    /*-*/fGAPF_1Logical          = new G4LogicalVolume(fGAPFSolid, FindMaterial("PMMA"), "GAPF_1Logical");
    /*-*/fGAPF_2Logical          = new G4LogicalVolume(fGAPFSolid, FindMaterial("PMMA"), "GAPF_2Logical");
    /*-*/fGAPF_3Logical          = new G4LogicalVolume(fGAPFSolid, FindMaterial("PMMA"), "GAPF_3Logical");
    /*-*/fI_PLLogical            = new G4LogicalVolume(fI_PLSolid, FindMaterial("G4_Fe"), "I_PLLogical");
    /*-*/fW_PL_1Logical          = new G4LogicalVolume(fW_PLSolid, FindMaterial("G4_W"), "W_PL_1Logical");
    /*-*/fW_PL_2Logical          = new G4LogicalVolume(fW_PLSolid, FindMaterial("G4_W"), "W_PL_2Logical");
    /*-*/fW_PL_3Logical          = new G4LogicalVolume(fW_PLSolid, FindMaterial("G4_W"), "W_PL_3Logical");
    /*-*/fSMDLogical             = new G4LogicalVolume(fSMDSolid, FindMaterial("G4_AIR"), "fSMDLogical");
    /*-*/// HORIZONTAL SMD BAR
    /*-*/fSMDHLogical            = new G4LogicalVolume(fSMDHStripSolid, FindMaterial("G4_PLASTIC_SC_VINYLTOLUENE"), "SMDHLogical");
    /*-*/fFEPLLogical            = new G4LogicalVolume(fFEPLSolid, FindMaterial("G4_Al"), "FEPLLogical");
    /*-*/fALPLLogical            = new G4LogicalVolume(fALPLSolid, FindMaterial("G4_Fe"), "ALPLLogical");
    /*-*/// VERTICAL SMD BAR
    /*-*/fSMDVLogical            = new G4LogicalVolume(fSMDVStripSolid, FindMaterial("G4_PLASTIC_SC_VINYLTOLUENE"), "SMDVLogical");



    /*-*///------------------------------------------------------------------------------------------------------------
    /*-*/// DEFINE PHYSICALVOLUME
    /*-*///
    /*-*/// PUT GAPS WHICH INCLUDE FIBERS INTO STARZDC
    /*-*/if(Nmod>0 && Nmod<4)
    /*-*/{
    /*-*/    for(G4int i=1; i<Nlay + 1; i++)
    /*-*/    {
    /*-*/        ypos            = 1.85*cos;
    /*-*/        interval        = (gapper[1]+wplPar[1])*2.0/sin;
    /*-*/        zpos            = 8 + Lmod - (2.0*iplPar[1]+2.0*wplPar[1]+gapper[1])/sin - interval*(i-1)-1.85*cos;
    /*-*/        if(Nmod>=1)
    /*-*/            fGAPF_1Physical = new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]* sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fGAPF_1Logical, "GAPF_1Physical", fSTARZDCLogical, true, 0, checkOverlaps);
    /*-*/        if(Nmod>=2)
    /*-*/            fGAPF_2Physical = new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]* sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fGAPF_2Logical, "GAPF_2Physical", fSTARZDCLogical, true, 0, checkOverlaps);
    /*-*/        if(Nmod>=3)
    /*-*/            fGAPF_3Physical = new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]* sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fGAPF_3Logical, "GAPF_3Physical", fSTARZDCLogical, true, 0, checkOverlaps);
    /*-*/    }
    /*-*/}

    /*-*/// PUT FRONT AND BEHIND IRON PLATE INTO STARZDC
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

    /*-*/// PUT TUNGSTEN PLATES INTO STARZDC
    /*-*/ypos                     = -zdcPar[1] + wplPar[1]*sin + wplPar[2]*cos +1.85*cos;
    /*-*/interval                 = (gapper[1] + wplPar[1])*2.0/sin;
    /*-*/// PUT GHOST ZDC
    /*-*/auto fGhostZDCPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (1*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/ sin + 36.2 + Lmod - ypos/tan - (2.0*iplPar[1] + wplPar[1])/sin )*cm), fGhostZDCLogical, "GhostZDCPhysical", fSTARZDCLogical, 0, false, checkOverlaps);
    /*-*/
    /*-*/if(Nmod>0 && Nmod<4)
    /*-*/{
    /*-*/
    /*-*/    for(G4int i=1; i<Nlay+2;i++)
    /*-*/    {
    /*-*/
    /*-*/        zpos                = 8 + Lmod - ypos/tan - (2.0*iplPar[1] + wplPar[1])/sin - interval*(i-1);
    /*-*/        if(Nmod>=1)
    /*-*/            fW_PL_1Physical = new G4PVPlacement(fQPhi, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fW_PL_1Logical, "W_PL_1Physical", fSTARZDCLogical, true, 0, checkOverlaps);
    /*-*/        if(Nmod>=2)
    /*-*/            fW_PL_2Physical = new G4PVPlacement(fQPhi, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fW_PL_2Logical, "W_PL_2Physical", fSTARZDCLogical, true, 0, checkOverlaps);
    /*-*/        if(Nmod>=3)
    /*-*/            fW_PL_3Physical = new G4PVPlacement(fQPhi, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fW_PL_3Logical, "W_PL_3Physical", fSTARZDCLogical, true, 0, checkOverlaps);
    /*-*/    }
    /*-*/}

    /*-*/// PUT 'FEPL' INTO 'STARZDC'
    /*-*/ypos                    = -7.7;
    /*-*/zpos                    = 8 + Lmod*Nmod + smdPar[1]/sin + Lmod/2 - 40;
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, ypos*cm, zpos*cm), fFEPLLogical, "FEPLPhysical", fSTARZDCLogical, false, 0, checkOverlaps);

    /*-*/// PUT 'ALPL' INTO 'STARZDC'
    /*-*/ypos                    = -11.7;
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, ypos*cm, zpos*cm), fALPLLogical, "ALPLPhysical", fSTARZDCLogical, false, 0, checkOverlaps);


    /*-*/// PUT 'SMDH' INTO 'SMD'
    /*-*/for(G4int i=1; i<33; i++)
    /*-*/{
    /*-*/    // ODD NUMBER SMDHSTRIP
    /*-*/    if(i%2 == 1)
    /*-*/    {
    /*-*/        zpos                = (-8.5+0.5*i);
    /*-*/        SMDHRotation -> rotateZ(90*deg);
    /*-*/        SMDHRotation -> rotateX(90*deg);
    /*-*/        new G4PVPlacement(SMDHRotation, G4ThreeVector(0.0, -0.36*cm, zpos*cm), fSMDHLogical, "SMDHPhysical", fSMDLogical, false, (i-1)/4, checkOverlaps);
    /*-*/
    /*-*/    }
    /*-*/
    /*-*/    // EVEN NUMBER SMDHSTRIP
    /*-*/    if(i%2 == 0)
    /*-*/    {
    /*-*/        zpos                = (-8.5+0.5*i);
    /*-*/        SMDH2Rotation -> rotateZ(-90*deg);
    /*-*/        SMDH2Rotation -> rotateX(90*deg);
    /*-*/        new G4PVPlacement(SMDH2Rotation, G4ThreeVector(0.0, -0.36*cm, zpos*cm), fSMDHLogical, "SMDHPhysical", fSMDLogical, false, (i-1)/4, checkOverlaps);
    /*-*/
    /*-*/    }
    /*-*/}


    /*-*/// PUT 'SMDV' INTO 'SMD'
    /*-*/for(G4int i=1; i<21; i++)
    /*-*/{
    /*-*/    // ODD NUMBER SMDVSTRIP
    /*-*/    if(i%2 == 1)
    /*-*/    {
    /*-*/        SMDVRotation            -> rotateZ(-45*deg);
    /*-*/        xpos                = 0.5*(i-11);
    /*-*/        new G4PVPlacement(SMDVRotation, G4ThreeVector(xpos*cm, 0.36*cm, 0.*cm), fSMDVLogical, "SMDVPhysical", fSMDLogical, true, (i-1)/3, checkOverlaps);
    /*-*/    }
    /*-*/
    /*-*/    // EVEN NUMBER SMDVSTRIP
    /*-*/    if(i%2 == 0)
    /*-*/    {
    /*-*/        SMDVRotation            -> rotateZ(45*deg);
    /*-*/        xpos                = 0.5*i-5.5;
    /*-*/        new G4PVPlacement(SMDVRotation, G4ThreeVector(xpos*cm, 0.36*cm, 0.*cm), fSMDVLogical, "SMDVPhysical", fSMDLogical, true, (i-1)/3, checkOverlaps);
    /*-*/    }
    /*-*/}

    /*-*/// PUT 'SMD' INTO LOCALPOL
    /*-*/zpos                    = 8 + Lmod*(Nmod-2)+smdPar[1]/sin;
    /*-*/new G4PVPlacement(GAPFRotation, G4ThreeVector(0.0, 0.0, zpos*cm), fSMDLogical, "SMDPhysical", fSTARZDCLogical, false, 0, checkOverlaps);

    /*-*/// ---------VISUALIZATION ATTRIBUTES--------------------
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    /*-*/visAttributes           -> SetVisibility(false);
    /*-*/fSTARZDCLogical        -> SetVisAttributes(visAttributes);
    /*-*/fVisAttributes.push_back(visAttributes);
    /*-*/visAttributes           = new G4VisAttributes(G4Colour(0.0, 0.8888, 0.0));
    /*-*/visAttributes           -> SetVisibility(false);
    /*-*///-fZDC_1Logical -> SetVisAttributes(visAttributes);
    /*-*///Junsang****fSMDLogical             -> SetVisAttributes(visAttributes);
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
}

void RHICFDetectorConstruction::PHENIXPIPE()
{
    /*-*///----------------------------------------------BEAM PIPE----------------------------------------------------
    /*-*/par[0] = 0.0;
    /*-*/par[1] = 0.1;
    /*-*/par[2] = 0.7;
    /*-*/// BEAM PIPE + STEEL SECTIONS
    /*-*///
    /*-*/// BEAM PIPE: VOL. DEFINITIONS: NAME SHAPE MAT. R_I R_O LZ/2 #PAR
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
    /*-*/par[0] = 20.0;
    /*-*/par[1] = 20.96;
    /*-*/par[2] = 108.58;
    /*-*/G4VSolid* SP81Solid = new G4Tubs("SP81Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/SP81Logical = new G4LogicalVolume(SP81Solid, FindMaterial("G4_Fe"), "SP81Logical");
    /*-*/G4VSolid* SP82Solid = new G4Tubs("SP82Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/SP82Logical = new G4LogicalVolume(SP82Solid, FindMaterial("G4_Fe"), "SP82Logical");
    /*-*/par[0] = 0.0;
    /*-*/par[1] = 20.0;
    /*-*/par[2] = 108.58;
    /*-*/G4VSolid* SM81Solid = new G4Tubs("SM81Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/SM81Logical = new G4LogicalVolume(SM81Solid, FindMaterial("G4_Galactic"), "SM81Logical");
    /*-*/G4VSolid* SM82Solid = new G4Tubs("SM82Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/SM82Logical = new G4LogicalVolume(SM82Solid, FindMaterial("G4_Galactic"), "SM82Logical");
    /*-*/par[0] = 0.0;
    /*-*/par[1] = 20.96;
    /*-*/par[2] = 0.47;
    /*-*/G4VSolid* SP91Solid = new G4Tubs("SP91Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/SP91Logical = new G4LogicalVolume(SP91Solid, FindMaterial("G4_Fe"), "SP91Logical");
    /*-*/G4VSolid* SP92Solid = new G4Tubs("SP92Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/SP92Logical = new G4LogicalVolume(SP92Solid, FindMaterial("G4_Fe"), "SP92Logical");
    /*-*/par[0] = 0.0;
    /*-*/par[1] = 6.35;
    /*-*/par[2] = 0.47;
    /*-*/G4VSolid* SHL1Solid = new G4Tubs("SHL1Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/SHL1Logical = new G4LogicalVolume(SHL1Solid, FindMaterial("G4_Galactic"), "SHL1Logical");
    /*-*/G4VSolid* SHL2Solid = new G4Tubs("SHL2Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/SHL2Logical = new G4LogicalVolume(SHL2Solid, FindMaterial("G4_Galactic"), "SHL2Logical");
    /*-*/G4VSolid* SHU1Solid = new G4Tubs("SHU1Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/SHU1Logical = new G4LogicalVolume(SHU1Solid, FindMaterial("G4_Galactic"), "SHU1Logical");
    /*-*/G4VSolid* SHU2Solid = new G4Tubs("SHU2Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/SHU2Logical = new G4LogicalVolume(SHU2Solid, FindMaterial("G4_Galactic"), "SHU2Logical");
    /*-*/par[0] = 6.07;
    /*-*/par[1] = 6.35;
    /*-*/par[2] = 91.5;
    /*-*/G4VSolid* S101Solid = new G4Tubs("S101Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/S101Logical = new G4LogicalVolume(S101Solid, FindMaterial("G4_Fe"), "S101Logical");
    /*-*/G4VSolid* S102Solid = new G4Tubs("S102Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/S102Logical = new G4LogicalVolume(S102Solid, FindMaterial("G4_Fe"), "S102Logical");
    /*-*/G4VSolid* S111Solid = new G4Tubs("S111Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/S111Logical = new G4LogicalVolume(S111Solid, FindMaterial("G4_Fe"), "S111Logical");
    /*-*/G4VSolid* S112Solid = new G4Tubs("S112Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/S112Logical = new G4LogicalVolume(S112Solid, FindMaterial("G4_Fe"), "S112Logical");
    /*-*/par[0] = 0.0;
    /*-*/par[1] = 6.35;
    /*-*/par[2] = 91.5;
    /*-*/G4VSolid* M101Solid = new G4Tubs("M101Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/M101Logical = new G4LogicalVolume(M101Solid, FindMaterial("G4_Galactic"), "M101Logical");
    /*-*/G4VSolid* M102Solid = new G4Tubs("M102Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/M102Logical = new G4LogicalVolume(M102Solid, FindMaterial("G4_Galactic"), "M102Logical");
    /*-*/G4VSolid* M111Solid = new G4Tubs("M111Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/M111Logical = new G4LogicalVolume(M111Solid, FindMaterial("G4_Galactic"), "M111Logical");
    /*-*/G4VSolid* M112Solid = new G4Tubs("M112Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    /*-*/M112Logical = new G4LogicalVolume(M112Solid, FindMaterial("G4_Galactic"), "M112Logical");
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -1642*cm), SM82Logical, "SM82Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -1642*cm), SP82Logical, "SP82Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 1642*cm), SM81Logical, "SM81Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 1642*cm), SP81Logical, "SP81Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(PipeRotation1, G4ThreeVector(12.82*cm, 0, -1843*cm), M102Logical, "M102Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(PipeRotation1, G4ThreeVector(12.82*cm, 0, -1843*cm), S102Logical, "S102Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(PipeRotation2, G4ThreeVector(-12.82*cm, 0, -1843*cm), M112Logical, "M112Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(PipeRotation2, G4ThreeVector(-12.82*cm, 0, -1843*cm), S112Logical, "S112Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(11.02*cm, 0, 0), SHL1Logical, "SHL1Physical", SP91Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(-11.02*cm, 0, 0), SHU1Logical, "SHU1Physical", SP91Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(11.02*cm, 0, 0), SHL2Logical, "SHL2Physical", SP92Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(-11.02*cm, 0, 0), SHU2Logical, "SHU2Physical", SP92Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 1750.99*cm), SP91Logical, "SP91Physical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -1750.99*cm), SP92Logical, "SP92Physical", fWorldLogical, false, 0, checkOverlaps);

    //---------------------MAGNETICFIELD---------------
    // DX MAGNET
    /*-*/G4VSolid* MagneticSolid = new G4Tubs("MagneticSolid", 0., 15.24*cm, 207.92*cm, 0., 360*deg);
    /*-*/fMagneticLogical = new G4LogicalVolume(MagneticSolid, FindMaterial("G4_Galactic"), "MagneticLogical");
    /*-*/G4VPhysicalVolume* MagneticPhysical = new G4PVPlacement(fNonRotation , G4ThreeVector(0, 0, -1165.12*cm), fMagneticLogical, "MagneticPhysical", fWorldLogical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement( 0, G4ThreeVector(0, 0, 1165.12*cm), fMagneticLogical, "MagneticPhysical", fWorldLogical, false, 0, checkOverlaps);

}


void RHICFDetectorConstruction::ARM1INSTALL(G4VPhysicalVolume* world_phys, G4ThreeVector vector, G4RotationMatrix* mat)
{
    /*-*/// Define ARM1
    /*-*/fARM1Solid          = new G4Box("ARM1Solid", kARM1par[0]*cm, kARM1par[1]*1.4*cm, kARM1par[2]*cm); 
    /*-*/fARM1Logical        = new G4LogicalVolume(fARM1Solid, FindMaterial("G4_Galactic"), "ARM1Logical");
    /*-*/fARM1Physical       = new G4PVPlacement(mat, vector, "ARM1Physical", fARM1Logical, world_phys, false, 0, checkOverlaps);



    //-------------------------------------FRONT COUNTER-----------------------
    //SCINTILLATOR
    auto fFrontCounterLargeSolid = new G4Box("FrontCounterLargeSolid", 2.*cm, 2.*cm, 0.15*cm);
    auto fFrontCounterLargeLogical = new G4LogicalVolume(fFrontCounterLargeSolid, FindMaterial("Scintillator"), "FrontCounterLargeLogical");
    auto fFrontCounterLargePhysical = new G4PVPlacement(fRotationZ45, G4ThreeVector(0.*cm, 0.*cm, 14.15*cm), fFrontCounterLargeLogical, "FrontCounterLargePhysical", fARM1Logical, 0, false, checkOverlaps);
    auto fFrontCounterSmallSolid = new G4Box("FrontCounterSmallSolid", 1.*cm, 1.*cm, 0.05*cm);
    auto fFrontCounterSmallLogical = new G4LogicalVolume(fFrontCounterSmallSolid, FindMaterial("Scintillator"), "FrontCounterSmallLogical");
    auto fFrontCounterSmallPhysical = new G4PVPlacement(fRotationZ45, G4ThreeVector(0.*cm, -(30.1*sqrt(2)+5)/10*cm, 14.15*cm), fFrontCounterSmallLogical, "FrontCounterSmallPhysical", fARM1Logical, 0, false, checkOverlaps);
    //ALUMINUM
    auto fFrontCounterCoverFrontSolid = new G4Box("FrontCounterCoverFrontSolid", 4.5*cm, 12.15*cm, 0.05*cm);
    auto fFrontCounterCoverFrontLogical = new G4LogicalVolume(fFrontCounterCoverFrontSolid, FindMaterial("G4_Al"), "fFrontCounterCoverFrontLogical");
    auto fFrontCounterCoverFrontPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 3.*cm, 16.45*cm), fFrontCounterCoverFrontLogical, "FrontCounterCoverFrontPhysical", fARM1Logical, 0, false, checkOverlaps);
    auto fFrontCounterCoverSideSolid = new G4Box("FrontCounterCoverSideSolid", 0.05*cm, 12.15*cm, 1.15*cm);
    auto fFrontCounterCoverSideLogical = new G4LogicalVolume(fFrontCounterCoverSideSolid, FindMaterial("G4_Al"), "FrontCounterCoverSideLogical");
    auto fFrontCounterCoverSideRPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(-4.45*cm, 3.*cm, 15.25*cm), fFrontCounterCoverSideLogical, "FrontCounterCoverSideRPhysical", fARM1Logical, 0, false, checkOverlaps);
    auto fFrontCounterCoverSideLPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(4.45*cm, 3.*cm, 15.25*cm), fFrontCounterCoverSideLogical, "FrontCounterCoverSideLPhysical", fARM1Logical, 0, false, checkOverlaps);
    


    //--------------------------------------ARM1-------------------------------
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
    /*-*/TwoDVectorForEdge = {-20.1/sqrt(2)*mm,0*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-(20.1/sqrt(2)-40.1*sqrt(2))*mm,40.1*sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {40.1*sqrt(2)*mm,(40.1*sqrt(2)-20.1/sqrt(2))*mm};
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
    /*-*/TwoDVectorForEdge = {(20.1/sqrt(2)-40.1*sqrt(2))*mm,40.1*sqrt(2)*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {-40.1*sqrt(2)*mm,(40.1*sqrt(2)-20.1/sqrt(2))*mm};
    /*-*/VectorForEdge.push_back(TwoDVectorForEdge);
    /*-*/TwoDVectorForEdge = {0.*mm,-20.1/sqrt(2)*mm};
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

    /*-*/// SOLID VOLUMES FOR TUNGSTEN PLATE
    /*-*/fLargeW_PLSolid                       = new G4Box("LargeW_PLSolid", 2.0*cm, 2.0*cm, 0.305*cm);
    /*-*/fSmallW_PLSolid                       = new G4Box("SmallW_PLSolid", 1.0*cm, 1.0*cm, 0.305*cm);

    /*-*/// SOLID VOLUMES FOR GSO PATE
    /*-*/fLargeGSO_PLSolid                     = new G4Box("LargeGSO_PLSolid", 2*cm, 2*cm, 0.05*cm); 
    /*-*/fSmallGSO_PLSolid                     = new G4Box("SmallGSO_PLSolid", 1*cm, 1*cm, 0.05*cm); 

    /* SOLID VOLUMES FOR LIGHT GUIDE */

    /*-*/// SOLIDVOLUME FOR NEGATIVE VOLUME OF LIGHT GUIDE
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

    /*-*/// SOLIDVOLUME FOR VOLUME OF SMALL GUIDE
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

    /*-*/// SOLIDVOLUME FOR GSO BAR BELT
    /*-*/fGSOSmallBarSolid                 = new G4Box("GSOSmallBarSolid", 10*mm, 0.5*mm, 0.5*mm); 
    /*-*/fGSOLargeBarSolid                 = new G4Box("GSOLargeBarSolid", 20*mm, 0.5*mm, 0.5*mm); 
    /*-*/fGSOSmallBarBeltSolid             = new G4Box("GSOSmallBarBeltSolid", 10*mm, 10*mm, 0.5*mm);
    /*-*/fGSOLargeBarBeltSolid             = new G4Box("GSOLargeBarBeltSolid", 20*mm, 20*mm, 0.5*mm);

    // SOLIDVOLUME FOR ALUMINUM FRAME
    /*-*/// SOLIDVOLUME FOR PART1
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
    /*-*/// SOLIDVOLUME FOR PART2
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
    /*-*/// SOLIDVOLUME FOR PART3(POLE)
    /*-*/fAlFramePart3Solid         = new G4Tubs("AlFramePart3Solid", 0*mm, 3.99*mm, 275/2*mm, 0*deg, 360*deg);
    /*-*/// SOLIDVOLUME FOR PART5(BOTTOM)
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
    /*-*/// SOLIDVOLUME FOR PART6,7,8,9,10
    /*-*/fAlFramePart6Solid         = new G4Box("AlFramePart6Solid", 5.0*mm, 109.9*mm, 5.0*mm);
    /*-*/fAlFramePart7Solid         = new G4Box("AlFramePart7Solid", 20.0*mm, 7.0*mm, 9.0/2*mm);
    /*-*/fAlFramePart8Solid         = new G4Box("AlFramePart8Solid", 40.0*mm, 10.0*mm, 5.0*mm);
    /*-*/fAlFramePart9Solid         = new G4Box("AlFramePart9Solid", 20.0*mm, 7.0*mm, 9.0*mm);
    /*-*/fAlFramePart10Solid         = new G4Box("AlFramePart10Solid", 10.0*mm, 10.0*mm, 9.0/2*mm);

    /*-*/// SOLIDVOLUME FOR AL FRAME TOP PART
    /*-*/// REAR & FRONT 
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2", fAlFramePart1Solid, fAlFramePart2Solid);
    /*-*/// TOP POLE
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2+Part3", fAlFrame1Solid, fAlFramePart3Solid, fNonRotation, G4ThreeVector(0*mm,((50.1/2+9)*sqrt(2)-8-2.65)*mm, 0*mm));
    /*-*/// COLUMN
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2+Part3+Part6", fAlFrame1Solid, fAlFramePart6Solid, fNonRotation, G4ThreeVector(39*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75+110.0)*mm, 134.0*mm));
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2+Part3+Part6+Part7", fAlFrame1Solid, fAlFramePart6Solid, fNonRotation, G4ThreeVector(-39*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75+110.0)*mm, 134.0*mm));
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2+Part3+Part6+Part7+Part8", fAlFrame1Solid, fAlFramePart6Solid, fNonRotation, G4ThreeVector(39*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75+110.0)*mm, -134.0*mm));
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2+Part3+Part6+Part7+Part8+Part9", fAlFrame1Solid, fAlFramePart6Solid, fNonRotation, G4ThreeVector(-39*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75+110.0)*mm, -134.0*mm));
    /*-*/// REAR PARTS
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2+Part3+Part6+Part7+Part8+Part9+Part10", fAlFrame1Solid, fAlFramePart8Solid, fNonRotation, G4ThreeVector(0.0*mm,((50.1/2+9)*sqrt(2)-8-2.65)*mm, -134.0*mm));
    /*-*/// FRONT PARTS
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2+Part3+Part6+Part7+Part8+Part9+Part10+Part11", fAlFrame1Solid, fAlFramePart8Solid, fNonRotation, G4ThreeVector(0.0*mm,((50.1/2+9)*sqrt(2)-8-2.65)*mm, 134.0*mm));
    /*-*/fAlFrame1Solid              = new G4UnionSolid("Part1+Part2+Part3+Part6+Part7+Part8+Part9+Part10+Part11+Part12", fAlFrame1Solid, fAlFramePart10Solid, fNonRotation, G4ThreeVector(0.0*mm,((50.1/2+9)*sqrt(2)-8-2.65)*mm, 125.5*mm));
    /*-*/fAlFrame1Solid                    = new G4SubtractionSolid( "AlFrameTopSolid", fAlFrame1Solid, fAlFramePart3Solid, fNonRotation, G4ThreeVector(0*mm, (-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75+10)*mm, 0*mm));

    /*-*/// SOLIDVOLUME FOR AL FRAME BOTTOM PART
    /*-*/fAlFrame2Solid              = new G4UnionSolid("Part1+Part2", fAlFramePart5Solid, fAlFramePart7Solid, fNonRotation, G4ThreeVector(0.0*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.85+7)*mm, -134.5*mm));
    /*-*/fAlFrame2Solid              = new G4UnionSolid("Part1+Part2+Part3", fAlFrame2Solid, fAlFramePart9Solid, fNonRotation, G4ThreeVector(0.0*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.85+7)*mm, 130.0*mm));
    /*-*/// Bottom pole
    /*-*/fAlFrame2Solid              = new G4UnionSolid("Part1+Part2+Part3+Part4", fAlFrame2Solid, fAlFramePart3Solid, fNonRotation, G4ThreeVector(0*mm,(-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75+10)*mm, 0*mm));

    /*-*/// SOLIDVOLUME FOR PANELS
    /*-*/fSidePanelSolid             = new G4Box("SidePanelSolid", 0.5*mm, 235/2*mm, 278/2*mm);
    /*-*/fFrontPanelSolid            = new G4Box("FrontPanelSolid", 44.0*mm, 235/2*mm, 0.5*mm);

    /*-*/// DEFINE LOGICAL VOLUME FOR FRONT/SIDE PANELS
    /*-*/fSidePanelLogical             = new G4LogicalVolume(fSidePanelSolid, FindMaterial("Duralumin"), "SidePanelLogical");
    /*-*/fFrontPanelLogical            = new G4LogicalVolume(fFrontPanelSolid, FindMaterial("Duralumin"), "FrontPanelLogical");

    /*-*/// DEFINE LOGICAL VOLUME FOR TUNGSTEN PLATE
    /*-*/fLargeW_PLLogical                     = new G4LogicalVolume(fLargeW_PLSolid, FindMaterial("G4_W"), "LargeW_PLLogical");
    /*-*/fSmallW_PLLogical                     = new G4LogicalVolume(fSmallW_PLSolid, FindMaterial("G4_W"), "SmallW_PLLogical");

    /*-*/// DEFINE LOGICAL VOLUMES FOR GSO PLATE
    /*-*/fLargeGSO_PLLogical                   = new G4LogicalVolume(fLargeGSO_PLSolid, FindMaterial("GSO"), "LargeGSO_PLLogical");
    /*-*/fSmallGSO_PLLogical                   = new G4LogicalVolume(fSmallGSO_PLSolid, FindMaterial("GSO"), "SmallGSO_PLLogical");

    /*-*/// DEFINE LOGICAL VOLUMES FOR GSO BAR BELT
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
    /*-*/// DEFINE LOGICAL VOLUMES FOR GSO BAR
    /*-*/fGSORightSmallBarLogical                           = new G4LogicalVolume(fGSOSmallBarSolid, FindMaterial("GSO"), "GSORightSmallBarLogical");
    /*-*/fGSOLeftSmallBarLogical                            = new G4LogicalVolume(fGSOSmallBarSolid, FindMaterial("GSO"), "GSOLeftSmallBarLogical");
    /*-*/fGSORightLargeBarLogical                           = new G4LogicalVolume(fGSOLargeBarSolid, FindMaterial("GSO"), "GSORightLargeBarLogical");
    /*-*/fGSOLeftLargeBarLogical                            = new G4LogicalVolume(fGSOLargeBarSolid, FindMaterial("GSO"), "GSOLeftLargeBarLogical");

    /*-*/// PUT PANELS INTO ARM1
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector( 0*mm, ((-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75-8)+235/2)*mm, 139.51*mm), fFrontPanelLogical, "FrontPanelPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector( 0*mm, ((-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75-8)+235/2)*mm, -139.51*mm), fFrontPanelLogical, "FrontPanelPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector( 44.51*mm, ((-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75-8)+235/2)*mm, 0.0*mm), fSidePanelLogical, "SidePanelPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*/new G4PVPlacement(fNonRotation, G4ThreeVector( -44.51*mm, ((-(30.1*sqrt(2)+5)-40.43+10.05*sqrt(2)-9.75-8)+235/2)*mm, 0.0*mm), fSidePanelLogical, "SidePanelPhysical", fARM1Logical, false, 0, checkOverlaps);

    /*-*///PUT GSO BAR INTO GSO BAR BELT
    /*-*/for(int i=0; i<4; i++)
    /*-*/{
    /*-*/
    /*-*/    // IMPLEMENT FIRST GSO BARS
    /*-*/    if(i==0)
    /*-*/        {
    /*-*/                // INSERT GSO BARS INTO GSO BAR BELT
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
    /*-*/    // IMPLEMENT SECOND GSO BARS
    /*-*/    if(i==1)
    /*-*/        {
    /*-*/                // INSERT GSO BARS INTO GSO BAR BELT
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
    /*-*/    // IMPLEMENT THIRD GSO BARS
    /*-*/    if(i==2)
    /*-*/        {
    /*-*/                // INSERT GSO BARS INTO GSO BAR BELT
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
    /*-*/    // IMPLEMENT FOURTH GSO BARS
    /*-*/    if(i==2)
    /*-*/        {
    /*-*/                // INSERT GSO BARS INTO GSO BAR BELT
    /*-*/                for(int j=60; j<80; j++)
    /*-*/                    {
    /*-*/                            ypos = j%20-9.5;
    /*-*/                            new G4PVPlacement(fNonRotation, G4ThreeVector(0*mm, ypos*mm, 0*mm), fGSOLeftSmallBarLogical, "GSOLeftSmallBarLogical", fGSOLeftSmallBarBelt_4Logical, false, j, checkOverlaps);
    /*-*/                            xpos = j%20-9.5;
    /*-*/                            new G4PVPlacement(fRotationZ90, G4ThreeVector(xpos*mm, 0*mm, 0*mm), fGSORightSmallBarLogical, "GSORightSmallBarLogical", fGSORightSmallBarBelt_4Logical, false, j, checkOverlaps);
    /*-*/                        }
    /*-*/        
    /*-*/                for(int j=120; j<160; j++)
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

    /*-*///DEFINE LOGICAL VOLUMES FOR TUNGSTEN PLATE HOLDER
    /*-*/fWHolder_1Logical                       = new G4LogicalVolume(fWHolder_1Solid, FindMaterial("G10"), "WHolder_1Logical");
    /*-*/fWHolder_2Logical                       = new G4LogicalVolume(fWHolder_2Solid, FindMaterial("G10"), "WHolder_2Logical");
    //Junsang****/*-*/fWHolderPhysical                       = new G4PVPlacement(fRotationZ45, G4ThreeVector(), fGSOLeftSmallBarBelt_1Logical, "WHolderPhysical", fARM1Logical, false, 0, checkOverlaps);
    /*-*///DEFINE LOGICAL VOLUME  FOR GSO PLATE HOLDER
    /*-*/fGSO_PLHolderLogical                = new G4LogicalVolume(fGSO_PLHolderSolid, FindMaterial("Acrylic"), "GSO_PLHolderLogical");

    /*-*///DEFINE LOGICAL VOLUME FOR GSO BAR HOLDER
    /*-*/fGSOBarHolderLogical                = new G4LogicalVolume(fGSOBarHolderSolid, FindMaterial("Acrylic"), "GSOBarHolderLogical");

    /*-*///DEFINE LOGICAL VOLIMES FOR LIGHT GUIDE
    /*-*/fLightGuideLargeLogical             = new G4LogicalVolume(fLightGuideLargeSolid, FindMaterial("ARM1Quartz"), "LightGuideLargeLogical");
    /*-*/fLightGuideSmallLogical             = new G4LogicalVolume(fLightGuideSmallSolid, FindMaterial("ARM1Quartz"), "LightGuideSmallLogical");

    /*-*/// PUT TUNGSTEM PLATE HOLDER INTO AMR1
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
    /*-*/fVisAttributes.push_back(visAttributes);
}

void RHICFDetectorConstruction::STARPIPEINSTALL(G4double arm1y, G4double arm1z)
{
    //------------------------------------------------UPSTREAM SECTION---------------------------------------
    G4double starpar[5] = {0., 35., 1472.709/2, 0., 360.};
    auto fUpstreamSectionSolid = new G4Tubs("UpstreamSectionSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fUpstreamSectionLogical = new G4LogicalVolume(fUpstreamSectionSolid, FindMaterial("G4_AIR"), "UpstreamSectionLogical");
    auto fUpstreamSectionPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 736.3545*cm), fUpstreamSectionLogical, "UpstreamSectionPhysical", fWorldLogical, 0, false, checkOverlaps);
    //3INCH SECTION
    starpar[0] = 0;
    starpar[1] = 100;
    starpar[2] = 464.593/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto f3InchSectionSolid = new G4Tubs("3InchSectionSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto f3InchSectionLogical = new G4LogicalVolume(f3InchSectionSolid, FindMaterial("G4_AIR"), "3InchSectionLogical");
    auto f3InchSectionPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -504.058*cm), f3InchSectionLogical, "3InchSectionPhysical", fUpstreamSectionLogical, 0, false, checkOverlaps);




    starpar[0] = 0.;
    starpar[1] = 3.54;
    starpar[2] = 437.339/2;
    starpar[3] = 0.;
    starpar[4] = 360.;
    auto f3InchVacuumSolid = new G4Tubs("3InchVacuumSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto f3InchVacuumLogical = new G4LogicalVolume(f3InchVacuumSolid, FindMaterial("G4_Galactic"), "3InchVacuumLogical");

    /*-*///IMPLEMENT OF GHOST VULUME
    /*-*/G4double ghostsize[3] = {2.*5/arm1z, 2.*5/arm1z, 0.0005};
    /*-*/auto fGhostCenterLargeSolid = new G4Box("GhostCenterLargeSolid", ghostsize[0]*cm, ghostsize[1]*cm, ghostsize[2]*cm);
    /*-*/auto fGhostCenterLargeLogical = new G4LogicalVolume(fGhostCenterLargeSolid, FindMaterial("G4_Galactic"), "GhostCenterLargeLogical");
    /*-*/auto fGhostCenterLargePhysical = new G4PVPlacement(fRotationZ45, G4ThreeVector(0.*cm, 5./arm1z*arm1y*cm, -213.667*cm), fGhostCenterLargeLogical, "GhostCenterLargePhysical", f3InchVacuumLogical, 0, false, checkOverlaps);
    /*-*/ghostsize[0] = 1.*5/arm1z;
    /*-*/ghostsize[1] = 1.*5/arm1z;
    /*-*/ghostsize[2] = 0.0005;
    /*-*/auto fGhostCenterSmallSolid = new G4Box("GhostCenterSmallSolid", ghostsize[0]*cm, ghostsize[1]*cm, ghostsize[2]*cm);
    /*-*/auto fGhostCenterSmallLogical = new G4LogicalVolume(fGhostCenterSmallSolid, FindMaterial("G4_Galactic"), "GhostCenterSmallLogical");
    /*-*/auto fGhostCenterSmallPhysical = new G4PVPlacement(fRotationZ45, G4ThreeVector(0.*cm, 5./arm1z*(arm1y-4.76)*cm, -213.667*cm), fGhostCenterSmallLogical, "GhostCenterSmallPhysical", f3InchVacuumLogical, 0, false, checkOverlaps);
    /*-*/G4double kghost[5] = {0., kARM1par[1]/(RHICFManager::GetInstance()->GetARM1Z()-16.45)*3, 0.0005, 0., 360.};
    /*-*/auto fGhostCircleSolid = new G4Tubs("GhostCircleSolid", kghost[0]*cm, kghost[1]*cm, kghost[2]*cm, kghost[3]*deg, kghost[4]*deg);
    /*-*/auto fGhostCircleLogical = new G4LogicalVolume(fGhostCircleSolid, FindMaterial("G4_Galactic"), "GhostCircleLogical");
    /*-*/auto fGhostCireclePhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 3./(RHICFManager::GetInstance()->GetARM1Z()-16.45)*(RHICFManager::GetInstance()->GetARM1Y()-2.37)*cm, -215.667*cm), fGhostCircleLogical, "GhostCirclePhysical", f3InchVacuumLogical, 0, false, checkOverlaps);


    auto f3InchVacuumPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -13.627*cm), f3InchVacuumLogical, "3InchVacuumPhysical", f3InchSectionLogical, 0, false, checkOverlaps);

    auto f3InchConeVacuumSolid = new G4Cons("3InchConeVacuumSolid", 0.*cm, 3.54*cm, 0.*cm, 6.07*cm, 27.254*cm, 0.*deg, 360.*deg);
    auto f3InchConeVacuumLogical = new G4LogicalVolume(f3InchConeVacuumSolid, FindMaterial("G4_Galactic"), "3InchConeVacuumLogical");
    auto f3InchConeVacuumPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 218.6695*cm), f3InchConeVacuumLogical, "3InchConeVacuumPhysical", f3InchSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 3.54;
    starpar[1] = 3.81;
    starpar[2] = 75.002/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto f3InchShortSolid = new G4Tubs("3InchShortPipeSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto f3InchShortLogical = new G4LogicalVolume(f3InchShortSolid, FindMaterial("G4_Be"), "3InchShortLogical");
    auto f3InchShortPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -194.7955*cm), f3InchShortLogical, "3InchShortPhysical", f3InchSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 3.54;
    starpar[1] = 3.81;
    starpar[2] = 362.337/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto f3InchLongSolid = new G4Tubs("3InchLongSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto f3InchLongLogical = new G4LogicalVolume(f3InchLongSolid, FindMaterial("G4_Fe"), "3InchLongLogical");
    auto f3InchLongPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 23.874*cm), f3InchLongLogical, "3InchLongPhysical", f3InchSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 3.81;
    starpar[1] = 5.8675;
    starpar[2] = 4.115/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto f3InchFlange1Solid = new G4Tubs("3InchFlange1Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto f3InchFlange1Logical = new G4LogicalVolume(f3InchFlange1Solid, FindMaterial("G4_Fe"), "3InchFlange1Logical");
    auto f3InchFlange1Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -157.2945*cm), f3InchFlange1Logical, "3InchFlange1Physical", f3InchSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 3.81;
    starpar[1] = 5.8675;
    starpar[2] = 3.962/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto f3InchFlange2Solid = new G4Tubs("3InchFlange2Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto f3InchFlange2Logical = new G4LogicalVolume(f3InchFlange2Solid, FindMaterial("G4_Fe"), "3InchFlange2Logical");
    auto f3InchFlange2Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 171.5655*cm), f3InchFlange2Logical, "3InchFlange2Physical", f3InchSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 3.81;
    starpar[1] = 5.8675;
    starpar[2] = 4.28/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto f3InchFlange3Solid = new G4Tubs("3InchFlange3Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto f3InchFlange3Logical = new G4LogicalVolume(f3InchFlange3Solid, FindMaterial("G4_Fe"), "3InchFlange3Logical");
    auto f3InchFlange3Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 202.9025*cm), f3InchFlange3Logical, "3InchFlange3Physical", f3InchSectionLogical, 0, false, checkOverlaps);
    auto f3InchConeSolid = new G4Cons("3InchConeSolid", 3.53*cm, 3.81*cm, 6.08*cm, 6.35*cm, 27.254*cm, 0*deg, 360*deg);
    auto f3InchConeLogical = new G4LogicalVolume(f3InchConeSolid, FindMaterial("G4_Fe"), "3InchConeLogical");
    auto f3InchConePhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 218.6695*cm), f3InchConeLogical, "3InchConePhysical", f3InchSectionLogical, 0, false, checkOverlaps);


    //5INCH SECTION
    starpar[0] = 0;
    starpar[1] = 8.5725;
    starpar[2] = 396.846/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto f5InchSectionSolid = new G4Tubs("5InchSectionSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto f5InchSectionLogical = new G4LogicalVolume(f5InchSectionSolid, FindMaterial("G4_AIR"), "5InchSectionLogical");
    auto f5InchSectionPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -73.3385*cm), f5InchSectionLogical, "5InchSectionPhysical", fUpstreamSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 0.;
    starpar[1] = 6.07;
    starpar[2] = 396.846/2;
    starpar[3] = 0.;
    starpar[4] = 360.;
    auto f5InchPipeVacuumSolid = new G4Tubs("5InchPipeVacuumSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto f5InchPipeVacuumLogical = new G4LogicalVolume(f5InchPipeVacuumSolid, FindMaterial("G4_Galactic"), "5InchPipeVacuumLogical");
    auto f5InchPipeVacuumPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 0.*cm), f5InchPipeVacuumLogical, "5InchPipeVacuumPhysical", f5InchSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 6.07;
    starpar[1] = 6.35;
    starpar[2] = 396.846/2;
    starpar[3] = 0.;
    starpar[4] = 360.;
    auto f5InchPipeSolid = new G4Tubs("5InchPipeSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto f5InchPipeLogical = new G4LogicalVolume(f5InchPipeSolid, FindMaterial("G4_Fe"), "5InchPipeLogical");
    auto f5InchPipePhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 0.*cm), f5InchPipeLogical, "5InchPipePhysical", f5InchSectionLogical, 0, false, checkOverlaps);

    starpar[0] = 6.35;
    starpar[1] = 8.5725;
    starpar[2] = 4.267/2;
    starpar[3] = 0.;
    starpar[4] = 360.;
    auto f5InchFlange1Solid = new G4Tubs("5InchFlange1Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto f5InchFlange1Logical = new G4LogicalVolume(f5InchFlange1Solid, FindMaterial("G4_Fe"), "5InchFlange1Logical");
    auto f5InchFlange1Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 51.218*cm), f5InchFlange1Logical, "5InchFlange1Physical", f5InchSectionLogical, 0, false, checkOverlaps);

    starpar[0] = 6.35;
    starpar[1] = 8.5725;
    starpar[2] = 4.271/2;
    starpar[3] = 0.;
    starpar[4] = 360.;
    auto f5InchFlange2Solid = new G4Tubs("5InchFlange2Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto f5InchFlange2Logical = new G4LogicalVolume(f5InchFlange2Solid, FindMaterial("G4_Fe"), "5InchFlange2Logical");
    auto f5InchFlange2Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 78.196*cm), f5InchFlange2Logical, "5InchFlange2Physical", f5InchSectionLogical, 0, false, checkOverlaps);

    starpar[0] = 6.35;
    starpar[1] = 8.5725;
    starpar[2] = 4.267/2;
    starpar[3] = 0.;
    starpar[4] = 360.;
    auto f5InchFlange3Solid = new G4Tubs("5InchFlange3Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto f5InchFlange3Logical = new G4LogicalVolume(f5InchFlange3Solid, FindMaterial("G4_Fe"), "5InchFlange3Logical");
    auto f5InchFlange3Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 111.521*cm), f5InchFlange3Logical, "5InchFlange3Physical", f5InchSectionLogical, 0, false, checkOverlaps);
    
    auto f5InchFlange4Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 150.573*cm), f5InchFlange3Logical, "5InchFlange4Physical", f5InchSectionLogical, 0, false, checkOverlaps);
    auto f5InchFlange5Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 188.381*cm), f5InchFlange3Logical, "5InchFlange5Physical", f5InchSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 6.35;
    starpar[1] = 8.5725;
    starpar[2] = 1.825/2;
    starpar[3] = 0.;
    starpar[4] = 360.;
    auto f5InchFlange6Solid = new G4Tubs("5InchFlange6Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto f5InchFlange6Logical = new G4LogicalVolume(f5InchFlange6Solid, FindMaterial("G4_Fe"), "5InchFlange6Logical");
    auto f5InchFlange6Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 197.5085*cm), f5InchFlange6Logical, "5InchFlange6Physical", f5InchSectionLogical, 0, false, checkOverlaps);

    //DX MAGNET SECTION
    starpar[0] = 0;
    starpar[1] = 32.08;
    starpar[2] = 611.27/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fDXMagnetSectionSolid = new G4Tubs("DXMagnetSectionSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDXMagnetSectionLogical = new G4LogicalVolume(fDXMagnetSectionSolid, FindMaterial("G4_AIR"), "DXMagnetSectionLogical");
    auto fDXMagnetSectionPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 430.7195*cm), fDXMagnetSectionLogical, "DXMagnetSectionPhysical", fUpstreamSectionLogical, 0, false, checkOverlaps);

    starpar[0] = 6.833;
    starpar[1] = 6.985;
    starpar[2] = 467.004/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto f55InchPipeSolid = new G4Tubs("55InchPipeSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto f55InchPipeLogical = new G4LogicalVolume(f55InchPipeSolid, FindMaterial("G4_Fe"), "55InchPipeLogical");
    auto f55InchPipePhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -21.918*cm), f55InchPipeLogical, "55InchPipePhysical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 0;
    starpar[1] = 6.833;
    starpar[2] = 467.004/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto f55InchPipeVacuumSolid = new G4Tubs("55InchPipeVacuumSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    f55InchPipeVacuumLogical = new G4LogicalVolume(f55InchPipeVacuumSolid, FindMaterial("G4_Galactic"), "55InchPipeVacuumLogical");
    auto f55InchPipeVacuumPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -21.918*cm), f55InchPipeVacuumLogical, "55InchPipeVacuumPhysical", fDXMagnetSectionLogical, 0, false, checkOverlaps);

    auto fDX5InchConeSolid = new G4Cons("DX5InchConeSolid", 6.07*cm, 6.35*cm, 6.833*cm, 6.985*cm, 5.73/2*cm, 0.*deg, 360.*deg);
    auto fDX5InchConeLogical = new G4LogicalVolume(fDX5InchConeSolid, FindMaterial("G4_Fe"), "DX5InchConeLogical");
    auto fDX5InchConePhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -258.285*cm), fDX5InchConeLogical, "DX5InchConePhysical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    auto fDX5InchConeVacuumSolid = new G4Cons("DX5InchConeVacuumSolid", 0.*cm, 6.07*cm, 0.*cm, 6.833*cm, 5.73/2*cm, 0.*deg, 360.*deg);
    auto fDX5InchConeVacuumLogical = new G4LogicalVolume(fDX5InchConeVacuumSolid, FindMaterial("G4_Galactic"), "DX5InchConeVacuumLogical");
    auto fDX5InchConeVacuumPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -258.285*cm), fDX5InchConeVacuumLogical, "DX5InchConeVacuumPhysical", fDXMagnetSectionLogical, 0, false, checkOverlaps);

    starpar[0] = 0.;
    starpar[1] = 6.07;
    starpar[2] = 20.32/2;
    starpar[3] = 0.;
    starpar[4] = 360.;
    auto fDX5InchVacuum1Solid = new G4Tubs("DX5InchVacuum1Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDX5InchVacuum1Logical = new G4LogicalVolume(fDX5InchVacuum1Solid, FindMaterial("G4_Galactic"), "DX5InchVacuum1Logical");
    auto fDX5InchVacuum1Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -295.475*cm), fDX5InchVacuum1Logical, "DX5InchVacuum1Physical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 0.;
    starpar[1] = 5.004;
    starpar[2] = 15.392/2;
    starpar[3] = 0.;
    starpar[4] = 360.;
    auto fDX5InchVacuum2Solid = new G4Tubs("DX5InchVacuum2Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDX5InchVacuum2Logical = new G4LogicalVolume(fDX5InchVacuum2Solid, FindMaterial("G4_Galactic"), "DX5InchVacuum2Logical");
    auto fDX5InchVacuum2Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -277.619*cm), fDX5InchVacuum2Logical, "DX5InchVacuum2Physical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 0.;
    starpar[1] = 6.07;
    starpar[2] = 8.773/2;
    starpar[3] = 0.;
    starpar[4] = 360.;
    auto fDX5InchVacuum3Solid = new G4Tubs("DX5InchVacuum3Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDX5InchVacuum3Logical = new G4LogicalVolume(fDX5InchVacuum3Solid, FindMaterial("G4_Galactic"), "DX5InchVacuum3Logical");
    auto fDX5InchVacuum3Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -265.5365*cm), fDX5InchVacuum3Logical, "DX5InchVacuum3Physical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    auto fDX55InchConeVacuumSolid = new G4Cons("DX55InchConeVacuumSolid", 0.*cm, 6.833*cm, 0.*cm, 15.6845*cm, 40.691/2*cm, 0.*deg, 360.*deg);
    auto fDX55InchConeVacuumLogical = new G4LogicalVolume(fDX55InchConeVacuumSolid, FindMaterial("G4_Galactic"), "DX55InchConeVacuumLogical");
    auto fDX55InchConeVacuumPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 231.9295*cm), fDX55InchConeVacuumLogical, "DX55InchConeVacuumPhysical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 0;
    starpar[1] = 14.605;
    starpar[2] = 53.361/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fDX12InchPipeVacuumSolid = new G4Tubs("DX12InchPipeVacuumSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDX12InchPipeVacuumLogical = new G4LogicalVolume(fDX12InchPipeVacuumSolid, FindMaterial("G4_Galactic"), "DX12InchPipeVacuumLogical");
    auto fDX12InchPipeVacuumPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 278.9555*cm), fDX12InchPipeVacuumLogical, "DX12InchPipeVacuumPhysical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 6.07;
    starpar[1] = 6.35;
    starpar[2] = 20.32/2;
    starpar[3] = 0.;
    starpar[4] = 360.;
    auto fDX5InchPipe1Solid = new G4Tubs("DX5InchPipe1Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDX5InchPipe1Logical = new G4LogicalVolume(fDX5InchPipe1Solid, FindMaterial("G4_Fe"), "DX5InchPipe1Logical");
    auto fDX5InchPipe1Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -295.475*cm), fDX5InchPipe1Logical, "DX5InchPipe1Physical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 5.004;
    starpar[1] = 6.985;
    starpar[2] = 15.382/2;
    starpar[3] = 0.;
    starpar[4] = 360.;
    auto fDX5InchPipe2Solid = new G4Tubs("DX5InchPipe2Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDX5InchPipe2Logical = new G4LogicalVolume(fDX5InchPipe2Solid, FindMaterial("G4_Fe"), "DX5InchPipe2Logical");
    auto fDX5InchPipe2Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -277.619*cm), fDX5InchPipe2Logical, "DX5InchPipe2Physical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 6.07;
    starpar[1] = 6.35;
    starpar[2] = 8.773/2;
    starpar[3] = 0.;
    starpar[4] = 360.;
    auto fDX5InchPipe3Solid = new G4Tubs("DX5InchPipe3Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDX5InchPipe3Logical = new G4LogicalVolume(fDX5InchPipe3Solid, FindMaterial("G4_Fe"), "DX5InchPipe3Logical");
    auto fDX5InchPipe3Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -265.5365*cm), fDX5InchPipe3Logical, "DX5InchPipe3Physical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    auto fDX55InchConeSolid = new G4Cons("DX55InchConeSolid", 6.833*cm, 6.985*cm, 15.6845*cm, 16.4465*cm, 40.691/2*cm, 0.*deg, 360.*deg);
    auto fDX55InchConeLogical = new G4LogicalVolume(fDX55InchConeSolid, FindMaterial("G4_Fe"), "DX55InchConeLogical");
    auto fDX55InchConePhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm,231.9295*cm), fDX55InchConeLogical, "DX55InchConePhysical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 14.605;
    starpar[1] = 15.24;
    starpar[2] = 53.361/2;
    starpar[3] = 0.;
    starpar[4] = 360.;
    auto fDX12InchPipeSolid = new G4Tubs("DX12InchPipeSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDX12InchPipeLogical = new G4LogicalVolume(fDX12InchPipeSolid, FindMaterial("G4_Fe"), "DX12InchPipeLogical");
    auto fDX12InchPipePhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 278.9555*cm), fDX12InchPipeLogical, "DX12InchPipePhysical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 6.35;
    starpar[1] = 8.5725;
    starpar[2] = 2.134/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fDXFlange1Solid = new G4Tubs("DXFlange1Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDXFlange1Logical = new G4LogicalVolume(fDXFlange1Solid, FindMaterial("G4_Fe"), "DXFlange1Logical");
    auto fDXFlange1Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -304.568*cm), fDXFlange1Logical, "DXFlange1Physical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 15.24;
    starpar[1] = 18.415;
    starpar[2] = 2.84/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fDXFlange2Solid = new G4Tubs("DXFlange2Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDXFlange2Logical = new G4LogicalVolume(fDXFlange2Solid, FindMaterial("G4_Fe"), "DXFlange2Logical");
    auto fDXFlange2Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 304.2135*cm), fDXFlange2Logical, "DXFlange2Physical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 10.159;
    starpar[1] = 10.795;
    starpar[2] = 27.94/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fDXVesselFrontSolid = new G4Tubs("DXVesselFrontSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDXVesselFrontLogical = new G4LogicalVolume(fDXVesselFrontSolid, FindMaterial("G4_Fe"), "DXVesselFrontLogical");
    auto fDXVesselFrontPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -277.06*cm), fDXVesselFrontLogical, "DXVesselFrontPhysical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 10.159;
    starpar[1] = 13.97;
    starpar[2] = 1.27/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fDXVesselFrontFlangeSolid = new G4Tubs("DXVesselFrontFlangeSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDXVesselFrontFlangeLogical = new G4LogicalVolume(fDXVesselFrontFlangeSolid, FindMaterial("G4_Fe"), "DXVesselFrontFlangeLogical");
    auto fDXVesselFrontFlangePhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -291.665*cm), fDXVesselFrontFlangeLogical, "DXVesselFrontFlangePhysical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 17.78;
    starpar[1] = 20.32;
    starpar[2] = 1.27/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fDXVesselRearFlangeSolid = new G4Tubs("DXVesselRearFlangeSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDXVesselRearFlangeLogical = new G4LogicalVolume(fDXVesselRearFlangeSolid, FindMaterial("G4_Fe"), "DXVesselRearFlangeLogical");
    auto fDXVesselRearFlangePhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 293.782*cm), fDXVesselRearFlangeLogical, "DXVesselRearFlangePhysical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 17.78;
    starpar[1] = 18.415;
    starpar[2] = 25.4/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fDXVesselRearSolid = new G4Tubs("DXVesselRearSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDXVesselRearLogical = new G4LogicalVolume(fDXVesselRearSolid, FindMaterial("G4_Fe"), "DXVesselRearLogical");
    auto fDXVesselRearPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 280.447*cm), fDXVesselRearLogical, "DXVesselRearPhysical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 8.0645;
    starpar[1] = 8.6995;
    starpar[2] = 463.423/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fDXVesselInnerSolid = new G4Tubs("DXVesselInnerSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDXVesselInnerLogical = new G4LogicalVolume(fDXVesselInnerSolid, FindMaterial("G4_Fe"), "DXVesselInnerLogical");
    auto fDXVesselInnerPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -20.1265*cm), fDXVesselInnerLogical, "DXVesselInnerPhysical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 31.128;
    starpar[1] = 32.08;
    starpar[2] = 424.028/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fDXVesselOuterSolid = new G4Tubs("DXVesselOuterSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDXVesselOuterLogical = new G4LogicalVolume(fDXVesselOuterSolid, FindMaterial("G4_Fe"), "DXVesselOuterLogical");
    auto fDXVesselOuterPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -0.479*cm), fDXVesselOuterLogical, "DXVesselOuterPhysical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 8.6995;
    starpar[1] = 31.128;
    starpar[2] = 0.559/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fDXVesselCapSolid = new G4Tubs("DXVesselCapSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDXVesselCapLogical = new G4LogicalVolume(fDXVesselCapSolid, FindMaterial("G4_Fe"), "DXVesselCapLogical");
    auto fDXVesselCap1Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -212.2135*cm), fDXVesselCapLogical, "DXVesselCap1Physical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    auto fDXVesselCap2Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 208.740*cm), fDXVesselCapLogical, "DXVesselCap2Physical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 9.00;
    starpar[1] = 10.34;
    starpar[2] = 421.84/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fDXCoilSolid = new G4Tubs("DXCoilSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDXCoilLogical = new G4LogicalVolume(fDXCoilSolid, FindMaterial("G4_Fe"), "DXCoilLogical");
    auto fDXCoilPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -0.479*cm), fDXCoilLogical, "DXCoilPhysical", fDXMagnetSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 14.35;
    starpar[1] = 31.1;
    starpar[2] = 421.84/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fDXYokeSolid = new G4Tubs("DXYokeSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDXYokeLogical = new G4LogicalVolume(fDXYokeSolid, FindMaterial("G4_Fe"), "DXYokeLogical");
    auto fDXYokePhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -0.479*cm), fDXYokeLogical, "DXYokePhysical", fDXMagnetSectionLogical, 0, false, checkOverlaps);


    //------DOWN DTREAM---------------
    //DOWNSTREAM SECTION
    G4double size[3] = {60/2., 80./2., 450.653/2};
    auto fDownStreamSection0Solid = new G4Box("DownStreamSection0Solid", size[0]*cm, size[1]*cm, size[2]*cm);
    size[0] = 12.6/2;
    size[1] = 81./2;
    size[2] = 160.025/2;
    auto fNegativeDownStreamSectionSolid = new G4Box("NegativeDownStreamSectionSolid", size[0]*cm, size[1]*cm, size[2]*cm);
    auto fDownStreamSectionSolid = new G4SubtractionSolid("DownStreamSectionSolidSolid", fDownStreamSection0Solid, fNegativeDownStreamSectionSolid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm,(295./2+4.)*cm));
    auto fDownStreamSectionLogical = new G4LogicalVolume(fDownStreamSectionSolid, FindMaterial("G4_AIR"), "DownStreamSectionLogical");
    auto fDownStreamSectionPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 1698.0355*cm), fDownStreamSectionLogical, "DownStreamSectionPhysical", fWorldLogical, 0, false, checkOverlaps);
    starpar[0] = 14.6175;
    starpar[1] = 14.9225;
    starpar[2] = 57.839/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fBellowsSolid = new G4Tubs("BellowsSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fBellowsLogical = new G4LogicalVolume(fBellowsSolid, FindMaterial("G4_Fe"), "BellowsLogical");
    auto fBellowsPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -196.4070*cm), fBellowsLogical, "BellowsPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 0;
    starpar[1] = 14.6175;
    starpar[2] = 52.07/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fBellowsVacuumSolid = new G4Tubs("BellowsVacuumSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fBellowsVacuumLogical = new G4LogicalVolume(fBellowsVacuumSolid, FindMaterial("G4_Galactic"), "BellowsVacuumLogical");
    auto fBellowsVacuumPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -196.4070*cm), fBellowsVacuumLogical, "BellowsVacuumPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 14.9225;
    starpar[1] = 18.415;
    starpar[2] = 2.845/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fBellowsFlange1Solid = new G4Tubs("BellowsFlange1Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fBellowsFlange1Logical = new G4LogicalVolume(fBellowsFlange1Solid, FindMaterial("G4_Fe"), "BellowsFlange1Logical");
    auto fBellowsFlange1Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -211.1015*cm), fBellowsFlange1Logical, "BellowsFlange1Physical", fDownStreamSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 14.9225;
    starpar[1] = 18.415;
    starpar[2] = 5.69/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fBellowsFlange2Solid = new G4Tubs("BellowsFlange2Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fBellowsFlange2Logical = new G4LogicalVolume(fBellowsFlange2Solid, FindMaterial("G4_Fe"), "BellowsFlange2Logical");
    auto fBellowsFlange2Physical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -179.6065*cm), fBellowsFlange2Logical, "BellowsFlange2Physical", fDownStreamSectionLogical, 0, false, checkOverlaps);

    
    size[0] = 40.269/2;
    size[1] = 40.005/2;
    size[2] = 2.54/2;
    auto fDownStreamBoxFlange1Solid = new G4Box("DownStreamBoxFlange1Solid", size[0]*cm, size[1]*cm, size[2]*cm);
    size[0] = 41.492/2;
    size[1] = 25.683/2;
    size[2] = 2.54/2;
    auto fDownStreamBoxFlange2Solid = new G4Box("DownStreamBoxFlange2Solid", size[0]*cm, size[1]*cm, size[2]*cm);
    size[0] = 43.226/2;
    size[1] = 31.354/2;
    size[2] = 2.54/2;
    auto fDownStreamBoxFlange3Solid = new G4Box("DownStreamBoxFlange3Solid", size[0]*cm, size[1]*cm, size[2]*cm);
    size[0] = 44.041/2;
    size[1] = 23.734/2;
    size[2] = 2.54/2;
    auto fDownStreamBoxFlange4Solid = new G4Box("DownStreamBoxFlange4Solid", size[0]*cm, size[1]*cm, size[2]*cm);
    size[0] = 45.269/2;
    size[1] = 23.734/2;
    size[2] = 2.54/2;
    auto fDownStreamBoxFlange5Solid = new G4Box("DownStreamBoxFlange5Solid", size[0]*cm, size[1]*cm, size[2]*cm);
    size[0] = 45.269/2;
    size[1] = 23.734/2;
    size[2] = 2.54/2;
    auto fDownStreamBoxFlange6Solid = new G4Box("DownStreamBoxFlange6Solid", size[0]*cm, size[1]*cm, size[2]*cm);
    size[0] = 47.762/2;
    size[1] = 23.734/2;
    size[2] = 2.54/2;
    auto fDownStreamBoxFlange7Solid = new G4Box("DownStreamBoxFlange7Solid", size[0]*cm, size[1]*cm, size[2]*cm);
    size[0] = 48.632/2;
    size[1] = 23.734/2;
    size[2] = 2.54/2;
    auto fDownStreamBoxFlange8Solid = new G4Box("DownStreamBoxFlange8Solid", size[0]*cm, size[1]*cm, size[2]*cm);
    size[0] = 49.491/2;
    size[1] = 23.734/2;
    size[2] = 2.54/2;
    auto fDownStreamBoxFlange9Solid = new G4Box("DownStreamBoxFlange9Solid", size[0]*cm, size[1]*cm, size[2]*cm);
    size[0] = 50.330/2;
    size[1] = 23.734/2;
    size[2] = 2.54/2;
    auto fDownStreamBoxFlange10Solid = new G4Box("DownStreamBoxFlange10Solid", size[0]*cm, size[1]*cm, size[2]*cm);
    size[0] = 51.604/2;
    size[1] = 23.734/2;
    size[2] = 2.54/2;
    auto fDownStreamBoxFlange11Solid = new G4Box("DownStreamBoxFlange11Solid", size[0]*cm, size[1]*cm, size[2]*cm);
    size[0] = 52.565/2;
    size[1] = 31.354/2;
    size[2] = 2.54/2;
    auto fDownStreamBoxFlange12Solid = new G4Box("DownStreamBoxFlange12Solid", size[0]*cm, size[1]*cm, size[2]*cm);

    auto fDownStreamBox11Solid = new G4Trd("DownStreamBox11Solid", 17.253*cm, 17.81*cm, 17.5*cm, 17.49*cm, 9.46*cm);
    auto fDownStreamBox1Solid = new G4UnionSolid("DownStreamBox1Solid", fDownStreamBox11Solid, fDownStreamBoxFlange1Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 16.0/2*cm));
    auto fDownStreamBox21Solid = new G4Trd("DownStreamBox21Solid", 17.81*cm, 19.22*cm, 17.5*cm, 9.309*cm, 10.657*cm);
    auto fDownStreamBox22Solid = new G4UnionSolid("DownStreamBox22Solid", fDownStreamBox21Solid, fDownStreamBoxFlange2Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, (18.127-10.657)*cm));

    auto fDownStreamBox31Solid = new G4Trd("DownStreamBox31Solid", 19.22*cm, 24.5*cm, 9.31*cm, 9.326*cm, 101.658*cm);
    auto fDownStreamBox32Solid = new G4UnionSolid("DownStreamBox32Solid", fDownStreamBox31Solid, fDownStreamBoxFlange3Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, (65.165-141.892)*cm));
    auto fDownStreamBox33Solid = new G4UnionSolid("DownStreamBox33Solid", fDownStreamBox32Solid, fDownStreamBoxFlange4Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, (78.5-141.892)*cm));
    auto fDownStreamBox34Solid = new G4UnionSolid("DownStreamBox34Solid", fDownStreamBox33Solid, fDownStreamBoxFlange5Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, (98.82-141.892)*cm));
    auto fDownStreamBox35Solid = new G4UnionSolid("DownStreamBox35Solid", fDownStreamBox34Solid, fDownStreamBoxFlange6Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, (118.84-141.892)*cm));
    auto fDownStreamBox36Solid = new G4UnionSolid("DownStreamBox36Solid", fDownStreamBox35Solid, fDownStreamBoxFlange7Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, (140.095-141.892)*cm));
    auto fDownStreamBox37Solid = new G4UnionSolid("DownStreamBox37Solid", fDownStreamBox36Solid, fDownStreamBoxFlange8Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, (154.659-141.892)*cm));
    auto fDownStreamBox38Solid = new G4UnionSolid("DownStreamBox38Solid", fDownStreamBox37Solid, fDownStreamBoxFlange9Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, (168.873-141.892)*cm));
    auto fDownStreamBox39Solid = new G4UnionSolid("DownStreamBox39Solid", fDownStreamBox38Solid, fDownStreamBoxFlange10Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, (182.741-141.892)*cm));
    auto fDownStreamBox310Solid = new G4UnionSolid("DownStreamBox310Solid", fDownStreamBox39Solid, fDownStreamBoxFlange11Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, (203.696-141.892)*cm));
    auto fDownStreamBox311Solid = new G4UnionSolid("DownStreamBox311Solid", fDownStreamBox310Solid, fDownStreamBoxFlange12Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, (219.47-141.892)*cm));
    //FRONT HOLE
    size[0] = 17.253;
    size[1] = 17.5;
    size[2] = 0.4;
    auto fDownStreamBoxFront1Solid = new G4Box("DownStreamBoxFront1Solid", size[0]*cm, size[1]*cm, size[2]*cm);
    starpar[0] = 0.;
    starpar[1] = 14.351;
    starpar[2] = 0.45;
    starpar[3] = 0.;
    starpar[4] = 360.;
    auto fNegativeDownStreamBoxFront1Solid = new G4Tubs("NegativeDownStreamBoxFront1Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDownStreamBoxFrontSolid = new G4SubtractionSolid("DownStreamBoxFrontSolid", fDownStreamBoxFront1Solid, fNegativeDownStreamBoxFront1Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 0.*cm));
    //REAR HOLES
    size[0] = 24.5;
    size[1] = 9.326;
    size[2] = 0.44;
    auto fDownStreamBoxRear1Solid = new G4Box("DownStreamBoxRear1Solid", size[0]*cm, size[1]*cm, size[2]*cm);
    starpar[0] = 0.;
    starpar[1] = 7.2;
    starpar[2] = 0.45;
    starpar[3] = 0.;
    starpar[4] = 360.;
    auto fNegativeDownStreamBoxRear1Solid = new G4Tubs("NegativeDownStreamBoxRear1Solid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fDownStreamBoxRear2Solid = new G4SubtractionSolid("DownStreamBoxRear2Solid", fDownStreamBoxRear1Solid, fNegativeDownStreamBoxRear1Solid, fNonRotation, G4ThreeVector(13.758*cm, 0.*cm, 0.*cm));
    auto fDownStreamBoxRearSolid = new G4SubtractionSolid("DownStreamBoxRearSolid", fDownStreamBoxRear2Solid, fNegativeDownStreamBoxRear1Solid, fNonRotation, G4ThreeVector(-13.758*cm, 0.*cm, 0.*cm));
    
    auto fNegativeDownStreamBox1Solid = new G4Trd("NegativeDownStreamBox1Solid", 17.171*cm, 17.729*cm, 16.485*cm, 16.485*cm, 9.463*cm);
    auto fNegativeDownStreamBox2Solid = new G4Trd("NegativeDownStreamBox2Solid", 17.729*cm, 18.372*cm, 16.485*cm, 8.35*cm, 10.658*cm);
    auto fNegativeDownStreamBox3Solid = new G4Trd("NegativeDownStreamBox3Solid", 18.372*cm, 23.865*cm, 8.35*cm, 8.35*cm, 101.67*cm);
    auto fNegativeDownStreamBox4Solid = new G4UnionSolid("NegativeDownStreamBox4Solid", fNegativeDownStreamBox1Solid, fNegativeDownStreamBox2Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 20.1169*cm));
    auto fNegativeDownStreamBoxSolid = new G4UnionSolid("NegativeDownStreamBoxSolid", fNegativeDownStreamBox4Solid, fNegativeDownStreamBox3Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 132.4318*cm));

    auto fDownStreamBoxPart1Solid = new G4SubtractionSolid("DownStreamBoxPart1Solid", fDownStreamBox1Solid, fNegativeDownStreamBox1Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 0*cm));
    auto fDownStreamBoxPart2Solid = new G4SubtractionSolid("DownStreamBoxPart2Solid", fDownStreamBox22Solid, fNegativeDownStreamBox2Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 0*cm));
    auto fDownStreamBoxPart3Solid = new G4SubtractionSolid("DownStreamBoxPart3Solid", fDownStreamBox311Solid, fNegativeDownStreamBox3Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 0*cm));

    
    auto fDownStreamBox4Solid = new G4UnionSolid("DownStreamBox4Solid", fDownStreamBoxPart1Solid, fDownStreamBoxPart2Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 20.1169*cm));
    auto fDownStreamBox5Solid = new G4UnionSolid("DownStreamBox5Solid", fDownStreamBox4Solid, fDownStreamBoxPart3Solid, fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 132.4318*cm));

    //PUT BOX COVER
    //
    G4double kBoxCoverPosition = -164.;
    auto fDownStreamBoxLogical = new G4LogicalVolume(fDownStreamBox5Solid, FindMaterial("G4_Fe"), "DownStreamBoxLogical");
    auto fDownStreamBoxPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, kBoxCoverPosition*cm), fDownStreamBoxLogical, "DownStreamBoxPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);
    //FRONT
    auto fDownStreamBoxFrontLogical = new G4LogicalVolume(fDownStreamBoxFrontSolid, FindMaterial("G4_Fe"), "DownStreamBoxFrontLogical");
    auto fDownStreamBoxFrontPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, (-9.+kBoxCoverPosition)*cm), fDownStreamBoxFrontLogical, "DownStreamBoxFrontPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);
    //REAR
    auto fDownStreamBoxRearLogical = new G4LogicalVolume(fDownStreamBoxRearSolid, FindMaterial("G4_Fe"), "DownStreamBoxRearLogical");
    auto fDownStreamBoxRearPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, (234.5+kBoxCoverPosition)*cm), fDownStreamBoxRearLogical, "DownStreamBoxRearPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);


    // PANTS STRUCTURE
    starpar[0] = 0.;
    starpar[1] = 14.246;
    starpar[2] = 14.681/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fPantsIncomeSolid = new G4Tubs("PantsIncomeSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fPantsIncomeLogical = new G4LogicalVolume(fPantsIncomeSolid, FindMaterial("G4_Fe"), "PantsIncomeLogical");
    starpar[0] = 0;
    starpar[1] = 14.191;
    starpar[2] = 14.682/2;
    starpar[3] = 0;
    starpar[4] = 360;
    G4double IncomePosition = -165.9160;
    auto fPantsIncomeVacuumSolid = new G4Tubs("fPantsIncomeVacuumSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fPantsIncomeVacuumLogical = new G4LogicalVolume(fPantsIncomeVacuumSolid, FindMaterial("G4_Galactic"), "PantsIncomeVacuumLogical");
    auto fPantsIncomeVacuumPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 0.*cm), fPantsIncomeVacuumLogical, "PantsIncomeVacuumPhysical", fPantsIncomeLogical, 0, false, checkOverlaps);
    auto fPantsIncomePhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, IncomePosition*cm), fPantsIncomeLogical, "PantsIncomePhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);
    auto fPantsShrinkSolid = new G4Cons("PantsShrinkSolid", 0.*cm, 14.146*cm, 0.*cm, 7.619*cm, 26.568/2*cm, 0*deg, 360.*deg);
    auto fPantsShrinkLogical = new G4LogicalVolume(fPantsShrinkSolid, FindMaterial("G4_Fe"), "PantsShrinkLogical");
    auto fPantsShrinkVacuumSolid = new G4Cons("PantsShrinkVacuumSolid", 0.*cm, 14.091*cm, 0.*cm, 7.464*cm, 26.568/2*cm, 0.*deg, 360.*deg);
    auto fPantsShrinkVacuumLogical = new G4LogicalVolume(fPantsShrinkVacuumSolid, FindMaterial("G4_Galactic"), "PantsShrinkVacuumLogical");
    auto fPantsShrinkVacuumPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 0.*cm), fPantsShrinkVacuumLogical, "PantsShrinkVacuumPhysical", fPantsShrinkLogical, 0, false, checkOverlaps);
    auto fPantsShrinkPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, (IncomePosition+20.6245)*cm), fPantsShrinkLogical, "PantsShrinkPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 0.;
    starpar[1] = 7.036;
    starpar[2] = 201.022/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fPantsPipeSolid = new G4Tubs("PantsPipeSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fPantsPipeLogical = new G4LogicalVolume(fPantsPipeSolid, FindMaterial("G4_Fe"), "PantsPipeLogical");
    starpar[0] = 0;
    starpar[1] = 6.98;
    starpar[2] = 201.024/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fPantsPipeVacuumSolid = new G4Tubs("PantsPipeVacuumSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fPantsPipeVacuumLogical = new G4LogicalVolume(fPantsPipeVacuumSolid, FindMaterial("G4_Galactic"), "PantsPipeVacuumLogical");
    auto fPantsPipeVacuumPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 0.*cm), fPantsPipeVacuumLogical, "PantsPipeVacuumPhysical", fPantsPipeLogical, 0, false, checkOverlaps);
    


    starpar[0] = 0.;
    starpar[1] = 7.722;
    starpar[2] = 2.807/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fPantsOutgoTubeSolid = new G4Tubs("PantsOutgoTubeSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fPantsOutgoTubeLogical = new G4LogicalVolume(fPantsOutgoTubeSolid, FindMaterial("G4_Fe"), "PantsOutgoTubeLogical");
    size[0] = 15.342/2;
    size[1] = 15.444/2;
    size[2] = 2.807/2;
    auto fPantsOutgoBoxSolid = new G4Box("PantsOutgoBoxSolid", size[0]*cm, size[1]*cm, size[2]*cm);
    auto fPantsOutgoBoxLogical = new G4LogicalVolume(fPantsOutgoBoxSolid, FindMaterial("G4_Fe"), "PantsOutgoBoxLogical");
    size[0] = 15.3421/2;
    size[1] = 14.128/2;
    size[2] = 2.489/2;
    auto fPantsOutgoBoxVacuumSolid = new G4Box("PantsOutgoBoxVacuumSolid", size[0]*cm, size[1]*cm, size[2]*cm);
    auto fPantsOutgoBoxVacuumLogical = new G4LogicalVolume(fPantsOutgoBoxVacuumSolid, FindMaterial("G4_Galactic"), "PantsOutgoBoxVacuumLogical");
    auto fPantsOutgoBoxVacuumPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, -0.159*cm), fPantsOutgoBoxVacuumLogical, "PantsOutgoBoxVacuumPhysical", fPantsOutgoBoxLogical, 0, false, checkOverlaps);
    auto fPantsOutgoBoxPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, (IncomePosition+35.153)*cm), fPantsOutgoBoxLogical, "PantsOutgoBoxPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);
    starpar[0] = 0.;
    starpar[1] = 7.064;
    starpar[2] = 2.807/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fPantsOutgoTubeVacuumSolid = new G4Tubs("PantsOutgoTubeVacuumSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fPantsOutgoTubeVacuumLogical = new G4LogicalVolume(fPantsOutgoTubeVacuumSolid, FindMaterial("G4_Galactic"), "PantsOutgoTubeVacuumLogical");
    auto fPantsOutgoTubeVacuumPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 0.*cm), fPantsOutgoTubeVacuumLogical, "PantsOutgoTubeVacuumPhysical", fPantsOutgoTubeLogical, 0, false, checkOverlaps);
    auto fPantsOutgoTubeLPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(-7.671*cm, 0.*cm, (IncomePosition+35.312)*cm), fPantsOutgoTubeLogical, "PantsOutgoTubeLPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);
    auto fPantsOutgoTubeRPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(7.671*cm, 0.*cm, (IncomePosition+35.312)*cm), fPantsOutgoTubeLogical, "PantsOutgoTubeRPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);

    
    size[0] = 15.342/2;
    size[1] = 15.238/2;
    size[2] = 27.7/2;
    auto fPantsSideBoxSolid = new G4Box("PantsSideBoxSolid", size[0]*cm, size[1]*cm, size[2]*cm);
    auto fPantsSideBoxLogical = new G4LogicalVolume(fPantsSideBoxSolid, FindMaterial("G4_Fe"), "PantsSideBoxLogical");
    size[0] = 15.36/2;
    size[1] = 15.183/2;
    size[2] = 27.71/2;
    auto fPantsSideBoxVacuumSolid = new G4Box("PantsSideBoxVacuumSolid", size[0]*cm, size[1]*cm, size[2]*cm);
    auto fPantsSideBoxVacuumLogical = new G4LogicalVolume(fPantsSideBoxVacuumSolid, FindMaterial("G4_Galactic"), "PantsSideBoxVacuumLogical");
    auto fPantsSideBoxVacuumPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 0.*cm), fPantsSideBoxVacuumLogical, "PantsSideBoxVacuumPhysical", fPantsSideBoxLogical, 0, false, checkOverlaps);
    auto fPantsSideBoxPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, (IncomePosition+20.6245)*cm), fPantsSideBoxLogical, "PantsSideBoxPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);

    starpar[0] = 0.;
    starpar[1] = 7.619;
    starpar[2] = 27.7/2;
    starpar[3] = -90;
    starpar[4] = 180;
    auto fPantsSideTubeSolid = new G4Tubs("PantsSideTubeSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fPantsSideTubeLogical = new G4LogicalVolume(fPantsSideTubeSolid, FindMaterial("G4_Fe"), "PantsSideTubeLogical");

    starpar[0] = 0;
    starpar[1] = 7.509;
    starpar[2] = 27.7/2;
    starpar[3] = -90;
    starpar[4] = 180;
    auto fPantsSideTubeVacuumSolid = new G4Tubs("PantsSideTubeVacuumSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fPantsSideTubeVacuumLogical = new G4LogicalVolume(fPantsSideTubeVacuumSolid, FindMaterial("G4_Galactic"), "PantsSideTubeVacuumLogical");
    auto fPantsSideTubeVacuumPhysical = new G4PVPlacement(fNonRotation, G4ThreeVector(0.*cm, 0.*cm, 0.*cm), fPantsSideTubeVacuumLogical, "PantsSideTubeVacuumPhysical", fPantsSideTubeLogical, 0, false, checkOverlaps);
    G4RotationMatrix* fSideLRotation = new G4RotationMatrix();
    fSideLRotation -> rotateY(2.24*deg);
    auto fPantsSideTubeRPhysical = new G4PVPlacement(fSideLRotation, G4ThreeVector(-7.671*cm, 0.*cm, (IncomePosition+20.6245)*cm), fPantsSideTubeLogical, "PantsSideTubeRPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);
    G4RotationMatrix* fSideRRotation = new G4RotationMatrix();
    fSideLRotation -> rotateY(-2.24*deg);
    fSideLRotation -> rotateZ(180.*deg);
    auto fPantsSideTubeLPhysical = new G4PVPlacement(fSideRRotation, G4ThreeVector(7.671*cm, 0.*cm, (IncomePosition+20.6245)*cm), fPantsSideTubeLogical, "PantsSideTubeLPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);
    G4RotationMatrix* fPantsPipeLRotation = new G4RotationMatrix();
    fPantsPipeLRotation -> rotateY(-1.74*deg);
    auto fPantsPipeLPhysical = new G4PVPlacement(fPantsPipeLRotation, G4ThreeVector(10.691*cm, 0.*cm, (IncomePosition+136.847)*cm), fPantsPipeLogical, "PantsPipeLPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);
    G4RotationMatrix* fPantsPipeRRotation = new G4RotationMatrix();
    fPantsPipeRRotation -> rotateY(1.74*deg);
    auto fPantsPipeRPhysical = new G4PVPlacement(fPantsPipeRRotation, G4ThreeVector(-10.691*cm, 0.*cm, (IncomePosition+136.847)*cm), fPantsPipeLogical, "PantsPipeRPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);

    starpar[0] = 6.2115;
    starpar[1] = 6.35;
    starpar[2] = 153.391/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fEndPipeSolid = new G4Tubs("EndPipeSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fEndPipeLogical = new G4LogicalVolume(fEndPipeSolid, FindMaterial("G4_Fe"), "EndPipeLogical");
    G4RotationMatrix* fEndPipeLRotation = new G4RotationMatrix();
    fEndPipeLRotation -> rotateY(-0.9*deg);
    auto fEndPipeLPhysical = new G4PVPlacement(fEndPipeLRotation, G4ThreeVector(13.75825*cm, 0.*cm, 148.*cm), fEndPipeLogical, "EndPipeLPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);
    G4RotationMatrix* fEndPipeRRotation = new G4RotationMatrix();
    fEndPipeRRotation -> rotateY(0.9*deg);
    auto fEndPipeRPhysical = new G4PVPlacement(fEndPipeRRotation, G4ThreeVector(-13.75825*cm, 0.*cm, 148.*cm), fEndPipeLogical, "EndPipeRPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);

    starpar[0] = 0.;
    starpar[1] = 6.2115;
    starpar[2] = 153.391/2;
    starpar[3] = 0;
    starpar[4] = 360;
    auto fEndPipeVacuumSolid = new G4Tubs("EndPipeVacuumSolid", starpar[0]*cm, starpar[1]*cm, starpar[2]*cm, starpar[3]*deg, starpar[4]*deg);
    auto fEndPipeVacuumLogical = new G4LogicalVolume(fEndPipeVacuumSolid, FindMaterial("G4_Galactic"), "EndPipeVacuumLogical");
    auto fEndPipeVacuumLPhysical = new G4PVPlacement(fEndPipeLRotation, G4ThreeVector(13.75825*cm, 0.*cm, 148.*cm), fEndPipeVacuumLogical, "EndPipeVacuumLPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);
    auto fEndPipeVacuumRPhysical = new G4PVPlacement(fEndPipeRRotation, G4ThreeVector(-13.75825*cm, 0.*cm, 148.*cm), fEndPipeVacuumLogical, "EndPipeVacuumRPhysical", fDownStreamSectionLogical, 0, false, checkOverlaps);

    
            

    /*-*///COLOR
    /*-*/visAttributes = new G4VisAttributes(G4Colour(0., 0., 0.));
    /*-*/visAttributes -> SetVisibility(false);
    /*-*/fUpstreamSectionLogical -> SetVisAttributes(visAttributes);
    /*-*/fDownStreamSectionLogical -> SetVisAttributes(visAttributes);
    /*-*/fPantsShrinkVacuumLogical -> SetVisAttributes(visAttributes);
    /*-*/fPantsIncomeVacuumLogical -> SetVisAttributes(visAttributes);
    /*-*/f3InchSectionLogical -> SetVisAttributes(visAttributes);
    /*-*/fPantsPipeVacuumLogical -> SetVisAttributes(visAttributes);
    /*-*/fDXMagnetSectionLogical -> SetVisAttributes(visAttributes);
    /*-*/fEndPipeVacuumLogical -> SetVisAttributes(visAttributes);
}

void RHICFDetectorConstruction::SetDEScorer(G4String string)
{
    /*-*///DEFINE SD FOR STORING DE(Deposit energy)
    /*-*/G4MultiFunctionalDetector* SDforDE = new G4MultiFunctionalDetector(string);
    /*-*/G4SDManager::GetSDMpointer()->AddNewDetector(SDforDE);
    /*-*/G4VPrimitiveScorer* PriDEL0;
    /*-*/PriDEL0 = new G4PSEnergyDeposit("DE",0);//DE: Deposit Energy
    /*-*/SDforDE -> RegisterPrimitive(PriDEL0);
    /*-*/SetSensitiveDetector(string, SDforDE);
}

void RHICFDetectorConstruction::SetDENOPScorer(G4String string)
{
    /*-*///DEFINE SD FOR STORING DE AND NOP(Number of photon)
    /*-*/G4MultiFunctionalDetector* SDforDE = new G4MultiFunctionalDetector(string);
    /*-*/G4SDParticleFilter* OPFilter = new G4SDParticleFilter("NOP", "opticalphoton");
    /*-*/G4SDManager::GetSDMpointer()->AddNewDetector(SDforDE);
    /*-*/G4VPrimitiveScorer* PriDEandNoP;
    /*-*/PriDEandNoP = new G4PSEnergyDeposit("DE",0);
    /*-*/SDforDE -> RegisterPrimitive(PriDEandNoP);
    /*-*/PriDEandNoP = new G4PSNofSecondary("NOP",0);
    /*-*/PriDEandNoP -> SetFilter(OPFilter);
    /*-*/SDforDE -> RegisterPrimitive(PriDEandNoP);
    /*-*/SetSensitiveDetector(string, SDforDE);
}

void RHICFDetectorConstruction::SetSDForFrontCounter()
{
    /*-*/// SETTING FOR SENSITIVE DETECTOR OF FRONT COUNTER
    /*-*/SetDENOPScorer("FrontCounterSmallLogical");
    /*-*/SetDENOPScorer("FrontCounterLargeLogical");
}

void RHICFDetectorConstruction::SetSDForARM1() //FUNCTION FOR SETTING SD OF ARM1
{
    /*-*/// FOR DEPOSIT ENERGY
    /*-*/SetDEScorer("LargeW_PLLogical");
    /*-*/SetDEScorer("SmallW_PLLogical");
    /*-*/SetDEScorer("WHolder_1Logical");
    /*-*/SetDEScorer("WHolder_2Logical");
    /*-*/SetDEScorer("GSO_PLHolderLogical");
    /*-*/SetDEScorer("GSOBarHolderLogical");
    /*-*/SetDEScorer("AlFrame1Logical");
    /*-*/SetDEScorer("AlFrame2Logical");
    /*-*/SetDEScorer("LightGuideLargeLogical");
    /*-*/SetDEScorer("LightGuideSmallLogical");
    /*-*/// FOR DEPOSIT ENERGY & NUMBER OF PHOTON
    /*-*/SetDENOPScorer("LargeGSO_PLLogical");
    /*-*/SetDENOPScorer("SmallGSO_PLLogical");
    /*-*/SetDENOPScorer("GSORightLargeBarLogical");
    /*-*/SetDENOPScorer("GSORightSmallBarLogical");
    /*-*/SetDENOPScorer("GSOLeftLargeBarLogical");
    /*-*/SetDENOPScorer("GSOLeftSmallBarLogical");
}

void RHICFDetectorConstruction::SetSDForSTARZDC() //FUNCTION FOR SETTING SD FOR STARZDC
{
    /*-*/// FOR DEPOSIT ENERGY
    /*-*/SetDEScorer("W_PL_1Logical");
    /*-*/SetDEScorer("W_PL_2Logical");
    /*-*/SetDEScorer("W_PL_3Logical");
    /*-*/SetDEScorer("I_PLLogical");
    /*-*/// FOR DEPOSIT ENERGY & NUMBER OF PHOTON
    /*-*/SetDENOPScorer("GAPF_1Logical");
    /*-*/SetDENOPScorer("GAPF_2Logical");
    /*-*/SetDENOPScorer("GAPF_3Logical");
    /*-*/SetDENOPScorer("SMDHLogical");
    /*-*/SetDENOPScorer("SMDVLogical");
}
