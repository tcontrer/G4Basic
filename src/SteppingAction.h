// -----------------------------------------------------------------------------
//  G4Basic | SteppingAction.H
//
// 
//   * Author: Taylor Contreras, Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#ifndef STEPPING_ACTION_H
#define STEPPING_ACTION_H

#include "EventAction.h"
#include "RunAction.h"

#include "TFile.h"
#include "TH1F.h"

#include <G4UserSteppingAction.hh>
#include <G4LogicalVolume.hh>
#include <G4OpBoundaryProcess.hh>

class SteppingAction: public G4UserSteppingAction
{
  public:
  SteppingAction(EventAction* eventAction, RunAction* runAction);
    virtual ~SteppingAction();
    virtual void UserSteppingAction(const G4Step*);

 private:    
    EventAction* fEventAction;
    RunAction* fRunAction;
    G4LogicalVolume* fEnergyPlane;
    G4LogicalVolume* fTrackingPlane;
    G4OpBoundaryProcess* fboundary;
    TFile* fMyFile;
    TH1F* fhedep;
};

#endif
