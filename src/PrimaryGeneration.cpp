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

PrimaryGeneration::PrimaryGeneration(RunAction* runAction):
  G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fRunAction(runAction)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  fParticleGun->SetParticleEnergy(1*GeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
}


PrimaryGeneration::~PrimaryGeneration()
{
  delete fParticleGun;
}


void PrimaryGeneration::GeneratePrimaries(G4Event* event)
{
  /////////////////////////////////////////////////////////////////////////////
  // Single Particle
  /////////////////////////////////////////////////////////////////////////////
  G4int nu_e = 12;
  G4int nu_mu = 14;
  G4int nu_tau = 16;
  G4float particle_energy = 1.0 * GeV;

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = particleTable->FindParticle("mu-");
  G4cout << "---------- Particle name: " << particle->GetParticleName() << G4endl;
  fParticleGun->SetParticleDefinition(particle);

  fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 0));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
  fParticleGun->SetParticleEnergy(particle_energy);
  fParticleGun->GeneratePrimaryVertex(event);

  G4int eventid = event->GetEventID();
  //fRunAction->FillInitials(rand_x, rand_y, rand_z, eventid);
  //G4cout << "\n\n"<<rand_x<<" "<<rand_y<<" "<<rand_z <<"\n\n"<<G4endl;
}
