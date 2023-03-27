//===-- pPIMFixupKinds.h - pPIM Specific Fixup Entries ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef pPIMFIXUPKINDS_H
#define pPIMFIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

namespace llvm {
namespace pPIM {

  // This table *must* be in the same order of
  // MCFixupKindInfo Infos[pPIM::NumTargetFixupKinds]
  // in pPIMAsmBackend.cpp.
  enum Fixups {
    fixup_pPIM_NONE = FirstTargetFixupKind,

    // Pure 32 but data fixup
    fixup_pPIM_32,

    // Marker
    LastTargetFixupKind,
    NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
  };
} // namespace pPIM
} // namespace llvm


#endif
