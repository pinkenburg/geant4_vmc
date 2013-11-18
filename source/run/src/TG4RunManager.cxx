// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4RunManager.cxx
/// \brief Implementation of the TG4RunManager class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RunManager.h"
#include "TG4RunConfiguration.h"
#include "TG4Globals.h"
#include "TG4GeometryManager.h"
#include "TG4GeometryServices.h"
#include "TG4SDManager.h"
#include "TG4SDServices.h"
#include "TG4PhysicsManager.h"
#include "TG4G3PhysicsManager.h"
#include "TG4StateManager.h"
#include "TG4ActionInitialization.h"
#include "TG4WorkerInitialization.h"
#include "TG4DetConstruction.h"
#include "TG4PostDetConstruction.h"
#include "TG4SDConstruction.h"
#include "TG4RegionsManager.h"

#ifdef G4MULTITHREADED  
#include <G4MTRunManager.hh>
#else
#include <G4RunManager.hh>
#endif

#include <Randomize.hh>
#include <G4UIsession.hh>
#include <G4UImanager.hh>
#ifdef G4UI_USE
#include <G4UIExecutive.hh>
#endif

#include <TROOT.h> 
#include <TRint.h>
#include <TCint.h> 
#include <TGeoManager.h>
#include <TRandom.h>
#include <TG4RootNavMgr.h>
#include <TVirtualMCApplication.h>

G4ThreadLocal TG4RunManager* TG4RunManager::fgInstance = 0;

//_____________________________________________________________________________
TG4RunManager::TG4RunManager(TG4RunConfiguration* runConfiguration, 
                             int argc, char** argv)                  
  : TG4Verbose("runManager"),
    fRunManager(0),
    fMessenger(this),
    fRunConfiguration(runConfiguration),
    fRegionsManager(0),
    fGeantUISession(0),
    fRootUISession(0),
    fRootUIOwner(false),
    fARGC(argc),
    fARGV(argv),  
    fUseRootRandom(true) 
{
/// Standard constructor

  G4cout << "TG4RunManager::TG4RunManager: " << this << G4endl;

  if (fgInstance) {
    TG4Globals::Exception(
      "TG4RunManager", "TG4RunManager",
      "Cannot create two instances of singleton.");
  }
      
  if (!fRunConfiguration) {
    TG4Globals::Exception(
      "TG4RunManager", "TG4RunManager",
      "Cannot create instance without runConfiguration.");
  }
      
  fgInstance = this;
  
  if (VerboseLevel() > 1) {
    G4cout << "TG4RunManager has been created." << this << G4endl;
  }  
  
  // filter out "-splash" from argument list
  FilterARGV("-splash");

  // create and configure G4 run manager
  //ConfigureRunManager();

  // create geant4 UI
  CreateGeantUI();
      // must be created before TG4VisManager::Initialize()
      // (that is invoked in TGeant4 constructor)

  // create root UI
  CreateRootUI();
}

//_____________________________________________________________________________
TG4RunManager::TG4RunManager(TG4RunConfiguration* runConfiguration)
  : TG4Verbose("runManager"),
    fRunManager(0),
    fMessenger(this),
    fRunConfiguration(runConfiguration),
    fRegionsManager(0),
    fGeantUISession(0),
    fRootUISession(0),
    fRootUIOwner(false),
    fARGC(0),
    fARGV(0),
    fUseRootRandom(true) 
{
/// Default constructor

  G4cout << "TG4RunManager::TG4RunManager: " << this << G4endl;

  if (fgInstance) {
    TG4Globals::Exception(
      "TG4RunManager", "TG4RunManager",
      "Cannot create two instances of singleton.");
  }
      
  if (!fRunConfiguration) {
    TG4Globals::Exception(
      "TG4RunManager", "TG4RunManager",
      "Cannot create instance without runConfiguration.");
  }
      
  fgInstance = this;

  if (VerboseLevel() > 1) {
    G4cout << "TG4RunManager has been created." << this << G4endl;
  }  
  
  // set primary UI
  fRootUISession = gROOT->GetApplication();
  if (fRootUISession) {
    fARGC = fRootUISession->Argc();
    fARGV = fRootUISession->Argv();
  }

  // filter out "-splash" from argument list
  FilterARGV("-splash");

/*
  // create and configure G4 run manager
  ConfigureRunManager();

  if (VerboseLevel() > 1) {
    G4cout << "G4RunManager has been created." << G4endl;
  }  
*/
  // create geant4 UI
  CreateGeantUI();
      // must be created before TG4VisManager::Initialize()
      // (that is invoked in TGeant4 constructor)

  // create root UI
  CreateRootUI();
}

//_____________________________________________________________________________
TG4RunManager::~TG4RunManager() 
{
/// Destructor

  delete fRunConfiguration;
  delete fRegionsManager;
#ifdef G4UI_USE
  delete fGeantUISession;
#endif
  delete fRunManager;
  if (fRootUIOwner) delete fRootUISession;
}

//
// private methods
//

//_____________________________________________________________________________
void TG4RunManager::ConfigureRunManager()
{
/// Set the user action classes defined by TG4RunConfiguration to G4RunManager.

  // Geometry construction and navigator
  //
  G4cout << "TG4RunManager::ConfigureRunManager " << this << G4endl;

  TString userGeometry = fRunConfiguration->GetUserGeometry();
  
  TG4GeometryManager::Instance()
    ->SetUserRegionConstruction(
        fRunConfiguration->CreateUserRegionConstruction());
    
/*
  // Root navigator
  TG4RootNavMgr* rootNavMgr = 0;
  if ( userGeometry == "VMCtoRoot" || userGeometry == "Root" ) {

    // Construct geometry via VMC application
    if ( TG4GeometryManager::Instance()->VerboseLevel() > 0 ) 
      G4cout << "Running TVirtualMCApplication::ConstructGeometry"; 

    TG4StateManager::Instance()->SetNewState(kConstructGeometry);
    TVirtualMCApplication::Instance()->ConstructGeometry();
    TG4StateManager::Instance()->SetNewState(kNotInApplication);
    
    // Set top volume and close Root geometry if not yet done
    if ( ! gGeoManager->IsClosed() ) {
      TGeoVolume *top = (TGeoVolume*)gGeoManager->GetListOfVolumes()->First();
      gGeoManager->SetTopVolume(top);
      gGeoManager->CloseGeometry();  
    }
      
    // Now that we have the ideal geometry, call application misalignment code
    TG4StateManager::Instance()->SetNewState(kMisalignGeometry);
    TVirtualMCApplication::Instance()->MisalignGeometry();
    TG4StateManager::Instance()->SetNewState(kNotInApplication);
    
    // Pass geometry to G4Root navigator
    rootNavMgr = TG4RootNavMgr::GetInstance(gGeoManager);
  }  
*/
  // G4 run manager
#ifdef G4MULTITHREADED  
  fRunManager = new G4MTRunManager(); 
  fRunManager
    ->SetUserInitialization(new TG4WorkerInitialization());   
#else    
  fRunManager =  new G4RunManager();
#endif

  if ( userGeometry != "VMCtoRoot" && userGeometry != "Root" ) {
    fRunManager
      ->SetUserInitialization(fRunConfiguration->CreateDetectorConstruction());
    G4cout << "CreateDetectorConstruction done." << G4endl;
  }    
  else {
    TG4Globals::Exception(
      "TG4RunManager", "ConfigureRunManager",
      "Root navigation is not yet supported.");
    //rootNavMgr->Initialize(new TG4PostDetConstruction());
    //rootNavMgr->ConnectToG4();  
  }  
    
  // Other mandatory classes
  //  
  fRunManager
    ->SetUserInitialization(fRunConfiguration->CreatePhysicsList());
  G4cout << "CreatePhysicsList done." << G4endl;
  fRunManager
    ->SetUserInitialization(new TG4ActionInitialization(fRunConfiguration));      
  G4cout << "Create ActionInitialization done." << G4endl;
  
  // Regions manager
  //
  fRegionsManager = new TG4RegionsManager();
  
  G4cout << "TG4RunManager::ConfigureRunManager done " << this << G4endl;
}

//_____________________________________________________________________________
void TG4RunManager::CreateGeantUI()
{
/// Create interactive Geant4.

#ifdef G4UI_USE
  if ( ! fGeantUISession )
  {
    fGeantUISession = new G4UIExecutive(fARGC, fARGV);
  }
#endif  
}

//_____________________________________________________________________________
void TG4RunManager::CreateRootUI()
{
/// Create interactive Root.

  if (!fRootUISession) 
  {
    // create session if it does not exist  
    fRootUISession = new TRint("aliroot", 0, 0, 0, 0);

    // set ownership of Root UI
    fRootUIOwner = true;
  }
}

//_____________________________________________________________________________
void TG4RunManager::FilterARGV(const G4String& arg)
{
/// Filter out the option argument from the arguments list fARGV,
/// if present.

  if (fARGC == 1) return;

  G4bool isArg = false;
  for (G4int i=0; i<fARGC; i++) {
    if (G4String(fARGV[i]) == arg) isArg = true;
    if (isArg && i+1 < fARGC) fARGV[i] = fARGV[i+1];  
  }

  if (isArg) fARGC--;
} 
 
//_____________________________________________________________________________
void TG4RunManager::SetRandomSeed()
{
/// Pass the random number seed from fRandom to Geant4 random number
/// generator

  long seeds[10];
  seeds[0] = gRandom->GetSeed();    
  seeds[1] = gRandom->GetSeed();    
  seeds[2] = 0;    
  CLHEP::HepRandom::setTheSeeds(seeds);
}

// public methods

//_____________________________________________________________________________
void TG4RunManager::Initialize()
{
/// Initialize G4.

  G4cout << "TG4RunManager::Initialize " << this << G4endl;

  // create G4ParRunManager
  ConfigureRunManager();
  if (VerboseLevel() > 1) {
    G4cout << "G4RunManager has been created." << G4endl;
  }  

  // create geant4 UI
  //CreateGeantUI();
      // must be created before TG4VisManager::Initialize()
      // (that is invoked in TGeant4 constructor)

  // create root UI
  //CreateRootUI();
  
  // initialize Geant4
  G4cout << "G4RunManager::Initialize " << G4endl;
  fRunManager->Initialize();
  G4cout << "G4RunManager::Initialize done" << G4endl;

  // finish geometry
  TG4GeometryManager::Instance()->FinishGeometry();
  
  // initialize SD manager
  TG4SDManager::Instance()->Initialize();

  G4cout << "TG4RunManager::Initialize done " << this << G4endl;
}

//_____________________________________________________________________________
void TG4RunManager::LateInitialize()
{
/// Finish initialization of G4 after the G4Run initialization
/// is finished. 

  G4cout << "TG4RunManager::LateInitialize " << this << G4endl;
  G4bool isMaster = ! G4Threading::IsWorkerThread();

  // define particles 
  TG4PhysicsManager::Instance()->DefineParticles();

  // set user limits
  if ( isMaster ) {
    TG4GeometryManager::Instance()
      ->SetUserLimits(*TG4G3PhysicsManager::Instance()->GetCutVector(),
                      *TG4G3PhysicsManager::Instance()->GetControlVector());
  }                    

  // pass info if cut on e+e- pair is activated to stepping action  
  // TO DO LATER - Stepping Action NOT AVAILABLE                  
  //((TG4SteppingAction*)fRunManager->GetUserSteppingAction())
  //  ->SetIsPairCut((*TG4G3PhysicsManager::Instance()->GetIsCutVector())[kEplus]);                   

  // convert tracking cuts in range cuts per regions
  if ( fRunConfiguration->IsSpecialCuts() ) fRegionsManager->DefineRegions();

  // activate/inactivate physics processes
  TG4PhysicsManager::Instance()->SetProcessActivation();
  TG4PhysicsManager::Instance()->RetrieveOpBoundaryProcess();

  // print statistics
  TG4GeometryServices::Instance()->PrintStatistics(true, false);  
  TG4SDServices::Instance()->PrintStatistics(false, true);  

  if (VerboseLevel() > 2) {
    TG4GeometryServices::Instance()->PrintLogicalVolumeStore();  
  }
  
  // set the random number seed
  if ( fUseRootRandom ) SetRandomSeed();

  G4cout << "TG4RunManager::LateInitialize done " << this << G4endl;
}

//_____________________________________________________________________________
void TG4RunManager::ProcessEvent()
{
/// Not yet implemented.

  TG4Globals::Warning(
    "TG4RunManager", "ProcessEvent", "Not implemented.");
}
    
//_____________________________________________________________________________
Bool_t TG4RunManager::ProcessRun(G4int nofEvents)
{
/// Process Geant4 run.

  fRunManager->BeamOn(nofEvents); 

  G4bool result = ! TG4SDServices::Instance()->GetIsStopRun();
  TG4SDServices::Instance()->SetIsStopRun(false);
  
  return result;
}
    
//_____________________________________________________________________________
void TG4RunManager::StartGeantUI()
{ 
/// Start interactive/batch Geant4.

  if ( ! fGeantUISession ) CreateGeantUI();
  
  if ( fGeantUISession ) {  
#ifdef G4UI_USE
    // interactive session
    G4cout << "Welcome back in Geant4" << G4endl;
    fGeantUISession->GetSession()->SessionStart();
    G4cout << "Welcome back in Root" << G4endl;  
#endif    
  }
  else {
    G4cout << "Geant4 UI not available" << G4endl;
  }
}

//_____________________________________________________________________________
void TG4RunManager::StartRootUI()
{
/// Start interactive Root.

  if (!fRootUISession) CreateRootUI();
  if (fRootUISession) { 
    G4cout << "Welcome back in Root" << G4endl;
    fRootUISession->Run(kTRUE);
    G4cout << "Welcome back in Geant4" << G4endl;  
  }
}
 
//_____________________________________________________________________________
void TG4RunManager::ProcessGeantMacro(G4String macroName)
{
/// Process Geant4 macro.

  G4String command = "/control/execute " + macroName;
  ProcessGeantCommand(command);
}
 
//_____________________________________________________________________________
void TG4RunManager::ProcessRootMacro(G4String macroName)
{
/// Process Root macro.

  // load macro file
  G4String macroFile = macroName;
  macroFile.append(".C");
  gROOT->LoadMacro(macroFile);

  // execute macro function
  G4String macroFunction = macroName;
  macroFunction.append("()");
  gInterpreter->ProcessLine(macroFunction);
}

//_____________________________________________________________________________
void TG4RunManager::ProcessGeantCommand(G4String command)
{
/// Process Geant4 command.

  G4UImanager* pUI = G4UImanager::GetUIpointer();  
  G4int result = pUI->ApplyCommand(command);

  // From G4UIbatch::ExecCommand():
  switch ( result )  {
    case fCommandSucceeded:
      break;
    case fCommandNotFound:
      G4cerr << "***** COMMAND NOT FOUND <"
             << command << "> *****" << G4endl;
      break;
    case fIllegalApplicationState:
      G4cerr << "***** Illegal application state <"
             << command << "> *****" << G4endl;
      break;
    default:
      G4int pn = result%100;
      G4cerr << "***** Illegal parameter (" << pn << ") <"
             << command << "> *****" << G4endl;
  }
}

//_____________________________________________________________________________
void TG4RunManager::ProcessRootCommand(G4String command)
{
/// Process Root command.

  gInterpreter->ProcessLine(command);
}

//_____________________________________________________________________________
void TG4RunManager::UseG3Defaults() 
{
/// Control G3 defaults usage.

  TG4G3PhysicsManager::Instance()->SetG3DefaultCuts();
  TG4G3PhysicsManager::Instance()->SetG3DefaultControls();
}

//_____________________________________________________________________________
Int_t TG4RunManager::CurrentEvent() const
{
/// Return the number of the current event.

  G4int eventID = fRunManager->GetCurrentEvent()->GetEventID();
  return eventID;
}

//_____________________________________________________________________________
Bool_t  TG4RunManager::SecondariesAreOrdered() const 
{
///  Secondaries are ordered if the special stacking
/// (defined in TG4SpecialStackingAction) is activated.
// ----

  return fRunConfiguration->IsSpecialStacking();
}  

