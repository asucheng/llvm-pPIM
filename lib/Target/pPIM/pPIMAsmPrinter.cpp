//===-- pPIMAsmPrinter.cpp - pPIM LLVM assembly writer ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to the pPIM assembly language.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "asm-printer"
#include "pPIM.h"
#include "InstPrinter/pPIMInstPrinter.h"
#include "pPIMInstrInfo.h"
#include "pPIMMCInstLower.h"
#include "pPIMSubtarget.h"
#include "pPIMTargetMachine.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineJumpTableInfo.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Mangler.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include <algorithm>
#include <cctype>

using namespace llvm;

namespace {
  class pPIMAsmPrinter : public AsmPrinter {
    pPIMMCInstLower MCInstLowering;

  public:
    explicit pPIMAsmPrinter(TargetMachine &TM,
                           std::unique_ptr<MCStreamer> Streamer)
        : AsmPrinter(TM, std::move(Streamer)),
          MCInstLowering(OutContext, *this) {}

    virtual StringRef getPassName() const {
        return StringRef("pPIM Assembly Printer");
    }
      
//    void EmitBasicBlockStart(const MachineBasicBlock &MBB) const;
    void EmitFunctionEntryLabel();
    void EmitInstruction(const MachineInstr *MI);
    void EmitFunctionBodyStart();
  };
} // end of anonymous namespace

void pPIMAsmPrinter::EmitFunctionBodyStart() {}

void pPIMAsmPrinter::EmitFunctionEntryLabel() {
  OutStreamer->EmitLabel(CurrentFnSym);
}

void pPIMAsmPrinter::EmitInstruction(const MachineInstr *MI) {
  MCInst TmpInst;
  MCInstLowering.Lower(MI, TmpInst);
  EmitToStreamer(*OutStreamer, TmpInst);
}

//void pPIMAsmPrinter::EmitBasicBlockStart(const MachineBasicBlock &MBB) const {
//    
//}

// Force static initialization.
extern "C" void LLVMInitializepPIMAsmPrinter() {
  RegisterAsmPrinter<pPIMAsmPrinter> X(getThepPIMTarget());
}
