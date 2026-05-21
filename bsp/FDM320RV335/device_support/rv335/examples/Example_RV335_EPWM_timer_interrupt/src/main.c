//###########################################################################
//
// FILE:   main.c
//
// TITLE:   
//
//###########################################################################
// $FDMTEK Release: $
// $Release Date:  $
// $Copyright:
//  (C) Copyright 2023 QingDao Fundamental Microelectronic .Ltd - http://www.fdmtek.com
//###########################################################################

// Device Headerfile and Examples Include File
#include "../include/RV335_Project.h"
#include "../include/RV335_SysCtrl.h"
#include "../routines/include/RV335_Device.h"

//
// Function Prototypes
//
void epwm1_timer_isr(void);
void epwm2_timer_isr(void);
void epwm3_timer_isr(void);
void epwm4_timer_isr(void);
void epwm5_timer_isr(void);
void epwm6_timer_isr(void);
void InitEPwmTimer(void);

//
// Defines that configure which ePWM timer interrupts are enabled at the
// PIE level: 1 = enabled,  0 = disabled
//
#define PWM1_INT_ENABLE  1
#define PWM2_INT_ENABLE  1
#define PWM3_INT_ENABLE  1
#define PWM4_INT_ENABLE  1
#define PWM5_INT_ENABLE  1
#define PWM6_INT_ENABLE  1

//
// Defines that configure the period for each timer
//
#define PWM1_TIMER_TBPRD   0x1FFF
#define PWM2_TIMER_TBPRD   0x1FFF
#define PWM3_TIMER_TBPRD   0x1FFF
#define PWM4_TIMER_TBPRD   0x1FFF
#define PWM5_TIMER_TBPRD   0x1FFF
#define PWM6_TIMER_TBPRD   0x1FFF


//
// Globals
//
Uint32  EPwm1TimerIntCount;
Uint32  EPwm2TimerIntCount;
Uint32  EPwm3TimerIntCount;
Uint32  EPwm4TimerIntCount;
Uint32  EPwm5TimerIntCount;
Uint32  EPwm6TimerIntCount;


void main(void)
{
	int i;
    // Initialize trap
    trap_init();
    init_plic();
    //init_priority();

    // Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks.
    InitSysCtrl();

    //
    // Init IntVectTable inpterrupt sources
    //
    IntVectTable.st.EPWM1_INT = &epwm1_timer_isr;
    IntVectTable.st.EPWM2_INT = &epwm2_timer_isr;
    IntVectTable.st.EPWM3_INT = &epwm3_timer_isr;
    IntVectTable.st.EPWM4_INT = &epwm4_timer_isr;
    IntVectTable.st.EPWM5_INT = &epwm5_timer_isr;
    IntVectTable.st.EPWM6_INT = &epwm6_timer_isr;

    InitEPwmTimer();

    // Enable interrupt sources
    enable_interrupt( GET_INT_NUM(IntVectTable.st.EPWM1_INT));
    enable_interrupt( GET_INT_NUM(IntVectTable.st.EPWM2_INT));
    enable_interrupt( GET_INT_NUM(IntVectTable.st.EPWM3_INT));
    enable_interrupt( GET_INT_NUM(IntVectTable.st.EPWM4_INT));
    enable_interrupt( GET_INT_NUM(IntVectTable.st.EPWM5_INT));
    enable_interrupt( GET_INT_NUM(IntVectTable.st.EPWM6_INT));

    for(;;)
    {
    	__asm("          NOP");
    	for(i=1;i<=10;i++)
    	{

    	}
    }

}

//
// InitEPwmTimer -
//
void
InitEPwmTimer()
{
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;      // Stop all the TB clocks

    //
    // Setup Sync
    //
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through
    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through
    EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through
    EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through
    EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through
    EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through

    //
    // Allow each timer to be sync'ed
    //
    EPwm1Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm4Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm5Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm6Regs.TBCTL.bit.PHSEN = TB_ENABLE;

    EPwm1Regs.TBPHS.half.TBPHS = 100;
    EPwm2Regs.TBPHS.half.TBPHS = 200;
    EPwm3Regs.TBPHS.half.TBPHS = 300;
    EPwm4Regs.TBPHS.half.TBPHS = 400;
    EPwm5Regs.TBPHS.half.TBPHS = 500;
    EPwm6Regs.TBPHS.half.TBPHS = 600;

    EPwm1Regs.TBPRD = PWM1_TIMER_TBPRD;
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;    // Count up
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm1Regs.ETSEL.bit.INTEN = PWM1_INT_ENABLE;  // Enable INT
    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event

    EPwm2Regs.TBPRD = PWM2_TIMER_TBPRD;
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;     // Count up
    EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;      // Enable INT on Zero event
    EPwm2Regs.ETSEL.bit.INTEN = PWM2_INT_ENABLE;   // Enable INT
    EPwm2Regs.ETPS.bit.INTPRD = ET_2ND;            // Generate INT on 2nd event

    EPwm3Regs.TBPRD = PWM3_TIMER_TBPRD;
    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;     // Count up
    EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;      // Enable INT on Zero event
    EPwm3Regs.ETSEL.bit.INTEN = PWM3_INT_ENABLE;   // Enable INT
    EPwm3Regs.ETPS.bit.INTPRD = ET_3RD;            // Generate INT on 3rd event

    EPwm4Regs.TBPRD = PWM4_TIMER_TBPRD;
    EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;     // Count up
    EPwm4Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;      // Enable INT on Zero event
    EPwm4Regs.ETSEL.bit.INTEN = PWM4_INT_ENABLE;   // Enable INT
    EPwm4Regs.ETPS.bit.INTPRD = ET_1ST;            // Generate INT on 1st event

    EPwm5Regs.TBPRD = PWM5_TIMER_TBPRD;
    EPwm5Regs.TBCTL.bit.CTRMODE= TB_COUNT_UP;      // Count up
    EPwm5Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;      // Enable INT on Zero event
    EPwm5Regs.ETSEL.bit.INTEN = PWM5_INT_ENABLE;   // Enable INT
    EPwm5Regs.ETPS.bit.INTPRD = ET_1ST;            // Generate INT on 1st event


    EPwm6Regs.TBPRD = PWM6_TIMER_TBPRD;
    EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;     // Count up
    EPwm6Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;      // Enable INT on Zero event
    EPwm6Regs.ETSEL.bit.INTEN = PWM6_INT_ENABLE;   // Enable INT
    EPwm6Regs.ETPS.bit.INTPRD = ET_1ST;            // Generate INT on 1st event

    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;       // Start all the timers synced
}

//
// epwm1_timer_isr - Interrupt routines uses in this example
//
void 
epwm1_timer_isr(void)
{
    EPwm1TimerIntCount++;

    //
    // Clear INT flag for this timer
    //
    EPwm1Regs.ETCLR.bit.INT = 1;

}

//
// epwm2_timer_isr -
//
void 
epwm2_timer_isr(void)
{
    EPwm2TimerIntCount++;

    //
    // Clear INT flag for this timer
    //
    EPwm2Regs.ETCLR.bit.INT = 1;

}

//
// epwm3_timer_isr -
//
void 
epwm3_timer_isr(void)
{
    EPwm3TimerIntCount++;

    //
    // Clear INT flag for this timer
    //
    EPwm3Regs.ETCLR.bit.INT = 1;

}

//
// epwm4_timer_isr -
//
void 
epwm4_timer_isr(void)
{
    EPwm4TimerIntCount++;

    //
    // Clear INT flag for this timer
    //
    EPwm4Regs.ETCLR.bit.INT = 1;

}

//
// epwm5_timer_isr -
//
void 
epwm5_timer_isr(void)
{
    EPwm5TimerIntCount++;

    //
    // Clear INT flag for this timer
    //
    EPwm5Regs.ETCLR.bit.INT = 1;

}

//
// epwm6_timer_isr -
//
void 
epwm6_timer_isr(void)
{
    EPwm6TimerIntCount++;

    //
    // Clear INT flag for this timer
    //
    EPwm6Regs.ETCLR.bit.INT = 1;

}



