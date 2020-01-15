// -----------------------------------------------------------------------------
//  G4Basic | EventAction.cpp
//
//
//   * Author: Justo Martin-Albo, Taylor Contreras
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#include "EventAction.h"

EventAction::EventAction(RunAction* runAction)
  : G4UserEventAction(),
    fRunAction(runAction),
    fEdep(0.)
{
}


EventAction::~EventAction()
{
}


void EventAction::BeginOfEventAction(const G4Event*)
{
  //G4cout << "%%%%%%%%%%%%%%%%%%%%%%%% BEGIN EVENT "<<fRunAction->EventNum()+1<<"  %%%%%%%%%%%%%%%%%%%%%%%%\n"<<G4endl;
  fEdep = 0.;
  //fNumPhotons = 0;
  fTrackMap.clear();
}


void EventAction::EndOfEventAction(const G4Event*)
{
  fRunAction->AddEdep(fEdep);
  //G4cout << "%%%%%%%%%%%%%%%%%%%%%%%% END EVENT %%%%%%%%%%%%%%%%%%%%%%%%\n"<<G4endl;
  fRunAction->NextEvent();
}
