//===-- pPIMInstrInfo.td - Target Description for pPIM -------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the pPIM instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

include "pPIMInstrFormats.td"


//===----------------------------------------------------------------------===//
// Type Profiles.
//===----------------------------------------------------------------------===//

// Ensure the two operands of compare are of the same type
def SDT_pPIMCmp          : SDTypeProfile<0, 2, [SDTCisSameAs<0, 1>]>;
def SDT_pPIMBrCC         : SDTypeProfile<0, 2, [SDTCisVT<0, OtherVT>,
                                               SDTCisVT<1, i32>]>;
def SDT_pPIMSelectCC     : SDTypeProfile<1, 3, [SDTCisSameAs<0, 1>,
                                               SDTCisSameAs<1, 2>, 
                                               SDTCisVT<3, i32>]>;
def SDT_pPIMWrapper      : SDTypeProfile<1, 1, [SDTCisSameAs<0, 1>,
                                               SDTCisPtrTy<0>]>;
def SDT_pPIMCall         : SDTypeProfile<0, -1, [SDTCisVT<0, iPTR>]>;
def SDT_pPIMCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i32>]>;
def SDT_pPIMCallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;

//===----------------------------------------------------------------------===//
// pPIM Specific Node Definitions.
//===----------------------------------------------------------------------===//

def pPIMretFlag : SDNode<"pPIMISD::RET_FLAG", SDTNone,
                     [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def pPIMcmp     : SDNode<"pPIMISD::CMP", SDT_pPIMCmp, [SDNPOutGlue]>;
def pPIMbrcc    : SDNode<"pPIMISD::BR_CC", SDT_pPIMBrCC, [SDNPHasChain,
                                                       SDNPInGlue]>;
def pPIMSelectCC: SDNode<"pPIMISD::SELECT_CC", SDT_pPIMSelectCC, [SDNPInGlue]>;
def pPIMWrapper : SDNode<"pPIMISD::Wrapper", SDT_pPIMWrapper>;
def pPIMcall    : SDNode<"pPIMISD::CALL", SDT_pPIMCall,
                      [SDNPHasChain, SDNPOutGlue, SDNPOptInGlue, SDNPVariadic]>;
def pPIMcallseq_start :
                 SDNode<"ISD::CALLSEQ_START", SDT_pPIMCallSeqStart,
                        [SDNPHasChain, SDNPOutGlue]>;
def pPIMcallseq_end :
                 SDNode<"ISD::CALLSEQ_END",   SDT_pPIMCallSeqEnd,
                        [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

//===----------------------------------------------------------------------===//
// pPIM Operand Definitions.
//===----------------------------------------------------------------------===//

// Immediate/Constant operand for ALU/data-transfer instructions
def pPIMimm16 : Operand<i32>, ImmLeaf<i32, [{
  return Imm >= -32768 && Imm < 32768;
}]>;

// Immediate/Constant operand for rotate/shift instructions
def pPIMimm6 : Operand<i32>, ImmLeaf<i32, [{
  return Imm >= 0 && Imm < 64;
}]>;

// Address operand for indexed addressing mode
def memsrc : Operand<i32> {
  let PrintMethod = "printMemSrcOperand";
  let EncoderMethod = "getMemSrcValue";
  let MIOperandInfo = (ops GPRegs, pPIMimm16);
}

// Pointer memory type
def memptr : Operand<iPTR> {
  let PrintMethod = "printMemSrcOperand";
  let EncoderMethod = "getMemSrcValue";
  let MIOperandInfo = (ops GPRegs, pPIMimm16);
}

// Short jump targets have OtherVT type and are printed as pcrel imm values.
def jmptarget : Operand<OtherVT> {
  let PrintMethod = "printPCRelImmOperand";
}

// Operand for printing out a condition code.
def cc : Operand<i32> {
  let PrintMethod = "printCCOperand";
}

//===----------------------------------------------------------------------===//
// pPIM Complex Pattern Definitions.
//===----------------------------------------------------------------------===//

def addr : ComplexPattern<iPTR, 2, "SelectAddr", [], []>;

//===----------------------------------------------------------------------===//
// ALU Instructions
//===----------------------------------------------------------------------===//

let Defs = [SR] in {

multiclass ALU<bits<5> opcode, string opstr, SDNode opnode> {

  def rr : ALU_Inst_RR<opcode, (outs GPRegs:$ri),
                   (ins GPRegs:$rj, GPRegs:$rk),
                   !strconcat(opstr, "\t$ri, $rj, $rk"),
                   [(set GPRegs:$ri, (opnode GPRegs:$rj, GPRegs:$rk)),
                    (implicit SR)]> {
  }
  
  def ri : ALU_Inst_RI<opcode, (outs GPRegs:$ri),
                   (ins GPRegs:$rj, pPIMimm16:$const),
                   !strconcat(opstr, "\t$ri, $rj, $const"),
                   [(set GPRegs:$ri, (opnode GPRegs:$rj, pPIMimm16:$const)),
                    (implicit SR)]> {
  }
}

defm ADD  : ALU<0b01000, "add", add>;
defm SUB  : ALU<0b01001, "sub", sub>;
defm AND  : ALU<0b01100, "and", and>;
defm OR   : ALU<0b01110, "or",  or>;
defm XOR  : ALU<0b01111, "xor", xor>;
defm MUL  : ALU<0b11010, "mul", mul>;
defm DIV  : ALU<0b11011, "div", udiv>;

// Instructions that need their own dag pattern

def NOTrr : ALU_Inst_RR<0b01011,
                    (outs GPRegs:$ri),
                    (ins GPRegs:$rj),
                    "not\t$ri, $rj",
                    [(set GPRegs:$ri, (not GPRegs:$rj)),
                     (implicit SR)]> {
  let rk = 0b00000; // rk not used
}

def BICrr : ALU_Inst_RR<0b01101,
                    (outs GPRegs:$ri),
                    (ins GPRegs:$rj, GPRegs:$rk),
                    "bic\t$ri, $rj, $rk",
                    [(set GPRegs:$ri, (and GPRegs:$rj, (not GPRegs:$rk))),
                     (implicit SR)]> {}

def BICri : ALU_Inst_RR<0b01101,
                    (outs GPRegs:$ri),
                    (ins GPRegs:$rj, pPIMimm16:$const),
                    "bic\t$ri, $rj, $const",
                    [(set GPRegs:$ri, (and GPRegs:$rj, (not pPIMimm16:$const))),
                     (implicit SR)]> {}

} // Defs = [SR]

//===----------------------------------------------------------------------===//
// Shift/Rotate Instructions
//===----------------------------------------------------------------------===//

let Defs = [SR] in {

multiclass RotShift<bits<3> rs_op, string opstr, SDNode opnode> {

  def rr : RotShift_Inst_RR<rs_op, 
                          (outs GPRegs:$ri),
                          (ins GPRegs:$rj, GPRegs:$rk),
                          !strconcat(opstr, "\t$ri, $rj, $rk"),
                          [(set GPRegs:$ri, (opnode GPRegs:$rj, GPRegs:$rk)),
                          (implicit SR)]> {}

  def ri : RotShift_Inst_RI<rs_op,
                   (outs GPRegs:$ri),
                   (ins GPRegs:$rj, pPIMimm6:$const),
                   !strconcat(opstr, "\t$ri, $rj, $const"),
                   [(set GPRegs:$ri, (opnode GPRegs:$rj, pPIMimm6:$const)),
                    (implicit SR)]> {}
}

defm SRL  : RotShift<0b000, "srl", srl>; // shift right logical
defm SLL  : RotShift<0b001, "sll", shl>; // shift left logical
defm SRA  : RotShift<0b010, "sra", sra>; // shift left arithmetic
defm RTR  : RotShift<0b100, "rtr", rotr>; // rotate right
defm RTL  : RotShift<0b101, "rtl", rotl>; // rotate left

let Uses = [SR] in {
  //defm RRC  : RotShift<0b110, "rrc">; // rotate right through carry
  //defm RLC  : RotShift<0b111, "rlc">; // rotate left through carry
} // Uses = [SR]
} // Defs = [SR]

//===----------------------------------------------------------------------===//
// Data Transfer Instructions
//===----------------------------------------------------------------------===//

def CPYrr  : DT_Inst_RR<0b00010,
                    (outs GPRegs:$ri), (ins GPRegs:$rj),
                    "cpy\t$ri, $rj",
                    []>;

let isAsCheapAsAMove = 1 in {
  def CPYri  : DT_Inst_RI<0b00010,
                      (outs GPRegs:$ri), (ins pPIMimm16:$const),
                      "cpy\t$ri, $const",
                      [(set GPRegs:$ri, pPIMimm16:$const)]>;
} // isAsCheapAsAMove = 1

let Defs = [SP], hasSideEffects=0 in {

  def PUSH  : DT_Inst_RR<0b00011,
                    (outs), (ins GPRegs:$rj),
                    "push\t$rj",
                    []> {
    let ri = 0b00000; // destination register not used
  }

  def POP   : DT_Inst_RR<0b00100,
                    (outs GPRegs:$ri), (ins),
                    "pop\t$ri",
                    []> {
    let rj = 0b00000; // source register not used
  }

} // Defs = [SP], hasSideEffects=0

//===----------------------------------------------------------------------===//
// Comparison Instructions
//===----------------------------------------------------------------------===//

let Defs = [SR] in {

multiclass CMP_Inst<bits<5> opcode, string opstr> {
  
  def rr : ALU_Inst_RR<opcode, (outs),
                   (ins GPRegs:$rj, GPRegs:$rk),
                   !strconcat(opstr, "\t$rj, $rk"),
                   [(pPIMcmp GPRegs:$rj, GPRegs:$rk),
                   (implicit SR)]> {
    let ri = 0b00000;
  }
  
  def ri : ALU_Inst_RI<opcode, (outs),
                   (ins GPRegs:$rj, pPIMimm16:$const),
                   !strconcat(opstr, "\t$rj, $const"),
                   [(pPIMcmp GPRegs:$rj, pPIMimm16:$const),
                   (implicit SR)]> {
    let ri = 0b00000;
  }
}

defm CMP  : CMP_Inst<0b01010, "cmp">;

} // Defs = [SR]

//===----------------------------------------------------------------------===//
// Load/Store Instructions
//===----------------------------------------------------------------------===//

def LD  : LS_Inst<0b00000,
                  (outs GPRegs:$ri), (ins memsrc:$addr),
                  "ld\t$ri, $addr",
                  [(set GPRegs:$ri, (load addr:$addr))]> {
  let control = 0b0;  // This forces indexed addressing mode
}

def ST  : LS_Inst<0b00001,
                  (outs), (ins memsrc:$addr, GPRegs:$ri),
                  "st\t$addr, $ri",
                  [(store GPRegs:$ri, addr:$addr)]> {
  let control = 0b0;  // This forces indexed addressing mode
}

//===----------------------------------------------------------------------===//
//  Call Instructions
//===----------------------------------------------------------------------===//

let isCall = 1 in
  // All calls clobber the non-callee saved registers. SPW is marked as
  // a use to prevent stack-pointer assignments that appear immediately
  // before calls from potentially appearing dead. Uses for argument
  // registers are added manually.
  let Uses = [SP] in {
    def CALLi : FC_Inst<0b00110,
                        (outs), (ins pPIMimm16:$addr),
                        "call\t$addr", [(pPIMcall imm:$addr)]> {
      let ri = 0b00000;
      let condition = 0b0000;
      let control = 0b1;
    }
  } // Uses = [SP]

//===----------------------------------------------------------------------===//
// Return Instructions
//===----------------------------------------------------------------------===//

let isTerminator = 1, isReturn = 1, isBarrier = 1 in {
  def RET : InstpPIM<(outs), (ins),
                    "ret",  [(pPIMretFlag)]> {
    let Opcode = 0b00111;
    let Inst{26-0}  = 0b000000000000000000000000000;
  }
}

//===----------------------------------------------------------------------===//
// Jump (branch) Instructions
//===----------------------------------------------------------------------===//

// Unconditional jump
let isBranch = 1, isTerminator = 1, isBarrier = 1 in {
  def JMP : FC_Inst<0b00101,
                    (outs), (ins jmptarget:$addr),
                    "jmp\t$addr",
                    [(br bb:$addr)]> {
  // set ri to 0 and control to 1 for absolute addressing mode
  let ri = 0b00000;
  let condition = 0b0000;
  let control = 0b1;
}

} // isBranch = 1, isTerminator = 1, isBarrier = 1

// Conditional jump
let isBranch = 1, isTerminator = 1, Uses=[SR] in {
def JCC : FC_Inst<0b00101,
                  (outs), (ins jmptarget:$addr, cc:$condition),
                  "j$condition\t$addr",
                  [(pPIMbrcc bb:$addr, imm:$condition)]> {
  // set ri to 0 and control to 1 for absolute addressing mode
  let ri = 0b00000;
  let control = 0b1;
}
} // isBranch = 1, isTerminator = 1

//===----------------------------------------------------------------------===//
// Pseudo Instructions
//===----------------------------------------------------------------------===//

let Defs = [SP], Uses = [SP] in {
def ADJCALLSTACKDOWN : pPIMPseudoInst<(outs), (ins pPIMimm16:$amt),
                                     "#ADJCALLSTACKDOWN",
                                     [(pPIMcallseq_start timm:$amt)]>;
def ADJCALLSTACKUP : pPIMPseudoInst<(outs), (ins pPIMimm16:$amt1, pPIMimm16:$amt2),
                                   "#ADJCALLSTACKUP",
                                   [(pPIMcallseq_end timm:$amt1, timm:$amt2)]>;
}

//===----------------------------------------------------------------------===//
// Non-Instruction Patterns
//===----------------------------------------------------------------------===//
let usesCustomInserter = 1 in
def Select : pPIMPseudoInst<(outs GPRegs:$dst),
                    (ins GPRegs:$src, GPRegs:$src2, i32imm:$condition),
                    "# Select PSEUDO",
                    [(set GPRegs:$dst,
                     (pPIMSelectCC GPRegs:$src, GPRegs:$src2, imm:$condition))]>;

// GlobalAddress, ExternalSymbol
def : Pat<(i32 (pPIMWrapper tglobaladdr:$dst)), (CPYri tglobaladdr:$dst)>;
def : Pat<(i32 (pPIMWrapper texternalsym:$dst)), (CPYri texternalsym:$dst)>;

def : Pat<(add GPRegs:$src, (pPIMWrapper tglobaladdr:$src2)),
          (ADDri GPRegs:$src, tglobaladdr:$src2)>;
def : Pat<(add GPRegs:$src, (pPIMWrapper texternalsym:$src2)),
          (ADDri GPRegs:$src, texternalsym:$src2)>;

def : Pat<(store (i32 (pPIMWrapper tglobaladdr:$src)), addr:$dst),
          (ST addr:$dst, tglobaladdr:$src)>;
def : Pat<(store (i32 (pPIMWrapper texternalsym:$src)), addr:$dst),
          (ST addr:$dst, texternalsym:$src)>;

// Calls
def : Pat<(pPIMcall (i32 tglobaladdr:$addr)),
          (CALLi tglobaladdr:$addr)>;
def : Pat<(pPIMcall (i32 texternalsym:$addr)),
          (CALLi texternalsym:$addr)>;
