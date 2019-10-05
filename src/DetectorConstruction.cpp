// -----------------------------------------------------------------------------
//  G4Basic | DetectorConstruction.cpp
//
//  Definition of detector geometry and materials.
//   * Author: Justo Martin-Albo, Taylor Contreras
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "DetectorConstruction.h"
#include "SDPlanes.h"

#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4VisAttributes.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4SDManager.hh>

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),
    fEnergyPlane(0),
    fTrackingPlane(0),
    fpressure(15.*bar)
{
}


DetectorConstruction::~DetectorConstruction()
{
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // WORLD /////////////////////////////////////////////////

  G4String world_name = "WORLD";
  G4double world_size = 10.*m;
  G4Material* world_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

  G4Box* world_solid_vol =
    new G4Box(world_name, world_size/2., world_size/2., world_size/2.);

  G4LogicalVolume* world_logic_vol =
    new G4LogicalVolume(world_solid_vol, world_mat, world_name);
  world_logic_vol->SetVisAttributes(G4VisAttributes::Invisible);

  G4VPhysicalVolume* world_phys_vol =
    new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
                      world_logic_vol, world_name, 0, false, 0, true);

  // XENON CHAMBER //////////////////////////////////////////////

  G4String xenon_name = "XENON";
  G4double xenon_diam   = 1.0*m;
  G4double xenon_length = 1.0*m;
  G4Material* xenon_mat = DefineXenon();

  G4Tubs* xenon_solid_vol =
    new G4Tubs(xenon_name, 0., xenon_diam/2., xenon_length/2., 0., 360.*deg);

  G4LogicalVolume* xenon_logic_vol =
    new G4LogicalVolume(xenon_solid_vol, xenon_mat, xenon_name);

  new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
                    xenon_logic_vol, xenon_name, world_logic_vol, false, 0, true);

  // REFLECTIVE BARREL ////////////////////////////////////////////

  G4String barrel_name = "BARREL";
  G4double barrel_inner_diam = xenon_diam;
  G4double barrel_thickness = 5.0*cm;
  G4double barrel_length = xenon_length;
  G4Material* barrel_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYETHYLENE");
  
  // OPTICAL SURFACE PROPERTIES
  G4OpticalSurface* refl_surface = new G4OpticalSurface("REFL_SURFACE");
  refl_surface->SetMaterialPropertiesTable(PTFE());

  G4Tubs* barrel_solid_vol =
    new G4Tubs(barrel_name, barrel_inner_diam/2, barrel_inner_diam/2.+barrel_thickness, 
		barrel_length/2., 0, 360.*deg);

  G4LogicalVolume* barrel_logic_vol = 
    new G4LogicalVolume(barrel_solid_vol, barrel_mat, barrel_name);
  new G4LogicalSkinSurface("REFL_BARREL", barrel_logic_vol, refl_surface); // make surface reflective

  new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
		barrel_logic_vol, barrel_name, world_logic_vol, false, 0, true);
  
  // TRACKING PLANE //////////////////////////////////////////////
  // FIXME: make sensitive detector

  G4String tracking_name = "TRACKING_PLANE";
  G4double tracking_diam = barrel_inner_diam + barrel_thickness*2.;
  G4double tracking_length = 12.*cm;
  G4ThreeVector tracking_pos = G4ThreeVector(0., 0., xenon_length/2. + tracking_length/2.);
  G4Material* tracking_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  
  G4Tubs* tracking_solid_vol = 
    new G4Tubs(tracking_name, 0., tracking_diam/2., tracking_length/2., 0., 360.*deg);
  
  G4LogicalVolume* tracking_logic_vol = 
    new G4LogicalVolume(tracking_solid_vol, tracking_mat, tracking_name);

  // Define this volume as sensistive plane detector
  SDPlanes* trackingplane = new SDPlanes("/TrackingPlane");
  tracking_logic_vol->SetSensitiveDetector(trackingplane);
  G4SDManager::GetSDMpointer()->AddNewDetector(trackingplane);

  new G4PVPlacement(0, tracking_pos, 
		    tracking_logic_vol, tracking_name, world_logic_vol, false, 0, true);

  // ENERGY PLANE //////////////////////////////////////////////                                                   
  G4String energy_name = "ENERGY_PLANE";
  G4double energy_diam = barrel_inner_diam + barrel_thickness*2.;
  G4double energy_length = 12.*cm;
  G4ThreeVector energy_pos = G4ThreeVector(0., 0., -(xenon_length/2. + tracking_length/2.));
  G4Material* energy_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");

  G4Tubs* energy_solid_vol =
    new G4Tubs(energy_name, 0., energy_diam/2., energy_length/2., 0., 360.*deg);

  G4LogicalVolume* energy_logic_vol =
    new G4LogicalVolume(energy_solid_vol, energy_mat, energy_name);

  new G4PVPlacement(0, energy_pos,
                    energy_logic_vol, energy_name, world_logic_vol, false, 0, true);

  
  //////////////////////////////////////////////////////////


  // SET VISIBILITY ATTRIBUTES ///////////////////////////////
  G4VisAttributes* Blue = new G4VisAttributes(G4Colour(0,0.5,1.0,0.1));
  G4VisAttributes* Grey = new G4VisAttributes(G4Colour(.5, .5, .5, .4));
  G4VisAttributes* Yellow = new G4VisAttributes(G4Colour(1.0, 1.0, 0, .1));
  tracking_logic_vol->SetVisAttributes(Yellow);
  barrel_logic_vol->SetVisAttributes(Grey);
  xenon_logic_vol->SetVisAttributes(Blue);
  energy_logic_vol->SetVisAttributes(Yellow);


  ///////////////////////////////////////////////////////////

  fEnergyPlane = energy_logic_vol;
  fTrackingPlane = tracking_logic_vol;

  return world_phys_vol;
}


G4Material* DetectorConstruction::DefineNeon() const
{
  G4String material_name = "GNe";
  G4Material* material = G4Material::GetMaterial("material_name", false);

  if (!material) {
    G4double density = 16.0 * kg/m3;
    //G4double pressure = 20.0 * bar;
    G4double temperature = 300. * kelvin;
    material = new G4Material(material_name, density, 1,
			                        kStateGas, temperature, fpressure);
    G4Element* Ne = G4NistManager::Instance()->FindOrBuildElement("Ne");
    material->AddElement(Ne,1);
  }

  return material;
}

G4Material* DetectorConstruction::DefineXenon() const
{
  G4String material_name = "GXe";
  G4double density = 88.56 * kg/m3;
  //G4double pressure = 15.0 * bar;
  G4double temperature = 300. * kelvin;
  G4double sc_yield = 20000*1/MeV; // Estimated ~50 photons/eV

  G4Material* material = new G4Material(material_name, density, 1,
			    kStateGas, temperature, fpressure);
  G4Element* Xe = G4NistManager::Instance()->FindOrBuildElement("Xe");
  material->AddElement(Xe,1);
  
  /*
  // Add optical properties
  G4MaterialPropertiesTable* GXe_mpt = new G4MaterialPropertiesTable();
  const G4int NUMENTRIES = 9;
  G4double ri_energy[NUMENTRIES] 
    = {1*eV, 2*eV, 3*eV, 5*eV, 6*eV, 7*eV, 8*eV, 9*eV};
  
  G4double rindex[NUMENTRIES];

  for (G4int i=0; i<NUMENTRIES; i++) {
    rindex[i] = RefractiveIndex(ri_energy[i]);
    G4cout << ri_energy[i]/eV <<  ", " << rindex[i] << G4endl;                               
  }

  // Sampling from ~150 nm to 200 nm <----> from 6.20625 eV to 8.20625 eV                                  
  const G4int sc_entries = 500;
  G4double sc_energy[NUMENTRIES] = {6.6*eV, 6.7*eV, 6.8*eV, 6.9*eV,
				    7.0*eV, 7.1*eV, 7.2*eV, 7.3*eV, 7.4*eV };
  //for (int j=0;j<sc_entries;j++){
  //  sc_energy[j] =  6.20625*eV + 0.004*j*eV;
  //}
  
  G4double intensity[NUMENTRIES]; // = { 0.000134, 0.004432, 0.053991, 0.241971,
  //			      0.398942, 0.000134, 0.004432, 0.053991,
  //			      0.241971 };
  for (G4int i=0; i<NUMENTRIES; i++){
    intensity[i] = Scintillation(sc_energy[i]);
    G4cout << intensity[i] << ", " << sc_energy[i]/eV << G4endl;
  }

  GXe_mpt->AddProperty("RINDEX", sc_energy, rindex, NUMENTRIES);
  GXe_mpt->AddProperty("FASTCOMPONENT", sc_energy, intensity, NUMENTRIES);
  //GXe_mpt->AddProperty("ELSPECTRUM", sc_energy, intensity, sc_entries);
  //GXe_mpt->AddProperty("SLOWCOMPONENT", sc_energy, intensity, sc_entries);
  GXe_mpt->AddConstProperty("SCINTILLATIONYIELD", sc_yield);
  //GXe_mpt->AddConstProperty("RESOLUTIONSCALE", 1.0);
  GXe_mpt->AddConstProperty("FASTTIMECONSTANT",1.0*ns); // emmisiont timeing of photons
  //GXe_mpt->AddConstProperty("SLOWTIMECONSTANT",100.*ns);
  //GXe_mpt->AddConstProperty("ELTIMECONSTANT", 50.*ns);                                                 
  GXe_mpt->AddConstProperty("YIELDRATIO",.1);
  //GXe_mpt->AddConstProperty("ATTACHMENT", 1000.*ms);

  G4double energy[2] = {0.01*eV, 100.*eV};
  G4double abslen[2] = {1.e8*m, 1.e8*m};
  //GXe_mpt->AddProperty("ABSLENGTH", energy, abslen, 2);
  
  material->SetMaterialPropertiesTable(GXe_mpt);
  */
  
  // Geant4 Liquid Xenon example
  const G4int LXe_NUMENTRIES = 3;
  G4double LXe_Energy[LXe_NUMENTRIES]    = { 7.0*eV , 7.07*eV, 7.14*eV };

  G4double LXe_SCINT[LXe_NUMENTRIES] = { 0.1, 1.0, 0.1 };
  G4double LXe_RIND[LXe_NUMENTRIES]  = {1.0, 1.0, 1.0};//{ 1.59 , 1.57, 1.54 };
  G4double LXe_ABSL[LXe_NUMENTRIES]  = {1.e8*m, 1.e8*m, 1.e8*m}; //{ 35.*cm, 35.*cm, 35.*cm}; 
  G4MaterialPropertiesTable* LXe_mt = new G4MaterialPropertiesTable();
  LXe_mt->AddProperty("FASTCOMPONENT", LXe_Energy, LXe_SCINT, LXe_NUMENTRIES);
  //LXe_mt->AddProperty("SLOWCOMPONENT", LXe_Energy, LXe_SCINT, LXe_NUMENTRIES);
  LXe_mt->AddProperty("RINDEX",        LXe_Energy, LXe_RIND,  LXe_NUMENTRIES);
  LXe_mt->AddProperty("ABSLENGTH",     LXe_Energy, LXe_ABSL,  LXe_NUMENTRIES);
  LXe_mt->AddConstProperty("SCINTILLATIONYIELD",sc_yield); 
  LXe_mt->AddConstProperty("RESOLUTIONSCALE",1.0);
  LXe_mt->AddConstProperty("FASTTIMECONSTANT",1.*ns);
  //LXe_mt->AddConstProperty("SLOWTIMECONSTANT",45.*ns);
  LXe_mt->AddConstProperty("YIELDRATIO",1.0);

  LXe_mt->AddProperty("ELSPECTRUM", LXe_Energy, LXe_SCINT, LXe_NUMENTRIES);  
  LXe_mt->AddConstProperty("ELTIMECONSTANT", 50.*ns);

  material->SetMaterialPropertiesTable(LXe_mt);
  
  return material;
}


G4MaterialPropertiesTable* DetectorConstruction::PTFE(){

  G4MaterialPropertiesTable* teflon_mpt = new G4MaterialPropertiesTable();

  // define props for a given number of energies
  const G4int REFL_NUMENTRIES = 2;

  G4double ENERGIES[REFL_NUMENTRIES] = {1.0*eV, 30.*eV};
  G4double REFLECTIVITY[REFL_NUMENTRIES] = {.72, .72};
  
  teflon_mpt->AddProperty("REFLECTIVITY", ENERGIES, REFLECTIVITY, REFL_NUMENTRIES);
  
  return teflon_mpt;
}

G4double DetectorConstruction::Scintillation(G4double energy) const
{
  // FWHM and peak of emission extracted from paper:                                                     
  // Physical review A, Volume 9, Number 2,                                                              
  // February 1974. Koehler, Ferderber, Redhead and Ebert.                                               
  // Pressure must be in atm = bar                                                                       

  G4double pressure = fpressure/atmosphere;

  G4double Wavelength_peak = (0.05 * pressure + 169.45)*nm;
    G4double Wavelength_sigma =
      2.*sqrt(2*log(2)) * (-0.117 * pressure + 15.42)*nm;

    G4double Energy_peak = (CLHEP::h_Planck*CLHEP::c_light/Wavelength_peak);
    G4double Energy_sigma = (CLHEP::h_Planck*CLHEP::c_light*Wavelength_sigma/pow(Wavelength_peak,2));
    // G4double bin = 6*Energy_sigma/500;                                                                  

    G4double intensity =
      exp(-pow(Energy_peak/eV-energy/eV,2)/(2*pow(Energy_sigma/eV, 2)))/(Energy_sigma/eV*sqrt(CLHEP::pi*2.));

    return intensity;
}


G4double DetectorConstruction::RefractiveIndex(G4double energy) const
{
  
  // Formula for the refractive index taken from                                                         
  // A. Baldini et al., "Liquid Xe scintillation calorimetry                                             
  // and Xe optical properties", arXiv:physics/0401072v1 [physics.ins-det]                               

  // The Lorentz-Lorenz equation (also known as Clausius-Mossotti equation)                              
  // relates the refractive index of a fluid with its density:                                           
  // (n^2 - 1) / (n^2 + 2) = - A · d_M,     (1)                                                          
  // where n is the refractive index, d_M is the molar density and                                       
  // A is the first refractivity viral coefficient:                                                      
  // A(E) = \sum_i^3 P_i / (E^2 - E_i^2),   (2)                                                          
  // with:                                                                                               
  G4double P[3] = {71.23, 77.75, 1384.89}; // [eV^3 cm3 / mole]                                          
  G4double E[3] = {8.4, 8.81, 13.2};       // [eV]                                                       

  // Note.- Equation (1) has, actually, a sign difference with respect                                   
  // to the one appearing in the reference. Otherwise, it yields values                                  
  // for the refractive index below 1.                                                                   

  // Let's calculate the virial coefficient.                                                             
  // We won't use the implicit system of units of Geant4 because                                         
  // it results in loss of numerical precision.                                                          

  energy = energy / eV;
  G4double virial = 0.;

  for (G4int i=0; i<3; i++)
    virial = virial + P[i] / (energy*energy - E[i]*E[i]);

  // Need to use g/cm3                                                                                   
  G4double density = 88.56 * kg/m3; //Density(_pressure) / g * cm3;

  G4double mol_density = density / 131.29;
  G4double alpha = virial * mol_density;

  // Isolating now the n2 from equation (1) and taking the square root                                   
  G4double n2 = (1. - 2*alpha) / (1. + alpha);
  if (n2 < 1.) {
    //      G4String msg = "Non-physical refractive index for energy "                                        
    // + bhep::to_string(energy) + " eV. Use n=1 instead.";                                            
    //      G4Exception("[XenonGasProperties]", "RefractiveIndex()",                                          
    //        JustWarning, msg);                                                                       
    n2 = 1.;
  }
  
  
  // Currently don't care about changing index based on energy
  return sqrt(n2);
}
