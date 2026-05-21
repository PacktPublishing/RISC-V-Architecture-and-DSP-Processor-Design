//###########################################################################
//
// FILE:    Example_2833xEPwmTimerInt.c
//
// TITLE:   DSP2833x ePWM Timer Interrupt example.
//
// ASSUMPTIONS:
//
//    This program requires the DSP2833x header files.
//
//    Other then boot mode configuration, no other hardware configuration
//    is required.
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
// 功能描述
//
//    本例配置ePWM定时器并且每次中断时相应计数变量增加一次
//
//    应用如下:
//
//    所有的ePWM都被初始化.注意28335有6个ePWM,但并不是所有的2833x系列
//    都有6个ePWM
//   
//    所有的定时器都有相等的定时周期，所有的定时器同步
//    对于每个ePWM定时器遇到计数0匹配事件时都发生以下响应：
//   
//       ePWM1: 每1次0匹配事件发生一次中断
//       ePWM2: 每2次0匹配事件发生一次中断
//       ePWM3: 每3次0匹配事件发生一次中断
//       ePWM4-ePWM6: 每1次0匹配事件发生一次中断
//
//    Thus the Interrupt count for ePWM1, ePWM4-ePWM6 should be equal
//    The interrupt count for ePWM2 should be about half that of ePWM1
//    and the interrupt count for ePWM3 should be about 1/3 that of ePWM1
//
//    Watch Variables:
//       EPwm1TimerIntCount
//       EPwm2TimerIntCount
//       EPwm3TimerIntCount
//       EPwm4TimerIntCount
//       EPwm5TimerIntCount
//       EPwm6TimerIntCount



#include "RV335_Device.h"     // DSP2833x Headerfile Include File
#include "RV335_Examples.h"   // DSP2833x Examples Include File

// 配置epwm定时中断，在PIE级使能
// 1 = enabled,  0 = disabled
#define PWM1_INT_ENABLE  1
#define PWM2_INT_ENABLE  1
#define PWM3_INT_ENABLE  1
#define PWM4_INT_ENABLE  1
#define PWM5_INT_ENABLE  1
#define PWM6_INT_ENABLE  1

// 为每个定时器配置周期
#define PWM1_TIMER_TBPRD   0x1FFF
#define PWM2_TIMER_TBPRD   0x1FFF
#define PWM3_TIMER_TBPRD   0x1FFF
#define PWM4_TIMER_TBPRD   0x1FFF
#define PWM5_TIMER_TBPRD   0x1FFF
#define PWM6_TIMER_TBPRD   0x1FFF


// 申明中断服务子程序
 void epwm1_timer_isr(void);
 void epwm2_timer_isr(void);
 void epwm3_timer_isr(void);
 void epwm4_timer_isr(void);
 void epwm5_timer_isr(void);
 void epwm6_timer_isr(void);
void InitEPwmTimer(void);

// Global variables used in this example
Uint32  EPwm1TimerIntCount;
Uint32  EPwm2TimerIntCount;
Uint32  EPwm3TimerIntCount;
Uint32  EPwm4TimerIntCount;
Uint32  EPwm5TimerIntCount;
Uint32  EPwm6TimerIntCount;


void main(void)
{
   int i;

// Step 1. 初始化系统控制
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. 初始化 GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example


// Step 3. 清除所有的中断，初始化中断向量表
// 禁止CPU中断
   /*DINT;*/

// 初始化PIE控制寄存器到期默认状态
// 默认状态是所有的PIE中断禁止，所有的中断标志位清除
// 这个函数可以在DSP2833x_PieCtrl.c文件里面找到
   /*InitPieCtrl();*/

// 禁止CPU中断和清除所有CPU中断标志位
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
    IntVectTable.st.EPWM1_INT = epwm1_timer_isr;
    // enable trap
//    trap_init();
    // enable interrupt
    enable_interrupt( GET_INT_NUM(IntVectTable.st.EPWM1_INT));
   //init plic default settings
//    init_plic();
    // config isr
    IntVectTable.st.EPWM2_INT = epwm2_timer_isr;
    // enable trap
//    trap_init();
    // enable interrupt
    enable_interrupt( GET_INT_NUM(IntVectTable.st.EPWM2_INT));
   //init plic default settings
//    init_plic();
    // config isr
    IntVectTable.st.EPWM3_INT = epwm3_timer_isr;
    // enable trap
//    trap_init();
    // enable interrupt
    enable_interrupt( GET_INT_NUM(IntVectTable.st.EPWM3_INT));
   //init plic default settings
//    init_plic();
    // config isr
    IntVectTable.st.EPWM4_INT = epwm4_timer_isr;
    // enable trap
//    trap_init();
    // enable interrupt
    enable_interrupt( GET_INT_NUM(IntVectTable.st.EPWM4_INT));
   //init plic default settings
//    init_plic();
    // config isr
    IntVectTable.st.EPWM5_INT = epwm5_timer_isr;
    // enable trap
//    trap_init();
    // enable interrupt
    enable_interrupt( GET_INT_NUM(IntVectTable.st.EPWM5_INT));
   //init plic default settings
//    init_plic();
    // config isr
    IntVectTable.st.EPWM6_INT = epwm6_timer_isr;
    // enable trap

    // enable interrupt
    enable_interrupt( GET_INT_NUM(IntVectTable.st.EPWM6_INT));

    trap_init();
   /*EDIS;*/    // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals();  // Not required for this example
   InitEPwmTimer();    // For this example, only initialize the ePWM Timers

// Step 5. User specific code, enable interrupts:

// Initalize counters:
   EPwm1TimerIntCount = 0;
   EPwm2TimerIntCount = 0;
   EPwm3TimerIntCount = 0;
   EPwm4TimerIntCount = 0;
   EPwm5TimerIntCount = 0;
   EPwm6TimerIntCount = 0;

// Enable CPU INT3 which is connected to EPWM1-6 INT:
   /*IER |= M_INT3;*/

// Enable EPWM INTn in the PIE: Group 3 interrupt 1-6
   /*PieCtrlRegs.PIEIER3.bit.INTx1 = PWM1_INT_ENABLE;*/
   /*PieCtrlRegs.PIEIER3.bit.INTx2 = PWM2_INT_ENABLE;*/
   /*PieCtrlRegs.PIEIER3.bit.INTx3 = PWM3_INT_ENABLE;*/
   /*PieCtrlRegs.PIEIER3.bit.INTx4 = PWM4_INT_ENABLE;*/
   /*PieCtrlRegs.PIEIER3.bit.INTx5 = PWM5_INT_ENABLE;*/
   /*PieCtrlRegs.PIEIER3.bit.INTx6 = PWM6_INT_ENABLE;*/

// Enable global Interrupts and higher priority real-time debug events:
   /*EINT;*/   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM

// Step 6. IDLE loop. Just sit and loop forever (optional):
   for(;;)
   {
       asm("          NOP");
       for(i=1;i<=10;i++)
       {}
   }

}


void InitEPwmTimer()
{

   /*EALLOW;*/
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;      // Stop all the TB clocks
   /*EDIS;*/

   // Setup Sync
   EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through
   EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through
   EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through
   EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through
   EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through
   EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through

   // Allow each timer to be sync'ed

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

   /*EALLOW;*/
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;         // Start all the timers synced
   /*EDIS;*/


}


// Interrupt routines uses in this example:
 void epwm1_timer_isr(void) {
   EPwm1TimerIntCount++;

   // Clear INT flag for this timer
   EPwm1Regs.ETCLR.bit.INT = 1;

   // Acknowledge this interrupt to receive more interrupts from group 3
   /*PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;*/
}

 void epwm2_timer_isr(void) {
   EPwm2TimerIntCount++;

   // Clear INT flag for this timer
   EPwm2Regs.ETCLR.bit.INT = 1;

   // Acknowledge this interrupt to receive more interrupts from group 3
   /*PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;*/
}

 void epwm3_timer_isr(void) {
   EPwm3TimerIntCount++;

   // Clear INT flag for this timer
   EPwm3Regs.ETCLR.bit.INT = 1;

   // Acknowledge this interrupt to receive more interrupts from group 3
   /*PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;*/
}

 void epwm4_timer_isr(void) {
   EPwm4TimerIntCount++;

   // Clear INT flag for this timer
   EPwm4Regs.ETCLR.bit.INT = 1;

   // Acknowledge this interrupt to receive more interrupts from group 3
   /*PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;*/
}

 void epwm5_timer_isr(void) {
   EPwm5TimerIntCount++;

   // Clear INT flag for this timer
   EPwm5Regs.ETCLR.bit.INT = 1;

   // Acknowledge this interrupt to receive more interrupts from group 3
   /*PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;*/
}

 void epwm6_timer_isr(void) {
   EPwm6TimerIntCount++;

   // Clear INT flag for this timer
   EPwm6Regs.ETCLR.bit.INT = 1;

   // Acknowledge this interrupt to receive more interrupts from group 3
   /*PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;*/
}




//===========================================================================
// No more.
//===========================================================================
