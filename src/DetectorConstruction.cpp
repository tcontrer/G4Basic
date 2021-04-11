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
#include <G4LogicalBorderSurface.hh>

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),
    fpressure(15.*bar)
{
}


DetectorConstruction::~DetectorConstruction()
{
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  /////////////////////////////////////////////////////////////////////////////
  // WORLD
  /////////////////////////////////////////////////////////////////////////////

  G4String world_name = "WORLD";
  G4double world_size = 15.*m;
  G4Material* world_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
  world_mat->SetMaterialPropertiesTable(TransparentMaterialsTable());

  G4Box* world_solid_vol =
    new G4Box(world_name, world_size/2., world_size/2., world_size/2.);

  G4LogicalVolume* world_logic_vol =
    new G4LogicalVolume(world_solid_vol, world_mat, world_name);
  world_logic_vol->SetVisAttributes(G4VisAttributes::Invisible);

  G4VPhysicalVolume* world_phys_vol =
    new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
                      world_logic_vol, world_name, 0, false, 0, true);

  /////////////////////////////////////////////////////////////////////////////
  // ARGON CHAMBER
  /////////////////////////////////////////////////////////////////////////////

  G4String argon_name = "ARGON";
  G4double argon_vertical   = 2.325*m;
  G4double argon_horizontal = 2.560*m;
  G4double argon_logitudinal = 10.368*m;
  G4Material* argon_mat = DefineArgon();

  G4Box* argon_solid_vol =
    new G4Box(argon_name, argon_horizontal/2., argon_vertical/2., argon_logitudinal/2.);

  G4LogicalVolume* argon_logic_vol =
    new G4LogicalVolume(argon_solid_vol, argon_mat, argon_name);

  G4VPhysicalVolume* argon_phys = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
					       argon_logic_vol, argon_name, world_logic_vol, false, 0, true);


  /////////////////////////////////////////////////////////////////////////////
  // ENERGY PLANE
  /////////////////////////////////////////////////////////////////////////////
  /*
  G4String tracking_name = "TRACKING_PLANE";
  G4double tracking_diam = barrel_inner_diam + barrel_thickness*2.;
  G4double tracking_length = 12.*cm;
  G4ThreeVector tracking_pos = G4ThreeVector(0., 0., argon_length/2. + tracking_length/2.);
  G4Material* tracking_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  tracking_mat->SetMaterialPropertiesTable(TransparentMaterialsTable());

  G4Tubs* tracking_solid_vol =
    new G4Tubs(tracking_name, 0., tracking_diam/2., tracking_length/2., 0., 360.*deg);

  G4LogicalVolume* tracking_logic_vol =
    new G4LogicalVolume(tracking_solid_vol, tracking_mat, tracking_name);

  // Optical Properties of plane (giving it a photon detection efficiency)
  G4OpticalSurface* tracking_plane = new G4OpticalSurface("TRACKING_SURFACE");

  new G4LogicalSkinSurface("TRACKING_PLANE", tracking_logic_vol, tracking_plane);
  tracking_plane->SetMaterialPropertiesTable(OpticalPlane());

  new G4PVPlacement(0, tracking_pos,
  		    tracking_logic_vol, tracking_name, world_logic_vol, false, 0, true);

  /////////////////////////////////////////////////////////////////////////////
  // ENERGY PLANE
  /////////////////////////////////////////////////////////////////////////////

  G4String energy_name = "ENERGY_PLANE";
  G4double energy_diam = barrel_inner_diam + barrel_thickness*2.;
  G4double energy_length = 12.*cm;
  G4ThreeVector energy_pos = G4ThreeVector(0., 0., -(argon_length/2. + tracking_length/2.));
  G4Material* energy_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  energy_mat->SetMaterialPropertiesTable(TransparentMaterialsTable());

  G4Tubs* energy_solid_vol =
    new G4Tubs(energy_name, 0., energy_diam/2., energy_length/2., 0., 360.*deg);

  G4LogicalVolume* energy_logic_vol =
    new G4LogicalVolume(energy_solid_vol, energy_mat, energy_name);

  // Optical Properties of plane (giving it a photon detection efficiency)
  G4OpticalSurface* energy_plane = new G4OpticalSurface("ENERGY_SURFACE");

  new G4LogicalSkinSurface("ENERGY_PLANE", energy_logic_vol, energy_plane);
  energy_plane->SetMaterialPropertiesTable(OpticalPlane());

  new G4PVPlacement(0, energy_pos,
                    energy_logic_vol, energy_name, world_logic_vol, false, 0, true);
  */
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // SET VISIBILITY ATTRIBUTES

  G4VisAttributes* Blue = new G4VisAttributes(G4Colour(0,0.5,1.0,0.1));
  G4VisAttributes* Grey = new G4VisAttributes(G4Colour(.5, .5, .5, .4));
  G4VisAttributes* Yellow = new G4VisAttributes(G4Colour(1.0, 1.0, 0, .1));
  //tracking_logic_vol->SetVisAttributes(Yellow);
  //barrel_logic_vol->SetVisAttributes(Grey);
  argon_logic_vol->SetVisAttributes(Blue);
  //energy_logic_vol->SetVisAttributes(Yellow);


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

  return world_phys_vol;
}


G4Material* DetectorConstruction::DefineArgon() const{
  // Defines the material and optical properties of gaseous argon

  G4String material_name = "GAr";
  G4double density = 1.394 * g/cm3;
  //G4double pressure = 1.24 * bar;
  G4double temperature = 87. * kelvin;
  G4double sc_yield = 20000*1/MeV; // Estimated ~50 photons/eV

  G4Material* material = new G4Material(material_name, density, 1,
			    kStateGas, temperature, fpressure);
  G4Element* Xe = G4NistManager::Instance()->FindOrBuildElement("Ar");
  material->AddElement(Xe,1);

  // Optical Properties of argon
  // From Geant4 Liquid argon example
  // FIXME: what should these number be for this gasous argon detector???
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

G4MaterialPropertiesTable* DetectorConstruction::TransparentMaterialsTable(){
  // Make a material transparent to optical photons

  const G4int NUMENTRIES = 2;
  G4double ENERGIES[NUMENTRIES] = {7.0*eV, 7.07*eV};
  G4double RIND[NUMENTRIES] = {1.0, 1.0};
  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
  mpt->AddProperty("RINDEX", ENERGIES, RIND, NUMENTRIES);

  return mpt;
}
