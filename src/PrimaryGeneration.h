// -----------------------------------------------------------------------------
//  G4Basic | PrimaryGeneration.h
//
//  Class for the definition of the primary generation action.
//   * Author: Justo Martin-Albo, Taylor Contreras
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#ifndef PRIMARY_GENERATION_H
#define PRIMARY_GENERATION_H

#include <G4VUserPrimaryGeneratorAction.hh>
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleDefinition;


class PrimaryGeneration: public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneration();
  virtual ~PrimaryGeneration();
  virtual void GeneratePrimaries(G4Event*);
  G4ParticleGun* GetParticleGun() { return fParticleGun;};

 private:
  G4ParticleGun* fParticleGun;
};

#endif
