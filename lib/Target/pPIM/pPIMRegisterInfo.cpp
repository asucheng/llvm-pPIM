//===-- pPIMRegisterInfo.cpp - pPIM Register Information ----------*- C++ -*-===//
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

#include "pPIMRegisterInfo.h"
#include "pPIM.h"
#include "pPIMSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetFrameLowering.h"
#include "llvm/Target/TargetInstrInfo.h"

#define GET_REGINFO_TARGET_DESC
#include "pPIMGenRegisterInfo.inc"

using namespace llvm;

pPIMRegisterInfo::pPIMRegisterInfo() : pPIMGenRegisterInfo(pPIM::PC) {}

const MCPhysReg *
pPIMRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CC_Save_SaveList; // from tablegen
}

BitVector pPIMRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  Reserved.set(pPIM::SR); // status regsiter
  Reserved.set(pPIM::PC); // program counter
  Reserved.set(pPIM::SP); // stack pointer
  
  return Reserved;
}

const TargetRegisterClass *
pPIMRegisterInfo::getPointerRegClass(const MachineFunction &MF, unsigned Kind)
                                                                         const {
  return &pPIM::GPRegsRegClass;
}

void pPIMRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                          int SPAdj, unsigned FIOperandNum,
                                          RegScavenger *RS) const {
  MachineInstr &MI = *II;
  MachineBasicBlock &MBB = *MI.getParent();
  const MachineFunction &MF = *MBB.getParent();
  const MachineFrameInfo MFI = MF.getFrameInfo();
  MachineOperand &FIOp = MI.getOperand(FIOperandNum);
  unsigned FI = FIOp.getIndex();
    
  // Determine if we can eliminate the index from this kind of instruction.
  unsigned ImmOpIdx = 0;
  switch (MI.getOpcode()) {
  default:
    llvm_unreachable("Instruction not supported");
  case pPIM::LD:
  case pPIM::ST:
  case pPIM::ADDri:
    ImmOpIdx = FIOperandNum + 1;
    break;
  }
    
  // FIXME: check the size of offset.
  MachineOperand &ImmOp = MI.getOperand(ImmOpIdx);
  int Offset = MFI.getObjectOffset(FI) + MFI.getStackSize() + ImmOp.getImm();
  if (Offset % 4) {
    llvm_unreachable("Offset must be aligned to 4 bytes because memory is "
                     "32-bit word addressable only");
  }
  FIOp.ChangeToRegister(pPIM::SP, false);
  ImmOp.setImm(Offset);
}

unsigned pPIMRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return pPIM::SP;
}
