// -----------------------------------------------------------------------------
//  G4Basic | RunAction.h
//
//
//   * Author: Justo Martin-Albo, Taylor Contreras
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


void RunAction::EndOfRunAction(const G4Run*)
{
  int numevents = fEdepMap.size();
  
  float edep, xinit, yinit, zinit, xfin, yfin, zfin;
  int pid;

  TFile* MyFile = new TFile("MyFile.root", "RECREATE");
  TTree* mytree = new TTree("mytree", "");
  mytree->Branch("hedep", &edep, "edep/F");
  mytree->Branch("nxinit", &xinit, "xinit/F");
  mytree->Branch("nyinit", &yinit, "yinit/F");
  mytree->Branch("nzinit", &zinit, "zinit/F");
  mytree->Branch("nxfin", &xfin, "xfin/F");
  mytree->Branch("nyfin", &yfin, "yfin/F");
  mytree->Branch("nzfin", &zfin, "zfin/F");
  mytree->Branch("npid", &pid, "pid/I");

  for (int i=0; i<numevents; i++){
    //G4cout << "Event num in filling files: " << i<<"\n"<<G4endl;
    edep = fEdepMap[i]/keV;
    xinit = fxinitMap[i]/cm;
    yinit = fyinitMap[i]/cm;
    zinit = fzinitMap[i]/cm;

    int size = fxfinMap[i].size();
    //G4cout <<"Size: "<<size << G4endl;
    for (int j=1; j<size+1; j++){
      //G4cout << "Inner loop: "<<j<<G4endl;
      xfin = fxfinMap[i][j]/cm;
      yfin = fyfinMap[i][j]/cm;
      zfin = fzfinMap[i][j]/cm;
      pid = fpidMap[i][j];
      }

    mytree->Fill();
    //G4cout << "\nMaps:"<<fxinitMap[i]<<" "<<fyinitMap[i]<<" "<<fzinitMap[i] <<G4endl;
  }

  MyFile->Write();
}

void RunAction::AddEdep(G4double edep){
  fEdep += edep;

  fEdepMap[feventnum] = edep;
}

void RunAction::FillInitials(G4double x, G4double y, G4double z){

  //G4cout << "EventNum for getting events: "<<feventnum<<"\n" <<G4endl;
  
  fxinitMap[feventnum] = x;
  fyinitMap[feventnum] = y;
  fzinitMap[feventnum] = z;

  //G4cout << "\nFilling Initial Maps:" << fxinitMap[feventnum]<<"\n"<<G4endl;
}

void RunAction::FillFinals(G4double x, G4double y, G4double z, G4int pid, G4int trackid){
  
  fxfinMap[feventnum][trackid] = x;
  fyfinMap[feventnum][trackid] = y;
  fzfinMap[feventnum][trackid] = z;
  fpidMap[feventnum][trackid] = pid;

  //G4cout << "\nFilling Final Maps:" << fxfinMap[feventnum][trackid] <<" "<<trackid<<"\n"<< G4endl;
}
