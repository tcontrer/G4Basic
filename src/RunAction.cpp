// -----------------------------------------------------------------------------
//  G4Basic | RunAction.h
//
//
//   * Author: Taylor Contreras, Justo Martin-Albo
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#include "RunAction.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

#include <G4SystemOfUnits.hh>
#include <G4AccumulableManager.hh>
#include <string.h>
#include <iostream>
using namespace std;

RunAction::RunAction()
  : G4UserRunAction(),
    fEdep(0.),
    feventnum(0)
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


void RunAction::EndOfRunAction(const G4Run*){
  // Make and fill output file with information from the run

  int numevents = fEdepMap.size();
  int numtracks = fxfinMap[0].size();

  float edep, xinit, yinit, zinit, xfin, yfin, zfin, dpos;
  int pid, trackid, eventid;

  // Make output file and branches
  TFile* MyFile = new TFile("MyFile.root", "RECREATE");
  TTree* tree1 = new TTree("tree1", ""); // for single fill per event
  TTree* tree2 = new TTree("tree2", ""); // for multiple fills per event
  tree1->Branch("hedep", &edep, "edep/F"); // total energy deposited in an event
  tree1->Branch("nxinit", &xinit, "xinit/F"); // initial x position
  tree1->Branch("nyinit", &yinit, "yinit/F"); // initial y position
  tree1->Branch("nzinit", &zinit, "zinit/F"); // initial z position
  tree2->Branch("nxfin", &xfin, "xfin/F"); // final x position
  tree2->Branch("nyfin", &yfin, "yfin/F"); // final y position
  tree2->Branch("nzfin", &zfin, "zfin/F"); // final z positions
  tree2->Branch("npid", &pid, "pid/I"); // particle ids
  tree2->Branch("ntrackid", &trackid, "trackid/I"); // track ids
  tree2->Branch("ndpos", &dpos, "dpos/F"); // difference between initial and final positions
  tree2->Branch("nevent", &eventid, "eventid/I"); // event number

  for (int i=0; i<numevents; i++){
    // Fill initial info and total edep from event

    edep = fEdepMap[i]/keV;
    xinit = fxinitMap[i]/cm;
    yinit = fyinitMap[i]/cm;
    zinit = fzinitMap[i]/cm;
    eventid = feventids[i];

    numtracks = fxfinMap[i].size();
    for (int j=1; j<numtracks+1; j++){
      // Fill final info on each particle in event

      xfin = fxfinMap[i][j]/cm;
      yfin = fyfinMap[i][j]/cm;
      zfin = fzfinMap[i][j]/cm;
      pid = fpidMap[i][j];
      trackid = ftrackMap[i][j];

      dpos = sqrt(pow(xinit - xfin,2.0) + pow(yinit - yfin,2.0) + pow(zinit - zfin,2.0));
      tree2->Fill();
    }

    tree1->Fill();
  }

  MyFile->Write();
}

void RunAction::AddEdep(G4double edep){
  fEdep += edep;

  fEdepMap[feventnum] = edep;
}

void RunAction::FillInitials(G4double x, G4double y, G4double z, int eventid){

  fxinitMap[feventnum] = x;
  fyinitMap[feventnum] = y;
  fzinitMap[feventnum] = z;
  feventids[feventnum] = eventid;
}

void RunAction::FillFinals(G4double x, G4double y, G4double z, G4int pid, G4int trackid){

  fxfinMap[feventnum][trackid] = x;
  fyfinMap[feventnum][trackid] = y;
  fzfinMap[feventnum][trackid] = z;
  fpidMap[feventnum][trackid] = pid;
  ftrackMap[feventnum][trackid] = trackid;
}
