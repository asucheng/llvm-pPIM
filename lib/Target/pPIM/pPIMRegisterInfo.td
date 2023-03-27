//===-- pPIMRegisterInfo.td - pPIM Register defs -------------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the pPIM register file 
//===----------------------------------------------------------------------===//

// General register definition
class pPIMReg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "pPIM";
}

// Define all CPU registers

// Special purpose registers
def SR : pPIMReg<0, "r0">;
def PC : pPIMReg<1, "r1">;
def SP : pPIMReg<2, "r2">;

// General purpose registers
foreach i = 3-31 in {
  def R#i : pPIMReg< #i, "r"##i>;
}

def StatusReg : RegisterClass<"pPIM", [i32], 32, (add
	SR // R0
)>;

def ProgramCounterReg : RegisterClass<"pPIM", [i32], 32, (add
	PC //R1
)>;

// General purpose registers class
def GPRegs : RegisterClass<"pPIM", [i32], 32, (add
	(sequence "R%u", 4, 31), SP, R3
)>;
