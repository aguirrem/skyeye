/* Copyright (C) 
* 
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
* @file emif_6713.c
* @brief The implementation of system controller
* @author 
* @version 78.77
*/

#include <skyeye_types.h>
#include <skyeye_sched.h>
#include <skyeye_signal.h>
#include <skyeye_class.h>
#include <skyeye_interface.h>
#include <skyeye_obj.h>
#include <skyeye_mm.h> 
#include <memory_space.h>
#include <skyeye_device.h>
#define DEBUG
#include <skyeye_log.h>

#include "emif_6713.h"

static exception_t emif_6713_read(conf_object_t *opaque, generic_address_t offset, void* buf, size_t count)
{
	struct emif_6713_device *dev = opaque->obj;
	emif_6713_reg_t* regs = dev->regs;
	switch(offset) {
		case 0x0:
			*(uint32_t *)buf = regs->gbl_ctl;
			break;
		case 0x4:
			*(uint32_t *)buf = regs->ce_ctl[1];
			break;
		case 0x8:
			*(uint32_t *)buf = regs->ce_ctl[0];
			break;
		case 0x10:
			*(uint32_t *)buf = regs->ce_ctl[2];
			break;
		case 0x14:
			*(uint32_t *)buf = regs->ce_ctl[3];
			break;

		case 0x18:
			*(uint32_t *)buf = regs->sd_ctl;
			break;
		case 0x1c:
			*(uint32_t *)buf = regs->sd_tim;
			break;
		case 0x20:
			*(uint32_t *)buf = regs->sd_ext;
			break;

		default:
			printf("Can not read the register at 0x%x in emif_6713\n", offset);
			return Invarg_exp;
	}
	return No_exp;

}

static exception_t emif_6713_write(conf_object_t *opaque, generic_address_t offset, uint32_t* buf, size_t count)
{
	struct emif_6713_device *dev = opaque->obj;
	emif_6713_reg_t* regs = dev->regs;
	uint32_t val = *(uint32_t*)buf;
	switch(offset) {
		case 0x0:
			regs->gbl_ctl = val;
			break;
		case 0x4:
			regs->ce_ctl[1] = val;
			break;
		case 0x8:
			regs->ce_ctl[0] = val;
			break;
		case 0x10:
			regs->ce_ctl[2] = val;
			break;
		case 0x14:
			regs->ce_ctl[3] = val;
			break;

		case 0x18:
			regs->sd_ctl = val;
			break;
		case 0x1c:
			regs->sd_tim = val;
			break;
		case 0x20:
			regs->sd_ext = val;
			break;

		default:
			printf("Can not write the register at 0x%x in emif_6713\n", offset);
			return Invarg_exp;
	}
	return No_exp;

}
static conf_object_t* new_emif_6713(char* obj_name){
	emif_6713_device* dev = skyeye_mm_zero(sizeof(emif_6713_device));
	emif_6713_reg_t* regs =  skyeye_mm_zero(sizeof(emif_6713_reg_t));
	dev->obj = new_conf_object(obj_name, dev);
	/* init emif_6713 regs */
	dev->regs = regs;

	/* Register io function to the object */
	memory_space_intf* io_memory = skyeye_mm_zero(sizeof(memory_space_intf));
	io_memory->conf_obj = dev->obj;
	io_memory->read = emif_6713_read;
	io_memory->write = emif_6713_write;
	SKY_register_interface(io_memory, obj_name, MEMORY_SPACE_INTF_NAME);	
	return dev->obj;
}
void free_emif_6713(conf_object_t* dev){
	
}

void init_emif_6713(){
	static skyeye_class_t class_data = {
		.class_name = "emif_6713",
		.class_desc = "emif_6713",
		.new_instance = new_emif_6713,
		.free_instance = free_emif_6713,
		.get_attr = NULL,
		.set_attr = NULL
	};

	SKY_register_class(class_data.class_name, &class_data);
}