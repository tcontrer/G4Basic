// -----------------------------------------------------------------------------
//  G4Basic | DetectorConstruction.h
//
//  Class for the definition of the detector geometry and materials.
//   * Author: Taylor Contreras, Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#ifndef DETECTOR_CONSTRUCTION_H
#define DETECTOR_CONSTRUCTION_H

#include <G4VUserDetectorConstruction.hh>
#include <G4MaterialPropertiesTable.hh>

class G4Material;


class DetectorConstruction: public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();
  virtual G4VPhysicalVolume* Construct();

  G4LogicalVolume* GetTrackingPlane() const { return fTrackingPlane; }

private:
  G4MaterialPropertiesTable* OpticalPlane();
  G4MaterialPropertiesTable* TeflonSurface();
  G4MaterialPropertiesTable* TeflonMaterialsTable();
  G4MaterialPropertiesTable* TransparentMaterialsTable();

  G4LogicalVolume* fTrackingPlane;
  G4double fpressure;
};

#endif
