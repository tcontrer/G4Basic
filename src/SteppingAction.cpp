// -----------------------------------------------------------------------------
//  G4Basic | SteppingAction.cpp
//
//  Definition of detector geometry and materials.
//   * Author: Justo Martin-Albo, Taylor Contreras
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "SteppingAction.h"
#include "DetectorConstruction.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"

SteppingAction::SteppingAction(EventAction* eventAction): 
  G4UserSteppingAction(),
  fEventAction(eventAction),
  fEnergyPlane(0),
  fTrackingPlane(0)
  //fTrackMap(),
  //fMyFile()
{
  // Make output file and ntuples to fill
  //fMyFile = new TFile("MyFile.root", "RECREATE");
  //TTree *mytree = new TTree("mytree", "");
  //fhedep = new TH1F("hedep", "", 100, 0, 100);
  //mytree->Branch("hedep", "TH1F", &fhedep, 32000, 0);
}


SteppingAction::~SteppingAction()
{
}


void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track* track = step->GetTrack();
  G4int id = track->GetTrackID();
  
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
  std::map<int, float>::iterator it = fTrackMap.find(id);
  if (it != fTrackMap.end()){
    //track already a key in TrackMap
    it->second += edepStep;
  }
  else {
    // track must be added to map
    fTrackMap.insert(std::pair<int, float>(id, edepStep));
  }
  
  G4TrackStatus status = track->GetTrackStatus();
  if (status != fAlive){
    // fill hist of total energy of track
    fEventAction->Fill(fTrackMap[id]);
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
