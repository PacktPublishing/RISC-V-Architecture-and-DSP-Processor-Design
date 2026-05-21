//###########################################################################
//
// FILE:   Example_RV335_ECAP_capture_pwm.c
//
// TITLE:   
//
//###########################################################################
// $FDMTEK Release: $
// $Release Date:  $
// $Copyright:
//  (C) Copyright 2023 QingDao Fundamental Microelectronic .Ltd - http://www.fdmtek.com
//###########################################################################

//###########################################################################
//
// FILE:   Example_RV335_ECAP_capture_pwm.c
//
// TITLE:  eCap capture PWM Example
//
//! \addtogroup RV335_example_list
//! <h1> eCap capture PWM (ecap_capture_pwm)</h1>
//!
//! This example configures ePWM3A for:
//! - Up count
//! - Period starts at 2 and goes up to 1000
//! - Toggle output on PRD
//!
//! eCAP1 is configured to capture the time between rising
//! and falling edge of the ePWM3A output.
//!
//! \b External \b Connections \n
//! - eCap1 is on GPIO24
//! - ePWM3A is on GPIO4
//! - Connect GPIO4 to GPIO24.
//!
//! \b Watch \b Variables \n
//! - ECap1IntCount - Successful captures
//! - ECap1PassCount - Interrupt counts
//
//###########################################################################

//
// Included Files
//
 
#include "../include/RV335_Project.h"     // Device Headerfile and Examples Include File
#include "../include/RV335_SysCtrl.h"
#include "../routines/include/RV335_Device.h"
#include "../routines/include/RV335_EPwm_defines.h"       // Macros used for PWM examples.

//
// Defines to configure the start/end period for the timer
//
#define PWM3_TIMER_MIN     2  //10  wuyn
#define PWM3_TIMER_MAX     1000//8000 wuyn

//
// Function Prototypes
//
void ecap1_isr(void);
void InitECapture(void);
void InitEPwmTimer(void);
void Fail(void);

//
// Globals
//
Uint32  ECap1IntCount;
Uint32  ECap1PassCount;
Uint32  EPwm3TimerDirection;

//
// Defines to keep track of which way the timer value is moving
//
#define EPWM_TIMER_UP   1
#define EPWM_TIMER_DOWN 0

//
// Main
//
void main(void)
{
    //
    // Step 1. Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the RV335_SysCtrl.c file.
    //
    InitSysCtrl();

    //
    // Step 2. Initialize GPIO:
    // This example function is found in the RV335_Gpio.c file and
    // illustrates how to set the GPIO to it's default state.
    //
    // InitGpio();  // Skipped for this example
    InitEPwm3Gpio();
    InitECap1Gpio();

    //
    // Step 3. Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    //
    /*DINT;*/

    //
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    // This function is found in the RV335_PieCtrl.c file.
    //
    /*InitPieCtrl();*/

    //
    // Disable CPU interrupts and clear all CPU interrupt flags
    //
    /*IER = 0x0000;*/
    /*IFR = 0x0000;*/

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in RV335_DefaultIsr.c.
    // This function is found in RV335_PieVect.c.
    //
//    InitPieVectTable();

    //
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    //
    init_plic();
 
        // enable trap
        trap_init();
        // config isr
        IntVectTable.st.ECAP1_INT = ecap1_isr;


    //
    // Step 4. Initialize all the Device Peripherals:
    // This function is found in RV335_InitPeripherals.c
    //
    // InitPeripherals();  // Not required for this example
    InitEPwmTimer();       // For this example, only initialize the ePWM Timers
    InitECapture();

    //
    // Step 5. User specific code, enable interrupts:
    //

    //
    // Initialize counters
    //
    ECap1IntCount = 0;
    ECap1PassCount = 0;

    //
    // Enable CPU INT4 which is connected to ECAP1-4 INT
    //
    /*IER |= M_INT4;*/

    //
    // Enable eCAP INTn in the PIE: Group 3 interrupt 1-6
    //
    /*PieCtrlRegs.PIEIER4.bit.INTx1 = 1;*/
    // enable interrupt
    enable_interrupt( GET_INT_NUM(IntVectTable.st.ECAP1_INT));

    //
    // Enable global Interrupts and higher priority real-time debug events
    //
    /*EINT;*/           // Enable Global interrupt INTM
    //ERTM;           // Enable Global realtime interrupt DBGM

    //
    // Step 6. IDLE loop. Just sit and loop forever (optional)
    //
    for(;;)
    {
        __asm("          NOP");
    }
}

//
// InitEPwmTimer -
//
void 
InitEPwmTimer()
{
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;

    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;   // Count up
    EPwm3Regs.TBPRD = PWM3_TIMER_MIN;
    EPwm3Regs.TBPHS.all = 0x00000000;
    EPwm3Regs.AQCTLA.bit.PRD = AQ_TOGGLE;        // Toggle on PRD

    //
    // TBCLK = SYSCLKOUT
    //
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = 1;
    EPwm3Regs.TBCTL.bit.CLKDIV = 0;

    EPwm3TimerDirection = EPWM_TIMER_UP;

    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
}

//
// InitECapture - 
//
void 
InitECapture()
{
    ECap1Regs.ECEINT.all = 0x0000;          // Disable all capture interrupts
    ECap1Regs.ECCLR.all = 0xFFFF;           // Clear all CAP interrupt flags
    ECap1Regs.ECCTL1.bit.CAPLDEN = 0;       // Disable CAP1-CAP4 register loads
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;     // Make sure the counter is stopped

    //
    // Configure peripheral registers
    //
    ECap1Regs.ECCTL2.bit.CONT_ONESHT = 1;   // One-shot
    ECap1Regs.ECCTL2.bit.STOP_WRAP = 3;     // Stop at 4 events
    ECap1Regs.ECCTL1.bit.CAP1POL = 1;       // Falling edge
    ECap1Regs.ECCTL1.bit.CAP2POL = 0;       // Rising edge
    ECap1Regs.ECCTL1.bit.CAP3POL = 1;       // Falling edge
    ECap1Regs.ECCTL1.bit.CAP4POL = 0;       // Rising edge
    ECap1Regs.ECCTL1.bit.CTRRST1 = 1;       // Difference operation
    ECap1Regs.ECCTL1.bit.CTRRST2 = 1;       // Difference operation
    ECap1Regs.ECCTL1.bit.CTRRST3 = 1;       // Difference operation
    ECap1Regs.ECCTL1.bit.CTRRST4 = 1;       // Difference operation
    ECap1Regs.ECCTL2.bit.SYNCI_EN = 1;      // Enable sync in
    ECap1Regs.ECCTL2.bit.SYNCO_SEL = 0;     // Pass through
    ECap1Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable capture units

    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;     // Start Counter
    ECap1Regs.ECCTL2.bit.REARM = 1;         // arm one-shot
    ECap1Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable CAP1-CAP4 register loads
    ECap1Regs.ECEINT.bit.CEVT4 = 1;         // 4 events = interrupt
}

//
// ecap1_isr -
//
 void ecap1_isr(void) {
    //
    // Cap input is syc'ed to SYSCLKOUT so there may be
    // a +/- 1 cycle variation
    //
    if(ECap1Regs.CAP2 > EPwm3Regs.TBPRD*2+1 || 
       ECap1Regs.CAP2 < EPwm3Regs.TBPRD*2-1)
    {
        Fail();
    }

    if(ECap1Regs.CAP3 > EPwm3Regs.TBPRD*2+1 || 
       ECap1Regs.CAP3 < EPwm3Regs.TBPRD*2-1)
    {
        Fail();
    }

    if(ECap1Regs.CAP4 > EPwm3Regs.TBPRD*2+1 || 
       ECap1Regs.CAP4 < EPwm3Regs.TBPRD*2-1)
    {
        Fail();
    }

    ECap1IntCount++;

    if(EPwm3TimerDirection == EPWM_TIMER_UP)
    {
        if(EPwm3Regs.TBPRD < PWM3_TIMER_MAX)
        {
            EPwm3Regs.TBPRD++;
        }
        else
        {
            EPwm3TimerDirection = EPWM_TIMER_DOWN;
            EPwm3Regs.TBPRD--;
        }
    }
    else
    {
        if(EPwm3Regs.TBPRD > PWM3_TIMER_MIN)
        {
            EPwm3Regs.TBPRD--;
        }
        else
        {
            EPwm3TimerDirection = EPWM_TIMER_UP;
            EPwm3Regs.TBPRD++;
        }
    }

    ECap1PassCount++;

    ECap1Regs.ECCLR.bit.CEVT4 = 1;
    ECap1Regs.ECCLR.bit.INT = 1;
    ECap1Regs.ECCTL2.bit.REARM = 1;

    //
    // Acknowledge this interrupt to receive more interrupts from group 4
    //
    /*PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;*/


}

//
// Fail -
//
void 
Fail()
{
    asm(" EBREAK");			
}

//
// End of File
//

