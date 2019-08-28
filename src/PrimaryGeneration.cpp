// -----------------------------------------------------------------------------
//  G4Basic | PrimaryGeneration.cpp
//
//  Class for the definition of the primary generation action.
//   * Author: Justo Martin-Albo, Taylor Contreras
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "PrimaryGeneration.h"
//#include "Kr83mGeneration.h"

#include "TFile.h"

#include <G4ParticleDefinition.hh>
#include <G4SystemOfUnits.hh>
#include <G4IonTable.hh>
#include <G4PrimaryParticle.hh>
#include <G4PrimaryVertex.hh>
#include <G4Event.hh>
#include <Randomize.hh>

PrimaryGeneration::PrimaryGeneration():
  G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
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
  G4int Z = 36;
  G4int A = 83;
  G4double exitEnergy = 41.6*keV;

  // Make Krypton 83 decay
  G4ParticleDefinition* particle_definition = G4IonTable::GetIonTable()->
    GetIon(Z, A, exitEnergy);

  // Generate random position within a cylinder
  G4double det_r = 0.5*m; // radius of Xenon chamber
  G4double det_z = 1.*m; // length of Xenon chamger     
  // FIXME: should make this automatic base on DetectorConstruction..
  
  G4double rand = G4UniformRand(); // random number between 0 and 1
  G4double rand_phi = G4UniformRand()*2.*CLHEP::pi;
  G4double rand_r = det_r*sqrt(rand);
  G4double rand_x = rand_r*cos(rand_phi);
  G4double rand_y = rand_r*sin(rand_phi);
  G4double rand_z = (G4UniformRand()-0.5)*det_z;

  fParticleGun->SetParticleDefinition(particle_definition);
  fParticleGun->SetParticlePosition(G4ThreeVector(rand_x, rand_y, rand_z));
  fParticleGun->GeneratePrimaryVertex(event);
  
}
