// -----------------------------------------------------------------------------
//  G4Basic | RunAction.h
//
//
//   * Author: Justo Martin-Albo, Taylor Contreras
//   * Creation date: 15 Aug 2019
// -----------------------------------------------------------------------------

#include "RunAction.h"
//#include "TFile.h"
#include <string.h>
#include <iostream>
using namespace std;

RunAction::RunAction(): G4UserRunAction()
{
}


RunAction::~RunAction()
{
}


void RunAction::BeginOfRunAction(const G4Run*)
{

  //TFile* MyFile = new TFile("G4Basic.root", "RECREATE");
  //cout << "\n\n\n------------------- RunAction ------------------------\n\n\n";
}


void RunAction::EndOfRunAction(const G4Run*)
{
}
