//===-- pPIMMCTargetDesc.cpp - pPIM Target Descriptions ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides pPIM specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "pPIMMCTargetDesc.h"
#include "pPIMMCAsmInfo.h"
#include "InstPrinter/pPIMInstPrinter.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "pPIMGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "pPIMGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "pPIMGenRegisterInfo.inc"

using namespace llvm;

static MCInstrInfo *createpPIMMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitpPIMMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createpPIMMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitpPIMMCRegisterInfo(X, pPIM::PC);
  return X;
}

static MCAsmInfo *createpPIMMCAsmInfo(const MCRegisterInfo &MRI,
                                       const Triple &TT) { 
  return new pPIMMCAsmInfo(TT);
}

static MCInstPrinter *
createpPIMMCInstPrinter(const Triple &TT, unsigned SyntaxVariant,
                       const MCAsmInfo &MAI, const MCInstrInfo &MII,
                       const MCRegisterInfo &MRI) {
  return new pPIMInstPrinter(MAI, MII, MRI);
}

extern "C" void LLVMInitializepPIMTargetMC() {
  RegisterMCAsmInfoFn X(getThepPIMTarget(), createpPIMMCAsmInfo);
  TargetRegistry::RegisterMCInstrInfo(getThepPIMTarget(), createpPIMMCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(getThepPIMTarget(), createpPIMMCRegisterInfo);
  TargetRegistry::RegisterMCAsmBackend(getThepPIMTarget(), createpPIMAsmBackend);
  TargetRegistry::RegisterMCCodeEmitter(getThepPIMTarget(), createpPIMMCCodeEmitter);
  TargetRegistry::RegisterMCInstPrinter(getThepPIMTarget(), createpPIMMCInstPrinter);
}
