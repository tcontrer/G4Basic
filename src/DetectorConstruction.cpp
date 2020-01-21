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
    fTrackingPlane(0),
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
  G4double world_size = 1.*m;
  G4Material* world_mat =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
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
  // TEFLON
  /////////////////////////////////////////////////////////////////////////////
  // FIXME: sizes and distances???
  G4String teflon_name = "TEFLON";
  G4double teflon_len = 10.*cm;
  G4double teflon_width = 1.5*mm; // 0.5, 1.0, 1.5 mm
  G4double teflon_xpos = 10.*cm;
  G4ThreeVector teflon_pos = G4ThreeVector(0., 0., teflon_xpos);
  G4Material* teflon_mat =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_TEFLON");

  G4Box* teflon_solid_vol =
    new G4Box(teflon_name, teflon_len/2., teflon_len/2., teflon_width/2.);
  G4LogicalVolume* teflon_logic_vol =
    new G4LogicalVolume(teflon_solid_vol, teflon_mat, teflon_name);

  teflon_mat->SetMaterialPropertiesTable(TeflonMaterialsTable());

  G4VPhysicalVolume* teflon_phys_vol = new G4PVPlacement(0, teflon_pos,
    teflon_logic_vol, teflon_name, world_logic_vol, false, 0, true);
  // Optical Properties of teflon surface
  // (need this to define reflectivity/transmittance different than rindex)
  G4OpticalSurface* teflon_surface = new G4OpticalSurface("TEFLON_SURFACE");
  G4LogicalBorderSurface* teflon_plane = new
    G4LogicalBorderSurface("TEFLON_PLANE", world_phys_vol,
    teflon_phys_vol, teflon_surface);

  teflon_surface->SetMaterialPropertiesTable(TeflonSurface());

  /////////////////////////////////////////////////////////////////////////////
  // TRACKING PLANE
  /////////////////////////////////////////////////////////////////////////////
  // FIXME: sizes and distances???
  G4String tracking_name = "TRACKING_PLANE";
  G4double tracking_len = 10.*cm;
  G4double tracking_width = 1.5*mm;
  G4double tracking_xpos = teflon_xpos+teflon_width/2.+tracking_width/2.;
  G4ThreeVector tracking_pos = G4ThreeVector(0., 0., tracking_xpos);
  G4Material* tracking_mat =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");

  G4Box* tracking_solid_vol =
    new G4Box(tracking_name, tracking_len/2.,
      tracking_len/2., tracking_width/2.);
  G4LogicalVolume* tracking_logic_vol =
    new G4LogicalVolume(tracking_solid_vol, tracking_mat, tracking_name);

  tracking_mat->SetMaterialPropertiesTable(TransparentMaterialsTable());
  G4VPhysicalVolume* tracking_phys_vol = new G4PVPlacement(0, tracking_pos,
    tracking_logic_vol, tracking_name, world_logic_vol, false, 0, true);

  // Optical Properties of plane (giving it a photon detection efficiency)
  G4OpticalSurface* tracking_plane = new G4OpticalSurface("TRACKING_PLANE");
  G4LogicalBorderSurface* tracking_surface = new
    G4LogicalBorderSurface("TRACKING_SURFACE", teflon_phys_vol,
    tracking_phys_vol, tracking_plane);

  tracking_plane->SetMaterialPropertiesTable(OpticalPlane());

  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // SET VISIBILITY ATTRIBUTES

  G4VisAttributes* Blue = new G4VisAttributes(G4Colour(0,0.5,1.0,0.1));
  G4VisAttributes* Grey = new G4VisAttributes(G4Colour(.5, .5, .5, .4));
  G4VisAttributes* Yellow = new G4VisAttributes(G4Colour(1.0, 1.0, 0, .1));
  tracking_logic_vol->SetVisAttributes(Yellow);
  teflon_logic_vol->SetVisAttributes(Grey);


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

  fTrackingPlane = tracking_logic_vol;

  return world_phys_vol;
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

G4MaterialPropertiesTable* DetectorConstruction::TeflonSurface(){
  // Defines the optical properties for the detector planes

  G4MaterialPropertiesTable* plane_mpt = new G4MaterialPropertiesTable();

  // define props for a given number of energies
  const G4int NUMENTRIES = 2;
  G4double ENERGIES[NUMENTRIES] = {1.0*eV, 30.*eV};
  G4double RINDEX[NUMENTRIES] = {1., 1.};
  G4double TRANSMITTANCE[NUMENTRIES] = {0.0, 0.0};

  plane_mpt->AddProperty("RINDEX", ENERGIES, RINDEX, NUMENTRIES);
  plane_mpt->AddProperty("TRANSMITTANCE", ENERGIES, TRANSMITTANCE, NUMENTRIES);

  return plane_mpt;
}

G4MaterialPropertiesTable* DetectorConstruction::TeflonMaterialsTable(){
  // Defines the optical properties for the detector planes

  G4MaterialPropertiesTable* teflon_mpt = new G4MaterialPropertiesTable();

  // define props for a given number of energies
  const G4int NUMENTRIES = 2;
  G4double ENERGIES[NUMENTRIES] = {1.0*eV, 30.*eV};

  G4double RINDEX[NUMENTRIES] = {1., 1.};
  G4double ABSLENGTH[NUMENTRIES] = {1.*mm, 1.*mm}; // Must be measured

  teflon_mpt->AddProperty("RINDEX", ENERGIES, RINDEX, NUMENTRIES);
  teflon_mpt->AddProperty("ABSLENGTH", ENERGIES, ABSLENGTH, NUMENTRIES);

  return teflon_mpt;
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
