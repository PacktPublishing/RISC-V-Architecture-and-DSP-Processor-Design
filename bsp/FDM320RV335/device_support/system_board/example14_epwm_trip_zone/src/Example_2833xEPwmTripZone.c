//###########################################################################
//
// 文件名:    Example_2833xEpwmTripZone.c
//
// 标题:   PWM错误控制模块测试
//
// ASSUMPTIONS:
//
//    此程序需要 DSP2833x头文件. 
//
//    初始化TZ1 (GPIO12) and TZ2 (GPIO13)为高
//
//    在测试期间, 观察按键SW1（TZ1）和SW2(TZ2)对ePWM1、ePWM2输出的影响 
//
//       EPWM1A对应引脚GPIO0
//       EPWM1B对应引脚GPIO1
//       EPWM2A对应引脚GPIO2
//       EPWM2B对应引脚GPIO3
//
//    当单次触发后ePWM1立即响应
// 
//    周期触发时ePWM2将响应，一旦TZ1和TZ2变为高电平后，
//    ePWM2引脚状态立即被清除 
//
//
//
// 功能描述:
//
//    本例配置ePWM1 和 ePWM2
// 
//    2个例子如下：
//    * ePWM1使用TZ1 和TZ2 作为单次触发源
//    * ePWM2 使用 TZ1 和 TZ2 作为周期触发源
//
//
//    通过示波器观察EPWM1A/B, EPWM2A/B波形，
//    可以发现按键SW1（TZ1）和SW2(TZ2)对EPWM1A/B, EPWM2A/B波形的影响
//
//    当按下按键SW1（TZ1）或SW2(TZ2)后EPWM1A变为高电平、EPWM1B变为低电平
//    松开按键后，输出波形不变,一直为高电平或低电平；
//    当长时间按下按键SW1（TZ1）或SW2(TZ2)后EPWM2A变为高电平、EPWM2B变为低电平
//    松开按键后，输出波形恢复之前的PWM波形；
//
//###########################################################################
// 释放日期：2013.11.26
//###########################################################################

#include "RV335_Device.h"     // DSP2833x Headerfile Include File
#include "RV335_Examples.h"   // DSP2833x Examples Include File

// Prototype statements for functions found within this file.
void InitEPwm1Example(void);
void InitEPwm2Example(void);
 void epwm1_tzint_isr(void);
 void epwm2_tzint_isr(void);


// Global variables used in this example
Uint32  EPwm1TZIntCount;
Uint32  EPwm2TZIntCount;

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

// For this case just init GPIO pins for ePWM1, ePWM2, and TZ pins
   InitEPwm1Gpio();
   InitEPwm2Gpio();
   InitTzGpio();
   
   //////////////////////////////以下3个IO口设置为输出，作为列扫描////////////////////////////
   /*EALLOW;*/
   	//初始化行1的GPIO48
   	GpioCtrlRegs.GPBPUD.bit.GPIO48 = 0;   									// Enable pullup on GPIO48
       GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1;   									// Load output latch
       GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0;  									// GPIO48 = GPIO
       GpioCtrlRegs.GPBDIR.bit.GPIO48 = 1;   									// GPIO48 = output

       //初始化行2的GPIO49
   	GpioCtrlRegs.GPBPUD.bit.GPIO49 = 0;   									// Enable pullup on GPIO49
       GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1;   									// Load output latch
       GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;  									// GPIO49 = GPIO
       GpioCtrlRegs.GPBDIR.bit.GPIO49 = 1;   									// GPIO49 = output

       //初始化行3的GPIO50
       GpioCtrlRegs.GPBPUD.bit.GPIO50 = 0;   									// Enable pullup on GPIO50
       GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1;   									// Load output latch
       GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;  									// GPIO50 = GPIO
       GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;   									// GPIO50 = output
       /*EDIS;*/
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
    IntVectTable.st.EPWM1_TZINT = epwm1_tzint_isr;
    // enable trap
    // enable interrupt
    enable_interrupt( GET_INT_NUM(IntVectTable.st.EPWM1_TZINT));
   //init plic default settings
    // config isr
    IntVectTable.st.EPWM2_TZINT = epwm2_tzint_isr;
    // enable trap
    trap_init();
    // enable interrupt
    enable_interrupt( GET_INT_NUM(IntVectTable.st.EPWM2_TZINT));
   /*EDIS;*/    // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals();  // Not required for this example


   /*EALLOW;*/
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
   /*EDIS;*/

   InitEPwm1Example();    
   InitEPwm2Example();

   /*EALLOW;*/
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
   /*EDIS;*/
   
// Step 5. User specific code, enable interrupts
// Initalize counters:   
   EPwm1TZIntCount = 0;
   EPwm2TZIntCount = 0;
   
// Enable CPU INT3 which is connected to EPWM1-3 INT:
   /*IER |= M_INT2;*/

// Enable EPWM INTn in the PIE: Group 2 interrupt 1-3
   /*PieCtrlRegs.PIEIER2.bit.INTx1 = 1;*/
   /*PieCtrlRegs.PIEIER2.bit.INTx2 = 1;*/

// Enable global Interrupts and higher priority real-time debug events:
   /*EINT;*/   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM



// Step 6. IDLE loop. Just sit and loop forever (optional):
   for(;;)
   {
       asm("          NOP");
   }

} 

 void epwm1_tzint_isr(void) {
   EPwm1TZIntCount++;

// Leave these flags set so we only take this
// interrupt once
//
// EALLOW;
// EPwm1Regs.TZCLR.bit.OST = 1;
// EPwm1Regs.TZCLR.bit.INT = 1;
// EDIS;

   // Acknowledge this interrupt to receive more interrupts from group 2
   /*PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;*/

}

 void epwm2_tzint_isr(void) {

   EPwm2TZIntCount++;

// Clear the flags - we will continue to take
// this interrupt until the TZ pin goes high
//
   /*EALLOW;*/
   EPwm2Regs.TZCLR.bit.CBC = 1;   
   EPwm2Regs.TZCLR.bit.INT = 1;
   /*EDIS;*/

   // Acknowledge this interrupt to receive more interrupts from group 2
   /*PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;*/

}


void InitEPwm1Example()
{
   // Enable TZ1 and TZ2 as one shot trip sources
   /*EALLOW;*/
   EPwm1Regs.TZSEL.bit.OSHT1 = 1;
   EPwm1Regs.TZSEL.bit.OSHT2 = 1;
   
   // What do we want the TZ1 and TZ2 to do?
   EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_HI;
   EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
   
   // Enable TZ interrupt
   EPwm1Regs.TZEINT.bit.OST = 1;
   /*EDIS;*/
   
   EPwm1Regs.TBPRD = 6000;                         // Set timer period
   EPwm1Regs.TBPHS.half.TBPHS = 0x0000;            // Phase is 0
   EPwm1Regs.TBCTR = 0x0000;                       // Clear counter

   // Setup TBCLK
   EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
   EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
   EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV4;       // Clock ratio to SYSCLKOUT
   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV4;

   EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
   EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
   EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;   

   // Setup compare 
   EPwm1Regs.CMPA.half.CMPA = 3000;

   // Set actions
   EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;             // Set PWM1A on Zero
   EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
   

   EPwm1Regs.AQCTLB.bit.CAU = AQ_CLEAR;          // Set PWM1A on Zero
   EPwm1Regs.AQCTLB.bit.CAD = AQ_SET;

}


void InitEPwm2Example()
{

   // Enable TZ1 and TZ2 as one cycle-by-cycle trip sources
   /*EALLOW;*/
   EPwm2Regs.TZSEL.bit.CBC1 = 1;
   EPwm2Regs.TZSEL.bit.CBC2 = 1;
   
   // What do we want the TZ1 and TZ2 to do?
   EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_HI;
   EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
   
   // Enable TZ interrupt
   EPwm2Regs.TZEINT.bit.CBC = 1;
   /*EDIS;*/

   EPwm2Regs.TBPRD = 6000;                        // Set timer period
   EPwm2Regs.TBPHS.half.TBPHS = 0x0000;           // Phase is 0
   EPwm2Regs.TBCTR = 0x0000;                      // Clear counter
   
   // Setup TBCLK
   EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
   EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
   EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV4;       // Clock ratio to SYSCLKOUT
   EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV4;          // Slow just to observe on the scope

   // Setup compare 
   EPwm2Regs.CMPA.half.CMPA = 3000;

   // Set actions
   EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;             // Set PWM2A on Zero
   EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;
   

   EPwm2Regs.AQCTLB.bit.CAU = AQ_CLEAR;           // Set PWM2A on Zero
   EPwm2Regs.AQCTLB.bit.CAD = AQ_SET;
}




//===========================================================================
// No more.
//===========================================================================
