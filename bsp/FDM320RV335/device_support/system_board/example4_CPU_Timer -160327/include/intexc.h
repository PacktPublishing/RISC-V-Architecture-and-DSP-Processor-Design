
#ifndef INCLUDE_INTEXC_H_
#define INCLUDE_INTEXC_H_

#include <stdint.h>

// define exception and interrupt handle function
typedef void(*TRAP_HANDLER)(void);

extern void trap_init();

//---------------------------------------------
//    exception related definition
//---------------------------------------------
// exception number
#define EXCEPTION_NUM 16

// excepition handler
void exception_handler();

// member of the structure should equals EXCEPTION_NUM
struct PE_HANDLER_TABLE {
/* 0 */	TRAP_HANDLER Instruction_address_misaligned_handler;
/* 1 */	TRAP_HANDLER Instruction_access_fault_handler;
/* 2 */	TRAP_HANDLER Illgal_instruction_handler;
/* 3 */	TRAP_HANDLER Breakpoint_handler;
/* 4 */	TRAP_HANDLER Load_address_misaligned_handler;
/* 5 */	TRAP_HANDLER Load_access_fault_handler;
/* 6 */	TRAP_HANDLER Store_address_misaligned_handler;
/* 7 */	TRAP_HANDLER Store_access_fault_handler;
/* 8 */	TRAP_HANDLER Never_occur_handler8;
/* 9 */	TRAP_HANDLER Never_occur_handler9;
/*10 */	TRAP_HANDLER Never_occur_handler10;
/*11 */	TRAP_HANDLER Never_occur_handler11;
/*12 */	TRAP_HANDLER Instruction_page_fault_handler;
/*13 */	TRAP_HANDLER Load_page_fault_handler;
/*14 */	TRAP_HANDLER Never_occur_handler14;
/*15 */	TRAP_HANDLER Store_page_fault_handler;
} __attribute__((aligned(4))) ;

typedef union {
	struct PE_HANDLER_TABLE st;
	TRAP_HANDLER arr[EXCEPTION_NUM];
} __attribute__((aligned(4))) PE_VEC_TABLE;


//---------------------------------------------
//    interrupt related definition
//---------------------------------------------

// interrupt number
#define INTERRUPT_NUM_EXTERNAL 98
#define INTERRUPT_NUM_USER_DEF 12
#define INTERRUPT_NUM 110
// plic registers related
#define PLIC_MIE_BASE 0x0080019C
//#define PLIC_MCLAIM  0x008001B0
#define PLIC_PRIO_BASE 0x00800000
#define PLIC_PRIO_USER_DEFINED_BASE 0x008001CC
#define HIGHEST_PRIORITY 0x1f
#define PLIC_H0_MTH 0x008001AC

// functions
void init_priority();
void init_plic(void);
void enable_interrupt(uint32_t id);
void disable_interrupt(uint32_t id);
//void interrupt_handler();

// member of the structure should equals INTERRUPT_NUM + 1
struct PI_HANDLER_TABLE {
	TRAP_HANDLER	 RESERVED;
    // Group 1 PIE Peripheral Vectors
    //
    TRAP_HANDLER     SEQ1INT;
    TRAP_HANDLER     SEQ2INT;
    TRAP_HANDLER     rsvd1_3;
    TRAP_HANDLER     XINT1;
    TRAP_HANDLER     XINT2;
    TRAP_HANDLER     ADCINT;    // ADC
    TRAP_HANDLER     TINT0;     // Timer 0
    TRAP_HANDLER     WAKEINT;   // WD

    //
    // Group 2 PIE Peripheral Vectors
    //
    TRAP_HANDLER     EPWM1_TZINT; // EPWM-1
    TRAP_HANDLER     EPWM2_TZINT; // EPWM-2
    TRAP_HANDLER     EPWM3_TZINT; // EPWM-3
    TRAP_HANDLER     EPWM4_TZINT; // EPWM-4
    TRAP_HANDLER     EPWM5_TZINT; // EPWM-5
    TRAP_HANDLER     EPWM6_TZINT; // EPWM-6
    TRAP_HANDLER     rsvd2_7;
    TRAP_HANDLER     rsvd2_8;

    //
    // Group 3 PIE Peripheral Vectors
    //
    TRAP_HANDLER     EPWM1_INT;  // EPWM-1
    TRAP_HANDLER     EPWM2_INT;  // EPWM-2
    TRAP_HANDLER     EPWM3_INT;  // EPWM-3
    TRAP_HANDLER     EPWM4_INT;  // EPWM-4
    TRAP_HANDLER     EPWM5_INT;  // EPWM-5
    TRAP_HANDLER     EPWM6_INT;  // EPWM-6
    TRAP_HANDLER     rsvd3_7;
    TRAP_HANDLER     rsvd3_8;

    //
    // Group 4 PIE Peripheral Vectors
    //
    TRAP_HANDLER     ECAP1_INT; // ECAP-1
    TRAP_HANDLER     ECAP2_INT; // ECAP-2
    TRAP_HANDLER     ECAP3_INT; // ECAP-3
    TRAP_HANDLER     ECAP4_INT; // ECAP-4
    TRAP_HANDLER     ECAP5_INT; // ECAP-5
    TRAP_HANDLER     ECAP6_INT; // ECAP-6
    TRAP_HANDLER     rsvd4_7;
    TRAP_HANDLER     rsvd4_8;

    //
    // Group 5 PIE Peripheral Vectors
    //
    TRAP_HANDLER     EQEP1_INT; // EQEP-1
    TRAP_HANDLER     EQEP2_INT; // EQEP-2
    TRAP_HANDLER     rsvd5_3;
    TRAP_HANDLER     rsvd5_4;
    TRAP_HANDLER     rsvd5_5;
    TRAP_HANDLER     rsvd5_6;
    TRAP_HANDLER     rsvd5_7;
    TRAP_HANDLER     rsvd5_8;

    //
    // Group 6 PIE Peripheral Vectors
    //
    TRAP_HANDLER     SPIRXINTA; // SPI-A
    TRAP_HANDLER     SPITXINTA; // SPI-A
    TRAP_HANDLER     MRINTB;    // McBSP-B
    TRAP_HANDLER     MXINTB;    // McBSP-B
    TRAP_HANDLER     MRINTA;    // McBSP-A
    TRAP_HANDLER     MXINTA;    // McBSP-A
    TRAP_HANDLER     rsvd6_7;
    TRAP_HANDLER     rsvd6_8;

    //
    // Group 7 PIE Peripheral Vectors
    //
    TRAP_HANDLER     DINTCH1;   // DMA
    TRAP_HANDLER     DINTCH2;   // DMA
    TRAP_HANDLER     DINTCH3;   // DMA
    TRAP_HANDLER     DINTCH4;   // DMA
    TRAP_HANDLER     DINTCH5;   // DMA
    TRAP_HANDLER     DINTCH6;   // DMA
    TRAP_HANDLER     rsvd7_7;
    TRAP_HANDLER     rsvd7_8;

    //
    // Group 8 PIE Peripheral Vectors
    //
    TRAP_HANDLER     I2CINT1A;  // I2C-A
    TRAP_HANDLER     I2CINT2A;  // I2C-A
    TRAP_HANDLER     rsvd8_3;
    TRAP_HANDLER     rsvd8_4;
    TRAP_HANDLER     SCIRXINTC;  // SCI-C
    TRAP_HANDLER     SCITXINTC;  // SCI-C
    TRAP_HANDLER     rsvd8_7;
    TRAP_HANDLER     rsvd8_8;

    //
    // Group 9 PIE Peripheral Vectors
    //
    TRAP_HANDLER     SCIRXINTA;  // SCI-A
    TRAP_HANDLER     SCITXINTA;  // SCI-A
    TRAP_HANDLER     SCIRXINTB;  // SCI-B
    TRAP_HANDLER     SCITXINTB;  // SCI-B
    TRAP_HANDLER     ECAN0INTA;  // eCAN-A
    TRAP_HANDLER     ECAN1INTA;  // eCAN-A
    TRAP_HANDLER     ECAN0INTB;  // eCAN-B
    TRAP_HANDLER     ECAN1INTB;  // eCAN-B

    //
    // Group 10 PIE Peripheral Vectors
    //
    TRAP_HANDLER     rsvd10_1;
    TRAP_HANDLER     rsvd10_2;
    TRAP_HANDLER     rsvd10_3;
    TRAP_HANDLER     rsvd10_4;
    TRAP_HANDLER     rsvd10_5;
    TRAP_HANDLER     rsvd10_6;
    TRAP_HANDLER     rsvd10_7;
    TRAP_HANDLER     rsvd10_8;

    //
    // Group 11 PIE Peripheral Vectors
    //
    TRAP_HANDLER     rsvd11_1;
    TRAP_HANDLER     rsvd11_2;
    TRAP_HANDLER     rsvd11_3;
    TRAP_HANDLER     rsvd11_4;
    TRAP_HANDLER     rsvd11_5;
    TRAP_HANDLER     rsvd11_6;
    TRAP_HANDLER     rsvd11_7;
    TRAP_HANDLER     rsvd11_8;

    //
    // Group 12 PIE Peripheral Vectors
    //
    TRAP_HANDLER     XINT3;      // External interrupt
    TRAP_HANDLER     XINT4;
    TRAP_HANDLER     XINT5;
    TRAP_HANDLER     XINT6;
    TRAP_HANDLER     XINT7;
    TRAP_HANDLER     rsvd12_6;
    TRAP_HANDLER     LVF;        // Latched overflow
    TRAP_HANDLER     LUF;        // Latched underflow

    // timer1
    TRAP_HANDLER     TIMER1;
    // timer2
    TRAP_HANDLER     TIMER2;
    // lpm
//    TRAP_HANDLER     LPM;
    // nmi nmi does not has id
    // TRAP_HANDLER     NMI;

    // user defined traps
    TRAP_HANDLER     USER1;
    TRAP_HANDLER     USER2;
    TRAP_HANDLER     USER3;
    TRAP_HANDLER     USER4;
    TRAP_HANDLER     USER5;
    TRAP_HANDLER     USER6;
    TRAP_HANDLER     USER7;
    TRAP_HANDLER     USER8;
    TRAP_HANDLER     USER9;
    TRAP_HANDLER     USER10;
    TRAP_HANDLER     USER11;
    TRAP_HANDLER     USER12;
} __attribute__((aligned(4))) ;

// nmi handler pointer
extern volatile TRAP_HANDLER  p_nmi_handler;

typedef union {
	struct PI_HANDLER_TABLE st;
	TRAP_HANDLER arr[INTERRUPT_NUM + 1];
} __attribute__((aligned(4))) PI_VEC_TABLE;


extern volatile PI_VEC_TABLE  IntVectTable;

#define GET_INT_NUM(x) (&x  - IntVectTable.arr)

// to generate user defined trap
void gen_user_trap(int ti_id);

#endif
