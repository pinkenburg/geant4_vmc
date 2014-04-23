// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4WorkerInitialization.cxx
/// \brief Implementation of the TG4WorkerInitialization class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4WorkerInitialization.h"
#include "TG4RunManager.h"

#include <TVirtualMCApplication.h>

#include <G4AutoLock.hh>

namespace {
  //Mutex to lock master application when merging data
  G4Mutex deleteMutex = G4MUTEX_INITIALIZER;
}

//_____________________________________________________________________________
TG4WorkerInitialization::TG4WorkerInitialization()
  : G4UserWorkerInitialization()
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4WorkerInitialization::~TG4WorkerInitialization()
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
void TG4WorkerInitialization::WorkerRunStart() const
{
/// Call post initialization on workers

  //G4cout << "TG4WorkerInitialization::WorkerRunStart() " << G4endl;

  TG4RunManager::Instance()->LateInitialize();
  
  TVirtualMCApplication::Instance()->BeginWorkerRun();

  //G4cout << "TG4WorkerInitialization::WorkerRunStart() end " << G4endl;
}   

/*
//_____________________________________________________________________________
void TG4WorkerInitialization::WorkerRunEnd() const
{
// This method is called for each thread, when the local event loop has
// finished but before the synchronization over threads.

  G4cout << "TG4WorkerInitialization::WorkerRunEnd() " << G4endl;

  TVirtualMCApplication::Instance()->FinishWorkerRun();

  G4cout << "TG4WorkerInitialization::WorkerRunEnd() end " << G4endl;

}
*/
//_____________________________________________________________________________
void TG4WorkerInitialization::WorkerStop() const
{
/// This method is called once at the end of simulation job.
/// It implements a clean up action, which is the clean-up of MC application
/// in our case.

  //G4cout << "TG4WorkerInitialization::WorkerStop() " << this << G4endl;

  TVirtualMCApplication::Instance()->FinishWorkerRun();

  //G4cout << "Go to delete MCApplication " << TVirtualMCApplication::Instance() << G4endl;

  G4AutoLock lm(&deleteMutex);
  delete TVirtualMCApplication::Instance();
  lm.unlock();

  //G4cout << "TG4WorkerInitialization::WorkerStop() end " << G4endl;
}
