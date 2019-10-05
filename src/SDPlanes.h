// -----------------------------------------------------------------------------            //  G4Basic | DetectorConstruction.h                                            
//                                                                             
//  Class for the definition of the detector geometry and materials.      
//   * Author: Taylor Contreras                                
//   * Creation date: 14 Aug 2019                                                 
// ----------------------------------------------------------------------------- 

// FIXME: ????
#ifndef __SDPLANES__
#define __SDPLANES__

#include <G4VSensitiveDetector.hh>
#include <G4Step.hh>

class SDPlanes: public G4VSensitiveDetector{

 public:
  SDPlanes(G4String sdname);
  ~SDPlanes();

  void Initialize();
  void EndOfEvent();

 private:
  G4bool ProcessHits(G4Step*, G4TouchableHistory *);
};

#endif
