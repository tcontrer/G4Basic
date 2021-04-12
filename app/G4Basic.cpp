// -----------------------------------------------------------------------------
//  G4Basic | G4Basic.cpp
//
//  Main program of the G4Basic detector simulation.
//   * Author: Justo Martin-Albo, Taylor Contreras
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "DetectorConstruction.h"
#include "PrimaryGeneration.h"
#include "SteppingAction.h"
#include "RunAction.h"

#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>
#include <FTFP_BERT_HP.hh>
#include <G4EmStandardPhysics_option4.hh>
#include <G4OpticalPhysics.hh>
#include <G4RadioactiveDecayPhysics.hh>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

int main(int argc, char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }
  
  // Construct the run manager and set the initialization classes
  G4RunManager* runmgr = new G4RunManager();

  // Set the physics used for this simulation
  G4VModularPhysicsList* physics_list = new G4VModularPhysicsList();
  //physics_list->RegisterPhysics(new G4OpticalPhysics());
  physics_list->RegisterPhysics(new G4EmStandardPhysics_option4());
  //physics_list->RegisterPhysics(new G4RadioactiveDecayPhysics());
  runmgr->SetUserInitialization(physics_list);

  // set up detector geometry
  runmgr->SetUserInitialization(new DetectorConstruction());

  // set user action classes
  RunAction* runAction = new RunAction();
  runmgr->SetUserAction(runAction);
  runmgr->SetUserAction(new PrimaryGeneration(runAction));
  EventAction* eventAction = new EventAction(runAction);
  runmgr->SetUserAction(eventAction);
  runmgr->SetUserAction(new SteppingAction(eventAction, runAction));

  // Initialize visualization
  G4VisManager* vismgr = new G4VisExecutive();
  vismgr->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* uimgr = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if (!ui) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    uimgr->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode
    uimgr->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program.

  delete vismgr;
  delete runmgr;
}
