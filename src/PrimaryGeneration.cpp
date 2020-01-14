// -----------------------------------------------------------------------------
//  G4Basic | PrimaryGeneration.cpp
//
//  Class for the definition of the primary generation action.
//   * Author: Taylor Contreras, Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "PrimaryGeneration.h"

#include <G4ParticleDefinition.hh>
#include <G4SystemOfUnits.hh>
#include <G4IonTable.hh>
#include <G4PrimaryParticle.hh>
#include <G4PrimaryVertex.hh>
#include <G4Event.hh>
#include <Randomize.hh>
#include <G4OpticalPhoton.hh>

PrimaryGeneration::PrimaryGeneration(RunAction* runAction):
  G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fRunAction(runAction)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  fParticleGun->SetParticleEnergy(0*eV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,0.));
}


PrimaryGeneration::~PrimaryGeneration()
{
  delete fParticleGun;
}


void PrimaryGeneration::GeneratePrimaries(G4Event* event)
{
  // Generate random photon in circular plane, towards teflon
  G4double det_r = 4.*cm; // radius of beam
  G4double rand = G4UniformRand(); // random number between 0 and 1
  G4double rand_phi = G4UniformRand()*2.*CLHEP::pi;
  G4double rand_r = det_r*std::sqrt(rand);
  G4double rand_x = rand_r*cos(rand_phi);
  G4double rand_y = rand_r*sin(rand_phi);

  /////////////////////////////////////////////////////////////////////////////
  // Testing with single photon
  /////////////////////////////////////////////////////////////////////////////
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = G4OpticalPhoton::OpticalPhotonDefinition();
  fParticleGun->SetParticleDefinition(particle);

  fParticleGun->SetParticlePosition(G4ThreeVector(rand_x, rand_y, 0.));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
  fParticleGun->SetParticleEnergy(2.8*eV); //450nm
  fParticleGun->GeneratePrimaryVertex(event);

  G4int eventid = event->GetEventID();
  fRunAction->FillInitials(rand_x, rand_y, 0, eventid);
}
