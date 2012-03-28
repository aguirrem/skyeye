#ifndef __ARM_DYNCOM_RUN__
#define __ARM_DYNCOM_RUN__

#include "skyeye_dyncom.h"
void arm_dyncom_run(cpu_t* cpu);
void arm_dyncom_init(arm_core_t* core);
void switch_mode(arm_core_t *core, uint32_t mode);
void arch_arm_undef(cpu_t *cpu, BasicBlock *bb, uint32_t instr);
void arch_arm_invalidate_by_mva(cpu_t *cpu, BasicBlock *bb, Value* mva);
void arch_arm_invalidate_by_asid(cpu_t *cpu, BasicBlock *bb, Value* asid);
extern uint32_t is_int_in_interpret(cpu_t *cpu);

/* FIXME, we temporarily think thumb instruction is always 16 bit */
static inline uint32 GET_INST_SIZE(arm_core_t* core){
	return core->TFlag? 2 : 4;
}

/**
* @brief Read R15 and forced R15 to wold align, used address calculation
*
* @param core
* @param Rn
*
* @return 
*/
static inline addr_t CHECK_READ_REG15_WA(arm_core_t* core, int Rn){
	return (Rn == 15)? ((core->Reg[15] & ~0x3) + GET_INST_SIZE(core) * 2) : core->Reg[Rn];
}

/**
* @brief Read R15, used to data processing with pc
*
* @param core
* @param Rn
*
* @return 
*/
static inline uint32 CHECK_READ_REG15(arm_core_t* core, int Rn){
	return (Rn == 15)? ((core->Reg[15] & ~0x1) + GET_INST_SIZE(core) * 2) : core->Reg[Rn];
}

#endif

