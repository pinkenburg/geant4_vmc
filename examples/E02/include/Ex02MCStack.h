// $Id: Ex02MCStack.h,v 1.2 2003/02/04 17:55:34 brun Exp $
//
// Geant4 ExampleN02 adapted to Virtual Monte Carlo 
//
// Class Ex02MCStack
// -----------------
// Implementation of the TVirtualMCStack interface
//
// by Ivana Hrivnacova, 5.4.2002


#ifndef EX02_STACK_H
#define EX02_STACK_H

#include "Ex02Particle.h"

#include <TVirtualMCStack.h>

#include <stack>

class Ex02MCStack : public TVirtualMCStack
{
  public:
    Ex02MCStack(Int_t size);
    Ex02MCStack();
    virtual ~Ex02MCStack();     

    // methods
    virtual void  SetTrack(Int_t toBeDone, Int_t parent, Int_t pdg,
  	              Double_t px, Double_t py, Double_t pz, Double_t e,
  		      Double_t vx, Double_t vy, Double_t vz, Double_t tof,
		      Double_t polx, Double_t poly, Double_t polz,
		      TMCProcess mech, Int_t& ntr, Double_t weight,
		      Int_t is) ;
    virtual TParticle* GetNextTrack(Int_t& track);
    virtual TParticle* GetPrimaryForTracking(Int_t i); 
    void Print() const;   
    void Reset();   
   
    // set methods
    virtual void  SetCurrentTrack(Int_t track);                           

    // get methods
    virtual Int_t  GetNtrack() const;
    virtual Int_t  GetNprimary() const;
    virtual Int_t  CurrentTrack() const;
    virtual Int_t  CurrentTrackParent() const;
    Ex02Particle*  GetParticle(Int_t id) const;
    
  private:
    // data members
    std::stack<Ex02Particle*>  fStack;    //!
    TObjArray*                 fParticles;
    Int_t                      fCurrentTrack;
    Int_t                      fNPrimary;
    
    ClassDef(Ex02MCStack,1) // Ex02MCStack
};

#endif //EX02_STACK_H   
   

