#ifndef TG4_RUN_ACTION_H
#define TG4_RUN_ACTION_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4RunAction.h
/// \brief Definition of the TG4RunAction class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Verbose.h"
#include "TG4CrossSectionManager.h"

#include <G4UserRunAction.hh>
#include <globals.hh>

class G4Timer;
    // in order to avoid the odd dependency for the
    // times system function this declaration must be the first
class G4Run;


/// \ingroup run
/// \brief Actions at the beginning and the end of run.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4RunAction : public G4UserRunAction,
                     public TG4Verbose
{
  public:
    TG4RunAction();
    virtual ~TG4RunAction();

    // methods
    virtual void BeginOfRunAction(const G4Run* run);
    virtual void EndOfRunAction(const G4Run* run);

  private:
    /// Not implemented
    TG4RunAction(const TG4RunAction& right);
    /// Not implemented
    TG4RunAction& operator=(const TG4RunAction& right);

    // data members
    TG4CrossSectionManager  fCrossSectionManager; ///< cross section manager
    G4Timer*  fTimer; ///< G4Timer
    G4int     fRunID; ///< run ID
};

#endif //TG4_RUN_ACTION_H
