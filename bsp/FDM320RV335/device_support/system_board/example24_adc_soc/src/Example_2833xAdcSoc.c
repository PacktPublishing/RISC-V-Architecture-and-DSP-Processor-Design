//###########################################################################
//
// 文件名:   Example_2833xAdc.c
//
// 标题:  ePWM周期触发ADC采样测试
//
// ASSUMPTIONS:
//
//     此程序需要 DSP2833x头文件.
//
//    编译此例程前，在DSP2833x_Examples.h文件中确保CPU时钟速
//    度被合适地定义
//
//   连接信号到转换通道A0和A1
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
// 功能描述:
//
//   CPU系统时钟为150MHz，高速时钟等于
//   系统时钟除以6为25MHz
//
//   ePWM1设置为在计数到比较寄存器值时产生SEQ1转换触发信号，
//   ADC两通道ADCINA0和ADCINA1完成转换后产生中断，在中断子程序中
//   分别提取ADC的采样值
//
//   观察变量:
//
//         Voltage1[10]     上10个ADCRESULT0值
//         Voltage2[10]     上10个ADCRESULT1值
//         ConversionCount  当前结果堆栈的数字（0-9）
//         LoopCount        无限循环计数
//
//


#include "RV335_Device.h"     // DSP2833x Headerfile Include File
#include "RV335_Examples.h"   // DSP2833x Examples Include File

// Prototype statements for functions found within this file.
 void adc_isr(void);

// Global variables used in this example:
Uint16 LoopCount;
Uint16 ConversionCount;
Uint16 Voltage1[10];
Uint16 Voltage2[10];

 #define ADC_MODCLK 0x3 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 150/(2*3)   = 25.0 MHz

main()
{

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

   // Specific clock setting for this example:
   /*EALLOW;*/
   SysCtrlRegs.HISPCP.all = ADC_MODCLK;	// HSPCLK = SYSCLKOUT/（2*ADC_MODCLK）=25MHZ
   /*EDIS;*/


// Step 2. Initialize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example

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
   /*EALLOW;*/  // This is needed to write to EALLOW protected register
   //init plic default settings
    init_plic();
    // config isr
    IntVectTable.st.ADCINT = adc_isr;
    // enable trap
    trap_init();
    // enable interrupt
    enable_interrupt( GET_INT_NUM(IntVectTable.st.ADCINT));
   /*EDIS;*/    // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals(); // Not required for this example
   InitAdc();  // For this example, init the ADC

// Step 5. User specific code, enable interrupts:

// Enable ADCINT in PIE
   /*PieCtrlRegs.PIEIER1.bit.INTx6 = 1;*/
   /*IER |= M_INT1;*/ // Enable CPU Interrupt 1
   /*EINT;*/          // Enable Global interrupt INTM
   ERTM;          // Enable Global realtime interrupt DBGM

   LoopCount = 0;
   ConversionCount = 0;

// Configure ADC
   AdcRegs.ADCMAXCONV.all = 0x0001;       // Setup 2 conv's on SEQ1
   AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0; // Setup ADCINA0 as 1st SEQ1 conv.
   AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1; // Setup ADCINA1 as 2nd SEQ1 conv
   AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;// Enable SOCA from ePWM to start SEQ1
   AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // Enable SEQ1 interrupt (every EOS)

// Assumes ePWM1 clock is already enabled in InitSysCtrl();
   EPwm1Regs.ETSEL.bit.SOCAEN = 1;        // Enable SOC on A group
   EPwm1Regs.ETSEL.bit.SOCASEL = 4;       // Select SOC from CPMA on upcount
   EPwm1Regs.ETPS.bit.SOCAPRD = 1;        // Generate pulse on 1st event
   EPwm1Regs.CMPA.half.CMPA = 0x0080;	  // Set compare A value
   EPwm1Regs.TBPRD = 0xFFFF;              // Set period for ePWM1
   EPwm1Regs.TBCTL.bit.CTRMODE = 0;		  // count up and start

// Wait for ADC interrupt
   for(;;)
   {
      LoopCount++;
   }

}


 void  adc_isr(void) {
  AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
  Voltage1[ConversionCount] = ( (AdcRegs.ADCRESULT0)>>4);
  Voltage2[ConversionCount] = ( (AdcRegs.ADCRESULT1)>>4);

  // If 40 conversions have been logged, start over
  if(ConversionCount == 9)
  {
     ConversionCount = 0;
  }
  else
  ConversionCount++;

  // Reinitialize for next ADC sequence

  AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
  /*PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;*/   // Acknowledge interrupt to PIE

//  return;
}



