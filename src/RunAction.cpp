// -----------------------------------------------------------------------------
//  G4Basic | RunAction.h
//
//
//   * Author: Justo Martin-Albo, Taylor Contreras
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#include "RunAction.h"

#include <G4AccumulableManager.hh>
#include <string.h>
#include <iostream>
using namespace std;

RunAction::RunAction()
  : G4UserRunAction(),
  fEdep(0.)
{
  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);
}


RunAction::~RunAction()
{
}


void RunAction::BeginOfRunAction(const G4Run*)
{
}


void RunAction::EndOfRunAction(const G4Run*)
{
}

void RunAction::AddEdep(G4double edep){
  fEdep += edep;
}
