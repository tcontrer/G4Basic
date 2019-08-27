// -----------------------------------------------------------------------------
//  G4Basic | DetectorConstruction.cpp
//
//  Definition of detector geometry and materials.
//   * Author: Justo Martin-Albo, Taylor Contreras
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

DetectorConstruction::DetectorConstruction(): G4VUserDetectorConstruction()
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
  
  G4String tracking_name = "TRACKING_PLANE";
  G4double tracking_diam = barrel_inner_diam + barrel_thickness*2.;
  G4double tracking_length = 12.*cm;
  G4ThreeVector tracking_pos = G4ThreeVector(0., 0., xenon_length/2. + tracking_length/2.);
  G4Material* tracking_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  
  G4Tubs* tracking_solid_vol = 
    new G4Tubs(tracking_name, 0., tracking_diam/2., tracking_length/2., 0., 360.*deg);
  
  G4LogicalVolume* tracking_logic_vol = 
    new G4LogicalVolume(tracking_solid_vol, tracking_mat, tracking_name);

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
  return world_phys_vol;
}


G4Material* DetectorConstruction::DefineNeon() const
{
  G4String material_name = "GNe";
  G4Material* material = G4Material::GetMaterial("material_name", false);

  if (!material) {
    G4double density = 16.0 * kg/m3;
    G4double pressure = 20.0 * bar;
    G4double temperature = 300. * kelvin;
    material = new G4Material(material_name, density, 1,
			                        kStateGas, temperature, pressure);
    G4Element* Ne = G4NistManager::Instance()->FindOrBuildElement("Ne");
    material->AddElement(Ne,1);
  }

  return material;
}

G4Material* DetectorConstruction::DefineXenon() const
{
  G4String material_name = "GXe";
  G4double density = 88.56 * kg/m3;
  G4double pressure = 15.0 * bar;
  G4double temperature = 300. * kelvin;

  G4Material* material = new G4Material(material_name, density, 1,
			    kStateGas, temperature, pressure);
  G4Element* Xe = G4NistManager::Instance()->FindOrBuildElement("Xe");
  material->AddElement(Xe,1);
  
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
