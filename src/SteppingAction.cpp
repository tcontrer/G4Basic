// -----------------------------------------------------------------------------
//  G4Basic | SteppingAction.cpp
//
//
//   * Author: Taylor Contreras, Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "SteppingAction.h"
#include "DetectorConstruction.h"
#include "RunAction.h"

#include "G4Step.hh"
#include "G4StepStatus.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4OpBoundaryProcess.hh"

SteppingAction::SteppingAction(EventAction* eventAction, RunAction* runAction):
  G4UserSteppingAction(),
  fEventAction(eventAction),
  fRunAction(runAction),
  fboundary(0)
{
}


SteppingAction::~SteppingAction()
{
}


void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track* track = step->GetTrack();
  G4int trackid = track->GetTrackID();

  const DetectorConstruction* detectorConstruction
    = static_cast<const DetectorConstruction*>
    ( G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  // Get volume of current step
  G4LogicalVolume* volume
    = step->GetPreStepPoint()->GetTouchableHandle()
    ->GetVolume()->GetLogicalVolume();

  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);

  std::map<int, int> TrackMap = fEventAction->GetTrackMap();
  G4TrackStatus status = track->GetTrackStatus();

  if (status == fAlive){
    G4int pid = track->GetParticleDefinition()->GetPDGEncoding();
    const G4ThreeVector& pos = track->GetPosition();
    fRunAction->FillSteps(pos.getX(), pos.getY(), pos.getZ(), trackid);
  }

  if (status != fAlive){
    //G4cout << "Trackid: "<<trackid<<" pid: "<<track->GetParticleDefinition()->GetPDGEncoding()<<"\n"<<G4endl;

    std::map<int, int>::iterator it = TrackMap.find(trackid);
    if (it == TrackMap.end()){
      //G4cout <<"new!\n"<<G4endl;
      G4int pid = track->GetParticleDefinition()->GetPDGEncoding();
      //G4cout << "particle: "<< track->GetParticleDefinition()->GetParticleName() << " "<<pid<<"\n" << G4endl;
      const G4ThreeVector& pos = track->GetPosition();
      fRunAction->FillFinals(pos.getX(), pos.getY(), pos.getZ(), pid, trackid);
      TrackMap[trackid] = trackid;

    }
  }

  return;
}
