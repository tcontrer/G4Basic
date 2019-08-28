// -----------------------------------------------------------------------------
//  G4Basic | EventAction.cpp
//
//
//   * Author: Justo Martin-Albo, Taylor Contreras
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#include "EventAction.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

EventAction::EventAction(RunAction* runAction)
  : G4UserEventAction(),
    fRunAction(runAction),
    fEdep(0.),
    fNumPhotons(0),
    fMyFile()
{
}


EventAction::~EventAction()
{
  //fMyFile = new TFile("MyFile.root", "RECREATE");
  //TTree *mytree = new TTree("myree", "");
  fhedep = new TH1F("hedep", "", 100, 0, 100);
  //mytree->Branch("hedep", "TH1F", &fhedep, 32000, 0);
}


void EventAction::BeginOfEventAction(const G4Event*)
{
  fEdep = 0.;
  fNumPhotons = 0;
}


void EventAction::EndOfEventAction(const G4Event*)
{
  fRunAction->AddEdep(fEdep);

  fMyFile = new TFile("MyFile.root", "RECREATE");
  TTree *mytree = new TTree("mytree", "");
  mytree->Branch("hedep", "TH1F", &fhedep, 32000, 0);
  fMyFile->Write();
}

void EventAction::Fill(G4double edep){
  //fMyFile = new TFile("MyFile.root", "UPDATE");
  //TObject* hedep = Get_hedep();
  //hedep->Fill(edep);
}

TObject* EventAction::Get_hedep(){
  //fMyFile = new TFile("MyFile.root", "READ");
  //auto tobj = fMyFile->Get("hedep");
  //tobj->SetDirectory(0);
  //return tobj;
}
