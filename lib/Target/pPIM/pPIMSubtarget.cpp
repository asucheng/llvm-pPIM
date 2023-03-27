//===-- pPIMSubtarget.cpp - pPIM Subtarget Information ----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the pPIM specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "pPIM.h"
#include "pPIMFrameLowering.h"
#include "pPIMSubtarget.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "pPIM-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "pPIMGenSubtargetInfo.inc"

void pPIMSubtarget::anchor() {}

pPIMSubtarget::pPIMSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                           pPIMTargetMachine &TM)
    : pPIMGenSubtargetInfo(TT, CPU, FS), InstrInfo(), FrameLowering(*this),
      TLInfo(TM, *this) {}
