// -----------------------------------------------------------------------------
//  G4Basic | SteppingAction.cpp
//
//  Definition of detector geometry and materials.
//   * Author: Justo Martin-Albo, Taylor Contreras
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "SteppingAction.h"
#include "DetectorConstruction.h"
#include "RunAction.h"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"

SteppingAction::SteppingAction(EventAction* eventAction, RunAction* runAction): 
  G4UserSteppingAction(),
  fEventAction(eventAction),
  fEnergyPlane(0),
  fTrackingPlane(0),
  fRunAction(runAction)
{
}


SteppingAction::~SteppingAction()
{
}


void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track* track = step->GetTrack();
  G4int trackid = track->GetTrackID();
  
  // check if particle is in map
  // if it is not, make a new key
  // check if dead or not
  // if dead, then add edep total to hedep
  
  const DetectorConstruction* detectorConstruction 
    = static_cast<const DetectorConstruction*>
    ( G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  fEnergyPlane = detectorConstruction->GetEnergyPlane();
  fTrackingPlane = detectorConstruction->GetTrackingPlane();
  
  // Get volume of current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
    ->GetVolume()->GetLogicalVolume();
  
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);
  //std::map<int, int>::iterator it = fTrackMap.find(trackid);
  //if (it != fTrackMap.end()){
    //track already a key in TrackMap
  //  it->second += edepStep;
  //}
  //else {
    // track must be added to map
  //  flasttrack = trackid;
  //  fTrackMap.insert(std::pair<int, float>(trackid, edepStep));
  //}
  std::map<int, int> TrackMap = fEventAction->GetTrackMap();
  G4TrackStatus status = track->GetTrackStatus();
  if (status != fAlive){
    //G4cout << "Trackid: "<<trackid<<"\n"<<G4endl;

    std::map<int, int>::iterator it = TrackMap.find(trackid);
    if (it == TrackMap.end()){
      //G4cout <<"new!\n"<<G4endl;
      G4int pid = track->GetParticleDefinition()->GetPDGEncoding();
      const G4ThreeVector& pos = track->GetPosition();
      fRunAction->FillFinals(pos.getX(), pos.getY(), pos.getZ(), pid, trackid);
      TrackMap[trackid] = trackid;

    }
  }
  
  /*
  G4TrackStatus status = track->GetTrackStatus();
  if (status != fAlive){
    std::cout <<"----- DEAD! ------\n";

    const G4ThreeVector& pos = track->GetPosition();
    const G4ParticleDefinition* pid = track->GetParticleDefinition();

    std::cout <<"--------- Posx: " << pos.getX()/CLHEP::cm <<" [cm] ---------\n";
    std::cout <<"--------- Particle id: " << pid->GetParticleName() << "------------\n";

    if (volume == fEnergyPlane || volume == fTrackingPlane){
      // count number of photons that reach planes
      // FIXME
      G4double edepStep = step->GetTotalEnergyDeposit();
      fEventAction->AddEdep(edepStep);
      fEventAction->AddNumPhotons();
    }
  }
  */
}
