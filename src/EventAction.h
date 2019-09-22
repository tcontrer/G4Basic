// -----------------------------------------------------------------------------
//  G4Basic | EventAction.h
//
//
//   * Author: Justo Martin-Albo, Taylor Contreras
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#ifndef EVENT_ACTION_H
#define EVENT_ACTION_H

#include "RunAction.h"

#include <G4UserEventAction.hh>

class EventAction: public G4UserEventAction
{
public:
  EventAction(RunAction* runAction);
  virtual ~EventAction();
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);

  void AddEdep(G4double edep) { fEdep += edep;}
  void FillTrackMap(G4int trackid) {fTrackMap[trackid] = trackid;}
  std::map<int, int> GetTrackMap() {return fTrackMap;}
  //void AddNumPhotons() {fNumPhotons++;}

 private:
  RunAction* fRunAction;
  G4double fEdep;
  std::map<int, int> fTrackMap;
};

#endif
