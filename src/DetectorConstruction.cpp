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

  /////////////////////////////////////////////////////////////////////////////
  // XENON CHAMBER
  /////////////////////////////////////////////////////////////////////////////

  G4String xenon_name = "XENON";
  G4double xenon_diam   = 1.0*m;
  G4double xenon_length = 1.0*m;
  G4Material* xenon_mat = DefineXenon();

  G4Tubs* xenon_solid_vol =
    new G4Tubs(xenon_name, 0., xenon_diam/2., xenon_length/2., 0., 360.*deg);

  G4LogicalVolume* xenon_logic_vol =
    new G4LogicalVolume(xenon_solid_vol, xenon_mat, xenon_name);

  G4VPhysicalVolume* xenon_phys = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
					       xenon_logic_vol, xenon_name, world_logic_vol, false, 0, true);


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

  return world_phys_vol;
}


G4Material* DetectorConstruction::DefineXenon() const{
  // Defines the material and properties of gaseous xenon

  G4String material_name = "GXe";
  G4double density = 88.56 * kg/m3;
  //G4double pressure = 15.0 * bar;
  G4double temperature = 300. * kelvin;
  G4double sc_yield = 20000*1/MeV; // Estimated ~50 photons/eV

  G4Material* material = new G4Material(material_name, density, 1,
			    kStateGas, temperature, fpressure);
  G4Element* Xe = G4NistManager::Instance()->FindOrBuildElement("Xe");
  material->AddElement(Xe,1);

  return material;
}
