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
#include <G4RunManager.hh>

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),
    fTrackingPlane(0),
    fteflon_mat(0),
    fpressure(15.*bar),
    fTransmittance(0.5),
    fAbsLength(1.82*mm),
    fTeflonWidth(1.5*mm)
{
  // define commands for this class
  fMessenger = new G4GenericMessenger(this, "/G4Basic/");
  fMessenger->SetGuidance("Commands for G4Basic"); // makes help info for user

  // declare a new command (under /G4Basic/ directory)
  fMessenger->DeclareMethod("setTransmittance", // name of command
          &DetectorConstruction::SetTransmittance) //function for the command
    .SetStates(G4State_PreInit, G4State_Idle) // Sets when it can be called
    .SetGuidance("Set teflon transittance");

  // Must be called before Initialize
  fMessenger->DeclareMethod("setTeflonWidth", &DetectorConstruction::SetTeflonWidth)
    .SetStates(G4State_PreInit)
    .SetGuidance("Set teflon width");

  fMessenger->DeclareMethod("setAbsLength", &DetectorConstruction::SetAbsLength)
    .SetStates(G4State_PreInit, G4State_Idle)
    .SetGuidance("Set teflon absorbtion length");
}


DetectorConstruction::~DetectorConstruction()
{
  delete fMessenger;
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  /////////////////////////////////////////////////////////////////////////////
  // WORLD
  /////////////////////////////////////////////////////////////////////////////

  G4String world_name = "WORLD";
  G4double world_size = 2.*m;
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
  G4String teflon_name = "TEFLON";
  G4double teflon_len = 10.*cm;
  G4double teflon_width = GetTeflonWidth();
  G4double teflon_zpos = teflon_len/2. + 10.*cm;
  G4ThreeVector teflon_pos = G4ThreeVector(0, 0, teflon_zpos);
  fteflon_mat =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_TEFLON");

  G4Box* teflon_solid_vol =
    new G4Box(teflon_name, teflon_len/2., teflon_len/2., teflon_width/2.);
  G4LogicalVolume* teflon_logic_vol =
    new G4LogicalVolume(teflon_solid_vol, fteflon_mat, teflon_name);

  fteflon_mat->SetMaterialPropertiesTable(TeflonMaterialsTable(GetAbsLength()));

  /////////// Optical Properties

  /*// Skin (reflects at each edge)
  G4OpticalSurface* teflon_surface = new G4OpticalSurface("TEFLON_SURFACE");
  new G4LogicalSkinSurface("TEFLON_PLANE", teflon_logic_vol, teflon_surface);
  teflon_surface->SetMaterialPropertiesTable(TeflonSurface());
  new G4PVPlacement(0, teflon_pos,teflon_logic_vol, teflon_name, world_logic_vol, false, 0, true);
  */

  // Border (reflect at one border in one direction)
  G4VPhysicalVolume* teflon_phys_vol = new G4PVPlacement(0, teflon_pos,
    teflon_logic_vol, teflon_name, world_logic_vol, false, 0, true);
  fteflon_surface = new G4OpticalSurface("TEFLON_SURFACE");
  fteflon_surface->SetType(dielectric_dielectric);
  G4LogicalBorderSurface* teflon_plane = new
    G4LogicalBorderSurface("TEFLON_PLANE", world_phys_vol,
    teflon_phys_vol, fteflon_surface);
  fteflon_surface->SetMaterialPropertiesTable(TeflonSurface(GetTransmittance()));

  ////////////////////////////////////////////////////////////////////////////
  // TRACKING PLANE
  /////////////////////////////////////////////////////////////////////////////
  G4String tracking_name = "TRACKING_PLANE";
  G4double tracking_len = 10.*cm;
  G4double tracking_width = 1.5*mm;
  G4double tracking_zpos = teflon_zpos+teflon_width/2.+tracking_width/2.;
  G4ThreeVector tracking_pos = G4ThreeVector(0., 0., tracking_zpos);
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
  G4VisAttributes* Grey = new G4VisAttributes(G4Colour(.5, .5, .5, .2));
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
  G4double ENERGIES[NUMENTRIES] = {2.8*eV, 2.8*eV};
  G4double EFFICIENCY[NUMENTRIES] = {1.0, 1.0};
  G4double RINDEX[NUMENTRIES] = {1, 1};
  G4double REFLECTIVITY[NUMENTRIES] = {0, 0};

  plane_mpt->AddProperty("EFFICIENCY", ENERGIES, EFFICIENCY, NUMENTRIES);
  plane_mpt->AddProperty("RINDEX", ENERGIES, RINDEX, NUMENTRIES);
  plane_mpt->AddProperty("REFLECTIVITY", ENERGIES, REFLECTIVITY, NUMENTRIES);

  return plane_mpt;
}

G4MaterialPropertiesTable* DetectorConstruction::TeflonSurface(G4double transmittance){
  // Defines the optical properties for the detector planes

  G4MaterialPropertiesTable* plane_mpt = new G4MaterialPropertiesTable();

  // define props for a given number of energies
  const G4int NUMENTRIES = 2;
  G4double ENERGIES[NUMENTRIES] = {2.8*eV, 2.8*eV};
  G4double RINDEX[NUMENTRIES] = {1., 1.};
  G4double TRANSMITTANCE[NUMENTRIES] = {transmittance, transmittance};

  plane_mpt->AddProperty("RINDEX", ENERGIES, RINDEX, NUMENTRIES);
  plane_mpt->AddProperty("TRANSMITTANCE", ENERGIES, TRANSMITTANCE, NUMENTRIES);

  plane_mpt->DumpTable();
  return plane_mpt;
}

G4MaterialPropertiesTable* DetectorConstruction::TeflonMaterialsTable(G4double abslength){
  // Defines the optic properties for the detector planes

  G4MaterialPropertiesTable* teflon_mpt = new G4MaterialPropertiesTable();

  // define props for a given number of energies
  const G4int NUMENTRIES = 2;
  G4double ENERGIES[NUMENTRIES] = {2.8*eV, 2.8*eV};

  G4double RINDEX[NUMENTRIES] = {1., 1.};
  G4double ABSLENGTH[NUMENTRIES] = {abslength, abslength};

  teflon_mpt->AddProperty("RINDEX", ENERGIES, RINDEX, NUMENTRIES);
  teflon_mpt->AddProperty("ABSLENGTH", ENERGIES, ABSLENGTH, NUMENTRIES);

  return teflon_mpt;
}

G4MaterialPropertiesTable* DetectorConstruction::TransparentMaterialsTable(){
  // Make a material transparent to optical photons

  const G4int NUMENTRIES = 2;
  G4double ENERGIES[NUMENTRIES] = {2.8*eV, 2.8*eV};
  G4double RIND[NUMENTRIES] = {1., 1.};

  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
  mpt->AddProperty("RINDEX", ENERGIES, RIND, NUMENTRIES);

  return mpt;
}

void DetectorConstruction::SetTransmittance(G4double newTransmittance){
  // Values between 0.00001 and 1.0 (never 0.0)
  if (!fteflon_surface){
    G4cerr << "Detector has not yet been constructed." <<G4endl;
    return;
  }

  fTransmittance = newTransmittance;
  fteflon_surface->SetMaterialPropertiesTable(TeflonSurface(GetTransmittance()));

  // Tell G4RunManager that we changed the geometry
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  return;
}

void DetectorConstruction::SetTeflonWidth(G4double newWidth){
  // Must be called before initialize
  // Experimental Values: 0.5, 1.0, 1.5 mm

  fTeflonWidth = newWidth;
  return;
}

void DetectorConstruction::SetAbsLength(G4double newAbsLength){
  if (!fteflon_mat){
    G4cerr << "Detector has not yet been constructed." <<G4endl;
    return;
  }

  fAbsLength = newAbsLength;
  fteflon_mat->SetMaterialPropertiesTable(TeflonMaterialsTable(GetAbsLength()));

  // Tell G4RunManager that we changed the geometry
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  return;
}
