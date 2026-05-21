/*
 * intexc.c
 *
 */

#include "intexc.h"


//uint32_t *PLIC_MCLAIM = (uint32_t *) 0x008001B0;

//----------------------------
// exception
//-----------------------------

// define exception vector table instance
volatile PE_VEC_TABLE ExcVectTable;

void exception_handler(){
	// read mcause register to get exception code
	uint32_t exception_code;
	__asm__ volatile(
			"csrr	%0, mcause\n"
			: "=r"(exception_code)
			:
			:
	);

	// call exception handler by exception_code
	ExcVectTable.arr[exception_code]();
}

//---------------------------
// interrupt
//-----------------------------

// define interrupt vector table instance
volatile PI_VEC_TABLE  IntVectTable;

void enable_interrupt(uint32_t id){

	// get plic_mie reg number and filed
	uint32_t reg_address = PLIC_MIE_BASE + (id / 32 * 4);
	uint32_t reg_value = 1 << (id % 32);

	// read current value
	uint32_t current_val = *((uint32_t*) reg_address);
	// add interrupt for id
	current_val |= reg_value;

	// write reg
	*((uint32_t*)reg_address) = current_val;
}

void disable_interrupt(uint32_t id){

	// get plic_mie reg number and filed
	uint32_t reg_address = PLIC_MIE_BASE + (id / 32 * 4);
	uint32_t reg_value = ~(1 << (id % 32));

	// read current value
	uint32_t current_val = *((uint32_t*) reg_address);
	// add interrupt for id
	current_val &= reg_value;

	// write reg
	*((uint32_t*)reg_address) = current_val;
}

void init_priority(){
	// Eight interrupt sources are grouped.
	// The priority decreases as the interrupt number increases
	uint32_t current_address;
	uint32_t current_priority;
	// write every plic_prio register
	for (int i = 1; i < INTERRUPT_NUM; i++){
		current_address = PLIC_PRIO_BASE + (i * 4);
		current_priority = HIGHEST_PRIORITY - ((i - 1) / 8);
		// write priority register
		*((uint32_t*)current_address) = current_priority;
	}

	// some user-defined trap
	uint32_t HIGHEST_PRIORITY_USER_DEFINED = current_priority - 1;
	for (int i = 1; i <= INTERRUPT_USER_DEFINED_NUM; i++){
			current_address = PLIC_PRIO_USER_DEFINED_BASE + ((i - 1) * 4);
			current_priority = HIGHEST_PRIORITY_USER_DEFINED - ((i - 1) / 8);
			// write priority register
			*((uint32_t*)current_address) = current_priority;
	}
	// write PLIC_H0_MTH (priority threshold)
	*((uint32_t*) PLIC_H0_MTH) = 2;
}

/*
void interrupt_handler(){

	// read mcause register
	uint32_t ex_code;
	__asm__ volatile(
			"csrr	t1, mcause\n"
			// for interrupt, highest bit is 1, clear it
			"slli	t1, t1,	1\n"
			"srli	%0, t1, 1\n"
			: "=r"(ex_code)
			:
			: "t1"
	);
	// check mcause to get interrupt type
	if (ex_code == 1 || ex_code == 3){
		// software interrupt
	} else if (ex_code == 5 || ex_code == 7){
		// timer interrupt
	} else if (ex_code == 9 || ex_code == 11){
		// external interrupt
		// read claim to get id
		uint32_t id = *((uint32_t*) PLIC_MCLAIM);
		if (IntVectTable.arr[id] != 0){
			// call isr according to id
			IntVectTable.arr[id]();
		} else {
			for(;;);
		}

		// write claim back
		*((uint32_t*) PLIC_MCLAIM) = id;
		
	} else {
		// Designated for platform use

	}

}*/

// to generate user defined trap
void gen_user_trap(int ti_id){
	// write PLIC_IP3 interrupt id bit to generate trap
	// ti's 20 => sc plic id 99; 21 => 100; ....
	// convert to sc plic interrupt id
	int intr_id = ti_id - 20 + 99;
	// PLIC_PL3 address: 0x800198, 32 bit register, for id 96 - 127
	int reg_bit = intr_id - 96;
	unsigned int reg_address = 0x800198;
	// read current value
	uint32_t current_val = *((uint32_t*) reg_address);
	// add interrupt for id
	current_val |= 1 << reg_bit;
	// write reg
	*((uint32_t*)reg_address) = current_val;
}
