// $Id: TG4PhysicsConstructorMuon.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: physics
//
// Author: I. Hrivnacova
//
// Class TG4PhysicsConstructorMuon
// -------------------------------
// Constructor of electromagnetic physics.
// According to ExN04MuonPhysics.hh, GEANT4 tag Name: geant4-06-00

#ifndef TG4_PHYSICS_CONSTRUCTOR_MUON_H
#define TG4_PHYSICS_CONSTRUCTOR_MUON_H

#include "TG4VPhysicsConstructor.h"

#include "G4MultipleScattering.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuIonisation.hh"
#include "G4MuonMinusCaptureAtRest.hh"
#include "G4hIonisation.hh"
#include <globals.hh>

class TG4PhysicsConstructorMuon: public TG4VPhysicsConstructor
{
  public:
    TG4PhysicsConstructorMuon(const G4String& name = "Muon");
    TG4PhysicsConstructorMuon(G4int verboseLevel,
                              const G4String& name = "Muon");
    virtual ~TG4PhysicsConstructorMuon();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    // data members
          // mu+ physics
   G4MultipleScattering   fMuPlusMultipleScattering;  //mu+ msc
   G4MuBremsstrahlung     fMuPlusBremsstrahlung ;     //mu+ Bremsstrahlung
   G4MuPairProduction     fMuPlusPairProduction;      //mu+ pair production
   G4MuIonisation         fMuPlusIonisation;          //mu+ ionisation

          // mu- physics
   G4MultipleScattering   fMuMinusMultipleScattering; //mu- msc
   G4MuBremsstrahlung     fMuMinusBremsstrahlung ;    //mu- Bremsstrahlung
   G4MuPairProduction     fMuMinusPairProduction;     //mu- pair production
   G4MuIonisation         fMuMinusIonisation;         //mu- ionisation
   G4MuonMinusCaptureAtRest fMuMinusCaptureAtRest;    //mu- capture

          // tau+ physics
   G4MultipleScattering   fTauPlusMultipleScattering; //tau+ msc
   G4hIonisation          fTauPlusIonisation;         //tau+ ionisation

          // tau+ physics
   G4MultipleScattering   fTauMinusMultipleScattering;//tau- msc
   G4hIonisation          fTauMinusIonisation;        //tau- ionisation
    
  private:
    // methods
    void ConstructProcessForMuonPlus();    
    void ConstructProcessForMuonMinus();    
    void ConstructProcessForTauPlus();    
    void ConstructProcessForTauMinus();    
};

#endif //TG4_PHYSICS_CONSTRUCTOR_MUON_H

