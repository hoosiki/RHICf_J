#include "RHICFMaterials.hh"

#include "G4SystemOfUnits.hh"

RHICFMaterials* RHICFMaterials::fInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RHICFMaterials::RHICFMaterials()
{
  fNistMan = G4NistManager::Instance();

  fNistMan->SetVerbose(2);

  CreateMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RHICFMaterials::~RHICFMaterials()
{
  delete    fPMMA;
  delete    fPethylene;
  delete    fFPethylene;
  delete    fPolystyrene;
  delete    fSilicone;
  delete    fFPMMA;
  delete    fGSO;
  delete    fGten;
  delete    fAcrylic;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RHICFMaterials* RHICFMaterials::GetInstance()
{
  if (fInstance == 0)
    {
      fInstance = new RHICFMaterials();
    }
  return fInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* RHICFMaterials::GetMaterial(const G4String material)
{
  G4Material* mat =  fNistMan->FindOrBuildMaterial(material);

  if (!mat) mat = G4Material::GetMaterial(material);
  if (!mat) {
     std::ostringstream o;
     o << "Material " << material << " not found!";
     G4Exception("RHICFMaterials::GetMaterial","",
                 FatalException,o.str().c_str());
  }

  return mat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RHICFMaterials::CreateMaterials()
{
  G4double density;
  G4int ncomponents;
  G4double fractionmass;
  std::vector<G4int> natoms;
  std::vector<G4double> fractionMass;
  std::vector<G4String> elements;

  // Materials Definitions
  // =====================

  //--------------------------------------------------
  // Vacuum
  //--------------------------------------------------

  fNistMan->FindOrBuildMaterial("G4_Si");

  //--------------------------------------------------
  // Vacuum
  //--------------------------------------------------

  fNistMan->FindOrBuildMaterial("G4_Galactic");

  //--------------------------------------------------
  // Pb
  //--------------------------------------------------

  fNistMan->FindOrBuildMaterial("G4_Pb");

  //--------------------------------------------------
  // Be
  //--------------------------------------------------

  fNistMan->FindOrBuildMaterial("G4_Be");


  //--------------------------------------------------
  // PL_SCINTI
  //--------------------------------------------------

  fPL_SCINTI = fNistMan->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  //--------------------------------------------------
  // Iron
  //--------------------------------------------------

  fNistMan->FindOrBuildMaterial("G4_Fe");
  //--------------------------------------------------
  // Air
  //--------------------------------------------------

  fAir = fNistMan->FindOrBuildMaterial("G4_AIR");

  //--------------------------------------------------
  // Tungsten
  //--------------------------------------------------

  fNistMan->FindOrBuildMaterial("G4_W");


  //--------------------------------------------------
  // GSO
  //--------------------------------------------------

  elements.push_back("Gd");     natoms.push_back(2);
  elements.push_back("Si");     natoms.push_back(1);
  elements.push_back("O");      natoms.push_back(5);

  density = 6.71*g/cm3;

  fGSO = fNistMan->
          ConstructNewMaterial("GSO", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Gten
  //--------------------------------------------------

  elements.push_back("C");      natoms.push_back(28);
  elements.push_back("H");      natoms.push_back(34);
  elements.push_back("O");      natoms.push_back(22);
  elements.push_back("N");      natoms.push_back(1);
  elements.push_back("Si");     natoms.push_back(6);
  elements.push_back("Ca");     natoms.push_back(3);
  elements.push_back("Al");     natoms.push_back(2);
  elements.push_back("B");      natoms.push_back(4);

  density = 1.75*g/cm3;

  fGten = fNistMan->
          ConstructNewMaterial("Gten", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // G10
  //--------------------------------------------------

  elements.push_back("C");      natoms.push_back(3);
  elements.push_back("H");      natoms.push_back(3);
  elements.push_back("O");      natoms.push_back(2);
  elements.push_back("Si");     natoms.push_back(1);

  density = 1.70*g/cm3;

  fG10 = fNistMan->
          ConstructNewMaterial("G10", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Duralumin
  //--------------------------------------------------

  elements.push_back("Al");      natoms.push_back(19);
  elements.push_back("Cu");      natoms.push_back(1);

  density = 2.7*g/cm3;

  fDuralumin = fNistMan->
          ConstructNewMaterial("Duralumin", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Aacrylic
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(5);
  elements.push_back("H");     natoms.push_back(8);
  elements.push_back("O");     natoms.push_back(2);

  density = 1.190*g/cm3;

  fAcrylic = fNistMan->
          ConstructNewMaterial("Acrylic", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // RHICFfiber PMMA (poly- methylmethacrylate)
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(5);
  elements.push_back("H");     natoms.push_back(8);
  elements.push_back("O");     natoms.push_back(2);

  density = 1.180*g/cm3;

  fPMMA = fNistMan->
          ConstructNewMaterial("PMMA", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Cladding (polyethylene)
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(2);
  elements.push_back("H");     natoms.push_back(4);

  density = 1.200*g/cm3;

  fPethylene = fNistMan->
          ConstructNewMaterial("Pethylene", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Double Cladding (fluorinated PMMA)
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(5);
  elements.push_back("H");     natoms.push_back(8);
  elements.push_back("O");     natoms.push_back(2);

  density = 1.380*g/cm3;

  fFPMMA = fNistMan->
          ConstructNewMaterial("FPMMA", elements, natoms, density);

  elements.clear();
  natoms.clear();


  //--------------------------------------------------
  // Double Cladding (fluorinated polyethylene)
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(2);
  elements.push_back("H");     natoms.push_back(4);

  density = 1.400*g/cm3;

  fFPethylene = fNistMan->
          ConstructNewMaterial("FPethylene", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Polystyrene
  //--------------------------------------------------
 
  elements.push_back("C");     natoms.push_back(8);
  elements.push_back("H");     natoms.push_back(8);

  density = 1.050*g/cm3;

  fPolystyrene = fNistMan->
          ConstructNewMaterial("Polystyrene", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Quartz 
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(2);
  elements.push_back("N");     natoms.push_back(1);
  
  density = 2.64*g/cm3;

  fQuartz = fNistMan->
          ConstructNewMaterial("Quartz", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // QuartzARM1 
  //--------------------------------------------------

  elements.push_back("Si");     natoms.push_back(1);
  elements.push_back("O");     natoms.push_back(2);
  
  density = 2.2*g/cm3;

  fQuartz = fNistMan->
          ConstructNewMaterial("Quartz", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Silicone (Template for Optical Grease)
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(2);
  elements.push_back("H");     natoms.push_back(6);
  
  density = 1.060*g/cm3;

  fSilicone = fNistMan->
          ConstructNewMaterial("Silicone", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Aluminium
  //--------------------------------------------------

  fNistMan->FindOrBuildMaterial("G4_Al");

  //--------------------------------------------------
  // TiO2
  //--------------------------------------------------

  elements.push_back("Ti");     natoms.push_back(1);
  elements.push_back("O");      natoms.push_back(2);

  density     = 4.26*g/cm3;

  G4Material* TiO2 = fNistMan->
          ConstructNewMaterial("TiO2", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Scintillator Coating - 15% TiO2 and 85% polystyrene by weight.
  //--------------------------------------------------

  density = 1.52*g/cm3;

  fCoating =
          new G4Material("Coating", density, ncomponents=2);

  fCoating->AddMaterial(TiO2,         fractionmass = 15*perCent);
  fCoating->AddMaterial(fPolystyrene, fractionmass = 85*perCent);

  //
  // ------------ Generate & Add Material Properties Table ------------
  //

  const G4int nEntries = 50;

  G4double photonEnergy[nEntries] =
  {2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
   2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
   2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
   2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
   2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
   2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
   2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
   3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
   3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
   3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};

  //--------------------------------------------------
  // Air
  //--------------------------------------------------

  G4double refractiveIndex[nEntries] =
  { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};

  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
  mpt->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);

  fAir->SetMaterialPropertiesTable(mpt);

  //--------------------------------------------------
  //  PMMA for RHICFfibers
  //--------------------------------------------------

  G4double refractiveIndexRHICFfiber[nEntries] =
  { 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
    1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
    1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
    1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
    1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49};

  G4double absRHICFfiber[nEntries] =
  {5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,
   5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,
   5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,1.10*m,
   1.10*m,1.10*m,1.10*m,1.10*m,1.10*m,1.10*m, 1.*mm, 1.*mm, 1.*mm, 1.*mm,
    1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm};

  G4double emissionFib[nEntries] =
  {0.05, 0.10, 0.30, 0.50, 0.75, 1.00, 1.50, 1.85, 2.30, 2.75,
   3.25, 3.80, 4.50, 5.20, 6.00, 7.00, 8.50, 9.50, 11.1, 12.4,
   12.9, 13.0, 12.8, 12.3, 11.1, 11.0, 12.0, 11.0, 17.0, 16.9,
   15.0, 9.00, 2.50, 1.00, 0.05, 0.00, 0.00, 0.00, 0.00, 0.00,
   0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};

  // Add entries into properties table
  G4MaterialPropertiesTable* mptRHICFfiber = new G4MaterialPropertiesTable();
  mptRHICFfiber->AddProperty("RINDEX",photonEnergy,refractiveIndexRHICFfiber,nEntries);
  // mptRHICFfiber->AddProperty("ABSLENGTH",photonEnergy,absRHICFfiber,nEntries);
  mptRHICFfiber->AddProperty("RHICFABSLENGTH",photonEnergy,absRHICFfiber,nEntries);
  mptRHICFfiber->AddProperty("RHICFCOMPONENT",photonEnergy,emissionFib,nEntries);
  mptRHICFfiber->AddConstProperty("RHICFTIMECONSTANT", 0.5*ns);

  fPMMA->SetMaterialPropertiesTable(mptRHICFfiber);
  fAcrylic->SetMaterialPropertiesTable(mptRHICFfiber);

  //--------------------------------------------------
  //  Polyethylene
  //--------------------------------------------------

  G4double refractiveIndexClad1[nEntries] =
  { 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
    1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
    1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
    1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
    1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49};

  G4double absClad[nEntries] =
  {20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m};

  // Add entries into properties table
  G4MaterialPropertiesTable* mptClad1 = new G4MaterialPropertiesTable();
  mptClad1->AddProperty("RINDEX",photonEnergy,refractiveIndexClad1,nEntries);
  mptClad1->AddProperty("ABSLENGTH",photonEnergy,absClad,nEntries);

  fPethylene->SetMaterialPropertiesTable(mptClad1);

  //--------------------------------------------------
  // Fluorinated Polyethylene
  //--------------------------------------------------

   G4double refractiveIndexClad2[nEntries] =
   { 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40,
     1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40,
     1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40,
     1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40,
     1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40};

  // Add entries into properties table
  G4MaterialPropertiesTable* mptClad2 = new G4MaterialPropertiesTable();
  mptClad2->AddProperty("RINDEX",photonEnergy,refractiveIndexClad2,nEntries);
  mptClad2->AddProperty("ABSLENGTH",photonEnergy,absClad,nEntries);

  fFPethylene->SetMaterialPropertiesTable(mptClad2);

  //--------------------------------------------------
  // Fluorinated PMMA
  //--------------------------------------------------

   G4double refractiveIndexFPMMA[nEntries] =
   { 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40,
     1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40,
     1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40,
     1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40,
     1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40, 1.40};

  // Add entries into properties table
  G4MaterialPropertiesTable* mptFPMMA = new G4MaterialPropertiesTable();
  mptFPMMA->AddProperty("RINDEX",photonEnergy,refractiveIndexFPMMA,nEntries);
  mptFPMMA->AddProperty("ABSLENGTH",photonEnergy,absClad,nEntries);

  fFPMMA->SetMaterialPropertiesTable(mptFPMMA);
  //--------------------------------------------------
  // Silicone
  //--------------------------------------------------

   G4double refractiveIndexSilicone[nEntries] =
   { 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46};

  // Add entries into properties table
  G4MaterialPropertiesTable* mptSilicone = new G4MaterialPropertiesTable();
  mptSilicone->
           AddProperty("RINDEX",photonEnergy,refractiveIndexSilicone,nEntries);
  mptSilicone->AddProperty("ABSLENGTH",photonEnergy,absClad,nEntries);

  fSilicone->SetMaterialPropertiesTable(mptSilicone);

  //--------------------------------------------------
  //  Polystyrene
  //--------------------------------------------------

  G4double refractiveIndexPS[nEntries] =
  { 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50};

  G4double absPS[nEntries] =
  {2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,
   2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,
   2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,
   2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,
   2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm,2.*cm};

  G4double scintilFast[nEntries] =
  {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
   1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
   1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  
  // Add entries into properties table
  G4MaterialPropertiesTable* mptPL_SCINTI = new G4MaterialPropertiesTable();
  mptPL_SCINTI->AddProperty("RINDEX",photonEnergy,refractiveIndexPS,nEntries);
  mptPL_SCINTI->AddProperty("ABSLENGTH",photonEnergy,absPS,nEntries);
  mptPL_SCINTI->
               AddProperty("FASTCOMPONENT",photonEnergy, scintilFast,nEntries);
  mptPL_SCINTI->AddConstProperty("SCINTILLATIONYIELD",10./keV);
  mptPL_SCINTI->AddConstProperty("RESOLUTIONSCALE",1.0);
  mptPL_SCINTI->AddConstProperty("FASTTIMECONSTANT", 10.*ns);
 
  fPL_SCINTI->SetMaterialPropertiesTable(mptPL_SCINTI);

  // Set the Birks Constant for the PL scintillator

  fPL_SCINTI->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

}
