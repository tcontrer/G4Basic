// -----------------------------------------------------------------------------
//  G4Basic | DetectorConstruction.cpp
//
//  Definition of detector geometry and materials.
//   * Author: Justo Martin-Albo
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

  // DETECTOR //////////////////////////////////////////////

  G4String detector_name = "DETECTOR";
  G4double detector_diam   = 1.0*m;
  G4double detector_length = 1.0*m;
  G4Material* detector_mat = DefineNeon();

  G4Tubs* detector_solid_vol =
    new G4Tubs(detector_name, 0., detector_diam/2., detector_length/2., 0., 360.*deg);

  G4LogicalVolume* detector_logic_vol =
    new G4LogicalVolume(detector_solid_vol, detector_mat, detector_name);

  new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
                    detector_logic_vol, detector_name, world_logic_vol, false, 0, true);

  //////////////////////////////////////////////////////////

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
