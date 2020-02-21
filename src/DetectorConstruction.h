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
#include <G4GenericMessenger.hh>
#include <G4OpticalSurface.hh>

class G4Material;


class DetectorConstruction: public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();
  virtual G4VPhysicalVolume* Construct();

  G4LogicalVolume* GetTrackingPlane() const { return fTrackingPlane; }
  G4double GetTransmittance() const {return fTransmittance;}
  G4double GetAbsLength() const {return fAbsLength;}
  G4double GetTeflonWidth() const {return fTeflonWidth;}

  void SetTransmittance(G4double newTransmittance);
  void SetTeflonWidth(G4double newWidth);
  void SetAbsLength(G4double newAbsLength);


private:
  G4MaterialPropertiesTable* OpticalPlane();
  G4MaterialPropertiesTable* TeflonSurface(G4double transmittance);
  G4MaterialPropertiesTable* TeflonMaterialsTable(G4double abslength);
  G4MaterialPropertiesTable* TransparentMaterialsTable();

  G4LogicalVolume* fTrackingPlane;
  G4OpticalSurface* fteflon_surface;
  G4Material* fteflon_mat;

  G4GenericMessenger* fMessenger;
  
  G4double fpressure;
  G4double fTransmittance;
  G4double fAbsLength;
  G4double fTeflonWidth;
};

#endif
