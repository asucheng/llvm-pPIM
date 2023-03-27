//===-- pPIMInstPrinter.cpp - Convert pPIM MCInst to asm syntax -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an pPIM MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "pPIM.h"
#include "pPIMInstPrinter.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

// Include the auto-generated portion of the assembly writer.
#include "pPIMGenAsmWriter.inc"

void pPIMInstPrinter::printInst(const MCInst *MI, raw_ostream &O,
                                 StringRef Annot, const MCSubtargetInfo &STI) {
  printInstruction(MI, O);
  printAnnotation(O, Annot);
}

void pPIMInstPrinter::printRegName(raw_ostream &O, unsigned RegNo) const {
  O << StringRef(getRegisterName(RegNo));
}

void pPIMInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                  raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);

  if (Op.isReg()) {
    printRegName(O, Op.getReg());
    return;
  }

  if (Op.isImm()) {
    O << Op.getImm();
    return;
  }

  if (Op.isExpr()) {
    Op.getExpr()->print(O, &MAI);
    return;
  }

  llvm_unreachable("Unknown operand");
}

// Print a memsrc (defined in pPIMInstrInfo.td)
// This is an operand which defines a location for loading or storing which
// is a register offset by an immediate value
void pPIMInstPrinter::printMemSrcOperand(const MCInst *MI, unsigned OpNo,
                                           raw_ostream &O) {
  const MCOperand &BaseAddr = MI->getOperand(OpNo);
  const MCOperand &Offset = MI->getOperand(OpNo + 1);
  
  assert(Offset.isImm() && "Expected immediate in displacement field");

  O << "M[";
  printRegName(O, BaseAddr.getReg());
  unsigned OffsetVal = Offset.getImm();
  if (OffsetVal) {
    O << "+" << Offset.getImm();
  }
  O << "]";
}

void pPIMInstPrinter::printPCRelImmOperand(const MCInst *MI, unsigned OpNo,
                                             raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isImm())
    O << Op.getImm();
  else {
    assert(Op.isExpr() && "unknown pcrel immediate operand");
    Op.getExpr()->print(O, &MAI);
  }
}

void pPIMInstPrinter::printCCOperand(const MCInst *MI, unsigned OpNo,
                                       raw_ostream &O) {
  unsigned CC = MI->getOperand(OpNo).getImm();

  switch (CC) {
  default:
   llvm_unreachable("Unsupported CC code");
  case pPIMCC::COND_C:
   O << "c";
   break;
  case pPIMCC::COND_N:
   O << "n";
   break;
  case pPIMCC::COND_V:
   O << "v";
   break;
  case pPIMCC::COND_Z:
   O << "eq";
   break;
  case pPIMCC::COND_NC:
   O << "nc";
   break;
  case pPIMCC::COND_NN:
   O << "nn";
   break;
  case pPIMCC::COND_NV:
   O << "nv";
   break;
  case pPIMCC::COND_NZ:
   O << "ne";
   break;
  case pPIMCC::COND_GE:
   O << "ge";
   break;
  case pPIMCC::COND_L:
   O << "l";
   break;

  }
}
