//===-- pPIMTargetInfo.cpp - pPIM Target Implementation ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/pPIMMCTargetDesc.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target &llvm::getThepPIMTarget() {
  static Target ThepPIMTarget;
  return ThepPIMTarget;
}

extern "C" void LLVMInitializepPIMTargetInfo() {
  RegisterTarget<Triple::pPIM> X(getThepPIMTarget(), "pPIM", "pPIM");
}
