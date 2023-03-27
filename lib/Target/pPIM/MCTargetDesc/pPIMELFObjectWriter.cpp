//===-- pPIMELFObjectWriter.cpp - pPIM ELF Writer ---------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/pPIMMCTargetDesc.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
  class pPIMELFObjectWriter : public MCELFObjectTargetWriter {
  public:
    pPIMELFObjectWriter(uint8_t OSABI);

    virtual ~pPIMELFObjectWriter();

    unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                          const MCFixup &Fixup, bool IsPCRel) const override;
    };
}

pPIMELFObjectWriter::pPIMELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/*Is64Bit*/ false, OSABI,
                              /*ELF::EM_pPIM*/ ELF::EM_pPIM,
                              /*HasRelocationAddend*/ false) {}

pPIMELFObjectWriter::~pPIMELFObjectWriter() {}

unsigned pPIMELFObjectWriter::getRelocType(MCContext &Ctx,
                                            const MCValue &Target,
                                            const MCFixup &Fixup,
                                            bool IsPCRel) const {
  llvm_unreachable("pPIMELFObjectWriter: invalid fixup kind");
}

MCObjectWriter *llvm::createpPIMELFObjectWriter(raw_pwrite_stream &OS,
                                                uint8_t OSABI) {
  MCELFObjectTargetWriter *MOTW = new pPIMELFObjectWriter(OSABI);
  return createELFObjectWriter(MOTW, OS, /*IsLittleEndian=*/true);
}
