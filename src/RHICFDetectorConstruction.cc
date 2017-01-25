// Edited by Junsang Park. 2014.12.10
#include "RHICFDetectorConstruction.hh"
#include "MagneticField.hh"
#include "RHICFMaterials.hh"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "G4PVReplica.hh"
#include "G4VSolid.hh"
#include "G4Polyhedra.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
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
#include "G4OpBoundaryProcess.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4UserLimits.hh"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using namespace CLHEP;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
G4ThreadLocal MagneticField* RHICFDetectorConstruction::fMagneticField = 0;
G4ThreadLocal G4FieldManager* RHICFDetectorConstruction::fFieldMgr = 0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const G4String ssdname[2][4] = {{"SSDy0","SSDy1","SSDy2","SSDy3"},{"SSDx0","SSDx1","SSDx2","SSDx3"}};
static const G4String ssdLVname[2][4] = {{"SSDyLV0","SSDyLV1","SSDyLV2","SSDyLV3"},{"SSDxLV0","SSDxLV1","SSDxLV2","SSDxLV3"}};
static const G4String ssdsiSname[2]={"ssdsiYS","ssdsiXS"};
// static const G4String ssdsiLVname[2][4]={{"ssdsiYLV0","ssdsiYLV1","ssdsiYLV2","ssdsiYLV3"},{"ssdsiXLV0","ssdsiXLV1","ssdsiXLV2","ssdsiXLV3"}};
static const G4String ssdsiLVname[2] = {"ssdsiYLV","ssdsiXLV"};
static const G4String towSname[2][7] = {{"TL0","TL1","TL2","TL3","TL4","TL5","TL6"}, {"TS0","TS1","TS2","TS3","TS4","TS5","TS6"}};
static const G4String towname[2][7] = {{"TLtow0","TLtow1","TLtow2","TLtow3","TLtow4","TLtow5","TLtow6"},{"TStow0","TStow1","TStow2","TStow3","TStow4","TStow5","TStow6"}};
static const G4String towLVname[2][7] =  {{"TLtowLV0","TLtowLV1","TLtowLV2","TLtowLV3","TLtowLV4","TLtowLV5","TLtowLV6"},{"TStowLV0","TStowLV1","TStowLV2","TStowLV3","TStowLV4","TStowLV5","TStowLV6"}};
static const G4String laySname[2] = {"TLLayer","TSLayer"};
////////////
static const G4String layname[2][16] = {{"TLLayer0","TLLayer1","TLLayer2","TLLayer3","TLLayer4","TLLayer5","TLLayer6","TLLayer7","TLLayer8","TLLayer9","TLLayer10","TLLayer11","TLLayer12","TLLayer13","TLLayer14","TLLayer15"},
    {"TSLayer0","TSLayer1","TSLayer2","TSLayer3","TSLayer4","TSLayer5","TSLayer6","TSLayer7","TSLayer8","TSLayer9","TSLayer10","TSLayer11","TSLayer12","TSLayer13","TSLayer14","TSLayer15"}};
///////////
static const G4String layLVname[2][16] = {{"TLLayerLV0","TLLayerLV1","TLLayerLV2","TLLayerLV3","TLLayerLV4","TLLayerLV5","TLLayerLV6","TLLayerLV7","TLLayerLV8","TLLayerLV9","TLLayerLV10","TLLayerLV11","TLLayerLV12","TLLayerLV13","TLLayerLV14","TLLayerLV15"}, 
    {"TSLayerLV0","TSLayerLV1","TSLayerLV2","TSLayerLV3","TSLayerLV4","TSLayerLV5","TSLayerLV6","TSLayerLV7","TSLayerLV8","TSLayerLV9","TSLayerLV10","TSLayerLV11","TSLayerLV12","TSLayerLV13","TSLayerLV14","TSLayerLV15"}};
////////////
static const G4String gsoSname[2] = {"TLGSO","TSGSO"};
////////////////
static const G4String gsoname[2][16] ={{"TLGSO0","TLGSO1","TLGSO2","TLGSO3","TLGSO4","TLGSO5","TLGSO6","TLGSO7","TLGSO8","TLGSO9","TLGSO10","TLGSO11","TLGSO12","TLGSO13","TLGSO14","TLGSO15"},
    {"TSGSO0","TSGSO1","TSGSO2","TSGSO3","TSGSO4","TSGSO5","TSGSO6","TSGSO7","TSGSO8","TSGSO9","TSGSO10","TSGSO11","TSGSO12","TSGSO13","TSGSO14","TSGSO15"}};
//////////
static const G4String gsoLVname[2][16] ={{"TLGSOLV0","TLGSOLV1","TLGSOLV2","TLGSOLV3","TLGSOLV4","TLGSOLV5","TLGSOLV6","TLGSOLV7","TLGSOLV8","TLGSOLV9","TLGSOLV10","TLGSOLV11","TLGSOLV12","TLGSOLV13","TLGSOLV14","TLGSOLV15"},
    {"TSGSOLV0","TSGSOLV1","TSGSOLV2","TSGSOLV3","TSGSOLV4","TSGSOLV5","TSGSOLV6","TSGSOLV7","TSGSOLV8","TSGSOLV9","TSGSOLV10","TSGSOLV11","TSGSOLV12","TSGSOLV13","TSGSOLV14","TSGSOLV15"}};
static const G4String acrylSname[2] = {"TLacrylS","TSacrylS"};
static const G4String acrylname[2][32] = {{"TLacryl0","TLacryl1","TLacryl2","TLacryl3","TLacryl4","TLacryl5","TLacryl6","TLacryl7","TLacryl8","TLacryl9","TLacryl10","TLacryl11","TLacryl12","TLacryl13","TLacryl14","TLacryl15","TLacryl16","TLacryl17","TLacryl18","TLacryl19","TLacryl20","TLacryl21","TLacryl22","TLacryl23","TLacryl24","TLacryl25","TLacryl26","TLacryl27","TLacryl28","TLacryl29","TLacryl30","TLacryl31"},
    {"TSacryl0","TSacryl1","TSacryl2","TSacryl3","TSacryl4","TSacryl5","TSacryl6","TSacryl7","TSacryl8","TSacryl9","TSacryl10","TSacryl11","TSacryl12","TSacryl13","TSacryl14","TSacryl15","TSacryl16","TSacryl17","TSacryl18","TSacryl19","TSacryl20","TSacryl21","TSacryl22","TSacryl23","TSacryl24","TSacryl25","TSacryl26","TSacryl27","TSacryl28","TSacryl29","TSacryl30","TSacryl31"}};
static const G4String acrylLVname[2][32] = {{"TLacrylLV0","TLacrylLV1","TLacrylLV2","TLacrylLV3","TLacrylLV4","TLacrylLV5","TLacrylLV6","TLacrylLV7","TLacrylLV8","TLacrylLV9","TLacrylLV10","TLacrylLV11","TLacrylLV12","TLacrylLV13","TLacrylLV14","TLacrylLV15","TLacrylLV16","TLacrylLV17","TLacrylLV18","TLacrylLV19","TLacrylLV20","TLacrylLV21","TLacrylLV22","TLacrylLV23","TLacrylLV24","TLacrylLV25","TLacrylLV26","TLacrylLV27","TLacrylLV28","TLacrylLV29","TLacrylLV30","TLacrylLV31"},
    {"TSacrylLV0","TSacrylLV1","TSacrylLV2","TSacrylLV3","TSacrylLV4","TSacrylLV5","TSacrylLV6","TSacrylLV7","TSacrylLV8","TSacrylLV9","TSacrylLV10","TSacrylLV11","TSacrylLV12","TSacrylLV13","TSacrylLV14","TSacrylLV15","TSacrylLV16","TSacrylLV17","TSacrylLV18","TSacrylLV19","TSacrylLV20","TSacrylLV21","TSacrylLV22","TSacrylLV23","TSacrylLV24","TSacrylLV25","TSacrylLV26","TSacrylLV27","TSacrylLV28","TSacrylLV29","TSacrylLV30","TSacrylLV31"}};
static const G4String wSname[2][22] = {{"TLwS0","TLwS1","TLwS2","TLwS3","TLwS4","TLwS5","TLwS6","TLwS7","TLwS8","TLwS9","TLwS10","TLwS11","TLwS12","TLwS13","TLwS14","TLwS15","TLwS16","TLwS17","TLwS18","TLwS19","TLwS20","TLwS21"},
    {"TSwS0","TSwS1","TSwS2","TSwS3","TSwS4","TSwS5","TSwS6","TSwS7","TSwS8","TSwS9","TSwS10","TSwS11","TSwS12","TSwS13","TSwS14","TSwS15","TSwS16","TSwS17","TSwS18","TSwS19","TSwS20","TSwS21"}};
static const G4String wname[2][22] = {{"TLw0","TLw1","TLw2","TLw3","TLw4","TLw5","TLw6","TLw7","TLw8","TLw9","TLw10","TLw11","TLw12","TLw13","TLw14","TLw15","TLw16","TLw17","TLw18","TLw19","TLw20","TLw21"},
    {"TSw0","TSw1","TSw2","TSw3","TSw4","TSw5","TSw6","TSw7","TSw8","TSw9","TSw10","TSw11","TSw12","TSw13","TSw14","TSw15","TSw16","TSw17","TSw18","TSw19","TSw20","TSw21"}};
static const G4String wLVname[2][22] = {{"TLwLV0","TLwLV1","TLwLV2","TLwLV3","TLwLV4","TLwLV5","TLwLV6","TLwLV7","TLwLV8","TLwLV9","TLwLV10","TLwLV11","TLwLV12","TLwLV13","TLwLV14","TLwLV15","TLwLV16","TLwLV17","TLwLV18","TLwLV19","TLwLV20","TLwLV21"},
    {"TSwLV0","TSwLV1","TSwLV2","TSwLV3","TSwLV4","TSwLV5","TSwLV6","TSwLV7","TSwLV8","TSwLV9","TSwLV10","TSwLV11","TSwLV12","TSwLV13","TSwLV14","TSwLV15","TSwLV16","TSwLV17","TSwLV18","TSwLV19","TSwLV20","TSwLV21"}};
static const G4String gtenSname[7][7] = {
    {"gtenS0p0","gtenS1p0","gtenS2p0","gtenS3p0","gtenS4p0","gtenS5p0","gtenS6p0"},
    {"gtenS0p1","gtenS1p1","gtenS2p1","gtenS3p1","gtenS4p1","gtenS5p1","gtenS6p1"},
    {"gtenS0p2","gtenS1p2","gtenS2p2","gtenS3p2","gtenS4p2","gtenS5p2","gtenS6p2"},
    {"gtenS0p3","gtenS1p3","gtenS2p3","gtenS3p3","gtenS4p3","gtenS5p3","gtenS6p3"},
    {"gtenS0p4","gtenS1p4","gtenS2p4","gtenS3p4","gtenS4p4","gtenS5p4","gtenS6p4"},
    {"gtenS0p5","gtenS1p5","gtenS2p5","gtenS3p5","gtenS4p5","gtenS5p5","gtenS6p5"},
    {"gtenS0p6","gtenS1p6","gtenS2p6","gtenS3p6","gtenS4p6","gtenS5p6","gtenS6p6"},
};
static const G4String gtenLVname[7][7] = {
    {"gtenLV0p0","gtenLV1p0","gtenLV2p0","gtenLV3p0","gtenLV4p0","gtenLV5p0","gtenLV6p0"},
    {"gtenLV0p1","gtenLV1p1","gtenLV2p1","gtenLV3p1","gtenLV4p1","gtenLV5p1","gtenLV6p1"},
    {"gtenLV0p2","gtenLV1p2","gtenLV2p2","gtenLV3p2","gtenLV4p2","gtenLV5p2","gtenLV6p2"},
    {"gtenLV0p3","gtenLV1p3","gtenLV2p3","gtenLV3p3","gtenLV4p3","gtenLV5p3","gtenLV6p3"},
    {"gtenLV0p4","gtenLV1p4","gtenLV2p4","gtenLV3p4","gtenLV4p4","gtenLV5p4","gtenLV6p4"},
    {"gtenLV0p5","gtenLV1p5","gtenLV2p5","gtenLV3p5","gtenLV4p5","gtenLV5p5","gtenLV6p5"},
    {"gtenLV0p6","gtenLV1p6","gtenLV2p6","gtenLV3p6","gtenLV4p6","gtenLV5p6","gtenLV6p6"},
};
static const G4String gtenname[7][7] = {
    {"gten0p0","gten1p0","gten2p0","gten3p0","gten4p0","gten5p0","gten6p0"},
    {"gten0p1","gten1p1","gten2p1","gten3p1","gten4p1","gten5p1","gten6p1"},
    {"gten0p2","gten1p2","gten2p2","gten3p2","gten4p2","gten5p2","gten6p2"},
    {"gten0p3","gten1p3","gten2p3","gten3p3","gten4p3","gten5p3","gten6p3"},
    {"gten0p4","gten1p4","gten2p4","gten3p4","gten4p4","gten5p4","gten6p4"},
    {"gten0p5","gten1p5","gten2p5","gten3p5","gten4p5","gten5p5","gten6p5"},
    {"gten0p6","gten1p6","gten2p6","gten3p6","gten4p6","gten5p6","gten6p6"},
};
static const G4String gholeSname[2][7] = {{"gholeSdp0","gholeSdp1","gholeSdp2","gholeSdp3","gholeSdp4","gholeSdp5","gholeSdp6"},
    {"gholeSup0","gholeSup1","gholeSup2","gholeSup3","gholeSup4","gholeSup5","gholeSup6"}};
static const G4String gholeLVname[2][4][7] = {{{"gholeLVd0p0","gholeLVd0p1","gholeLVd0p2","gholeLVd0p3","gholeLVd0p4","gholeLVd0p5","gholeLVd0p6"},{"gholeLVd1p0","gholeLVd1p1","gholeLVd1p2","gholeLVd1p3","gholeLVd1p4","gholeLVd1p5","gholeLVd1p6"},{"gholeLVd2p0","gholeLVd2p1","gholeLVd2p2","gholeLVd2p3","gholeLVd2p4","gholeLVd2p5","gholeLVd2p6"},{"gholeLVd3p0","gholeLVd3p1","gholeLVd3p2","gholeLVd3p3","gholeLVd3p4","gholeLVd3p5","gholeLVd3p6"}},
    {{"gholeLVu0p0","gholeLVu0p1","gholeLVu0p2","gholeLVu0p3","gholeLVu0p4","gholeLVu0p5","gholeLVu0p6"},{"gholeLVu1p0","gholeLVu1p1","gholeLVu1p2","gholeLVu1p3","gholeLVu1p4","gholeLVu1p5","gholeLVu1p6"},{"gholeLVu2p0","gholeLVu2p1","gholeLVu2p2","gholeLVu2p3","gholeLVu2p4","gholeLVu2p5","gholeLVu2p6"},{"gholeLVu3p0","gholeLVu3p1","gholeLVu3p2","gholeLVu3p3","gholeLVu3p4","gholeLVu3p5","gholeLVu3p6"}}};
static const G4String gholename[2][4][7] = {{{"gholed0p0","gholed0p1","gholed0p2","gholed0p3","gholed0p4","gholed0p5","gholed0p6"},{"gholed1p0","gholed1p1","gholed1p2","gholed1p3","gholed1p4","gholed1p5","gholed1p6"},{"gholed2p0","gholed2p1","gholed2p2","gholed2p3","gholed2p4","gholed2p5","gholed2p6"},{"gholed3p0","gholed3p1","gholed3p2","gholed3p3","gholed3p4","gholed3p5","gholed3p6"}},
    {{"gholeu0p0","gholeu0p1","gholeu0p2","gholeu0p3","gholeu0p4","gholeu0p5","gholeu0p6"},{"gholeu1p0","gholeu1p1","gholeu1p2","gholeu1p3","gholeu1p4","gholeu1p5","gholeu1p6"},{"gholeu2p0","gholeu2p1","gholeu2p2","gholeu2p3","gholeu2p4","gholeu2p5","gholeu2p6"},{"gholeu3p0","gholeu3p1","gholeu3p2","gholeu3p3","gholeu3p4","gholeu3p5","gholeu3p6"}}};
static const G4String psedepname[2][16] = {{"Edep0","Edep1","Edep2","Edep3","Edep4","Edep5","Edep6","Edep7","Edep8","Edep9","Edep10","Edep11","Edep12","Edep13","Edep14","Edep15"},
    {"Edep16","Edep17","Edep18","Edep19","Edep20","Edep21","Edep22","Edep23","Edep24","Edep25","Edep26","Edep27","Edep28","Edep29","Edep30","Edep31"}};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RHICFDetectorConstruction::RHICFDetectorConstruction ( ):G4VUserDetectorConstruction(),  fVisAttributes()
                                                         ///////////////////////////////////////////////////////////////////////////////
{

    DefineDimension();
}


///////////////////////////////////////////////////////////////////////////////
RHICFDetectorConstruction::~RHICFDetectorConstruction ( )
    ///////////////////////////////////////////////////////////////////////////////
{
    delete fMaterials;
}

///////////////////////////////////////////////////////////////////////////////
G4VPhysicalVolume* RHICFDetectorConstruction::Construct ( )
    ///////////////////////////////////////////////////////////////////////////////
{
    G4double lhollowThickness[2][16] = {{0.538*mm,0.63*mm,0.871*mm,0.63*mm,0.597*mm,0.481*mm,0.641*mm,0.486*mm,0.438*mm,0.45*mm,0.535*mm,0.1*mm,0.492*mm,0.581*mm,0.616*mm,0.1*mm},{0.538*mm,0.63*mm,0.748*mm,0.63*mm,0.597*mm,0.382*mm,0.641*mm,0.486*mm,0.422*mm,0.45*mm,0.535*mm,0.1*mm,0.492*mm,0.581*mm,0.616*mm,0.1*mm}};  // measured
    G4double wThickness[2][22] = {{7.027*mm,7.043*mm,7.035*mm,7.021*mm,7.029*mm,7.012*mm,7.031*mm,7.016*mm,7.023*mm,7.014*mm,7.051*mm,7.033*mm,7.004*mm,6.998*mm,6.98*mm,6.991*mm,6.985*mm,6.99*mm,7.082*mm,7.014*mm,7.032*mm,7.031*mm},{7.09*mm,7.09*mm,7.09*mm,7.11*mm,7.08*mm,7.09*mm,7.09*mm,7.09*mm,7.*mm,7.06*mm,7.06*mm,7.04*mm,7.02*mm,7.04*mm,7.11*mm,7.03*mm,7.01*mm,7.*mm,6.99*mm,7.14*mm,6.99*mm,7.*mm}};  //new
    G4double whollowThickness[2][22] = {{0.121*mm,0.102*mm,0.1*mm,0.001*mm,0.001*mm,0.1*mm,0.001*mm,0.043*mm,0.1*mm,0.272*mm,0.001*mm,0.001*mm,0.565*mm,0.178*mm,0.111*mm,0.1*mm,0.083*mm,0.001*mm,0.086*mm,0.001*mm,0.558*mm,0.006*mm},{0.058*mm,0.055*mm,0.1*mm,0.001*mm,0.001*mm,0.1*mm,0.001*mm,0.001*mm,0.1*mm,0.226*mm,0.001*mm,0.001*mm,0.546*mm,0.136*mm,0.001*mm,0.1*mm,0.094*mm,0.001*mm,0.178*mm,0.001*mm,0.565*mm,0.037*mm}};  //new


    G4double layerSizeXY[2] = {32.*mm, 25.*mm};
    G4double towSizeXY[2] = {TL*mm, TS*mm};
    G4double wSizeX[2][22] = {{32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.05*mm,32.15*mm,32.1*mm,32.1*mm,32.1*mm,32.15*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.2*mm,32.1*mm,32.15*mm,32.1*mm},{25.1*mm,25.*mm,25.2*mm,25.2*mm,25.*mm,25.2*mm,25.2*mm,25.*mm,25.2*mm,25.2*mm,25.*mm,25.2*mm,25.*mm,25.2*mm,25.2*mm,25.2*mm,25.1*mm,25.*mm,25.*mm,25.2*mm,25.2*mm,25.*mm}};  //new
    G4double wSizeY[2][22] = {{32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.15*mm,32.2*mm,32.15*mm,32.1*mm,32.05*mm},{25.*mm,25.*mm,25.*mm,25.*mm,25.*mm,25.1*mm,25.1*mm,25.*mm,25.1*mm,25.*mm,24.95*mm,25.*mm,24.9*mm,25.1*mm,25.*mm,25.*mm,24.95*mm,25.*mm,25.*mm,25.*mm,25.1*mm,25.*mm}}; //new

    G4int nolet[7] = {2,3,3,3,2,2,1};//noflayer for each tower
    G4int nowet[7] = {3,3,3,3,4,4,2};//nofw for each tower

    G4double gtenSizeX[7] = {80.5*mm, 18.3*mm, 25.2*mm, 1.8*mm, 32.2*mm, 32.2*mm, 3.*mm};
    G4double gtenSizeY[7] = {16.88*mm, 103.12*mm, 77.92*mm, 103.12*mm, 27.*mm, 43.92*mm, 103.12*mm};


    G4double towThickness[7] = {29.52*mm,32.77*mm,32.03*mm,32.39*mm,35.76*mm,35.94*mm,17.73*mm}; //new

    // G4double wepicsposX[2] = {0.2*mm,0.*mm};//後から細かく配列化
    G4double wepicsposX[2][22] = {{0.1*mm,0.1*mm,0.1*mm,0.1*mm,0.15*mm,0.05*mm,0.1*mm,0.1*mm,0.1*mm,0.05*mm,0.1*mm,0.1*mm,0.1*mm,0.1*mm,0.1*mm,0.1*mm,0.1*mm,0.1*mm,0.*mm,0.1*mm,0.05*mm,0.1*mm},{0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm}};
    // G4double wepicsposY[2] = {0.*mm,0.2*mm};//後から細かく配列化
    G4double wepicsposY[2][22] = {{0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm},{0.2*mm,0.2*mm,0.2*mm,0.2*mm,0.2*mm,0.1*mm,0.1*mm,0.2*mm,0.1*mm,0.2*mm,0.25*mm,0.2*mm,0.3*mm,0.1*mm,0.2*mm,0.2*mm,0.25*mm,0.2*mm,0.2*mm,0.2*mm,0.1*mm,0.2*mm}};

    G4double towposX[2] = {-Arm2SizeX/2 + gtenSizeX[6] + towSizeXY[0]/2, -Arm2SizeX/2 + gtenSizeX[6] + gtenSizeX[4] + gtenSizeX[3] + towSizeXY[1]/2};
    G4double towposY[2] = {-Arm2SizeY/2 + gtenSizeY[0] + gtenSizeY[4] + towSizeXY[0]/2, -Arm2SizeY/2 + gtenSizeY[0] + towSizeXY[1]/2};
    G4double layerposX[2] = {towSizeXY[0]/2-layerSizeXY[0]/2,layerSizeXY[1]/2-towSizeXY[1]/2};
    G4double layerposY[2] = {layerSizeXY[0]/2-towSizeXY[0]/2,towSizeXY[1]/2-layerSizeXY[1]/2};


    fMaterials          = RHICFMaterials::GetInstance();

    G4LogicalSkinSurface::CleanSurfaceTable();
    G4LogicalBorderSurface::CleanSurfaceTable();
    G4GeometryManager::GetInstance()        
        -> OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()    
        -> Clean();
    G4LogicalVolumeStore::GetInstance()     
        -> Clean();
    G4SolidStore::GetInstance()             
        -> Clean();
    // Option to switch on/off checking of volumes overlaps
    // 
    //
    checkOverlaps           = false;

    // geometries --------------------------------------------------------------
    // Define World Volume
    fWorldSolid             = new G4Box("WorldSolid", 10*worX*cm, 10*worY*cm, 10*worZ*cm);
    fWorldLogical           = new G4LogicalVolume(fWorldSolid, FindMaterial("G4_AIR"), "WorldLogical");
    fWorldPhysical          = new G4PVPlacement(0, G4ThreeVector(), fWorldLogical, "WorldPhysical", 0, false, checkOverlaps);

    visAttributes           = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    visAttributes           -> SetVisibility(false);
    fWorldLogical           -> SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);


    // Define Rotation matrix
    //
    HODORotation            = new G4RotationMatrix();
    HODORotation            -> rotateZ(90*deg);
    INVERSERotation         = new G4RotationMatrix();
    INVERSERotation         -> rotateY(180*deg);
    MagRotation             = new G4RotationMatrix();
    MagRotation             -> rotateX(90.*deg);
    PipeRotation1           = new G4RotationMatrix();
    PipeRotation1           -> rotateY(1.074*deg);
    PipeRotation2           = new G4RotationMatrix();
    PipeRotation2           -> rotateY(-1.074*deg);
    fNonRotation            = new G4RotationMatrix();
    fNonRotation            -> rotateX(twopi);
    fQPhi                   = new G4RotationMatrix();
    fQPhi                   -> rotateX(twopi/8);
    GAPFRotation            = new G4RotationMatrix();
    GAPFRotation            -> rotateX(27*twopi/72);



    // Define 'WCNT'
    // WCNT is space which contains 'ZDC', 'RCSC', 'FCSC'
    //
    // Structure of 'WCNT'
    // 
    // 'WCNT'-----Nmods of 'ZDC'----26 of 'GAPF's----200 of 'FIBR's
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
    G4ThreeVector a         = G4ThreeVector(towposX[1]-Arm2SizeX/2 + 2*gtenSizeX[6] + gtenSizeX[4] + gtenSizeX[3],-towposY[1]-12.5*mm+12.5*mm,0);
    //G4ThreeVector a         = G4ThreeVector(towposX[1]-4.5*mm,-towposY[1]-12.5*mm+25*mm,0);
    //G4ThreeVector a         = G4ThreeVector(towposX[1]-4.5*mm,-towposY[1]-12.5*mm+42.5*mm,0);
    //G4ThreeVector a         = G4ThreeVector(towposX[1]-4.5*mm,-towposY[1]-12.5*mm+57.5*mm,0);
    //G4ThreeVector a         = G4ThreeVector(towposX[1]-4.5*mm,-towposY[1]-12.5*mm+72.5*mm,0);

    //G4ThreeVector b         = G4ThreeVector(0,-4.05/sqrt(2.)*mm,-1860*cm);
    G4ThreeVector b         = G4ThreeVector(0,-4.05/sqrt(2.)*mm,-62*cm);

    //HODO                    = HODOSCOPE(fWorldPhysical,INVERSERotation);
    LOCALPOLINSTALL         = WCNT(fWorldPhysical, b, fNonRotation);

    //BBCINSTALL              = BBC(fWorldPhysical, a, fNonRotation);

    //PIPEINSTALL             = PIPE();

    //Junsang****ARM2INSTALL             = ARM2(fWorldPhysical, a, INVERSERotation);

    return                  fWorldPhysical;

}


///////////////////////////////////////////////////////////////////////////////
void RHICFDetectorConstruction::DefineDimension()
    ///////////////////////////////////////////////////////////////////////////////
{
    smdHeight           = smdPar[2];
    sin                 = std::sqrt(2.0)/2.0;
    cos                 = std::sqrt(2.0)/2.0;
    tan                 = 1.0;
    Nlay                = 26;
    //Nlay                = 1;

    iplPar[0]           = 5.0;
    iplPar[1]           = 0.025;
    iplPar[2]           = 20.0;

    wplPar[0]           = 5.0;
    wplPar[1]           = 0.213;
    wplPar[2]           = 9.35;

    gapper[0]           = 5.0;
    gapper[1]           = 0.09;
    gapper[2]           = 20.0;

    fcpPar[0]           = 5.080;
    fcpPar[1]           = 12.065;
    fcpPar[2]           = 0.3175;

    rcPar[0]            = 4.445;
    rcPar[1]            = 10.795;
    rcPar[2]            = 0.6350;

    wcntPar[0]          = 6.0;
    wcntPar[1]          = 25.0;
    wcntPar[2]          = 70.0;

    fibPar[0]           = 0.0;
    fibPar[1]           = 0.025;
    fibPar[2]           = 20.0;

    hodoPar[0]          = 40.0;
    hodoPar[1]          = 40.0;
    hodoPar[2]          = 1.0;

    scinPar[0]          = 4.075;
    scinPar[1]          = 40.0;
    scinPar[2]          = 0.5;

    smdPar[0]           = 5.5;
    smdPar[1]           = 0.81;
    smdPar[2]           = 9.0;

    smdhPar[0]          = 5.25;
    smdhPar[1]          = 0.4;
    smdhPar[2]          = 1.0;

    smdvPar[0]          = 0.75;
    smdvPar[1]          = 0.4;
    smdvPar[2]          = 8.0;

    alPar[0]            = 12.0;
    alPar[1]            = 3.0;
    alPar[2]            = 40.0;

    fePar[0]            = 5.0;
    fePar[1]            = 1.0;
    fePar[2]            = 40.0;











    Lmod                = (2.0*iplPar[1] + (Nlay + 1)*wplPar[1] + Nlay*gapper[1])/sin;

    worX                = 50*cm;
    worY                = 50*cm;
    worZ                = 50*cm;


    pipeOutr            = 5*cm;
    pipeInr             = 4.5*cm;
    pipeLength          = 20*cm;
    Nmod                = 3;
    zdcPar[0]           = 5.0;
    zdcPar[1]           = (gapper[2]*cos + gapper[1]*sin);
    zdcPar[2]           = (zdcPar[1]/tan + Lmod);
    zdcPar1             = zdcPar[1]*1.2;      
    zdcPar2             = zdcPar[2]*1.1;

    // Parameters for BBC

    kBBAbsorb[0]        = 5.5;
    kBBAbsorb[1]        = 14.5;
    kBBAbsorb[2]        = 0.01;

    kBBAttach[0]        = 0.0;
    kBBAttach[1]        = 360;
    kBBAttach[2]        = 6.0;
    kBBAttach[3]        = 2.0;
    kBBAttach[4]        = -0.5;
    kBBAttach[5]        = 0.2;
    kBBAttach[6]        = 1.4;
    kBBAttach[7]        = 0.5;
    kBBAttach[8]        = 0.2;
    kBBAttach[9]        = 1.4;

    kBBBackBD[0]        = 5.5;
    kBBBackBD[1]        = 15.0;
    kBBBackBD[2]        = 0.5;

    kBBBreede[0]        = 0.0;
    kBBBreede[1]        = 1.2;
    kBBBreede[2]        = 1.95;

    kBBCovert           = 0.2;

    kBBFrontb[0]        = 5.5;
    kBBFrontb[1]        = 15.0;
    kBBFrontb[2]        = 0.5;

    kBBPMTSiz[0]        = 1.09;
    kBBPMTSiz[1]        = 1.29;
    kBBPMTSiz[2]        = 2.2;

    kBBQuartz[0]        = 0.0;
    kBBQuartz[1]        = 360;
    kBBQuartz[2]        = 6.0;
    kBBQuartz[3]        = 2.0;
    kBBQuartz[4]        = -1.5;
    kBBQuartz[5]        = 0.0;
    kBBQuartz[6]        = 1.27;
    kBBQuartz[7]        = 1.5;
    kBBQuartz[8]        = 0.0;
    kBBQuartz[9]        = 1.27;

    kBBShithi           = 0.1;

    kBBSpacin           = 0.3;

    kBBStruct[0]        = 5.0;
    kBBStruct[1]        = 5.5;
    kBBStruct[2]        = 12.5;

    kBBZPosit[0]        = 144.35;
    kBBZPosit[1]        = -144.35;

    kBBDetect[0]        = kBBQuartz[0];
    kBBDetect[1]        = kBBQuartz[1];
    kBBDetect[2]        = kBBQuartz[2];
    kBBDetect[3]        = kBBQuartz[3];
    kBBDetect[4]        = kBBAttach[4]+kBBQuartz[4]-kBBPMTSiz[2]-kBBBreede[2];
    kBBDetect[5]        = kBBQuartz[5];
    kBBDetect[6]        = kBBAttach[6];
    kBBDetect[7]        = kBBAttach[7]+kBBQuartz[7]+kBBPMTSiz[2]+kBBBreede[2];
    kBBDetect[8]        = kBBQuartz[8];
    kBBDetect[9]        = kBBAttach[9];

    kBBBCover[0]        = kBBFrontb[1];
    kBBBCover[1]        = kBBBCover[0]+kBBCovert;
    kBBBCover[2]        = kBBStruct[2];

    kBBMother[0]        = kBBStruct[0]; 
    kBBMother[1]        = kBBBCover[1];
    kBBMother[2]        = kBBStruct[2];

    kBBShield[0]        = kBBQuartz[0];
    kBBShield[1]        = kBBQuartz[1];
    kBBShield[2]        = kBBQuartz[2];
    kBBShield[3]        = kBBQuartz[3];
    kBBShield[4]        = kBBQuartz[4]-kBBPMTSiz[2]-kBBBreede[2];
    kBBShield[5]        = kBBAttach[6]-kBBShithi;
    kBBShield[6]        = kBBAttach[6];
    kBBShield[7]        = kBBQuartz[7]+kBBPMTSiz[2]+kBBBreede[2];
    kBBShield[8]        = kBBAttach[9]-kBBShithi;
    kBBShield[9]        = kBBAttach[9];

    kYSTP               = kBBQuartz[6]+kBBSpacin*0.5;
    kXSTP               = kYSTP*std::sqrt(3.0);

    kMrow               = (int)(kBBFrontb[1]/kYSTP)+1;
    kMcol               = (int)(kBBFrontb[1]/kXSTP)+1;

    kRmax               = kBBFrontb[1]-kBBDetect[6]*2./sqrt(3);
    kRmin               = kBBFrontb[0]-kBBDetect[6]*2./sqrt(3);
    kRRMA               = kRmax*kRmax;
    kRRMI               = kRmin*kRmin;

    kMaxPMT             = 100;


    // Arm2 Geometry parameters
    //

    TL                  = 32.2;
    TS                  = 25.2;
    SSDX                = 80.5;
    SSDY                = 120.;
    noflayer            = 16;
    nofssdset           = 4;
    gsoThickness        = 1.*mm;
    acrylThickness      = 1.*mm;
    ssdsiThickness      = 0.285*mm;
    ssdalThickness      = 0.5*mm;
    ssdacrylThickness   = 0.35*mm;
    firstThickness      = 1*mm;
    Arm2SizeX           = SSDX*mm;
    Arm2SizeY           = SSDY*mm;
    firstSizeY          = 100.*mm;

    gapThickness        = 0.001*mm;
    layerThickness      = acrylThickness*2 + gsoThickness + gapThickness*2; 
    ssdThickness        = ssdsiThickness*2 + ssdalThickness*2 + ssdacrylThickness*2;
    ssdxyThickness      = ssdsiThickness + ssdalThickness + ssdacrylThickness;
    Arm2Thickness       = 226.22*mm; 


    lcur_num            = 0;
    wcur_num            = 0;




}


///////////////////////////////////////////////////////////////////////////////
void RHICFDetectorConstruction::ConstructSDandField()
    ///////////////////////////////////////////////////////////////////////////////
{


    G4SDManager::GetSDMpointer() -> SetVerboseLevel(1);

    G4String detName;

    G4String calName[8] = {"W_PL", "SMDH", "SMDV", "I_PL", "GAP"}; 
    
//Junsang****
//Junsang****
//Junsang****
    //Junsang****for(G4int j=0; j<5; ++j)
    //Junsang****{
//Junsang****
        //Junsang****G4String detName = calName[j];
//Junsang****
        //Junsang****if(j==0)
        for( G4int i=0; i<3; i++)
        {


            detName = calName[0];

            if(i==0)
            {
                detName+="_1";
            }else if(i==1)
            {
                detName+="_2";
            }else if(i==2)
            {
                detName+="_3";
            }
        }

        G4MultiFunctionalDetector* det = new G4MultiFunctionalDetector(detName);

        G4VPrimitiveScorer* primitive;
        primitive = new G4PSEnergyDeposit("DE",1);
        det -> RegisterPrimitive(primitive);

        detName += "Logical";


        SetSensitiveDetector(detName, det);
    //Junsang****}









    // Set Magnetic Field
    //
    fMagneticField          = new MagneticField();
    fFieldMgr               = new G4FieldManager();
    fFieldMgr               -> SetDetectorField(fMagneticField);
    fFieldMgr               -> CreateChordFinder(fMagneticField);
    //G4bool forceToAllDaughters = true;
    //fMagneticLogical -> SetFieldManager(fFieldMgr, forceToAllDaughters);

    G4AutoDelete::Register(fMagneticField);
    G4AutoDelete::Register(fFieldMgr);


    G4SDManager::GetSDMpointer() -> SetVerboseLevel(0);

}





///////////////////////////////////////////////////////////////////////////////
void RHICFDetectorConstruction::ConstructMaterials ( )
///////////////////////////////////////////////////////////////////////////////
{









}



//Implementation of deleting Materials
//
///////////////////////////////////////////////////////////////////////////////
void RHICFDetectorConstruction::DestructMaterials ( )
///////////////////////////////////////////////////////////////////////////////
{



}


G4Material* RHICFDetectorConstruction::FindMaterial(G4String name)
{

    G4Material* material                = G4Material::GetMaterial(name, true);
    return material;
}

///////////////////////////////////////////////////////////////////////////////
G4VPhysicalVolume* RHICFDetectorConstruction::HODOSCOPE(G4VPhysicalVolume* world_phys, G4RotationMatrix* mat)
///////////////////////////////////////////////////////////////////////////////
{

    // Define 'HODO'
    //
    fHODOSolid              = new G4Box("HODOSolid", hodoPar[0]*cm, hodoPar[1]*cm, hodoPar[2]*cm);
    fHODOLogical            = new G4LogicalVolume(fHODOSolid, FindMaterial("G4_Galactic"), "HODOLogical");
    fHODOPhysical           = new G4PVPlacement(mat, G4ThreeVector(0.0, 0.0, 185.0*cm), "HODOPhysical", fHODOLogical, world_phys, 0, false, checkOverlaps);


    // Define 'SCIN'
    //
    //
    fSCINSolid              = new G4Box("SCINSolid", scinPar[0]*cm, scinPar[1]*cm, scinPar[2]*cm);
    fSCINLogical            = new G4LogicalVolume(fSCINSolid, FindMaterial("G4_PLASTIC_SC_VINYLTOLUENE"), "SCINLogical");

    // Put 'SCIN' into 'HODO'
    //
    for(G4int i=1; i<5; i++)
    {
        pos                 = 7.4 + i*8.15 - 4.075;
        new G4PVPlacement(fNonRotation, G4ThreeVector(pos*cm, 0.0, 0.5*cm), fSCINLogical, "SCINPhysical", fHODOLogical, i, false, checkOverlaps);
        //pos = 7.4 + (i+4)*8.15 - 4.075;
        new G4PVPlacement(fNonRotation, G4ThreeVector(-pos*cm, 0.0, 0.5*cm), fSCINLogical, "SCINPhysical", fHODOLogical, i, false, checkOverlaps);
        //pos = 7.4 + (i+8)*8.15 - 4.075;
        new G4PVPlacement(HODORotation, G4ThreeVector(0.0, pos*cm, -0.5*cm), fSCINLogical, "SCINPhysical", fHODOLogical, i, false, checkOverlaps);
        //pos = 7.4 + (i+12)*8.15 - 4.075;
        new G4PVPlacement(HODORotation, G4ThreeVector(0.0, -pos*cm, -0.5*cm), fSCINLogical, "SCINPhysical", fHODOLogical, i, false, checkOverlaps);


        visAttributes       = new G4VisAttributes(G4Colour(0.0, 0.0, 0.1));
        visAttributes       -> SetVisibility(false);
        fHODOLogical        -> SetVisAttributes(visAttributes);
        fVisAttributes.push_back(visAttributes);

        visAttributes       = new G4VisAttributes(G4Colour(0.7, 0.4, 0.1));
        fSCINLogical        -> SetVisAttributes(visAttributes);
        fVisAttributes.push_back(visAttributes);
    }

    return fHODOPhysical;
}



///////////////////////////////////////////////////////////////////////////////
G4VPhysicalVolume* RHICFDetectorConstruction::WCNT(G4VPhysicalVolume* world_phys, G4ThreeVector vector, G4RotationMatrix* mat)
///////////////////////////////////////////////////////////////////////////////
{

    fWCNTSolid              = new G4Box("WCNTSolid", wcntPar[0]*cm, wcntPar[1]*cm, wcntPar[2]*3.6/5*cm);
    fWCNTLogical            = new G4LogicalVolume(fWCNTSolid, FindMaterial("G4_AIR"), "WCNTLogical");
    fWCNTPhysical           = new G4PVPlacement(mat, vector, "WCNTPhysical", fWCNTLogical,  world_phys, false, 0, checkOverlaps);


    //-----------------------------------------------------------------------------------------------------------
    // Define SolidVolume

    // Define 'ZDC'
    fZDCSolid               = new G4Box("ZDCSolid", zdcPar[0]*cm, zdcPar1*cm, zdcPar2*cm);
    // Define 'GAPF' : GAPF is gap which has fibers
    fGAPFSolid              = new G4Box("GAPFSolid", gapper[0]*cm, gapper[1]*cm, (gapper[2]+0.5)*cm);
    // Define 'Layer between Cladding and Gap'
    fGAPF1Solid             = new G4Box("GAPF1Solid", gapper[0]*cm, gapper[1]*cm, gapper[2]/2.*cm);
    // Define 'Half of PMMA fiber' : FIBR is fiber made of PMMA(H+O+C)
    fFIBRSolid              = new G4Tubs("FIBRSolid", fibPar[0]*cm, fibPar[1]*cm, fibPar[2]/2.*cm, 0, twopi);
    // Define 'Iron plate' : I_PL is plate which made of iron
    fI_PLSolid              = new G4Box("I_PLSolid", iplPar[0]*cm, iplPar[1]*cm, iplPar[2]*cm);
    // Define 'Half of PMMA fiber'
    fFIBSolid               = new G4Tubs("FIBSolid", fibPar[0]*cm, fibPar[1]*cm, gapper[2]/2.*cm, 0, twopi);
    // Define 'Tengsten plate' : W_PL is plate which made of tungsten
    fW_PLSolid              = new G4Box("W_PLSolid", wplPar[0]*cm, wplPar[2]*cm, wplPar[1]*cm);
    // Define 'Bar for eliminating optical photon propagating downward'
    fBlockerSolid           = new G4Box("BlockerSolid", gapper[0]*cm, fibPar[1]*cm, 0.005*cm);
    // Define 'SMD'
    fSMDSolid               = new G4Box("SMDSolid", smdPar[0]*cm, smdPar[1]*cm, smdPar[2]*cm);
    // Define 'SMDH'
    fSMDHSolid     = new G4Box("SMDHSolid", smdhPar[0]*cm, smdhPar[1]*cm, smdhPar[2]*cm);
    // Define 'FEPL'
    fFEPLSolid              = new G4Box("FEPLSolid", fePar[0]*cm, fePar[1]*cm, fePar[2]);
    // Define 'ALPL' 
    fALPLSolid              = new G4Box("ALPLSolid",alPar[0]*cm, alPar[1]*cm, alPar[2]*cm);
    // Define 'SMDV'
    fSMDVSolid              = new G4Box("SMDVSolid", smdvPar[0]*cm, smdvPar[1]*cm, smdvPar[2]*cm);


    //------------------------------------------------------------------------------------------------------------
    // Define LogicalVolume

    fZDCLogical             = new G4LogicalVolume(fZDCSolid, FindMaterial("G4_AIR"), "ZDCLogical");
    fGAPF_1Logical          = new G4LogicalVolume(fGAPFSolid, FindMaterial("G4_AIR"), "GAPF_1Logical");
    fGAPF_2Logical          = new G4LogicalVolume(fGAPFSolid, FindMaterial("G4_AIR"), "GAPF_2Logical");
    fGAPF_3Logical          = new G4LogicalVolume(fGAPFSolid, FindMaterial("G4_AIR"), "GAPF_3Logical");
    fGAPF1Logical           = new G4LogicalVolume(fGAPF1Solid, FindMaterial("G4_Galactic"), "GAPF1Logical");
    fGAPF2Logical           = new G4LogicalVolume(fGAPF2Solid, FindMaterial("FPMMA"), "GAPF2Logical");
    fI_PLLogical            = new G4LogicalVolume(fI_PLSolid, FindMaterial("G4_Fe"), "I_PLLogical");
    fFIBLogical             = new G4LogicalVolume(fFIBSolid, FindMaterial("PMMA"), "FIBLogical");
    fW_PL_1Logical          = new G4LogicalVolume(fW_PLSolid, FindMaterial("G4_W"), "W_PL_1Logical");
    fW_PL_2Logical          = new G4LogicalVolume(fW_PLSolid, FindMaterial("G4_W"), "W_PL_2Logical");
    fW_PL_3Logical          = new G4LogicalVolume(fW_PLSolid, FindMaterial("G4_W"), "W_PL_3Logical");
    fFIBRLogical            = new G4LogicalVolume(fFIBRSolid, FindMaterial("PMMA"), "FIBRLogical");
    fBlockerLogical         = new G4LogicalVolume(fBlockerSolid, FindMaterial("G4_Galactic"), "BlockerLogical");
    fSMDLogical             = new G4LogicalVolume(fSMDSolid, FindMaterial("G4_AIR"), "fSMDLogical");
    // Horizontal smd bar
    fSMDHLogical            = new G4LogicalVolume(fSMDHSolid, FindMaterial("G4_PLASTIC_SC_VINYLTOLUENE"), "SMDHLogical");
    fFEPLLogical            = new G4LogicalVolume(fFEPLSolid, FindMaterial("G4_Al"), "FEPLLogical");
    fALPLLogical            = new G4LogicalVolume(fALPLSolid, FindMaterial("G4_Fe"), "ALPLLogical");
    // Vertical smd bar
    fSMDVLogical            = new G4LogicalVolume(fSMDVSolid, FindMaterial("G4_PLASTIC_SC_VINYLTOLUENE"), "SMDVLogical");



    //------------------------------------------------------------------------------------------------------------
    // Define PhysicalVolume

    fGAPF2Physical          = new G4PVPlacement(fNonRotation, G4ThreeVector(0,0,0), fGAPF2Logical, "GAPF2Physical", fGAPF1Logical, false, 0, checkOverlaps);

    fBlockerPhysical        = new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -(gapper[2]+0.005)*cm), fBlockerLogical, "BlockerPhysical", fGAPF_1Logical, false, 0, checkOverlaps);
    fBlockerPhysical        = new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -(gapper[2]+0.005)*cm), fBlockerLogical, "BlockerPhysical", fGAPF_2Logical, false, 0, checkOverlaps);
    fBlockerPhysical        = new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -(gapper[2]+0.005)*cm), fBlockerLogical, "BlockerPhysical", fGAPF_3Logical, false, 0, checkOverlaps);


    fGAPF1_1Physical        = new G4PVPlacement(fNonRotation, G4ThreeVector(0,0,gapper[2]/2.*cm), fGAPF1Logical, "GAPF1_1Physical", fGAPF_1Logical, false, 0, false);
    fGAPF1_2Physical        = new G4PVPlacement(fNonRotation, G4ThreeVector(0,0,gapper[2]/2.*cm), fGAPF1Logical, "GAPF1_2Physical", fGAPF_2Logical, false, 0, false);
    fGAPF1_3Physical        = new G4PVPlacement(fNonRotation, G4ThreeVector(0,0,gapper[2]/2.*cm), fGAPF1Logical, "GAPF1_3Physical", fGAPF_3Logical, false, 0, false);








    // Put gaps which include fibers into WCNT
    //



    // Put front and behind iron plate into WCNT
    //

    if(Nmod>0 && Nmod<4)
    {
        ypos                = 1.85*cos;
        zpos                = 8 + Lmod - iplPar[1]/sin - 1.85*cos;
        if(Nmod>=1)
        {
            new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fI_PLLogical, "I_PLPhysical", fWCNTLogical, false, 0, checkOverlaps);
        }
        if(Nmod>=2)
        {
            new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm,(Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fI_PLLogical, "I_PLPhysical", fWCNTLogical, false, 0, checkOverlaps);
        }
        if(Nmod>=3)
        {
            new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fI_PLLogical, "I_PLPhysical", fWCNTLogical, false, 0, checkOverlaps);
        }
        zpos                = 8 + -Lmod + iplPar[1]/sin -1.85*cos;
        if(Nmod>=1)
        {
            new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fI_PLLogical, "I_PLPhysical", fWCNTLogical, false, 0, checkOverlaps);
        }
        if(Nmod>=2)
        {
            new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fI_PLLogical, "I_PLPhysical", fWCNTLogical, false, 0, checkOverlaps);
        }
        if(Nmod>=3)
        {
            new G4PVPlacement(GAPFRotation, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fI_PLLogical, "I_PLPhysical", fWCNTLogical, false, 0, checkOverlaps);
        }
    }





    // Put tungsten plates into WCNT
    //
    ypos                     = -zdcPar[1] + wplPar[1]*sin + wplPar[2]*cos +1.85*cos;
    interval                 = (gapper[1] + wplPar[1])*2.0/sin;


    if(Nmod>0 && Nmod<4)
    {

        for(G4int i=1; i<Nlay+2;i++)
        {

            zpos                = 8 + Lmod - ypos/tan - (2.0*iplPar[1] + wplPar[1])/sin - interval*(i-1);
            if(Nmod>=1)
                fW_PL_1Physical = new G4PVPlacement(fQPhi, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-2) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + smdPar[1]*2.0/sin + zpos)*cm), fW_PL_1Logical, "W_PL_1Physical", fWCNTLogical, true, i, checkOverlaps);
            if(Nmod>=2)
                fW_PL_2Physical = new G4PVPlacement(fQPhi, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-4) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fW_PL_2Logical, "W_PL_2Physical", fWCNTLogical, true, i, checkOverlaps);
            if(Nmod>=3)
                fW_PL_3Physical = new G4PVPlacement(fQPhi, G4ThreeVector(0, (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos + ypos)*cm, (Lmod*(Nmod+1-6) - (zdcPar[1] - wplPar[2]*sin - wplPar[1]*cos)/tan + zpos)*cm), fW_PL_3Logical, "W_PL_3Physical", fWCNTLogical, true, i, checkOverlaps);
        }
    }






    // Put 'FEPL' into 'WCNT'
    ypos                    = -7.7;
    zpos                    = 8 + Lmod*Nmod + smdPar[1]/sin + Lmod/2 - 40;
    new G4PVPlacement(fNonRotation, G4ThreeVector(0, ypos*cm, zpos*cm), fFEPLLogical, "FEPLPhysical", fWCNTLogical, false, 0, checkOverlaps);


    // Put 'ALPL' into 'WCNT'
    ypos                    = -11.7;
    new G4PVPlacement(fNonRotation, G4ThreeVector(0, ypos*cm, zpos*cm), fALPLLogical, "ALPLPhysical", fWCNTLogical, false, 0, checkOverlaps);


    // Put 'SMDH' into 'SMD'
    for(G4int i=1; i<9; i++)
    {
        zpos                = (4.5-i)*2.0*11.0/10.5;
        new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, -0.405*cm, zpos*cm), fSMDHLogical, "SMDHPhysical", fSMDLogical, true, i, checkOverlaps);
    }


    fSMDRotation            = new G4RotationMatrix();
    fSMDRotation            -> rotateY(90*deg);

    // Put 'SMDV' into 'SMD'
    for(G4int i=1; i<8; i++)
    {
        xpos                = (i-4)*1.5*11.0/10.5;
        new G4PVPlacement(fNonRotation, G4ThreeVector(xpos*cm, 0.405*cm, 0.*cm), fSMDVLogical, "SMDVPhysical", fSMDLogical, true, i, checkOverlaps);
    }


    zpos                    = 8 + Lmod*(Nmod-2)+smdPar[1]/sin;
    new G4PVPlacement(GAPFRotation, G4ThreeVector(0.0, 0.0, zpos*cm), fSMDLogical, "SMDPhysical", fWCNTLogical, false, 0, checkOverlaps);




    // ---------visualization attributes--------------------
    //


    visAttributes           = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    visAttributes           -> SetVisibility(false);
    fWCNTLogical            -> SetVisAttributes(visAttributes);
    fGAPF_1Logical          -> SetVisAttributes(visAttributes);
    fGAPF_2Logical          -> SetVisAttributes(visAttributes);
    fGAPF_3Logical          -> SetVisAttributes(visAttributes);
    fGAPF1Logical           -> SetVisAttributes(visAttributes);
    fGAPF2Logical           -> SetVisAttributes(visAttributes);

    fVisAttributes.push_back(visAttributes);

    visAttributes           = new G4VisAttributes(G4Colour(0.0, 0.8888, 0.0));
    visAttributes           -> SetVisibility(false);
    //-fZDC_1Logical -> SetVisAttributes(visAttributes);
    fSMDLogical             -> SetVisAttributes(visAttributes);

    /*
       fMagneticLogical -> SetVisAttributes(visAttributes);
       fVisAttributes.push_back(visAttributes);

*/


    visAttributes           = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));
    visAttributes           -> SetVisibility(false);
    fVisAttributes.push_back(visAttributes);

    visAttributes           = new G4VisAttributes(G4Colour(2.0, 2.0, 0)); //LighteGray
    fW_PL_1Logical          -> SetVisAttributes(visAttributes);
    fW_PL_2Logical          -> SetVisAttributes(visAttributes);
    fW_PL_3Logical          -> SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    visAttributes           = new G4VisAttributes(G4Colour(0.9, 0.9, 0.9)); //Red
    fSMDHLogical            -> SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    visAttributes           = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));//Lightgreen
    fI_PLLogical            -> SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    visAttributes           = new G4VisAttributes(G4Colour(0.0, 0.0 ,0.0));
    //fALPLLogical -> SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    visAttributes           = new G4VisAttributes(G4Colour(0.0, 5.0, 5.0));
    fVisAttributes.push_back(visAttributes);


    visAttributes           = new G4VisAttributes(G4Colour(0.0, 10.0, 0.0));
    fSMDHLogical            -> SetVisAttributes(visAttributes);
    fSMDVLogical            -> SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);



    //return testPhysical;
    return                  fWCNTPhysical;


}

///////////////////////////////////////////////////////////////////////////////
G4VPhysicalVolume* RHICFDetectorConstruction::BBC(G4VPhysicalVolume* world_phys, G4ThreeVector vector, G4RotationMatrix* mat)
///////////////////////////////////////////////////////////////////////////////
{

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


    //----------------------------------------------BEAM PIPE----------------------------------------------------
    //
    //
    //
    judgeNS = 1;
    ivolu = 0;
    iron = 4;

    par[0] = 0.0;
    par[1] = 0.1;
    par[2] = 0.7;

    // BEAM PIPE + STEEL SECTIONS
    //
    // Beam pipe: vol. definitions: name shape mat. r_i r_o lz/2 #par
    //

    par[0] = 3.71;
    par[1] = 3.81;
    par[2] = 600.0;

    G4VSolid* BEAMSolid = new G4Tubs("BEAMSolid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* BEAMLogical = new G4LogicalVolume(BEAMSolid, FindMaterial("G4_Be"), "BEAMLogical");


    par[0] = 0.0;
    par[1] = 3.71;
    par[2] = 600.0;

    G4VSolid* BEAVSolid = new G4Tubs("BEAVSolid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    BEAVLogical = new G4LogicalVolume(BEAVSolid, FindMaterial("G4_Galactic"), "BEAVLogical");

    par[0] = 3.81;
    par[1] = 5.85;
    par[2] = 2.0;

    G4VSolid* FLB1Solid = new G4Tubs("FLB1Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    FLB1Logical = new G4LogicalVolume(FLB1Solid, FindMaterial("G4_Be"), "FLB1Logical");

    G4VSolid* FLB2Solid = new G4Tubs("FLB2Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    FLB2Logical = new G4LogicalVolume(FLB2Solid, FindMaterial("G4_Be"), "FLB2Logical");

    G4VSolid* FLS1Solid = new G4Tubs("FLS1Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    G4LogicalVolume* FLS1Logical = new G4LogicalVolume(FLS1Solid, FindMaterial("G4_Fe"), "FLS1Logical");

    G4VSolid* FLS2Solid = new G4Tubs("FLS2Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    G4LogicalVolume* FLS2Logical = new G4LogicalVolume(FLS2Solid, FindMaterial("G4_Fe"), "FLS2Logical");


    par[0] = 3.54;
    par[1] = 3.81;
    par[2] = 2.0;

    G4VSolid* SPL1Solid = new G4Tubs("SPL1Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    G4LogicalVolume* SPL1Logical = new G4LogicalVolume(SPL1Solid, FindMaterial("G4_Fe"), "SPL1VLogical");

    G4VSolid* SPL2Solid = new G4Tubs("SPL2Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    SPL2Logical = new G4LogicalVolume(SPL2Solid, FindMaterial("G4_Fe"), "SPL2VLogical");


    par[0] = 0.0;
    par[1] = 3.54;
    par[2] = 2.0;

    G4VSolid* SM11Solid = new G4Tubs("SM11Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    G4LogicalVolume* SM11Logical = new G4LogicalVolume(SM11Solid, FindMaterial("G4_Galactic"), "SM11Logical");

    G4VSolid* SM12Solid = new G4Tubs("SM12Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    G4LogicalVolume* SM12Logical = new G4LogicalVolume(SM12Solid, FindMaterial("G4_Galactic"), "SM12Logical");

    par[0] = 6.35;
    par[1] = 8.39;
    par[2] = 2.0;

    G4VSolid* FLS3Solid = new G4Tubs("FLS3Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    G4LogicalVolume* FLS3Logical = new G4LogicalVolume(FLS3Solid, FindMaterial("G4_Fe"), "FLS3Logical");

    G4VSolid* FLS4Solid = new G4Tubs("FLS4Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    FLS4Logical = new G4LogicalVolume(FLS4Solid, FindMaterial("G4_Fe"), "FLS4Logical");


    G4VSolid* FLS5Solid = new G4Tubs("FLS5Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    FLS5Logical = new G4LogicalVolume(FLS5Solid, FindMaterial("G4_Fe"), "FLS5Logical");

    G4VSolid* FLS6Solid = new G4Tubs("FLS6Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    FLS6Logical = new G4LogicalVolume(FLS6Solid, FindMaterial("G4_Fe"), "FLS6Logical");


    par[0] = 6.08;
    par[1] = 6.35;
    par[2] = 58.5;


    G4VSolid* SPU1Solid = new G4Tubs("SPU1Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    SPU1Logical = new G4LogicalVolume(SPU1Solid, FindMaterial("G4_Fe"), "SPU1Logical");

    G4VSolid* SPU2Solid = new G4Tubs("SPU2Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    G4LogicalVolume* SPU2Logical = new G4LogicalVolume(SPU2Solid, FindMaterial("G4_Fe"), "SPU2Logical");

    par[0] = 0.0;
    par[1] = 6.08;
    par[2] = 58.5;


    G4VSolid* SM21Solid = new G4Tubs("SM21Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    SM21Logical = new G4LogicalVolume(SM21Solid, FindMaterial("G4_Galactic"), "SM21Logical");

    G4VSolid* SM22Solid = new G4Tubs("SM22Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    G4LogicalVolume* SM22Logical = new G4LogicalVolume(SM22Solid, FindMaterial("G4_Galactic"), "SM22Logical");


    par[0] = 6.35;
    par[1] = 7.35;
    par[2] = 0.20;


    G4VSolid* RB11Solid = new G4Tubs("RB11Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    RB11Logical = new G4LogicalVolume(RB11Solid, FindMaterial("G4_Fe"), "RB11Logical");

    G4VSolid* RB12Solid = new G4Tubs("RB12Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    G4LogicalVolume* RB12Logical = new G4LogicalVolume(RB12Solid, FindMaterial("G4_Fe"), "RB12Logical");


    G4VSolid* RB13Solid = new G4Tubs("RB13Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    RB13Logical = new G4LogicalVolume(RB13Solid, FindMaterial("G4_Fe"), "RB13Logical");

    G4VSolid* RB14Solid = new G4Tubs("RB14Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    RB14Logical = new G4LogicalVolume(RB14Solid, FindMaterial("G4_Fe"), "RB14Logical");


    G4VSolid* RB15Solid = new G4Tubs("RB15Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    G4LogicalVolume* RB15Logical = new G4LogicalVolume(RB15Solid, FindMaterial("G4_Fe"), "RB15Logical");

    G4VSolid* RB16Solid = new G4Tubs("RB16Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    RB16Logical = new G4LogicalVolume(RB16Solid, FindMaterial("G4_Fe"), "RB16Logical");


    G4VSolid* RB17Solid = new G4Tubs("RB17Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    RB17Logical = new G4LogicalVolume(RB17Solid, FindMaterial("G4_Fe"), "RB17Logical");

    G4VSolid* RB18Solid = new G4Tubs("RB18Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    G4LogicalVolume* RB18Logical = new G4LogicalVolume(RB18Solid, FindMaterial("G4_Fe"), "RB18Logical");



    G4VSolid* RB21Solid = new G4Tubs("RB21Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    RB21Logical = new G4LogicalVolume(RB21Solid, FindMaterial("G4_Fe"), "RB21Logical");

    G4VSolid* RB22Solid = new G4Tubs("RB22Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    RB22Logical = new G4LogicalVolume(RB22Solid, FindMaterial("G4_Fe"), "RB22Logical");


    G4VSolid* RB23Solid = new G4Tubs("RB23Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    G4LogicalVolume* RB23Logical = new G4LogicalVolume(RB23Solid, FindMaterial("G4_Fe"), "RB23Logical");

    G4VSolid* RB24Solid = new G4Tubs("RB24Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    G4LogicalVolume* RB24Logical = new G4LogicalVolume(RB24Solid, FindMaterial("G4_Fe"), "RB24Logical");



    G4VSolid* RB25Solid = new G4Tubs("RB25Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    RB25Logical = new G4LogicalVolume(RB25Solid, FindMaterial("G4_Fe"), "RB25Logical");

    G4VSolid* RB26Solid = new G4Tubs("RB26Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    G4LogicalVolume* RB26Logical = new G4LogicalVolume(RB26Solid, FindMaterial("G4_Fe"), "RB26Logical");


    G4VSolid* RB27Solid = new G4Tubs("RB27Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    RB27Logical = new G4LogicalVolume(RB27Solid, FindMaterial("G4_Fe"), "RB27Logical");

    G4VSolid* RB28Solid = new G4Tubs("RB28Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0 ,360);
    RB28Logical = new G4LogicalVolume(RB28Solid, FindMaterial("G4_Fe"), "RB28Logical");


    par[0] = 12.5;
    par[1] = 3.54;
    par[2] = 3.81;
    par[3] = 6.08;
    par[4] = 6.35;

    G4VSolid* CN11Solid = new G4Cons("CN11Solid", par[1]*cm, par[2]*cm, par[3]*cm, par[4]*cm, par[0]*cm, 0, 360);
    CN11Logical = new G4LogicalVolume(CN11Solid, FindMaterial("G4_Galactic"), "CN11Logical");



    par[0] = 12.5;
    par[1] = 6.08;
    par[2] = 6.35;
    par[3] = 3.54;
    par[4] = 3.81;

    G4VSolid* CN12Solid = new G4Cons("CN12Solid", par[1]*cm, par[2]*cm, par[3]*cm, par[4]*cm, par[0]*cm, 0, 360);
    CN12Logical = new G4LogicalVolume(CN12Solid, FindMaterial("G4_Galactic"), "CN12Logical");


    par[0] = 12.5;
    par[1] = 0.0;
    par[2] = 3.54;
    par[3] = 0.0;
    par[4] = 6.08;

    G4VSolid* CM11Solid = new G4Cons("CM11Solid", par[1]*cm, par[2]*cm, par[3]*cm, par[4]*cm, par[0]*cm, 0, 360);
    CM11Logical = new G4LogicalVolume(CM11Solid, FindMaterial("G4_Galactic"), "CM11Logical");



    par[0] = 12.5;
    par[1] = 0.0;
    par[2] = 6.08;
    par[3] = 0.0;
    par[4] = 3.54;

    G4VSolid* CM12Solid = new G4Cons("CM12Solid", par[1]*cm, par[2]*cm, par[3]*cm, par[4]*cm, par[0]*cm, 0, 360);
    CM12Logical = new G4LogicalVolume(CM12Solid, FindMaterial("G4_Galactic"), "CM12Logical");

    // Place beam pipe
    //
    //

    new G4PVPlacement(fNonRotation, G4ThreeVector(0,0,0*cm), BEAMLogical, "BEAMPhysical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(), BEAVLogical, "BEAVPhysical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 200.0*cm), FLB1Logical, "FLB1Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -200.0*cm), FLB2Logical, "FLB2Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 600.0*cm), FLS1Logical, "FLS1Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -600.0*cm), FLS2Logical, "FLS2Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 602.0*cm), SM11Logical, "SM11Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -602.0*cm), SM12Logical,"SM12Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 602.0*cm), SPL1Logical, "SPL1Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -602.0*cm), SPL2Logical, "SPL2Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 616.50*cm), CM11Logical, "CM11Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -616.50*cm), CM12Logical, "CM12Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 616.50*cm), CN11Logical, "CN11Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -616.50*cm), CN12Logical, "CN12Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 629.0*cm), FLS3Logical, "FLS3Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -629.0*cm), FLS4Logical, "FLS4Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 746.0*cm), FLS5Logical, "FLS5Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -746.0*cm), FLS6Logical, "FLS6Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 687.50*cm), SM21Logical, "SM21Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -687.50*cm), SM22Logical, "SM22Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 687.50*cm), SPU1Logical, "SPU1Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -687.50*cm), SPU2Logical, "SPU2Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 679.80*cm), RB11Logical, "RB11Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 681.95*cm), RB12Logical, "RB12Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 684.10*cm), RB13Logical, "RB13Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 686.25*cm), RB14Logical, "RB14Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 688.40*cm), RB15Logical, "RB15Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 690.55*cm), RB16Logical, "RB16Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 692.70*cm), RB17Logical, "RB17Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, 694.85*cm), RB18Logical, "RB18Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -679.80*cm), RB21Logical, "RB21Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -681.95*cm), RB22Logical, "RB22Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -684.10*cm), RB23Logical, "RB23Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -686.25*cm), RB24Logical, "RB24Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -688.40*cm), RB25Logical, "RB25Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -690.55*cm), RB26Logical, "RB26Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -692.70*cm), RB27Logical, "RB27Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0, 0, -694.85*cm), RB28Logical, "RB28Physical", fWorldLogical, false, 0, checkOverlaps);


    //---------------------UPSTREAM AREA------------------------

    par[0] = 6.08;
    par[1] = 6.35;
    par[2] = 103.94;

    G4VSolid* SP31Solid = new G4Tubs("SP31Solid", par[0]*cm, par[1]*cm, par[2]*cm,0 ,360*deg);
    G4LogicalVolume* SP31Logical = new G4LogicalVolume(SP31Solid, FindMaterial("G4_Fe"), "SP31Logical");

    G4VSolid* SP32Solid = new G4Tubs("SP32Solid", par[0]*cm, par[1]*cm, par[2]*cm,0 ,360*deg);
    G4LogicalVolume* SP32Logical = new G4LogicalVolume(SP32Solid, FindMaterial("G4_Fe"), "SP31Logical");

    par[0] = 0.0;
    par[1] = 6.08;
    par[2] = 103.94;

    G4VSolid* SM31Solid = new G4Tubs("SM31Solid", par[0]*cm, par[1]*cm, par[2]*cm,0 ,360*deg);
    G4LogicalVolume* SM31Logical = new G4LogicalVolume(SM31Solid, FindMaterial("G4_Galactic"), "SM31Logical");

    G4VSolid* SM32Solid = new G4Tubs("SM32Solid", par[0]*cm, par[1]*cm, par[2]*cm,0 ,360*deg);
    G4LogicalVolume* SM32Logical = new G4LogicalVolume(SM32Solid, FindMaterial("G4_Galactic"), "SM32Logical");

    par[0] = 6.99;
    par[1] = 7.14;
    par[2] = 210.92;

    G4VSolid* SP41Solid = new G4Tubs("SP41Solid", par[0]*cm, par[1]*cm, par[2]*cm,0 ,360*deg);
    G4LogicalVolume* SP41Logical = new G4LogicalVolume(SP41Solid, FindMaterial("G4_Fe"), "SP41Logical");

    G4VSolid* SP42Solid = new G4Tubs("SP42Solid", par[0]*cm, par[1]*cm, par[2]*cm,0 ,360*deg);
    G4LogicalVolume* SP42Logical = new G4LogicalVolume(SP42Solid, FindMaterial("G4_Fe"), "SP42Logical");

    par[0] = 0.0;
    par[1] = 6.99;
    par[2] = 378.89/2.0;

    G4VSolid* SM41Solid = new G4Tubs("SM41Solid", par[0], par[1], par[2],0 ,360*deg);
    G4LogicalVolume* SM41Logical = new G4LogicalVolume(SM41Solid, FindMaterial("G4_Galactic"), "SM41Logical");

    G4VSolid* SM42Solid = new G4Tubs("SM42Solid", par[0], par[1] ,par[2],0 ,360*deg);
    G4LogicalVolume* SM42Logical = new G4LogicalVolume(SM42Solid, FindMaterial("G4_Galactic"), "SM42Logical");

    par[0] = 0.0;
    par[1] = 6.99;
    par[2] = 210.92 - (378.89/2.0);

    G4VSolid* SM43Solid = new G4Tubs("SM43Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* SM43Logical = new G4LogicalVolume(SM43Solid, FindMaterial("G4_Galactic"), "SM43Logical");

    G4VSolid* SM44Solid = new G4Tubs("SM44Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* SM44Logical = new G4LogicalVolume(SM44Solid, FindMaterial("G4_Galactic"), "SM44Logical");

    G4VSolid* SM45Solid = new G4Tubs("SM45Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* SM45Logical = new G4LogicalVolume(SM45Solid, FindMaterial("G4_Galactic"), "SM45Logical");

    G4VSolid* SM46Solid = new G4Tubs("SM46Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* SM46Logical = new G4LogicalVolume(SM46Solid, FindMaterial("G4_Galactic"), "SM46Logical");

    par[0] = 9.53;
    par[1] = 10.16;
    par[2] = 210.92;

    G4VSolid* SP51Solid = new G4Tubs("SP51Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* SP51Logical = new G4LogicalVolume(SP51Solid, FindMaterial("G4_Fe"), "SP51Logical");

    G4VSolid* SP52Solid = new G4Tubs("SP52Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* SP52Logical = new G4LogicalVolume(SP52Solid, FindMaterial("G4_Fe"), "SP52Logical");

    par[0] = 7.14;
    par[1] = 9.53;
    par[2] = 210.92;

    G4VSolid* SM51Solid = new G4Tubs("SM51Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* SM51Logical = new G4LogicalVolume(SM51Solid, FindMaterial("G4_Galactic"), "SM51Logical");

    G4VSolid* SM52Solid = new G4Tubs("SM52Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* SM52Logical = new G4LogicalVolume(SM52Solid, FindMaterial("G4_Galactic"), "SM52Logical");


    par[0] = 15.34;
    par[1] = 37.00;
    par[2] = 210.92;

    G4VSolid* SP61Solid = new G4Tubs("SP61Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* SP61Logical = new G4LogicalVolume(SP61Solid, FindMaterial("G4_Fe"), "SP61Logical");

    G4VSolid* SP62Solid = new G4Tubs("SP62Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* SP62Logical = new G4LogicalVolume(SP62Solid, FindMaterial("G4_Fe"), "SP62Logical");

    par[0] = 10.16;
    par[1] = 15.34;
    par[2] = 210.92;

    G4VSolid* SM61Solid = new G4Tubs("SM61Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* SM61Logical = new G4LogicalVolume(SM61Solid, FindMaterial("G4_Galactic"), "SM61Logical");

    G4VSolid* SM62Solid = new G4Tubs("SM62Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* SM62Logical = new G4LogicalVolume(SM62Solid, FindMaterial("G4_Galactic"), "SM62Logical");

    par[0] = 20.87;
    par[1] = 7.14;
    par[2] = 7.77;
    par[3] = 14.60;
    par[4] = 15.24;

    G4VSolid* CN21Solid = new G4Cons("CN21Solid", par[1]*cm, par[2]*cm, par[3]*cm, par[4]*cm, par[0]*cm, 0, 360*deg);
    G4LogicalVolume* CN21Logical = new G4LogicalVolume(CN21Solid, FindMaterial("G4_Fe"), "CN21Logical");

    par[0] = 20.87;
    par[1] = 14.60;
    par[2] = 15.24;
    par[3] = 7.14;
    par[4] = 7.77;

    G4VSolid* CN22Solid = new G4Cons("CN22Solid", par[1]*cm, par[2]*cm, par[3]*cm, par[4]*cm, par[0]*cm, 0, 360*deg);
    G4LogicalVolume* CN22Logical = new G4LogicalVolume(CN22Solid, FindMaterial("G4_Fe"), "CN22Logical");

    par[0] = 20.87;
    par[1] = 0.0;
    par[2] = 7.14;
    par[3] = 0.0;
    par[4] = 14.60;

    G4VSolid* CM21Solid = new G4Cons("CM21Solid", par[1]*cm, par[2]*cm, par[3]*cm, par[4]*cm, par[0]*cm, 0, 360*deg);
    G4LogicalVolume* CM21Logical = new G4LogicalVolume(CM21Solid, FindMaterial("G4_Galactic"), "CM21Logical");

    par[0] = 20.87;
    par[1] = 0.0;
    par[2] = 14.60;
    par[3] = 0.0;
    par[4] = 7.14;

    G4VSolid* CM22Solid = new G4Cons("CM22Solid", par[1]*cm, par[2]*cm, par[3]*cm, par[4]*cm, par[0]*cm, 0, 360*deg);
    G4LogicalVolume* CM22Logical = new G4LogicalVolume(CM22Solid, FindMaterial("G4_Galactic"), "CM22Logical");

    par[0] = 14.60;
    par[1] = 15.24;
    par[2] = 57.895;

    G4VSolid* SP71Solid = new G4Tubs("SP71Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* SP71Logical = new G4LogicalVolume(SP71Solid, FindMaterial("G4_Fe"), "SP71Logical");

    G4VSolid* SP72Solid = new G4Tubs("SP72Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* SP72Logical = new G4LogicalVolume(SP72Solid, FindMaterial("G4_Fe"), "SP72Logical");

    par[0] = 0.0;
    par[1] = 14.60;
    par[2] = 57.895;

    G4VSolid* SM71Solid = new G4Tubs("SM71Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* SM71Logical = new G4LogicalVolume(SM71Solid, FindMaterial("G4_Galactic"), "SM71Logical");

    G4VSolid* SM72Solid = new G4Tubs("SM72Solid", par[0]*cm, par[1]*cm, par[2]*cm, 0, 360*deg);
    G4LogicalVolume* SM72Logical = new G4LogicalVolume(SM72Solid, FindMaterial("G4_Galactic"), "SM72Logical");

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -850.14*cm), SM32Logical, "SM32Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -850.14*cm), SP32Logical, "SP32Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -945.08*cm), SM46Logical, "SM46Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1375.92*cm), SM44Logical, "SM44Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1165.0*cm), SM41Logical, "SM41Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1165.0*cm), SP41Logical, "SP41Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1165.0*cm), SM51Logical, "SM51Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1165.0*cm), SP51Logical, "SP51Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1165.0*cm), SM61Logical, "SM61Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1165.0*cm), SP61Logical, "SP61Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1394.25*cm), CM22Logical, "CM22Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1394.25*cm), CN22Logical, "CN22Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1475.935*cm), SM71Logical, "SM71Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, -1475.935*cm), SP71Logical, "SP71Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 850.14*cm), SM31Logical, "SM31Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 850.14*cm), SP31Logical, "SP31Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 945.08*cm), SM45Logical, "SM45Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1375.92*cm), SM43Logical, "SM43Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1165.0*cm), SM52Logical, "SM52Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1165.0*cm), SP52Logical, "SP52Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1165.0*cm), SM62Logical, "SM62Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1165.0*cm), SP62Logical, "SP62Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1165.0*cm), SM42Logical, "SM42Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1165.0*cm), SP42Logical, "SP42Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1394.25*cm), CM21Logical, "CM21Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1394.25*cm), CN21Logical, "CN21Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1475.935*cm), SM72Logical, "SM72Physical", fWorldLogical, false, 0, checkOverlaps);

    new G4PVPlacement(fNonRotation, G4ThreeVector(0.0, 0.0, 1475.935*cm), SP72Logical, "SP72Physical", fWorldLogical, false, 0, checkOverlaps);

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


G4VPhysicalVolume* RHICFDetectorConstruction::ARM2(G4VPhysicalVolume* world_phys, G4ThreeVector vector, G4RotationMatrix* mat)
{


    G4double lhollowThickness[2][16] = {{0.538*mm,0.63*mm,0.871*mm,0.63*mm,0.597*mm,0.481*mm,0.641*mm,0.486*mm,0.438*mm,0.45*mm,0.535*mm,0.1*mm,0.492*mm,0.581*mm,0.616*mm,0.1*mm},{0.538*mm,0.63*mm,0.748*mm,0.63*mm,0.597*mm,0.382*mm,0.641*mm,0.486*mm,0.422*mm,0.45*mm,0.535*mm,0.1*mm,0.492*mm,0.581*mm,0.616*mm,0.1*mm}};  // measured
    G4double wThickness[2][22] = {{7.027*mm,7.043*mm,7.035*mm,7.021*mm,7.029*mm,7.012*mm,7.031*mm,7.016*mm,7.023*mm,7.014*mm,7.051*mm,7.033*mm,7.004*mm,6.998*mm,6.98*mm,6.991*mm,6.985*mm,6.99*mm,7.082*mm,7.014*mm,7.032*mm,7.031*mm},{7.09*mm,7.09*mm,7.09*mm,7.11*mm,7.08*mm,7.09*mm,7.09*mm,7.09*mm,7.*mm,7.06*mm,7.06*mm,7.04*mm,7.02*mm,7.04*mm,7.11*mm,7.03*mm,7.01*mm,7.*mm,6.99*mm,7.14*mm,6.99*mm,7.*mm}};  //new
    G4double whollowThickness[2][22] = {{0.121*mm,0.102*mm,0.1*mm,0.001*mm,0.001*mm,0.1*mm,0.001*mm,0.043*mm,0.1*mm,0.272*mm,0.001*mm,0.001*mm,0.565*mm,0.178*mm,0.111*mm,0.1*mm,0.083*mm,0.001*mm,0.086*mm,0.001*mm,0.558*mm,0.006*mm},{0.058*mm,0.055*mm,0.1*mm,0.001*mm,0.001*mm,0.1*mm,0.001*mm,0.001*mm,0.1*mm,0.226*mm,0.001*mm,0.001*mm,0.546*mm,0.136*mm,0.001*mm,0.1*mm,0.094*mm,0.001*mm,0.178*mm,0.001*mm,0.565*mm,0.037*mm}};  //new


    G4double layerSizeXY[2] = {32.*mm, 25.*mm};
    G4double towSizeXY[2] = {TL*mm, TS*mm};
    G4double wSizeX[2][22] = {{32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.05*mm,32.15*mm,32.1*mm,32.1*mm,32.1*mm,32.15*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.2*mm,32.1*mm,32.15*mm,32.1*mm},{25.1*mm,25.*mm,25.2*mm,25.2*mm,25.*mm,25.2*mm,25.2*mm,25.*mm,25.2*mm,25.2*mm,25.*mm,25.2*mm,25.*mm,25.2*mm,25.2*mm,25.2*mm,25.1*mm,25.*mm,25.*mm,25.2*mm,25.2*mm,25.*mm}};  //new
    G4double wSizeY[2][22] = {{32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.1*mm,32.15*mm,32.2*mm,32.15*mm,32.1*mm,32.05*mm},{25.*mm,25.*mm,25.*mm,25.*mm,25.*mm,25.1*mm,25.1*mm,25.*mm,25.1*mm,25.*mm,24.95*mm,25.*mm,24.9*mm,25.1*mm,25.*mm,25.*mm,24.95*mm,25.*mm,25.*mm,25.*mm,25.1*mm,25.*mm}}; //new

    G4int nolet[7] = {2,3,3,3,2,2,1};//noflayer for each tower
    G4int nowet[7] = {3,3,3,3,4,4,2};//nofw for each tower

    G4double gtenSizeX[7] = {80.5*mm, 18.3*mm, 25.2*mm, 1.8*mm, 32.2*mm, 32.2*mm, 3.*mm};
    G4double gtenSizeY[7] = {16.88*mm, 103.12*mm, 77.92*mm, 103.12*mm, 27.*mm, 43.92*mm, 103.12*mm};


    G4double towThickness[7] = {29.52*mm,32.77*mm,32.03*mm,32.39*mm,35.76*mm,35.94*mm,17.73*mm}; //new

    // G4double wepicsposX[2] = {0.2*mm,0.*mm};//後から細かく配列化
    G4double wepicsposX[2][22] = {{0.1*mm,0.1*mm,0.1*mm,0.1*mm,0.15*mm,0.05*mm,0.1*mm,0.1*mm,0.1*mm,0.05*mm,0.1*mm,0.1*mm,0.1*mm,0.1*mm,0.1*mm,0.1*mm,0.1*mm,0.1*mm,0.*mm,0.1*mm,0.05*mm,0.1*mm},{0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm}};
    // G4double wepicsposY[2] = {0.*mm,0.2*mm};//後から細かく配列化
    G4double wepicsposY[2][22] = {{0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm,0.*mm},{0.2*mm,0.2*mm,0.2*mm,0.2*mm,0.2*mm,0.1*mm,0.1*mm,0.2*mm,0.1*mm,0.2*mm,0.25*mm,0.2*mm,0.3*mm,0.1*mm,0.2*mm,0.2*mm,0.25*mm,0.2*mm,0.2*mm,0.2*mm,0.1*mm,0.2*mm}};

    G4double towposX[2] = {-Arm2SizeX/2 + gtenSizeX[6] + towSizeXY[0]/2, -Arm2SizeX/2 + gtenSizeX[6] + gtenSizeX[4] + gtenSizeX[3] + towSizeXY[1]/2};
    G4double towposY[2] = {-Arm2SizeY/2 + gtenSizeY[0] + gtenSizeY[4] + towSizeXY[0]/2, -Arm2SizeY/2 + gtenSizeY[0] + towSizeXY[1]/2};
    G4double layerposX[2] = {towSizeXY[0]/2-layerSizeXY[0]/2,layerSizeXY[1]/2-towSizeXY[1]/2};
    G4double layerposY[2] = {layerSizeXY[0]/2-towSizeXY[0]/2,towSizeXY[1]/2-layerSizeXY[1]/2};
    //                               
    // Arm2
    //  
    G4VSolid* Arm2S
        = new G4Box("Arm2",     // its name
                Arm2SizeX/2, Arm2SizeY/2, Arm2Thickness/2); // its size

    G4LogicalVolume* Arm2LV
        = new G4LogicalVolume(
                Arm2S,    // its solid
                FindMaterial("Gten"),  // fFindMaterial("Gten"), // its material
                "Arm2");  // its name
    G4VPhysicalVolume* Arm2PV                                   
        = 
        new G4PVPlacement(
                mat,                // no rotation
                vector,
                "Arm2",    // its name
                Arm2LV,
                world_phys,          // its mother  volume
                false,            // no boolean operation
                0,                // copy number
                checkOverlaps);  // checking overlaps


    //
    // First Al layer
    //
    G4VSolid* firstS = new G4Box("firstlayerS",Arm2SizeX/2, Arm2SizeY/2, firstThickness/2);
    G4LogicalVolume* firstLV = new G4LogicalVolume(firstS, FindMaterial("G4_Galactic"), "firstlayerLV");
    new G4PVPlacement(0,
            G4ThreeVector(0., 0., -Arm2Thickness/2+firstThickness/2),
            firstLV,
            "firstlayer",
            Arm2LV,
            false,
            0,
            checkOverlaps);
    G4VSolid* firstalS = new G4Box("firstalS",Arm2SizeX/2,firstSizeY/2,firstThickness/2);
    G4LogicalVolume* firstalLV = new G4LogicalVolume(firstalS, FindMaterial("G4_Al"), "firstalLV");
    new G4PVPlacement(0,
            G4ThreeVector(0.,-Arm2SizeY/2+firstSizeY/2,0.),
            firstalLV,
            "firstal",
            firstLV,
            false,
            0,
            checkOverlaps);

    //
    // define SSD xy=0:stripy xy=1:stripx
    //
    G4int towcur_num;
    G4VSolid* ssdS = new G4Box("SSD",Arm2SizeX/2,Arm2SizeY/2,ssdxyThickness/2);
    G4LogicalVolume* ssdLV[2][4];
    for(int xy=0;xy<2;xy++){
        towcur_num=0;
        for(int i=0;i<nofssdset;i++){
            if(!xy){
                switch(i){
                    case 0:
                        place = -Arm2Thickness/2 + firstThickness + towThickness[towcur_num] + ssdxyThickness/2;
                        towcur_num+=1;
                        break;
                    case 3:
                        place += ssdxyThickness*2 + towThickness[towcur_num] + towThickness[towcur_num+1];
                        break;
                    default:
                        place += ssdxyThickness*2 + towThickness[towcur_num];
                        towcur_num+=1;
                        break;
                }
            }
            if(xy){
                switch(i){
                    case 0:
                        place = -Arm2Thickness/2 + firstThickness + towThickness[towcur_num] +ssdxyThickness*3/2;
                        towcur_num+=1;
                        break;
                    case 1:
                        place += ssdxyThickness*2 + towThickness[towcur_num];
                        towcur_num+=1;
                        break;
                    default:
                        place += ssdxyThickness*2 + towThickness[towcur_num] + towThickness[towcur_num+1];
                        towcur_num+=2;
                        break;
                }
            }
            ssdLV[xy][i] = new G4LogicalVolume(ssdS,FindMaterial("G4_Galactic"),ssdLVname[xy][i]);
            new G4PVPlacement(0,
                    G4ThreeVector(0.,0.,place),
                    ssdLV[xy][i],
                    ssdname[xy][i],
                    Arm2LV,
                    false,
                    0,
                    checkOverlaps);
            ssdLV[xy][i]->SetVisAttributes(G4VisAttributes(true,G4Colour(1.0,0.,0.)));
            G4cout <<place<<G4endl;
        }
    }

    G4VSolid* ssdalS = new G4Box("ssdAlS",Arm2SizeX/2,Arm2SizeY/2,ssdalThickness/2);
    G4LogicalVolume* ssdalLV = new G4LogicalVolume(ssdalS,FindMaterial("G4_Al"),"ssdAlLV");
    for(int xy=0;xy<2;xy++){
        for(int i=0;i<nofssdset;i++){
            new G4PVPlacement(0,
                    G4ThreeVector(0.,0.,pow(-1,xy)*(ssdxyThickness/2-ssdalThickness/2)),
                    ssdalLV,
                    "ssdAlLV",
                    ssdLV[xy][i],
                    false,
                    2*i+xy,
                    checkOverlaps);
        }
    }

    G4VSolid* ssdacrylS = new G4Box("ssdacrylS",Arm2SizeX/2,Arm2SizeY/2,ssdacrylThickness/2);
    G4LogicalVolume* ssdacrylLV = new G4LogicalVolume(ssdacrylS,FindMaterial("Acrylic"),"ssdacrylLV");
    for(int xy=0;xy<2;xy++){
        for(int i=0;i<nofssdset;i++){
            new G4PVPlacement(0,
                    G4ThreeVector(0.,0.,pow(-1,xy)*(ssdxyThickness/2-ssdalThickness-ssdacrylThickness/2)),
                    ssdacrylLV,
                    "ssdacrylLV",
                    ssdLV[xy][i],
                    false,
                    2*i+xy,
                    checkOverlaps);
        }
    }

    G4double ssdsiSizeXY[2] = {63.96*mm,63.56*mm};
    G4double ssdsiepicsposX[2] = {0.9*mm,1.1*mm};
    G4double ssdsiepicsposY[2] = {14.6*mm,14.4*mm};

    G4VSolid* ssdsiS[2];
    G4LogicalVolume* ssdsiLV[2][4];
    for(int xy=0;xy<2;xy++){
        ssdsiS[xy] = new G4Box(ssdsiSname[xy],ssdsiSizeXY[xy]/2,ssdsiSizeXY[1-xy]/2,ssdsiThickness/2);
        for(int i=0;i<nofssdset;i++){
            ssdsiLV[xy][i] = new G4LogicalVolume(ssdsiS[xy],FindMaterial("G4_Si"),ssdsiLVname[xy]);
            new G4PVPlacement(0,
                    G4ThreeVector(-Arm2SizeX/2+ssdsiepicsposX[xy]+ssdsiSizeXY[xy]/2,-Arm2SizeY/2+ssdsiepicsposY[xy]+ssdsiSizeXY[1-xy]/2,pow(-1,xy)*(-ssdxyThickness/2+ssdsiThickness/2)),
                    ssdsiLV[xy][i],
                    ssdsiLVname[xy],
                    ssdLV[xy][i],
                    false,
                    0,
                    checkOverlaps);
        }
    }
    //
    // ssd
    //

    //
    //define Tower
    //
    G4double towposZ[7];
    G4VSolid* towS[2][7];
    G4LogicalVolume* towLV[2][7];
    G4VPhysicalVolume* towPV[2][7];
    for(int t=0;t<2;t++){
        for(int i=0;i<7;i++){
            switch(i){
                case 0:
                    place = -Arm2Thickness/2 + firstThickness + towThickness[i]/2;
                    break;
                case 1:
                case 2:
                    place += ssdThickness + towThickness[i-1]/2 + towThickness[i]/2;
                    break;
                default:
                    place += ssdxyThickness + towThickness[i-1]/2 + towThickness[i]/2;
                    break;
            }

            towS[t][i] = new G4Box(towSname[t][i],
                    towSizeXY[t]/2, towSizeXY[t]/2, towThickness[i]/2);

            towLV[t][i] = new G4LogicalVolume(towS[t][i],
                    FindMaterial("G4_Galactic"),
                    towLVname[t][i]);
            towPV[t][i] = new G4PVPlacement(0,
                    G4ThreeVector(towposX[t], towposY[t], place),
                    towLV[t][i],
                    towname[t][i],
                    Arm2LV,
                    false,
                    0,
                    checkOverlaps);
            G4cout<<towPV[t][i]->GetTranslation()<<G4endl;
            towposZ[i]=place;
        }
        place=0;
    }
    //
    //
    //

    for(int i=0;i<7;i++) G4cout<<towposZ[i]<<G4endl;
    //
    // define names of components
    //
    //
    // Define GSO Layer -------------------------------------------
    // Layer
    //
    G4VSolid* layerS[2]; //配列化注意
    // G4VSolid* layerS[2][16]; //new
    G4LogicalVolume* layerLV[2][16];

    for(int t=0;t<2;t++){
        layerS[t] = new G4Box(laySname[t],
                layerSizeXY[t]/2, layerSizeXY[t]/2, layerThickness/2);// 配列化注意
        lcur_num = 0;
        wcur_num = 0;
        for(int tow=0;tow<7;tow++){
            for(int l=lcur_num;l<lcur_num+nolet[tow];l++){
                switch(l){
                    case 11:
                    case 13:
                    case 15:
                        wcur_num+=1;
                    case 0:
                        place = -towThickness[tow]/2 + wThickness[t][wcur_num] + whollowThickness[t][wcur_num] + lhollowThickness[t][l] + layerThickness/2;
                        wcur_num+=1;
                        break;
                    case 2:
                    case 5:
                    case 8:
                        // place = -towThickness[tow]/2 + layerThickness/2;
                        place = -towThickness[tow]/2 + lhollowThickness[t][l] + layerThickness/2; //new
                        wcur_num+=1;
                        break;
                    case 12:
                    case 14:
                        // place += wlThickness*2 + layerThickness;
                        place += whollowThickness[t][wcur_num] + wThickness[t][wcur_num] + wThickness[t][wcur_num+1] + whollowThickness[t][wcur_num+1] + lhollowThickness[t][l] + layerThickness; //new
                        wcur_num+=2;
                        break;
                    default:
                        // place += wlThickness + layerThickness;
                        place += whollowThickness[t][wcur_num] + wThickness[t][wcur_num] + lhollowThickness[t][l] + layerThickness; //new
                        wcur_num+=1;
                        break;
                }
                // G4cout << place <<"\n";
                layerLV[t][l] = new G4LogicalVolume(layerS[t], //caution
                        // layerS[t][l],
                        FindMaterial("G4_Galactic"),
                        layLVname[t][l]);
                new G4PVPlacement(0,
                        G4ThreeVector(layerposX[t], layerposY[t], place),
                        layerLV[t][l],
                        layname[t][l],
                        towLV[t][tow],
                        false,
                        0,
                        checkOverlaps);
            }
            lcur_num+=nolet[tow];
        }
    }

    //                           
    //Absorber of Layer
    //
    G4VSolid* gsoS[2];
    G4LogicalVolume* gsoLV[2][16];

    for(int t=0;t<2;t++){
        gsoS[t] = new G4Box(gsoSname[t], layerSizeXY[t]/2, layerSizeXY[t]/2, gsoThickness/2);
        for(int i=0;i<noflayer;i++){
            gsoLV[t][i] = new G4LogicalVolume(gsoS[t], FindMaterial("GSO"), gsoLVname[t][i]);
            new G4PVPlacement(0,
                    // G4ThreeVector(0.,0.,-layerThickness/2+lhollowThickness+acrylThickness+gsoThickness/2+gapThickness), //caution
                    G4ThreeVector(0.,0.,0.), //new
                    gsoLV[t][i],
                    gsoname[t][i],
                    layerLV[t][i],
                    false,
                    0,
                    checkOverlaps);
            gsoLV[t][i]->SetVisAttributes(G4VisAttributes(true,G4Colour(0.,0.,1.0)));
        }
    }
    //
    //Acrylic of Layer
    //
    G4VSolid* acrylS[2];
    G4LogicalVolume* acrylLV[2][32];
    for(int t=0;t<2;t++){
        acrylS[t] = new G4Box(acrylSname[t], layerSizeXY[t]/2, layerSizeXY[t]/2, acrylThickness/2);
        for(int i=0;i<noflayer;i++){
            acrylLV[t][2*i] = new G4LogicalVolume(acrylS[t],FindMaterial("Acrylic"),acrylLVname[t][2*i]);
            new G4PVPlacement(0,
                    // G4ThreeVector(0.,0.,-layerThickness/2+lhollowThickness+acrylThickness/2), //caution
                    G4ThreeVector(0.,0.,-layerThickness/2+acrylThickness/2),
                    acrylLV[t][2*i],
                    acrylname[t][2*i],
                    layerLV[t][i],
                    false,
                    0,
                    checkOverlaps);

            acrylLV[t][2*i+1] = new G4LogicalVolume(acrylS[t],FindMaterial("Acrylic"),acrylLVname[t][2*i+1]);
            new G4PVPlacement(0,
                    // G4ThreeVector(0.,0.,layerThickness/2-acrylThickness/2/*-layerThickness/2+lhollowThickness+acrylThickness/2*/), //caution
                    G4ThreeVector(0.,0.,layerThickness/2-acrylThickness/2), //new
                    acrylLV[t][2*i+1],
                    acrylname[t][2*i+1],
                    layerLV[t][i],
                    false,
                    0,
                    checkOverlaps);
            acrylLV[t][2*i]->SetVisAttributes(G4VisAttributes(true,G4Colour(1.0,1.0,1.0)));
            acrylLV[t][2*i+1]->SetVisAttributes(G4VisAttributes(true,G4Colour(1.0,1.0,1.0)));
        }
    }

    // //
    // // Layers were defined -----------------------------------------
    // //
    // //

    //
    // define W

    G4VSolid* wS[2][22];
    G4LogicalVolume* wLV[2][22];

    for(int t=0;t<2;t++){
        lcur_num=0;
        wcur_num=0;
        for(int tow=0;tow<7;tow++){
            for(int w=wcur_num;w<wcur_num+nowet[tow];w++){
                // wS[t][w]= new G4Box(wSname[t],wSizeX[t]/2,wSizeY[t]/2,wThickness/2);//caution
                wS[t][w]= new G4Box(wSname[t][w],wSizeX[t][w]/2,wSizeY[t][w]/2,wThickness[t][w]/2);
                switch(w){
                    case 0:
                    case 12:
                    case 16:
                    case 20:
                        // place = -towThickness[tow]/2 + whollowThickness + wThickness/2;
                        place = -towThickness[tow]/2 + whollowThickness[t][w] + wThickness[t][w]/2;
                        break;
                    case 3:
                    case 6:
                    case 9:
                        // place = -towThickness[tow]/2 + layerThickness + whollowThickness + wThickness/2; //caution
                        place = -towThickness[tow]/2 + lhollowThickness[t][lcur_num] + layerThickness + whollowThickness[t][w] + wThickness[t][w]/2;
                        lcur_num+=1;
                        break;
                    case 14:
                    case 18:
                        // place += whollowThickness + wThickness;
                        place += whollowThickness[t][w] + wThickness[t][w-1]/2 + wThickness[t][w]/2;
                        break;
                    default:
                        // place += layerThickness + whollowThickness + wThickness;
                        place += lhollowThickness[t][lcur_num] + layerThickness + whollowThickness[t][w] + wThickness[t][w-1]/2 + wThickness[t][w]/2;
                        lcur_num+=1;
                        break;
                }

                wLV[t][w] = new G4LogicalVolume(wS[t][w],FindMaterial("G4_W"),wLVname[t][w]);
                new G4PVPlacement(0,
                        // G4ThreeVector(-towSizeXY[t]/2+wepicsposX[t]+wSizeX[t]/2,-towSizeXY[t]/2+wepicsposY[t]+wSizeY[t]/2,place),
                        G4ThreeVector(-towSizeXY[t]/2+wepicsposX[t][w]+wSizeX[t][w]/2,-towSizeXY[t]/2+wepicsposY[t][w]+wSizeY[t][w]/2,place),
                        wLV[t][w],
                        wname[t][w],
                        towLV[t][tow],
                        false,
                        0,
                        checkOverlaps);
                wLV[t][w]->SetVisAttributes(G4VisAttributes(true,G4Colour(0.47,0.53,0.6)));
            }
            wcur_num+=nowet[tow];
        }
        place = 0;
    }

    //
    // end of W definition

    //
    // define G10
    G4double inradius=0.*mm;
    G4double outradius=2.5*mm;
    G4double startangle[2]={180*deg,0*deg};
    G4double spanning[2]={180*deg,180*deg};
    G4VSolid* HoleS[2][7];
    for(int ud=0;ud<2;ud++){
        for(int i=0;i<7;i++){
            HoleS[ud][i] = new G4Tubs(gholeSname[ud][i],inradius,outradius,towThickness[i]/2,startangle[ud],spanning[ud]);
        }
    }
    G4LogicalVolume* HoleLV[2][4][7];
    G4VPhysicalVolume* HolePV[2][4][7];
    G4double holeposX[4]={gtenSizeX[0]/2-gtenSizeX[1]-outradius,gtenSizeX[0]/2-gtenSizeX[1]-gtenSizeX[2]+outradius,-gtenSizeX[0]/2+gtenSizeX[6]+gtenSizeX[5]-outradius,-gtenSizeX[0]/2+gtenSizeX[6]+outradius};
    G4double holeposY[4]={-Arm2SizeY/2+gtenSizeY[0],Arm2SizeY/2-gtenSizeY[2],-Arm2SizeY/2+gtenSizeY[0]+gtenSizeY[4],Arm2SizeY/2-gtenSizeY[5]};
    G4int posynum[2][4]={{0,0,2,2},{1,1,3,3}};
    for(int ud=0;ud<2;ud++){
        for(int i=0;i<4;i++){
            for(int tow=0;tow<7;tow++){
                HoleLV[ud][i][tow] = new G4LogicalVolume(HoleS[ud][tow],FindMaterial("G4_Galactic"),gholeLVname[ud][i][tow]);
                HolePV[ud][i][tow]=new G4PVPlacement(0,
                        G4ThreeVector(holeposX[i],holeposY[posynum[ud][i]],towposZ[tow]),
                        HoleLV[ud][i][tow],
                        gholename[ud][i][tow],
                        Arm2LV,
                        false,
                        0,
                        checkOverlaps);
                G4cout<<HolePV[ud][i][tow]->GetTranslation()<<G4endl;
            }
        }
    }
    // end of G10 definition
    //

    //                                        
    // Visualization attributes
    //

    G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    simpleBoxVisAtt->SetVisibility(true);
    Arm2LV->SetVisAttributes(simpleBoxVisAtt);

    return Arm2PV;
}
