// -----------------------------------------------------------------------------
//  G4Basic | SteppingAction.cpp
//
//
//   * Author: Taylor Contreras, Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "SteppingAction.h"
#include "DetectorConstruction.h"
#include "RunAction.h"

#include "G4Step.hh"
#include "G4StepStatus.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4OpBoundaryProcess.hh"

SteppingAction::SteppingAction(EventAction* eventAction, RunAction* runAction):
  G4UserSteppingAction(),
  fEventAction(eventAction),
  fTrackingPlane(0),
  fRunAction(runAction),
  fboundary(0)
{
}


SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track* track = step->GetTrack();
  G4int trackid = track->GetTrackID();

  const DetectorConstruction* detectorConstruction
    = static_cast<const DetectorConstruction*>
    ( G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  fTrackingPlane = detectorConstruction->GetTrackingPlane();

  // Get volume of current step
  G4LogicalVolume* volume
    = step->GetPreStepPoint()->GetTouchableHandle()
    ->GetVolume()->GetLogicalVolume();

  ////////////////////////////////////////////// Testing stepping actions
  //G4ParticleDefinition* pdef = step->GetTrack()->GetDefinition();
  //G4TrackStatus trackstatus = track->GetTrackStatus();
  /*
  // Testing what OpBoundary is doing
  const G4DynamicParticle* aParticle = track->GetDynamicParticle();
  G4double thePhotonMomentum = aParticle->GetTotalMomentum();
  G4LogicalSurface* Surface = NULL;
  G4OpticalSurface* OpticalSurface = NULL;
  Surface = G4LogicalBorderSurface::GetSurface
     (step->GetPreStepPoint()->GetPhysicalVolume(),
     step->GetPostStepPoint()->GetPhysicalVolume());

  if (Surface == NULL){
     G4bool enteredDaughter=(step->GetPostStepPoint()->GetPhysicalVolume()
                             ->GetMotherLogical() ==
                             step->GetPreStepPoint()->GetPhysicalVolume()
                             ->GetLogicalVolume());
      if (enteredDaughter) {
         Surface = G4LogicalSkinSurface::GetSurface
               (step->GetPostStepPoint()->GetPhysicalVolume()->
                GetLogicalVolume());
          if(Surface == NULL)
               Surface = G4LogicalSkinSurface::GetSurface
               (step->GetPreStepPoint()->GetPhysicalVolume()->
                GetLogicalVolume());
      }
      else {
          Surface = G4LogicalSkinSurface::GetSurface
               (step->GetPreStepPoint()->GetPhysicalVolume()->
                GetLogicalVolume());
          if(Surface == NULL)
               Surface = G4LogicalSkinSurface::GetSurface
               (step->GetPostStepPoint()->GetPhysicalVolume()->
                GetLogicalVolume());
      }
  }
  if (Surface) {
      OpticalSurface =
        dynamic_cast <G4OpticalSurface*> (Surface->GetSurfaceProperty());
      G4MaterialPropertiesTable* aMaterialPropertiesTable =
        OpticalSurface->GetMaterialPropertiesTable();
      G4MaterialPropertyVector* PropertyPointer =
        aMaterialPropertiesTable->GetProperty("REFLECTIVITY");
      G4double theReflectivity = PropertyPointer->Value(thePhotonMomentum);

      G4cout << "Testing G4OpBoundaryProcess: R="<<theReflectivity<<"\n"<<G4endl;
  }

  if (!fboundary) { //pointer is not defined yet
    // Get list of processes defined for optical photon
    // and loop through it to find optical boundary process
    G4ProcessVector* pv = pdef->GetProcessManager()->GetProcessList();
    for (G4int i=0; i<pv->size(); i++){
      if ((*pv)[i]->GetProcessName() == "OpBoundary"){
    fboundary = (G4OpBoundaryProcess*) (*pv)[i];
    break;
      }
    }
  }

  G4String detector_name =
    step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();

  if (trackstatus == fAlive){
    G4String detector_namepost =
      step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
    //if (fboundary->GetStatus()==3){
      G4cout << "boundary status: "<<fboundary->GetStatus()<<" Boundaries: "<<detector_name<<", "<<detector_namepost<<"\n" << G4endl;
    //}
  }
  G4StepStatus poststepstatus = step->GetPostStepPoint()->GetStepStatus();
  if (poststepstatus == fGeomBoundary){
    if (fboundary->GetStatus() == Detection){
      G4String detector_namepost =
        step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
      G4cout << "#################### Sensitive Volume: " << detector_name <<", "<<detector_namepost<<G4endl;
    }
  }
  if (trackstatus != fAlive){
    G4cout << "dead boundary status/track status: "<<fboundary->GetStatus()<<"/"<<step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()<<" End Boundaries: "<<detector_name<<"\n"<<G4endl;
  }

*//////////////////////////////////////////////////////////////////////// End test

  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);

  std::map<int, int> TrackMap = fEventAction->GetTrackMap();
  G4TrackStatus status = track->GetTrackStatus();

  if (status != fAlive){
    //G4cout << "Trackid: "<<trackid<<" pid: "<<track->GetParticleDefinition()->GetPDGEncoding()<<"\n"<<G4endl;

    std::map<int, int>::iterator it = TrackMap.find(trackid);
    if (it == TrackMap.end()){
      //G4cout <<"new!\n"<<G4endl;
      G4int pid = track->GetParticleDefinition()->GetPDGEncoding();
      //G4cout << "particle: "<< track->GetParticleDefinition()->GetParticleName() << " "<<pid<<"\n" << G4endl;
      const G4ThreeVector& pos = track->GetPosition();
      fRunAction->FillFinals(pos.getX(), pos.getY(), pos.getZ(), pid, trackid);
      TrackMap[trackid] = trackid;

    }
  }

  // Do optical analysis stuff /////////////////////////////////////////
  // FIXME: see if this can be combined with stuff above?

  G4ParticleDefinition* pdef = step->GetTrack()->GetDefinition();

  // Only continue if it is an optical photon
  if (pdef != G4OpticalPhoton::Definition()) return;
  //G4cout << " Optical Photon!\n" << G4endl;
  // Retrieve point to optical boundary process
  // Only do this once per run
  //static G4OpBoundaryProcess* boundary = 0;

  if (!fboundary) { //pointer is not defined yet
    // Get list of processes defined for optical photon
    // and loop through it to find optical boundary process
    G4ProcessVector* pv = pdef->GetProcessManager()->GetProcessList();
    for (G4int i=0; i<pv->size(); i++){
      if ((*pv)[i]->GetProcessName() == "OpBoundary"){
	  fboundary = (G4OpBoundaryProcess*) (*pv)[i];
	  break;
      }
    }
  }

  // Note: fGeomBoundary is the current volume
  G4StepStatus stat = step->GetPostStepPoint()->GetStepStatus();
  //G4cout << "volume: "<< volume->GetName()<<"\n"<<G4endl;
  if (stat == fGeomBoundary){
    G4String detector_name =
      step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
    G4String detector_namepost =
      step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
    //G4cout << "status: "<<fboundary->GetStatus()<<" Boundaries: "<<detector_name<<", "<<detector_namepost<<"\n" << G4endl;
    if (fboundary->GetStatus() == Detection){
      //G4String detector_name =
      //  step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
      //G4String detector_namepost =
      //  step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
      fRunAction->DetectedOptical();
      //G4cout << "#################### Sensitive Volume: " << detector_name <<", "<<detector_namepost<<G4endl;
    }
  }

  return;
}
