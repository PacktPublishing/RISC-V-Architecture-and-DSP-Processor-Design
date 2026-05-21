//###########################################################################
//
// FILE:   intexc.c
//
// TITLE:   
//
//###########################################################################
// $FDMTEK Release: $
// $Release Date:  $
// $Copyright:
//  (C) Copyright 2023 QingDao Fundamental Microelectronic .Ltd - http://www.fdmtek.com
//###########################################################################

/*
 * intexc.c
 *
 */

#include "../include/intexc.h"


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

// nmi handler pointer
volatile TRAP_HANDLER  p_nmi_handler;

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

void Default_ISR(void){
	asm(" EBREAK");
	for(;;);
}

/* Default isr for plic vector table init*/
const struct PI_HANDLER_TABLE PlicVectTableInit = {
		Default_ISR, // RESERVED,
		// Group 1 Plic Peripheral Vectors
		Default_ISR, // default isr for SEQ1INT,
		Default_ISR, // default isr for SEQ2INT,
		Default_ISR, // default isr for rsvd1_3,
		Default_ISR, // default isr for XINT1,
		Default_ISR, // default isr for XINT2,
		Default_ISR, // default isr for ADCINT,
		Default_ISR, // default isr for TINT0,
		Default_ISR, // default isr for WAKEINT,
		// Group 2 Plic Peripheral Vectors
		Default_ISR, // default isr for EPWM1_TZINT,
		Default_ISR, // default isr for EPWM2_TZINT,
		Default_ISR, // default isr for EPWM3_TZINT,
		Default_ISR, // default isr for EPWM4_TZINT,
		Default_ISR, // default isr for EPWM5_TZINT,
		Default_ISR, // default isr for EPWM6_TZINT,
		Default_ISR, // default isr for rsvd2_7,
		Default_ISR, // default isr for rsvd2_8,
		// Group 3 Plic Peripheral Vectors
		Default_ISR, // default isr for EPWM1_INT,
		Default_ISR, // default isr for EPWM2_INT,
		Default_ISR, // default isr for EPWM3_INT,
		Default_ISR, // default isr for EPWM4_INT,
		Default_ISR, // default isr for EPWM5_INT,
		Default_ISR, // default isr for EPWM6_INT,
		Default_ISR, // default isr for rsvd3_7,
		Default_ISR, // default isr for rsvd3_8,
		// Group 4 Plic Peripheral Vectors
		Default_ISR, // default isr for ECAP1_INT,
		Default_ISR, // default isr for ECAP2_INT,
		Default_ISR, // default isr for ECAP3_INT,
		Default_ISR, // default isr for ECAP4_INT,
		Default_ISR, // default isr for ECAP5_INT,
		Default_ISR, // default isr for ECAP6_INT,
		Default_ISR, // default isr for rsvd4_7,
		Default_ISR, // default isr for rsvd4_8,
		// Group 5 Plic Peripheral Vectors
		Default_ISR, // default isr for EQEP1_INT,
		Default_ISR, // default isr for EQEP2_INT,
		Default_ISR, // default isr for rsvd5_3,
		Default_ISR, // default isr for rsvd5_4,
		Default_ISR, // default isr for rsvd5_5,
		Default_ISR, // default isr for rsvd5_6,
		Default_ISR, // default isr for rsvd5_7,
		Default_ISR, // default isr for rsvd5_8,
		// Group 6 Plic Peripheral Vectors
		Default_ISR, // default isr for SPIRXINTA,
		Default_ISR, // default isr for SPITXINTA,
		Default_ISR, // default isr for MRINTB,
		Default_ISR, // default isr for MXINTB,
		Default_ISR, // default isr for MRINTA,
		Default_ISR, // default isr for MXINTA,
		Default_ISR, // default isr for rsvd6_7,
		Default_ISR, // default isr for rsvd6_8,
		// Group 7 Plic Peripheral Vectors
		Default_ISR, // default isr for DINTCH1,
		Default_ISR, // default isr for DINTCH2,
		Default_ISR, // default isr for DINTCH3,
		Default_ISR, // default isr for DINTCH4,
		Default_ISR, // default isr for DINTCH5,
		Default_ISR, // default isr for DINTCH6,
		Default_ISR, // default isr for rsvd7_7,
		Default_ISR, // default isr for rsvd7_8,
		// Group 8 Plic Peripheral Vectors
		Default_ISR, // default isr for I2CINT1A,
		Default_ISR, // default isr for I2CINT2A,
		Default_ISR, // default isr for rsvd8_3,
		Default_ISR, // default isr for rsvd8_4,
		Default_ISR, // default isr for SCIRXINTC,
		Default_ISR, // default isr for SCITXINTC,
		Default_ISR, // default isr for rsvd8_7,
		Default_ISR, // default isr for rsvd8_8,
		// Group 9 Plic Peripheral Vectors
		Default_ISR, // default isr for SCIRXINTA,
		Default_ISR, // default isr for SCITXINTA,
		Default_ISR, // default isr for SCIRXINTB,
		Default_ISR, // default isr for SCITXINTB,
		Default_ISR, // default isr for ECAN0INTA,
		Default_ISR, // default isr for ECAN1INTA,
		Default_ISR, // default isr for ECAN0INTB,
		Default_ISR, // default isr for ECAN1INTB,
		// Group 10 Plic Peripheral Vectors
		Default_ISR, // default isr for rsvd10_1,
		Default_ISR, // default isr for rsvd10_2,
		Default_ISR, // default isr for rsvd10_3,
		Default_ISR, // default isr for rsvd10_4,
		Default_ISR, // default isr for rsvd10_5,
		Default_ISR, // default isr for rsvd10_6,
		Default_ISR, // default isr for rsvd10_7,
		Default_ISR, // default isr for rsvd10_8,
		// Group 11 Plic Peripheral Vectors
		Default_ISR, // default isr for rsvd11_1,
		Default_ISR, // default isr for rsvd11_2,
		Default_ISR, // default isr for rsvd11_3,
		Default_ISR, // default isr for rsvd11_4,
		Default_ISR, // default isr for rsvd11_5,
		Default_ISR, // default isr for rsvd11_6,
		Default_ISR, // default isr for rsvd11_7,
		Default_ISR, // default isr for rsvd11_8,
		// Group 12 Plic Peripheral Vectors
		Default_ISR, // default isr for XINT3,
		Default_ISR, // default isr for XINT4,
		Default_ISR, // default isr for XINT5,
		Default_ISR, // default isr for XINT6,
		Default_ISR, // default isr for XINT7,
		Default_ISR, // default isr for rsvd12_6,
		Default_ISR, // default isr for LVF,
		Default_ISR, // default isr for LUF,
		// timer1
		Default_ISR, // default isr for TIMER1,
		// timer2
		Default_ISR, // default isr for TIME2,
		// user defined traps
		Default_ISR, // default isr for USER1,
		Default_ISR, // default isr for USER2,
		Default_ISR, // default isr for USER3,
		Default_ISR, // default isr for USER4,
		Default_ISR, // default isr for USER5,
		Default_ISR, // default isr for USER6,
		Default_ISR, // default isr for USER7,
		Default_ISR, // default isr for USER8,
		Default_ISR, // default isr for USER9,
		Default_ISR, // default isr for USER10,
		Default_ISR, // default isr for USER11,
		Default_ISR, // default isr for USER12,
};

const uint8_t PlicPriorityInit[INTERRUPT_NUM + 1] = {
		0x0, // reserved
		// Group 1 Plic Peripheral Vectors
		0xf, //default priority for SEQ1INT,
		0xf, //default priority for SEQ2INT,
		0xf, //default priority for rsvd1_3,
		0xf, //default priority for XINT1,
		0xf, //default priority for XINT2,
		0xf, //default priority for ADCINT,
		0xf, //default priority for TINT0,
		0xf, //default priority for WAKEINT,
		// Group 2 Plic Peripheral Vectors
		0xf, //default priority for EPWM1_TZINT,
		0xf, //default priority for EPWM2_TZINT,
		0xf, //default priority for EPWM3_TZINT,
		0xf, //default priority for EPWM4_TZINT,
		0xf, //default priority for EPWM5_TZINT,
		0xf, //default priority for EPWM6_TZINT,
		0xf, //default priority for rsvd2_7,
		0xf, //default priority for rsvd2_8,
		// Group 3 Plic Peripheral Vectors
		0xf, //default priority for EPWM1_INT,
		0xf, //default priority for EPWM2_INT,
		0xf, //default priority for EPWM3_INT,
		0xf, //default priority for EPWM4_INT,
		0xf, //default priority for EPWM5_INT,
		0xf, //default priority for EPWM6_INT,
		0xf, //default priority for rsvd3_7,
		0xf, //default priority for rsvd3_8,
		// Group 4 Plic Peripheral Vectors
		0xf, //default priority for ECAP1_INT,
		0xf, //default priority for ECAP2_INT,
		0xf, //default priority for ECAP3_INT,
		0xf, //default priority for ECAP4_INT,
		0xf, //default priority for ECAP5_INT,
		0xf, //default priority for ECAP6_INT,
		0xf, //default priority for rsvd4_7,
		0xf, //default priority for rsvd4_8,
		// Group 5 Plic Peripheral Vectors
		0xf, //default priority for EQEP1_INT,
		0xf, //default priority for EQEP2_INT,
		0xf, //default priority for rsvd5_3,
		0xf, //default priority for rsvd5_4,
		0xf, //default priority for rsvd5_5,
		0xf, //default priority for rsvd5_6,
		0xf, //default priority for rsvd5_7,
		0xf, //default priority for rsvd5_8,
		// Group 6 Plic Peripheral Vectors
		0xf, //default priority for SPIRXINTA,
		0xf, //default priority for SPITXINTA,
		0xf, //default priority for MRINTB,
		0xf, //default priority for MXINTB,
		0xf, //default priority for MRINTA,
		0xf, //default priority for MXINTA,
		0xf, //default priority for rsvd6_7,
		0xf, //default priority for rsvd6_8,
		// Group 7 Plic Peripheral Vectors
		0xf, //default priority for DINTCH1,
		0xf, //default priority for DINTCH2,
		0xf, //default priority for DINTCH3,
		0xf, //default priority for DINTCH4,
		0xf, //default priority for DINTCH5,
		0xf, //default priority for DINTCH6,
		0xf, //default priority for rsvd7_7,
		0xf, //default priority for rsvd7_8,
		// Group 8 Plic Peripheral Vectors
		0xf, //default priority for I2CINT1A,
		0xf, //default priority for I2CINT2A,
		0xf, //default priority for rsvd8_3,
		0xf, //default priority for rsvd8_4,
		0xf, //default priority for SCIRXINTC,
		0xf, //default priority for SCITXINTC,
		0xf, //default priority for rsvd8_7,
		0xf, //default priority for rsvd8_8,
		// Group 9 Plic Peripheral Vectors
		0xf, //default priority for SCIRXINTA,
		0xf, //default priority for SCITXINTA,
		0xf, //default priority for SCIRXINTB,
		0xf, //default priority for SCITXINTB,
		0xf, //default priority for ECAN0INTA,
		0xf, //default priority for ECAN1INTA,
		0xf, //default priority for ECAN0INTB,
		0xf, //default priority for ECAN1INTB,
		// Group 10 Plic Peripheral Vectors
		0xf, //default priority for rsvd10_1,
		0xf, //default priority for rsvd10_2,
		0xf, //default priority for rsvd10_3,
		0xf, //default priority for rsvd10_4,
		0xf, //default priority for rsvd10_5,
		0xf, //default priority for rsvd10_6,
		0xf, //default priority for rsvd10_7,
		0xf, //default priority for rsvd10_8,
		// Group 11 Plic Peripheral Vectors
		0xf, //default priority for rsvd11_1,
		0xf, //default priority for rsvd11_2,
		0xf, //default priority for rsvd11_3,
		0xf, //default priority for rsvd11_4,
		0xf, //default priority for rsvd11_5,
		0xf, //default priority for rsvd11_6,
		0xf, //default priority for rsvd11_7,
		0xf, //default priority for rsvd11_8,
		// Group 12 Plic Peripheral Vectors
		0xf, //default priority for XINT3,
		0xf, //default priority for XINT4,
		0xf, //default priority for XINT5,
		0xf, //default priority for XINT6,
		0xf, //default priority for XINT7,
		0xf, //default priority for rsvd12_6,
		0xf, //default priority for LVF,
		0xf, //default priority for LUF,
		// timer1
		0xf, //default priority for TIMER1,
		// timer2
		0xf, //default priority for TIME2,
		// user defined traps
		0xf, //default priority for USER1,
		0xf, //default priority for USER2,
		0xf, //default priority for USER3,
		0xf, //default priority for USER4,
		0xf, //default priority for USER5,
		0xf, //default priority for USER6,
		0xf, //default priority for USER7,
		0xf, //default priority for USER8,
		0xf, //default priority for USER9,
		0xf, //default priority for USER10,
		0xf, //default priority for USER11,
		0xf, //default priority for USER12,
};

void init_plic_vect_table(void){
	int i = 0;
	uint32_t *Source = (void *) &PlicVectTableInit;
	volatile uint32_t *Dest = (void *) &IntVectTable;

	for(i=0; i < 128; i++) {
		*Dest++ = *Source++;
	}
}

void init_priority(){
//	// Eight interrupt sources are grouped.
//	// The priority decreases as the interrupt number increases
//	uint32_t current_address;
//	uint32_t current_priority;
//	// write every plic_prio register
//	for (int i = 1; i < INTERRUPT_NUM; i++){
//		current_address = PLIC_PRIO_BASE + (i * 4);
//		current_priority = HIGHEST_PRIORITY - ((i - 1) / 8);
//		// write priority register
//		*((uint32_t*)current_address) = current_priority;
//	}
//
//	// some user-defined trap
//	uint32_t HIGHEST_PRIORITY_USER_DEFINED = current_priority - 1;
//	for (int i = 1; i <= INTERRUPT_USER_DEFINED_NUM; i++){
//			current_address = PLIC_PRIO_USER_DEFINED_BASE + ((i - 1) * 4);
//			current_priority = HIGHEST_PRIORITY_USER_DEFINED - ((i - 1) / 8);
//			// write priority register
//			*((uint32_t*)current_address) = current_priority;
//	}
//	// write PLIC_H0_MTH (priority threshold)
//	*((uint32_t*) PLIC_H0_MTH) = 2;
	for (int i = 1; i <= INTERRUPT_NUM_EXTERNAL; i++){
		uint32_t address = PLIC_PRIO_BASE + (i * 4);
		*((uint32_t*)address) = (uint32_t)PlicPriorityInit[i];
	}

	for (int i = 1; i <= INTERRUPT_NUM_USER_DEF; i++) {
		uint32_t address = PLIC_PRIO_USER_DEFINED_BASE + ((i - 1) * 4);
		// write priority register
		*((uint32_t*) address) = (uint32_t)PlicPriorityInit[i + INTERRUPT_NUM_EXTERNAL];
	}
}

void init_plic(void){
	init_plic_vect_table();
	init_priority();
}
