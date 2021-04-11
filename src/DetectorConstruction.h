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

private:
  G4Material* DefineArgon() const;
  G4MaterialPropertiesTable* TransparentMaterialsTable();

  G4double fpressure;
};

#endif
