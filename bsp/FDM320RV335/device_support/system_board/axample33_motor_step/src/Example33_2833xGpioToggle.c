//###########################################################################
//
// 文件名:    Example_280xStepMotor.c
//
//功能:初始化GPIO口然后完成步进电机旋转1圈控制
//
//作者: Tony from YH Technology
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
// 功能描述：
//
//程序控制GPIO2直到GPIO5依次输出高电平，则步进电机
// 先顺时钟旋转一周，然后代码控制GPIO5直到GPIO2依次输出高电平
//则步进电机逆时针旋转一周
//
//###########################################################################
// 释放日期: 2013.11.12
//###########################################################################

#include "RV335_Device.h"     // DSP2833x Headerfile Include File
#include "RV335_Examples.h"   // DSP2833x Examples Include File

// 申明用到的子函数
static void _delay_loop(Uint32 a);
void Gpio_select(void);

unsigned char Step_table[]={0x0004,0x0008,0x0010,0x0020};
unsigned char Step_table1[]={0x0020,0x0010,0x0008,0x0004};

void main(void)
{

unsigned int i,j;
// 步骤 1. 初始化系统控制:
// 设置PLL, WatchDog, 使能外设时钟
// 下面这个函数可以从DSP2833x_SysCtrl.c文件中找到.
   InitSysCtrl();
   
// 步骤 2. 初始化通用输入输出多路复用器GPIO: 
// 这个函数在DSP2833x_Gpio.c源文件中被定义了
// 这个函数使GPIO控制类寄存器初始化到默认状态
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
	
// 步骤 5. 用户特定的代码来允许中断

// 步骤 6. 空循环. 仅无限循环等待中断:
 
  j=512;
   while(j--)
   {
      for(i=0;i<4;i++)
      {
       GpioDataRegs.GPADAT.all=Step_table[i]; //GPIO2-GPIO5依次输出高电平
       _delay_loop(80000);

      }
    }
	
	
//反转一圈
	j=512;
	GpioDataRegs.GPADAT.all=0x0000;
	 while(j--)
   {
      for(i=0;i<4;i++)
      {
       GpioDataRegs.GPADAT.all=Step_table1[i]; //GPIO2-GPIO5依次输出高电平
       _delay_loop(80000);

      }
    }
	while(1);

} 	


//延时函数
static void _delay_loop(Uint32 a)
{
   while(a--);
}

//初始化GPIO
void Gpio_select(void)
{

   
    /*EALLOW;*/
	GpioCtrlRegs.GPAMUX1.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPAMUX2.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPBMUX1.all = 0x00000000;  // All GPIO
    GpioCtrlRegs.GPADIR.all = 0xFFFFFFFF;   // All outputs
    GpioCtrlRegs.GPBDIR.all = 0x0000000F;   // All outputs

	 //GPIO0-GPIO31输出低电平
   GpioDataRegs.GPADAT.all=0x00000000;
   /*EDIS;*/
    /*EDIS;*/
     
}     
//===========================================================================
// No more.
//===========================================================================

