//###########################################################################
//
// FILE:    Example_2833xCpuTimer.c
//
// TITLE:   DSP2833x Device Getting Started Program.
//
// ASSUMPTIONS:
//
//    This program requires the DSP2833x header files.
//
//    Other then boot mode configuration, no other hardware configuration
//    is required.
//
//
//    根据在RAM中调试的需要，这个项目配置成"boot to SARAM".2833x引导模式
//    表如下显示. 常用的还有"boot to Flash"模式，当程序在RAM调试完善后就
//    可以将代码烧进Flash中并使用"boot to Flash"引导模式. 
//
//       $Boot_Table:
//
//         GPIO87   GPIO86     GPIO85   GPIO84
//          XA15     XA14       XA13     XA12
//           PU       PU         PU       PU
//        ==========================================
//            1        1          1        1    Jump to Flash
//            1        1          1        0    SCI-A boot
//            1        1          0        1    SPI-A boot
//            1        1          0        0    I2C-A boot
//            1        0          1        1    eCAN-A boot
//            1        0          1        0    McBSP-A boot
//            1        0          0        1    Jump to XINTF x16
//            1        0          0        0    Jump to XINTF x32
//            0        1          1        1    Jump to OTP
//            0        1          1        0    Parallel GPIO I/O boot
//            0        1          0        1    Parallel XINTF boot
//            0        1          0        0    Jump to SARAM	    <- "boot to SARAM"
//            0        0          1        1    Branch to check boot mode
//            0        0          1        0    Boot to flash, bypass ADC cal
//            0        0          0        1    Boot to SARAM, bypass ADC cal
//            0        0          0        0    Boot to SCI-A, bypass ADC cal
//                                              Boot_Table_End$
//
// DESCRIPTION:
//
//    This example configures CPU Timer0, 1, and 2 and increments
//    a counter each time the timers assert an interrupt.
//
//       Watch Variables:
//          CpuTimer0.InterruptCount
//          CpuTimer1.InterruptCount
//          CpuTimer2.InterruptCount
//
//###########################################################################
// $TI Release: DSP2833x Header Files V1.01 $
// $Release Date: September 26, 2007 $
//###########################################################################


#include "RV335_Device.h"     // DSP2833x Headerfile Include File
#include "RV335_Examples.h"   // DSP2833x Examples Include File

/***************************************************************************************************
**Description  ** 全局变量定义
***************************************************************************************************/
volatile unsigned int timer_int_cnt;

// Prototype statements for functions found within this file.
 void cpu_timer0_isr(void);
 void cpu_timer1_isr(void);
 void cpu_timer2_isr(void);
 void Gpio_select(void);


void main(void)
{

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example

// 初始化使用到的GPIO口；
   Gpio_select();	  


// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   /*DINT;*/

// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the DSP2833x_PieCtrl.c file.
   /*InitPieCtrl();*/

// Disable CPU interrupts and clear all CPU interrupt flags:
   /*IER = 0x0000;*/
   /*IFR = 0x0000;*/

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP2833x_DefaultIsr.c.
// This function is found in DSP2833x_PieVect.c.
   /*InitPieVectTable();*/

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   /*EALLOW;*/  // This is needed to write to EALLOW protected registers
   //init plic default settings
    init_plic();
    // config isr
    IntVectTable.st.TINT0 = cpu_timer0_isr;
    // enable trap
//    trap_init();
    // enable interrupt
    enable_interrupt( GET_INT_NUM(IntVectTable.st.TINT0));
   //init plic default settings
//    init_plic();
    // config isr
    IntVectTable.st.TIMER1 = cpu_timer1_isr;
    // enable trap
//    trap_init();
    // enable interrupt
    enable_interrupt( GET_INT_NUM(IntVectTable.st.TIMER1));
   //init plic default settings
//    init_plic();
    // config isr
    IntVectTable.st.TIMER2 = cpu_timer2_isr;
    // enable trap
    trap_init();
    // enable interrupt
    enable_interrupt( GET_INT_NUM(IntVectTable.st.TIMER2));
   /*EDIS;*/    // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize the Device Peripheral. This function can be
//         found in DSP2833x_CpuTimers.c
   InitCpuTimers();   // For this example, only initialize the Cpu Timers

#if (CPU_FRQ_150MHZ)
// 配置CPU定时器 0, 1, and 2 中断周期:
// 150MHz CPU 频率, 周期单位为us；

   ConfigCpuTimer(&CpuTimer0, 150, 100000);   //Freq 150; Period 100000.
   ConfigCpuTimer(&CpuTimer1, 150, 200000);
   ConfigCpuTimer(&CpuTimer2, 150, 400000);
#endif



// To ensure precise timing, use write-only instructions to write to the entire register. Therefore, if any
// of the configuration bits are changed in ConfigCpuTimer and InitCpuTimers (in DSP2833x_CpuTimers.h), the
// below settings must also be updated.

   CpuTimer0Regs.TCR.all = 0x4001; // 设置TIE = 1，开启定时器0中断
   CpuTimer1Regs.TCR.all = 0x0001; // 设置TIE = 0，关闭定时器1中断
   CpuTimer2Regs.TCR.all = 0x0001; // 设置TIE = 0，关闭定时器2中断

// Step 5. User specific code, enable interrupts:


// Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
// which is connected to CPU-Timer 1, and CPU int 14, which is connected
// to CPU-Timer 2:
   /*IER |= M_INT1;*/
   /*IER |= M_INT13;*/
   /*IER |= M_INT14;*/

// Enable TINT0 in the PIE: Group 1 interrupt 7
   /*PieCtrlRegs.PIEIER1.bit.INTx7 = 1;*/

// Enable global Interrupts and higher priority real-time debug events:
   /*EINT;*/   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM

// Step 6. IDLE loop. Just sit and loop forever (optional):
timer_int_cnt = 0;
   for(;;);

}


 void cpu_timer0_isr(void) {
   CpuTimer0.InterruptCount++;

   /*EALLOW;*/
    if(timer_int_cnt++ >= 12) 
    {
        timer_int_cnt = 0;
        CpuTimer0Regs.TCR.all = 0x0001;           // 设置TIE = 0，关闭定时器0中断
        CpuTimer1Regs.TCR.all = 0x4001;           // 设置TIE = 1，开启定时器1中断
        CpuTimer2Regs.TCR.all = 0x0001;           // 设置TIE = 0，关闭定时器2中断
    }

	GpioDataRegs.GPBTOGGLE.all =0x30000000; 
    GpioDataRegs.GPCTOGGLE.all =0x00000007;
   // Acknowledge this interrupt to receive more interrupts from group 1
   /*PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;*/

   /*EDIS;*/
}

 void cpu_timer1_isr(void) {
   CpuTimer1.InterruptCount++;

   /*EALLOW;*/

   if( timer_int_cnt++ >= 12) 
    {
        timer_int_cnt = 0;
        CpuTimer0Regs.TCR.all = 0x0001;          // 设置TIE = 0，关闭定时器0中断
        CpuTimer1Regs.TCR.all = 0x0001;          // 设置TIE = 0，关闭定时器1中断
        CpuTimer2Regs.TCR.all = 0x4001;          // 设置TIE = 1，开启定时器2中断
    }
    GpioDataRegs.GPBTOGGLE.all =0x30000000; 
    GpioDataRegs.GPCTOGGLE.all =0x00000007;

   // The CPU acknowledges the interrupt.
   /*EDIS;*/
}

 void cpu_timer2_isr(void) {  
   CpuTimer2.InterruptCount++;
    /*EALLOW;*/
    if( timer_int_cnt++ >= 12) 
    {
        timer_int_cnt = 0;
        CpuTimer0Regs.TCR.all = 0x4001;          // 设置TIE = 1，开启定时器0中断
        CpuTimer1Regs.TCR.all = 0x0001;          // 设置TIE = 0，关闭定时器1中断
        CpuTimer2Regs.TCR.all = 0x0001;          // 设置TIE = 0，关闭定时器2中断
    }
    GpioDataRegs.GPBTOGGLE.all =0x30000000; 
    GpioDataRegs.GPCTOGGLE.all =0x00000007;
   // The CPU acknowledges the interrupt.
   /*EDIS;*/
}

void Gpio_select(void)
{

   
    /*EALLOW;*/
	GpioCtrlRegs.GPAMUX1.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPAMUX2.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPBMUX1.all = 0x00000000;  // All GPIO
    GpioCtrlRegs.GPBMUX2.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPCMUX1.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPCMUX2.all = 0x00000000;  // All GPIO


    GpioCtrlRegs.GPADIR.all = 0xFFFFFFFF;   // All outputs
    GpioCtrlRegs.GPBDIR.all = 0xFFFFFFFF;   // All outputs
    GpioCtrlRegs.GPCDIR.all = 0xFFFFFFFF;   // All outputs

    GpioDataRegs.GPBDAT.all    =0x30000000;
    GpioDataRegs.GPCDAT.all    =0x00000007;

    /*EDIS;*/
     
}    

//===========================================================================
// No more.
//===========================================================================
