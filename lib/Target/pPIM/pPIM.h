//===-- pPIM.h - Top-level interface for pPIM representation ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM pPIM back-end.
//
//===----------------------------------------------------------------------===//

#ifndef TARGET_pPIM_H
#define TARGET_pPIM_H

#include "MCTargetDesc/pPIMMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace pPIMCC {
  // pPIM specific condition codes
  enum CondCodes {
    COND_U      = 0,    // unconditional
    COND_C      = 8,    // carry
    COND_N      = 4,    // negative
    COND_V      = 2,    // overflow
    COND_Z      = 1,    // zero
    COND_NC     = 7,    // not carry
    COND_NN     = 11,   // not negative
    COND_NV     = 13,   // not overflow
    COND_NZ     = 14,   // not zero
    COND_GE     = 6,    // greater or equal
    COND_L      = 9,    // less than

    COND_INVALID = -1
  };
}

namespace llvm {
class pPIMTargetMachine;
class MCInst;
class MachineInstr;

FunctionPass *createpPIMISelDag(pPIMTargetMachine &TM,
                               CodeGenOpt::Level OptLevel);

} // end namespace llvm;

#endif
