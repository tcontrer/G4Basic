// -----------------------------------------------------------------------------
//  G4Basic | RunAction.h
//
//
//   * Author: Taylor Contreras, Justo Martin-Albo
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#ifndef RUN_ACTION_H
#define RUN_ACTION_H

#include <G4UserRunAction.hh>
#include "G4Accumulable.hh"

class RunAction: public G4UserRunAction
{
public:
  RunAction();
  virtual ~RunAction();
  virtual void BeginOfRunAction(const G4Run*);
  virtual void   EndOfRunAction(const G4Run*);

  void AddEdep (G4double edep);
  void FillInitials (G4double x, G4double y, G4double z, G4int eventid);
  void FillSteps (G4double x, G4double y, G4double z, G4int trackid);
  void FillFinals (G4double x, G4double y, G4double z, G4int pid, G4int trackid);
  void NextEvent () {feventnum++;}
  int EventNum () {return feventnum;}
  
 private:
  G4Accumulable<G4double> fEdep;
  int feventnum;
  std::map<int, float> fEdepMap;
  std::map<int, float> fxinitMap;
  std::map<int,float> fyinitMap;
  std::map<int,float> fzinitMap;
  std::map<int, int> feventids;
  std::map<int, std::map<int, std::vector<float>>> fxtrack;
  std::map<int, std::map<int, std::vector<float>>> fytrack;
  std::map<int, std::map<int, std::vector<float>>> fztrack;
  std::map<int,std::map<int,float>> fxfinMap;
  std::map<int,std::map<int,float>> fyfinMap;
  std::map<int,std::map<int,float>> fzfinMap;
  std::map<int,std::map<int,int>> fpidMap;
  std::map<int,std::map<int,int>> ftrackMap;
};

#endif
