// $Id: TG4TrackingActionMessenger.cxx,v 1.2 2002/12/18 09:35:31 brun Exp $
// Category: event
//
// Author: I. Hrivnacova
//
// Class TG4TrackingActionMessenger
// --------------------------------
// See the class description in the header file.
 
#include "TG4TrackingActionMessenger.h"
#include "TG4TrackingAction.h"
#include "TG4Globals.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithABool.hh>

//_____________________________________________________________________________
TG4TrackingActionMessenger::TG4TrackingActionMessenger(
                               TG4TrackingAction* trackingAction)
  : G4UImessenger(),
    fTrackingAction(trackingAction)
{
// 
  fTrackingDirectory = new G4UIdirectory("/mcTracking/");
  fTrackingDirectory->SetGuidance("TG4TrackingAction control commands.");

  fNewVerboseCmd = new G4UIcmdWithAnInteger("/mcTracking/newVerbose", this);
  fNewVerboseCmd->SetGuidance("Set new verbose level (/tracking/verbose)");
  fNewVerboseCmd->SetGuidance("when a track with specified track ID ");
  fNewVerboseCmd->SetGuidance("(/TG4Tracking/newVerboseTrack)\n starts tracking");
  fNewVerboseCmd->SetParameterName("NewVerboseLevel", false);
  fNewVerboseCmd->SetRange("NewVerboseLevel >= 0 && NewVerboseLevel <= 5");
  fNewVerboseCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fNewVerboseTrackCmd = new G4UIcmdWithAnInteger("/mcTracking/newVerboseTrack", this);
  fNewVerboseTrackCmd->SetGuidance("Set the track ID for which the new verbose level");
  fNewVerboseTrackCmd->SetGuidance("(/TG4Tracking/newVerbose) will be applied.");
  fNewVerboseTrackCmd->SetParameterName("NewVerboseLevelTrackID", false);
  fNewVerboseTrackCmd->SetRange("NewVerboseLevelTrackID >= 0");
  fNewVerboseTrackCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fSaveSecondariesCmd = new G4UIcmdWithABool("/mcTracking/saveSecondaries", this);
  fSaveSecondariesCmd->SetGuidance("Option to save secondaries in the stack");
  fSaveSecondariesCmd->SetParameterName("SaveSecondaries", false);
  fSaveSecondariesCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
}

//_____________________________________________________________________________
TG4TrackingActionMessenger::TG4TrackingActionMessenger() 
  : G4UImessenger(),
    fTrackingAction(0),
    fTrackingDirectory(0),
    fNewVerboseCmd(0),
    fNewVerboseTrackCmd(0),
    fSaveSecondariesCmd(0)    
{
//
}

//_____________________________________________________________________________
TG4TrackingActionMessenger::TG4TrackingActionMessenger(
                                 const TG4TrackingActionMessenger& right) {
//				 
  TG4Globals::Exception(
    "TG4TrackingActionMessenger is protected from copying.");
}

//_____________________________________________________________________________
TG4TrackingActionMessenger::~TG4TrackingActionMessenger() {
//
  delete fTrackingDirectory;
  delete fNewVerboseCmd;
  delete fNewVerboseTrackCmd;
  delete fSaveSecondariesCmd;
}

// operators

//_____________________________________________________________________________
TG4TrackingActionMessenger& 
TG4TrackingActionMessenger::operator=(const TG4TrackingActionMessenger &right)
{
  // check assignement to self
  if (this == &right) return *this;
  
  TG4Globals::Exception(
    "TG4TrackingActionMessenger is protected from assigning.");

  return *this;
}

// public methods

//_____________________________________________________________________________
void TG4TrackingActionMessenger::SetNewValue(G4UIcommand* command, 
       G4String newValue)
{ 
// Applies command to the associated object.
// ---

  if(command == fNewVerboseCmd) { 
    fTrackingAction
      ->SetNewVerboseLevel(fNewVerboseCmd->GetNewIntValue(newValue)); 
  }   
  else if(command == fNewVerboseTrackCmd) { 
    fTrackingAction
      ->SetNewVerboseTrackID(fNewVerboseTrackCmd->GetNewIntValue(newValue)); 
  }   
  else if(command == fSaveSecondariesCmd) { 
    fTrackingAction
      ->SetSaveSecondaries(fSaveSecondariesCmd->GetNewBoolValue(newValue)); 
  }   
}
