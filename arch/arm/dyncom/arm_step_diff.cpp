/* Copyright (C) 
* 2011 - Michael.Kang blackfin.kang@gmail.com
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/
/**
* @file arm_step_diff.cpp
* @brief The step diff with the other simulator
* @author Michael.Kang blackfin.kang@gmail.com
* @version 78.77
* @date 2011-12-31
*/
#include "arm_dyncom_mmu.h"
#include "armdefs.h"
#include <skyeye_class.h>
#include <skyeye_core_intf.h>
#include <skyeye_interface.h>
#include <memory_space.h>

/**
* @brief Diff with old interpreter in every step
*
* @param cpu
*/
int diff_single_step(cpu_t *cpu){
#if 1
	static conf_object_t* arm11_core_obj = NULL;
	static core_run_intf* arm_run = NULL;
	static arm_core_t* state = NULL;
	int i;

	arm_core_t* core = (arm_core_t*)(cpu->cpu_data->obj);
	/* initialization for original interpreter */
	if(arm11_core_obj == NULL){
		/* initilize a arm11 core */
		arm11_core_obj = pre_conf_obj("arm11_core_0", "arm11_core");
		/* get the interface from the core */
		arm_run = (core_run_intf*)SKY_get_interface(arm11_core_obj, CORE_RUN_INTF_NAME);
		core_signal_intf* core_signal = (core_signal_intf*)SKY_get_interface(arm11_core_obj, CORE_SIGNAL_INTF_NAME);
		memory_space_intf* core_space = (memory_space_intf*)SKY_get_interface(arm11_core_obj, MEMORY_SPACE_INTF_NAME);

		/* initlize the s3c6410 machine */
		conf_object_t* s3c6410_mach = pre_conf_obj("s3c6410_mach_0", "s3c6410_mach");
		/* get the interface from the machine */	
		core_signal_intf* mach_signal = (core_signal_intf*)SKY_get_interface(s3c6410_mach, CORE_SIGNAL_INTF_NAME);
		memory_space_intf* mach_space = (memory_space_intf*)SKY_get_interface(s3c6410_mach, MEMORY_SPACE_INTF_NAME);

		/* connect the signal line between core and mach */	
		if(mach_signal->obj != NULL || mach_signal->signal != NULL){
			SKYEYE_ERR("Wrong value for interface\n");
			exit(-1);
		}
		mach_signal->obj = core_signal->obj;
		mach_signal->signal = core_signal->signal;

		/* connect the address space between core and mach */
		if(core_space->conf_obj != NULL || core_space->read != NULL || core_space->write != NULL)
		{
			SKYEYE_ERR("Wrong value for interface\n");
			exit(-1);

		}	
		core_space->conf_obj = mach_space->conf_obj;
		core_space->read = mach_space->read;
		core_space->write = mach_space->write;
		/* Should keep the same with the one in arm11_core.c */
		typedef struct arm11_core{
			conf_object_t* obj;
			ARMul_State* state;
			memory_space_intf* space;
		}arm11_core_t;
		arm11_core_t* arm11_core = (arm11_core_t*)(arm11_core_obj->obj);
		state = arm11_core->state;
		/* the core os*/
		#if 0
		arm11_core = (arm_core_t*)(arm11_core_obj->obj);
		arm11_core->mmu.phys_space.conf_obj = mach_space->conf_obj;
		arm11_core->mmu.phys_space.read = mach_space->read;
		arm11_core->mmu.phys_space.write = mach_space->write;
		#endif
		/* machine ID for SMDK6410 */
		arm_run->set_regval_by_id(arm11_core_obj, 1, 1626);
		//arch_instance->set_regval_by_id(1, 1626);			/* Sync all the register with fast interpreter */
		for(i = 0; i < 16; i++){
			arm_run->set_regval_by_id(arm11_core_obj, i, core->Reg[i]);
		}
	}
	/* the code of fast interpreter */
	//printf("ICOUNTER(0x%x)\n", core->icounter);
#if 0
	if(core->icounter == 100)
		exit(0);
#endif
	if(core->Reg[15] == 0xffff0018 || core->Reg[15] == 0xffff0214 
		|| core->Reg[15] == 0xffff020c /* irq */
		|| core->Reg[15] == 0xc002dba0 /* irq_svc */
		|| core->Reg[15] == 0xc002dbdc
		|| core->Reg[15] == 0xc002fcc0 /* __irq_svc */
		|| core->Reg[15] == 0xc002fd10 /* __irq_svc */
		|| core->Reg[15] == 0xc0048f14
		|| core->Reg[15] == 0xc004d7c0
		|| core->Reg[15] == 0xc002dda0 /* irq_usr */
		|| core->Reg[15] == 0xc0045060
		|| core->Reg[15] == 0xffff0208 /* irq */
		|| core->Reg[15] == 0xffff0010 /* abort irq */
		|| core->Reg[15] == 0xc0049700 /* interrupt enable */
		|| core->Reg[15] == 0xffff0308 /* irq */
		|| core->Reg[15] == 0xffff000c /* irq */
		|| core->Reg[15] == 0xffff0288 /* irq */
		)
		goto SYNC;
	/* diff all the register for last instruction */
	core->Cpsr = (core->Cpsr & 0x0fffffff) | \
                                                (core->NFlag << 31)   |                 \
                                                (core->ZFlag << 30)   |                 \
                                                (core->CFlag << 29)   |                 \
                                                (core->VFlag << 28);

	uint32 regval;
	uint32 instr;
	uint32 cpsr;
	for(i = 0; i < 16; i++){
		regval = arm_run->get_regval_by_id(arm11_core_obj, i);
		if(core->Cpsr & 0xF == 2){/* irq */
			if(i == 13){
				regval = arm_run->get_regval_by_id(arm11_core_obj, R13_IRQ);
			}
			else if(i == 14){
				regval = arm_run->get_regval_by_id(arm11_core_obj, R14_IRQ);
			}
		}
		if(core->Cpsr & 0xF == 3){/* svc */
			if(i == 13){
				regval = arm_run->get_regval_by_id(arm11_core_obj, R13_SVC);
			}
			else if(i == 14){
				regval = arm_run->get_regval_by_id(arm11_core_obj, R14_SVC);
			}
		}

		if(core->Cpsr & 0xF == 7){/* abort */
			if(i == 13){
				regval = arm_run->get_regval_by_id(arm11_core_obj, R13_ABORT);
			}
			else if(i == 14){
				regval = arm_run->get_regval_by_id(arm11_core_obj, R14_ABORT);
			}
		}

		if(regval != core->Reg[i]){
			instr = arm_run->get_regval_by_id(arm11_core_obj, 0xFF);
			skyeye_printf_in_color(RED, "ICOUNTER=%d(0x%x), instr=0x%x, diff Fail, orginal R[%d]=0x%x, wrong value 0x%x\n",core->icounter, core->Reg[15], instr, i, regval, core->Reg[i]);
			int j;
			for(j = 0; j < 16; j++){
				regval = arm_run->get_regval_by_id(arm11_core_obj, j);
				skyeye_printf_in_color(BLUE, "R[%d]=0x%x:0x%x\t", j, regval, core->Reg[j]);
			}
			printf("\n");
		}
		//printf("R[%d]=0x%x:0x%x\t", i, regval, core->Reg[i]);
	}
	cpsr = arm_run->get_regval_by_id(arm11_core_obj, CPSR_REG);
	if((core->Cpsr & 0xF0000000) != (cpsr & 0xF0000000)){
			instr = arm_run->get_regval_by_id(arm11_core_obj, 0xFF);
			skyeye_printf_in_color(RED, "ICOUNTER=%d(0x%x), instr=0x%x, diff Fail, orginal CPSR=0x%x, wrong value 0x%x\n",core->icounter, core->Reg[15], instr, arm_run->get_regval_by_id(arm11_core_obj, CPSR_REG), core->Cpsr);
			int j;
			for(j = 0; j < 16; j++){
				regval = arm_run->get_regval_by_id(arm11_core_obj, j);
				skyeye_printf_in_color(BLUE, "R[%d]=0x%x:0x%x\t", j, regval, core->Reg[j]);
			}
			printf("\n");

	}
	//printf("\n");
SYNC:
	/* Sync all the register with fast interpreter */
	for(i = 0; i < 16; i++){
		if(core->Cpsr & 0xF == 2){/* irq */
			if(i == 13){
				arm_run->set_regval_by_id(arm11_core_obj, R13_IRQ, core->Reg[i]);
			}
			else if(i == 14){
				arm_run->set_regval_by_id(arm11_core_obj, R14_IRQ, core->Reg[i]);
			}
			continue;
		}
		if(core->Cpsr & 0xF == 3){/* svc */
			if(i == 13){
				arm_run->set_regval_by_id(arm11_core_obj, R13_SVC, core->Reg[i]);
			}
			else if(i == 14){
				arm_run->set_regval_by_id(arm11_core_obj, R14_SVC, core->Reg[i]);
			}
			continue;
		}
		if(core->Cpsr & 0xF == 7){/* abort */
			if(i == 13){
				arm_run->set_regval_by_id(arm11_core_obj, R13_ABORT, core->Reg[i]);
			}
			else if(i == 14){
				arm_run->set_regval_by_id(arm11_core_obj, R14_ABORT, core->Reg[i]);
			}
			continue;
		}

		arm_run->set_regval_by_id(arm11_core_obj, i, core->Reg[i]);
	}
	arm_run->set_regval_by_id(arm11_core_obj, CPSR_REG, core->Cpsr);
	/* run the instruction before the fast interpreter */
	arm_run->step_once(arm11_core_obj);
#if 0
	skyeye_printf_in_color(BLUE, "After old interp run:");
	for(i = 0; i < 16; i++){
		regval = arm_run->get_regval_by_id(arm11_core_obj, i);
		skyeye_printf_in_color(BLUE, "R[%d]=0x%x:0x%x\t", i, regval, core->Reg[i]);
	}
	printf("\n");
#endif
#endif
	return 0;
}
