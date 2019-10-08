// -----------------------------------------------------------------------------
//  G4Basic | DetectorConstruction.cpp
//
//  Definition of detector geometry and materials.
//   * Author: Taylor Contreras, Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "DetectorConstruction.h"

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
  
  // Optical Surface Properties (making the barrel reflective)
  G4OpticalSurface* refl_surface = new G4OpticalSurface("REFL_SURFACE");
  refl_surface->SetMaterialPropertiesTable(PTFE());

  G4Tubs* barrel_solid_vol =
    new G4Tubs(barrel_name, barrel_inner_diam/2, barrel_inner_diam/2.+barrel_thickness, 
		barrel_length/2., 0, 360.*deg);

  G4LogicalVolume* barrel_logic_vol = 
    new G4LogicalVolume(barrel_solid_vol, barrel_mat, barrel_name);
  new G4LogicalSkinSurface("REFL_BARREL", barrel_logic_vol, refl_surface);

  new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
		barrel_logic_vol, barrel_name, world_logic_vol, false, 0, true);
  
  // TRACKING PLANE //////////////////////////////////////////////
  // FIXME: make optical surface with effecienty (rather than sensitive detector)

  G4String tracking_name = "TRACKING_PLANE";
  G4double tracking_diam = barrel_inner_diam + barrel_thickness*2.;
  G4double tracking_length = 12.*cm;
  G4ThreeVector tracking_pos = G4ThreeVector(0., 0., xenon_length/2. + tracking_length/2.);
  G4Material* tracking_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  
  G4Tubs* tracking_solid_vol = 
    new G4Tubs(tracking_name, 0., tracking_diam/2., tracking_length/2., 0., 360.*deg);
  
  G4LogicalVolume* tracking_logic_vol = 
    new G4LogicalVolume(tracking_solid_vol, tracking_mat, tracking_name);

  // Optical Properties of plane (giving it a photon detection efficiency)
  G4OpticalSurface* tracking_plane = new G4OpticalSurface("TRACKING_SURFACE");
  tracking_plane->SetMaterialPropertiesTable(OpticalPlane());
  new G4LogicalSkinSurface("TRACKING_PLANE", tracking_logic_vol, tracking_plane);

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

  // Optical Properties of plane (giving it a photon detection efficiency)
  G4OpticalSurface* energy_plane = new G4OpticalSurface("ENERGY_SURFACE");
  energy_plane->SetMaterialPropertiesTable(OpticalPlane());
  new G4LogicalSkinSurface("ENERGY_PLANE", energy_logic_vol, energy_plane);

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


G4Material* DetectorConstruction::DefineXenon() const{
  // Defines the material and optical properties of gaseous xenon

  G4String material_name = "GXe";
  G4double density = 88.56 * kg/m3;
  //G4double pressure = 15.0 * bar;
  G4double temperature = 300. * kelvin;
  G4double sc_yield = 20000*1/MeV; // Estimated ~50 photons/eV

  G4Material* material = new G4Material(material_name, density, 1,
			    kStateGas, temperature, fpressure);
  G4Element* Xe = G4NistManager::Instance()->FindOrBuildElement("Xe");
  material->AddElement(Xe,1);
  
  // Optical Properties of Xenon
  // From Geant4 Liquid Xenon example
  // FIXME: what should these number be for this gasous xenon detector???
  const G4int GXe_NUMENTRIES = 3;
  G4double GXe_Energy[GXe_NUMENTRIES]    = { 7.0*eV , 7.07*eV, 7.14*eV };
  G4double GXe_SCINT[GXe_NUMENTRIES] = { 0.1, 1.0, 0.1 };
  G4double GXe_RIND[GXe_NUMENTRIES]  = {1.0, 1.0, 1.0}; 
  G4double GXe_ABSL[GXe_NUMENTRIES]  = {1.e8*m, 1.e8*m, 1.e8*m};

  G4MaterialPropertiesTable* GXe_mt = new G4MaterialPropertiesTable();

  GXe_mt->AddProperty("FASTCOMPONENT", GXe_Energy, GXe_SCINT, GXe_NUMENTRIES);
  GXe_mt->AddProperty("RINDEX",        GXe_Energy, GXe_RIND,  GXe_NUMENTRIES);
  GXe_mt->AddProperty("ABSLENGTH",     GXe_Energy, GXe_ABSL,  GXe_NUMENTRIES);
  GXe_mt->AddConstProperty("SCINTILLATIONYIELD",sc_yield); 
  GXe_mt->AddConstProperty("RESOLUTIONSCALE",1.0);
  GXe_mt->AddConstProperty("FASTTIMECONSTANT",1.*ns);
  GXe_mt->AddConstProperty("YIELDRATIO",1.0);
  GXe_mt->AddProperty("ELSPECTRUM", GXe_Energy, GXe_SCINT, GXe_NUMENTRIES);  
  GXe_mt->AddConstProperty("ELTIMECONSTANT", 50.*ns);

  material->SetMaterialPropertiesTable(GXe_mt);
  
  return material;
}


G4MaterialPropertiesTable* DetectorConstruction::PTFE(){
  // Defines the optical properties for PTFE

  G4MaterialPropertiesTable* teflon_mpt = new G4MaterialPropertiesTable();

  // define props for a given number of energies
  const G4int REFL_NUMENTRIES = 2;
  G4double ENERGIES[REFL_NUMENTRIES] = {1.0*eV, 30.*eV};
  G4double REFLECTIVITY[REFL_NUMENTRIES] = {.72, .72};
  
  teflon_mpt->AddProperty("REFLECTIVITY", ENERGIES, REFLECTIVITY, REFL_NUMENTRIES);
  
  return teflon_mpt;
}

G4MaterialPropertiesTable* DetectorConstruction::OpticalPlane(){
  // Defines the optical properties for the detector planes

  G4MaterialPropertiesTable* plane_mpt = new G4MaterialPropertiesTable();

  // define props for a given number of energies
  const G4int NUMENTRIES = 2;
  G4double ENERGIES[NUMENTRIES] = {1.0*eV, 30.*eV};
  G4double EFFICIENCY[NUMENTRIES] = {1.0, 1.0};
  G4double RINDEX[NUMENTRIES] = {1.0, 1.0};
  G4double REFLECTIVITY[NUMENTRIES] = {0, 0};

  plane_mpt->AddProperty("EFFICIENCY", ENERGIES, EFFICIENCY, NUMENTRIES);
  plane_mpt->AddProperty("RINDEX", ENERGIES, RINDEX, NUMENTRIES);
  plane_mpt->AddProperty("REFLECTIVITY", ENERGIES, REFLECTIVITY, NUMENTRIES);
  
  return plane_mpt;
}
