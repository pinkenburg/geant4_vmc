// $Id: TG4PhysicsConstructorStackPopper.cxx,v 1.1 2007/05/10 14:44:53 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics
// Class TG4PhysicsConstructorStackPopper
// --------------------------------------
// See the class description in the header file.

#include "TG4PhysicsConstructorStackPopper.h"
#include "TG4StackPopper.h"

#include <G4ProcessManager.hh>


//_____________________________________________________________________________
TG4PhysicsConstructorStackPopper::TG4PhysicsConstructorStackPopper(
                                                   const G4String& name)
  : TG4VPhysicsConstructor(name),
    fStackPopperProcess(0),
    fSelection() 
{
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorStackPopper::TG4PhysicsConstructorStackPopper(
                                                   G4int verboseLevel,
                                                   const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel), 
    fStackPopperProcess(0), 
    fSelection() 
{
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorStackPopper::~TG4PhysicsConstructorStackPopper() 
{
//
  delete fStackPopperProcess;
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4PhysicsConstructorStackPopper::ConstructParticle()
{
/// No particles instatiated 

}

//_____________________________________________________________________________
void TG4PhysicsConstructorStackPopper::ConstructProcess()
{
/// Set step limiter process to all particles

  fStackPopperProcess = new TG4StackPopper();

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){

    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    // add this as an option
    if ( fSelection.size() == 0 ||
         fSelection.find(particle->GetParticleName()) != std::string::npos )
         
      G4cout << "Adding StackPopper process to " 
             <<  particle->GetParticleName() << G4endl;
             
      pmanager ->AddContinuousProcess(fStackPopperProcess);
  }
  
  // no G3 control for this process
  // no TMCProcess code

  if (VerboseLevel() > 0) {
    G4cout << "### Stack popper special physics constructed." << G4endl;
  }  
}