//===-- pPIMTargetMachine.h - Define TargetMachine for pPIM -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the pPIM specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef pPIMTARGETMACHINE_H
#define pPIMTARGETMACHINE_H

#include "pPIM.h"
#include "pPIMSubtarget.h"
#include "MCTargetDesc/pPIMMCTargetDesc.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class pPIMTargetMachine : public LLVMTargetMachine {
  pPIMSubtarget Subtarget;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;

public:
  pPIMTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                   StringRef FS, const TargetOptions &Options,
                   Optional<Reloc::Model> RM, CodeModel::Model CM,
                   CodeGenOpt::Level OL);

  // Pass Pipeline Configuration
  virtual TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  
  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

  virtual const TargetSubtargetInfo *
  getSubtargetImpl(const Function &) const override {
    return &Subtarget;
  }
  
};

} // end namespace llvm

#endif
