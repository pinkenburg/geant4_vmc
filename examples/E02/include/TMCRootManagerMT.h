// @(#)root/vmc:$Name:  $:$Id$
// Author: Ivana Hrivnacova, IPN Orsay 17/02/2012

/*************************************************************************
 * Copyright (C) 2006, Rene Brun and Fons Rademakers.                    *
 * Copyright (C) 2012, Ivana Hrivnacova.                         *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TMCRootManagerMT
#define ROOT_TMCRootManagerMT

#include "TVirtualMCRootManager.h"

class TMCRootManagerImpl;

//
// Class TMCRootManagerMT
// ----------------------
// The Root IO manager for VMC examples for multi-threaded applications.
// It implements the TVirtualMCRootManager interface.

class TMCRootManagerMT : public TVirtualMCRootManager
{
  public:
    TMCRootManagerMT(const char* projectName, FileMode fileMode = kWrite);
    virtual ~TMCRootManagerMT();     
  
    // static methods
    static void Initialize(Int_t nofWorkers);

    // methods
    virtual void  Register(const char* name, const char* className, void* objAddress);
    virtual void  Fill();
    virtual void  WriteAll();
    virtual void  Close();
    virtual void  ReadEvent(Int_t i);
    
  private:
    // not implemented
    TMCRootManagerMT(const TMCRootManagerMT& rhs);
    TMCRootManagerMT& operator=(const TMCRootManagerMT& rhs);

    // global static data members
    static  Int_t    fgNofWorkers;      // The total number of workers
    static  Int_t    fgCounter;         // The counter of instances
    static  Bool_t   fgIsFillLock;      // The if the Fill should be locked 
    static  Bool_t*  fgIsFillLocks;     // The info per threadif the Fill should be locked

    // data members 
    Int_t                fId;           // This manager ID 
    TMCRootManagerImpl*  fRootManager;  // The Root manager
};

#endif //ROOT_TMCRootManagerMT
