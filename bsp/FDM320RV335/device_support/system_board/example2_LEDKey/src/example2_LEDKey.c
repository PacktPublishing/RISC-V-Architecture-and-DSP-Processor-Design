// TI File $Revision: /main/7 $
// Checkin $Date: August 10, 2007   09:05:43 $
//###########################################################################
//
// FILE:    example2_LEDKey.c
//
// TITLE:   初始化GPIO口然后通过按键控制LED流水灯闪烁
//
// ASSUMPTIONS:
//
//    This program requires the DSP2833x header files.  
//
//    ALL OF THE I/O'S TOGGLE IN THIS PROGRAM.  MAKE SURE
//    THIS WILL NOT DAMAGE YOUR HARDWARE BEFORE RUNNING THIS
//    EXAMPLE.
//
//    Monitor desired pins on an oscilloscope.
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
//     Three different examples are included. Select the example 
//     (data, set/clear or toggle) to execute before compiling using
//     the #define statements found at the top of the code.   
//
//
//     Toggle all of the GPIO PORT pins 
//        
//    The pins can be observed using Oscilloscope.  
// 
//
//###########################################################################
// $TI Release: DSP2833x Header Files V1.01 $
// $Release Date: September 26, 2007 $
//###########################################################################

#include "RV335_Device.h"     // DSP2833x Headerfile Include File
#include "RV335_Examples.h"   // DSP2833x Examples Include File

/****************端口宏定义*****************/
#define S1 GpioDataRegs.GPADAT.bit.GPIO12


/****************常量宏定义*****************/ 

/***************全局变量定义****************/
Uint32 temp; 
Uint32 direction; 

/****************函数声明*******************/ 
void Gpio_select(void); //gpio端口配置子程序
void delay_loop(void);
void Scan_Key(void);

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
 
// For this example use the following configuration:
   Gpio_select();	  

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts 
   /*DINT;*/

// Initialize PIE control registers to their default state.
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

	
// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals(); // Not required for this example
	
// Step 5. User specific code:
   GpioDataRegs.GPBDAT.all    =0x30000000;//GPIO60,61,64,65,66,67,68输出清1，使LED1灯灭
   GpioDataRegs.GPCDAT.all    =0x0000001f;
   direction=0;
   for(;;)
   {
    unsigned int i;
	if(direction%2!=0)//控制流水灯的方向
		temp=0x10000000;
	else
		temp=0x20000000;


	while(temp!=0x40000000&&temp!=0x08000000)//轮流点亮GPIO60,61；
	{
		//delay_loop();
		GpioDataRegs.GPBCLEAR.all |=temp;
        delay_loop();


		GpioDataRegs.GPBSET.all |=temp;
		if(direction%2!=0)//控制流水灯的方向
		temp<<=1;
		else
			temp>>=1;
        
	}
	if(temp==0x40000000)//根据temp值选择GPIO64-GPIO67是从64开始还是从67开始点亮LED
        temp=0x00000001;
	else
		temp=0x00000010;
	while(temp!=0x00000020&&temp!=0x00000000)//轮流点亮GPIO64-GPIO67；
	{
        //delay_loop();
			GpioDataRegs.GPCCLEAR.all |=temp;
        delay_loop();

		

		GpioDataRegs.GPCSET.all |=temp;

		if(direction%2!=0)//控制流水灯的方向
				temp<<=1;
				else
					temp>>=1;
        
	
	}	
    
   }
} 	

void delay_loop()
{
    Uint32      i;
	Uint32      j;
	for(i=0;i<4;i++)
    for (j = 0; j < 100000; j++) {Scan_Key();}
}

/*------------------------------------------*/
/*形式参数：void		            		*/
/*返回值:void				    			*/
/*函数描述:扫描键值             	    	*/
/*------------------------------------------*/
void Scan_Key(void)      
{
Uint32 i,j;
	if(S1==0)//扫描是否按键按下       	
	{
	  for(i=0;i<10000;i++);    //键盘消抖动
	  if(S1==0)
	  {
      direction+=1;
	  for(i=0;i<32;i++)
      for (j = 0; j < 100000; j++) {}

	  /*
	     if(direction%2!=0)//第一按下按键LED停止闪烁，下次按下时LED又开始闪烁
         {
          while(S1!=0);
		 }
		 */

	  }
	}		  
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


    GpioCtrlRegs.GPADIR.all = 0x00000000;  // All inputs           Configure the GPIO pins as inputs
    GpioCtrlRegs.GPBDIR.all = 0xFFFFFFFF;  // All outputs,gpio50   按键EQEP1A  Configure the GPIO pins as outputs
    GpioCtrlRegs.GPCDIR.all = 0xFFFFFFFF;  // All outputs

    /*EDIS;*/
     
}     
//===========================================================================
// No more.
//===========================================================================

