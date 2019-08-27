// -----------------------------------------------------------------------------
//  G4Basic | SteppingAction.cpp
//
//  Definition of detector geometry and materials.
//   * Author: Justo Martin-Albo, Taylor Contreras
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "SteppingAction.h"

#include "TFile.h"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4Track.hh"

SteppingAction::SteppingAction(): G4UserSteppingAction()
{
}


SteppingAction::~SteppingAction()
{
}


void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track* track = step->GetTrack();
  G4int id = track->GetTrackID();
  
  G4TrackStatus status = track->GetTrackStatus();
  if (status != fAlive){
    std::cout <<"----- DEAD! ------\n";

    const G4ThreeVector& pos = track->GetPosition();
    const G4ParticleDefinition* pid = track->GetParticleDefinition();

    std::cout <<"--------- Posx: " << pos.getX()/CLHEP::cm <<" [cm] ---------\n";
    std::cout <<"--------- Particle id: " << pid->GetParticleName() << "------------\n";
  }
}
