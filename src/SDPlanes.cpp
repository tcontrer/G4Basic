// -----------------------------------------------------------------------------     
//  G4Basic | DetectorConstruction.h                                                 
//                                                                                     
//  Class for the definition of the detector geometry and materials.
//   * Author: Taylor Contreras                     
//   * Creation date: 14 Aug 2019                                                   
// ----------------------------------------------------------------------------- 

#include "SDPlanes.h"

// other includes?

SDPlanes::SDPlanes(G4String sdname):
  G4VSensitiveDetector(sdname)
{
}

SDPlanes::~SDPlanes(){
}

void SDPlanes::Initialize(){
}

G4bool SDPlanes::ProcessHits(G4Step* step, G4TouchableHistory *){
  return true;
}

void SDPlanes::EndOfEvent(){
}
