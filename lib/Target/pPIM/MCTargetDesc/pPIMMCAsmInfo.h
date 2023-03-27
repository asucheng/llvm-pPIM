//===-- pPIMMCAsmInfo.h - pPIM asm properties --------------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the pPIMMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef pPIMTARGETASMINFO_H
#define pPIMTARGETASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class pPIMMCAsmInfo : public MCAsmInfoELF {
  virtual void anchor();

public:
  explicit pPIMMCAsmInfo(const Triple &TT);
};

} // namespace llvm

#endif
