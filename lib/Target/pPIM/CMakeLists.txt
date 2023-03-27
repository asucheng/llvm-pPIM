set(LLVM_TARGET_DEFINITIONS pPIM.td)

tablegen(LLVM pPIMGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM pPIMGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM pPIMGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM pPIMGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM pPIMGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM pPIMGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM pPIMGenCallingConv.inc -gen-callingconv)
tablegen(LLVM pPIMGenDAGISel.inc -gen-dag-isel)

add_public_tablegen_target(pPIMCommonTableGen)

add_llvm_target(pPIMCodeGen
  pPIMAsmPrinter.cpp
  pPIMFrameLowering.cpp
  pPIMInstrInfo.cpp
  pPIMISelDAGToDAG.cpp
  pPIMISelLowering.cpp
  pPIMMCInstLower.cpp
  pPIMRegisterInfo.cpp
  pPIMSubtarget.cpp
  pPIMTargetMachine.cpp
  pPIMMachineFunctionInfo.cpp
  )

add_subdirectory(InstPrinter)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
