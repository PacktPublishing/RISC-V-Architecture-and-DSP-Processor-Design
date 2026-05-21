//###########################################################################
//
// FILE:    Example1_GpioLED.c
//
// TITLE:   用DSP点亮发光二极管实验 
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
// 功能描述：
//
//   程序编译下载成功后，运行程序时LED灯组会一直闪烁   
// 
//
//###########################################################################
// 释放日期: 2013.11.11
//###########################################################################

#include "RV335_Device.h"     // RV335 头文件
#include "RV335_Examples.h"   // RV335 例子相关头文件



// 使用前，声明本文件中的相关函数；
void delay_loop(void);
void Gpio_select(void);

void main(void)
{

// 步骤 1. 初始化系统控制:
// 设置PLL, WatchDog, 使能外设时钟
// 下面这个函数可以从RV335_SysCtrl.c文件中找到..
   InitSysCtrl();
   
// 步骤 2. 初始化通用输入输出多路复用器GPIO: 
// 这个函数在RV335_Gpio.c源文件中被定义了
// 这个函数使GPIO控制类寄存器初始化到默认状态
// InitGpio();  // 本例不用此子函数
 
// 本例使用下面的GPIO配置
   Gpio_select();	  

// 步骤 3. 清除所有中断初始化中断向量表:
// 禁止CPU全局中断
   /*DINT;*/



	
// 步骤 4. 初始化片内外设:
// 这个函数可以在DSP280x_CpuTimers.c源文件中找到
//   InitCpuTimers();   // 这个例子仅初始化了Cpu定时器
	
// 步骤 5. 用户特定的代码
   
   GpioDataRegs.GPBDAT.all    =0x30000000;
   GpioDataRegs.GPCDAT.all    =0x00000007;
   for(;;)
   {   
       
	   delay_loop();
       GpioDataRegs.GPBTOGGLE.all =0x30000000; 
       GpioDataRegs.GPCTOGGLE.all =0x00000007; 			  
       
       delay_loop();

       GpioDataRegs.GPBTOGGLE.all =0x30000000; 
       GpioDataRegs.GPCTOGGLE.all =0x00000007; 
    
    }



} 	
//延迟子函数
void delay_loop()
{
    Uint32      i;
	Uint32      j;
	for(i=0;i<32;i++)
    for (j = 0; j < 100000; j++) {}
}
//GPIO初始化函数
void Gpio_select(void)
{

   
    EALLOW;
	GpioCtrlRegs.GPAMUX1.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPAMUX2.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPBMUX1.all = 0x00000000;  // All GPIO
    GpioCtrlRegs.GPBMUX2.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPCMUX1.all = 0x00000000;  // All GPIO
	GpioCtrlRegs.GPCMUX2.all = 0x00000000;  // All GPIO


    GpioCtrlRegs.GPADIR.all = 0xFFFFFFFF;   // All outputs
    GpioCtrlRegs.GPBDIR.all = 0xFFFFFFFF;   // All outputs
    GpioCtrlRegs.GPCDIR.all = 0xFFFFFFFF;   // All outputs

    EDIS;
     
}     
//===========================================================================
// No more.
//===========================================================================

