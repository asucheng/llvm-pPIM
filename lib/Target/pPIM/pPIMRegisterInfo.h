//===-- pPIMRegisterInfo.h - pPIM Register Information Impl ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the pPIM implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef pPIMREGISTERINFO_H
#define pPIMREGISTERINFO_H

#include "llvm/Target/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "pPIMGenRegisterInfo.inc"

namespace llvm {

class TargetInstrInfo;

struct pPIMRegisterInfo : public pPIMGenRegisterInfo {

  pPIMRegisterInfo();

  const uint16_t *getCalleeSavedRegs(const MachineFunction *MF = 0) const
      override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  const TargetRegisterClass*
  getPointerRegClass(const MachineFunction &MF,
                     unsigned Kind = 0) const override;

  void eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  unsigned getFrameRegister(const MachineFunction &MF) const override;
};
}

#endif
