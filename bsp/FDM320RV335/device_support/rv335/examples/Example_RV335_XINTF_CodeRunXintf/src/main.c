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

//###########################################################################
//
// FILE:	main.c
//
// TITLE:	Code Run from XINTF Example
//
//! <h1>Code Run from XINTF (xintf_run_from)</h1>
//!
//! This example configures CPU Timer0 and increments
//! a counter each time the timer asserts an interrupt.
//!
//! The code is loaded into SARAM.  The XINTF Zone 7 is
//! configured for x16-bit data bus.  A portion of the code
//! is copied to XINTF for execution there.
//!
//! \b Watch \b Variables \n
//! - CpuTimer0.InterruptCount
//! - CpuTimer1.InterruptCount
//


//
// Included Files
//
#include <string.h>
#include "../include/RV335_Project.h"
#include "../routines/include/RV335_Device.h"

#include "../include/RV335_SysCtrl.h"

//
// Function Prototypes
//
void init_zone7(void);
void cpu_timer0_isr(void) __attribute__ ((section("xintffuncs")));
void cpu_timer1_isr(void) __attribute__ ((section("xintffuncs")));

//
// Main
//
void main(void)
{
    //
    // Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    //
    InitSysCtrl();

    // set interrupt priority
    init_plic();//init_priority();
    // config isr
    IntVectTable.st.TINT0 = cpu_timer0_isr;
    IntVectTable.st.TIMER1 = cpu_timer1_isr;

    // Initialize the Device Peripheral. This function can be
    //
    InitCpuTimers();   // For this example, only initialize the Cpu Timers

#define CPU_FRQ_150MHZ 1

#if (CPU_FRQ_150MHZ)
    //
    // Configure CPU-Timer 0, 1, and 2 to interrupt every second:
    // 150MHz CPU Freq, 1 second Period (in uSeconds)
    //
    ConfigCpuTimer(&CpuTimer0, 150, 1000000);
    ConfigCpuTimer(&CpuTimer1, 150, 1000000);
#endif

#if (CPU_FRQ_100MHZ)
    //
    // Configure CPU-Timer 0, 1, and 2 to interrupt every second:
    // 100MHz CPU Freq, 1 second Period (in uSeconds)
    //
    ConfigCpuTimer(&CpuTimer0, 100, 1000000);
    ConfigCpuTimer(&CpuTimer1, 100, 1000000);
#endif

    //
    // To ensure precise timing, use write-only instructions to write to the
    // entire register. Therefore, if any of the configuration bits are changed
    // in ConfigCpuTimer and InitCpuTimers, the
    // below settings must also be updated.
    //

    //
    // Use write-only instruction to set TSS bit = 0
    //
    CpuTimer0Regs.TCR.all = 0x4000;

    //
    // Use write-only instruction to set TSS bit = 0
    //
    CpuTimer1Regs.TCR.all = 0x4000;

    //
    // Initialize XINTF Zone 7
    //
    init_zone7();

    //
    // Copy non-time critical code to XINTF
    // This includes the following ISR functions: cpu_timer0_isr(),
    // cpu_timer1_isr(). The  _xintffuncs_load_start, _xintffuncs_run_start,
    // and _xintffuncs_run_end symbols are created by the linker.
    extern char _xintffuncs_load_start;
    void* p_xintffuncs_load_start = &_xintffuncs_load_start;
    extern char _xintffuncs_run_start;
    void* p_xintffuncs_run_start = &_xintffuncs_run_start;
    extern char _xintffuncs_run_end;
    void* p_xintffuncs_run_end = &_xintffuncs_run_end;

    size_t xintffuncs_size = p_xintffuncs_run_end - p_xintffuncs_run_start;
    memcpy((void *)p_xintffuncs_run_start, (void *)p_xintffuncs_load_start, xintffuncs_size);

    // enable interrupt
	enable_interrupt( GET_INT_NUM(IntVectTable.st.TINT0));
	enable_interrupt( GET_INT_NUM(IntVectTable.st.TIMER1));
	// enable trap
	trap_init();
    // IDLE loop. Just sit and loop forever (optional)
    //
    for(;;);
}

//
// cpu_timer0_isr -
//
 void cpu_timer0_isr(void) {
    CpuTimer0.InterruptCount++;
}

//
// cpu_timer1_isr -
//
 void cpu_timer1_isr(void) {
    CpuTimer1.InterruptCount++;
}

//
// init_zone7 - Configure the timing parameters for Zone 7.
// Notes: This function should not be executed from XINTF. Adjust the timing
//        based on the data manual and external device requirements.
//
void
init_zone7(void)
{
    //
    // Make sure the XINTF clock is enabled
    //

    SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1;


    //
    // Configure the GPIO for XINTF with a 16-bit data bus
    //
    InitXintf16Gpio();



    //
    // All Zones
    // Timing for all zones based on XTIMCLK = SYSCLKOUT
    //
    XintfRegs.XINTCNF2.bit.XTIMCLK = 0;

    //
    // Buffer up to 3 writes
    //
    XintfRegs.XINTCNF2.bit.WRBUFF = 3;

    //
    // XCLKOUT is enabled
    //
    XintfRegs.XINTCNF2.bit.CLKOFF = 0;

    //
    // XCLKOUT = XTIMCLK
    //
    XintfRegs.XINTCNF2.bit.CLKMODE = 0;

    //
    // Disable XHOLD to prevent XINTF bus from going into high impedance state
    // whenever TZ3 signal goes low. This occurs because TZ3 on GPIO14 is
    // shared with HOLD of XINTF
    //
    XintfRegs.XINTCNF2.bit.HOLD = 1;

    //
    // Zone 7
    // When using ready, ACTIVE must be 1 or greater. Lead must always be 1 or
    // greater. Zone write timing
    //
    XintfRegs.XTIMING7.bit.XWRLEAD = 3;
    XintfRegs.XTIMING7.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING7.bit.XWRTRAIL = 3;

    //
    // Zone read timing
    //
    XintfRegs.XTIMING7.bit.XRDLEAD = 3;
    XintfRegs.XTIMING7.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING7.bit.XRDTRAIL = 3;

    //
    // don't double all Zone read/write lead/active/trail timing
    //
    XintfRegs.XTIMING7.bit.X2TIMING = 0;

    //
    // Zone will not sample XREADY signal
    //
    XintfRegs.XTIMING7.bit.USEREADY = 0;
    XintfRegs.XTIMING7.bit.READYMODE = 0;

    //
    // 1,1 = x16 data bus
    // 0,1 = x32 data bus
    // other values are reserved
    //
    XintfRegs.XTIMING7.bit.XSIZE = 3;


    //
    // Force a pipeline flush to ensure that the write to
    // the last register configured occurs before returning.
    //
    __asm__(
        	" NOP\n"
        	" NOP\n"
        	" NOP\n"
        	" NOP\n"
        	" NOP\n"
        	" NOP\n"
        	" NOP\n"
        	" NOP\n"

        );
}

//
// End of File
//

