//###########################################################################
//
// 文件名:    280xLCD1602_YH.c
//
// 功能:   I/O口实现1602液晶显示功能；
//
//作者: Tony from YH Technology
// 描述:
// 
//
//     使用GPIO0-GPIO11，液晶与其对应的引脚如下所示：
//
//        RS--GPIO1引脚，R/W--GPIO2引脚，EN--GPIO3引脚,D0--GPIO4引脚
//        D1--GPIO5引脚,D2--GPIO6引脚,D3--GPIO7引脚,D4--GPIO8引脚
//       D5--GPIO9引脚,D6--GPIO10引脚,D7--GPIO11引脚;
//    根据在RAM中调试的需要，这个项目配置成"boot to SARAM".280x引导模式
//    表如下显示. 常用的还有"boot to Flash"模式，当程序在RAM调试完善后就
//    可以将代码烧进Flash中并使用"boot to Flash"引导模式.
//
//        Boot      GPIO18     GPIO29    GPIO34
//        Mode      SPICLKA    SCITXDA
//                  SCITXB
//        -------------------------------------
//        Flash       1          1        1
//        SCI-A       1          1        0
//        SPI-A       1          0        1
//        I2C-A       1          0        0
//        ECAN-A      0          1        1        
//        SARAM       0          1        0  <- "boot to SARAM"
//        OTP         0          0        1
//        I/0         0          0        0 
//
//
//###########################################################################
// 释放日期: 2013.1.17
//###########################################################################

#include "RV335_Device.h"     // DSP2833x Headerfile Include File
#include "RV335_Examples.h"   // DSP2833x Examples Include File

/***************全局变量定义****************/
#define uchar unsigned char

/****************端口宏定义*****************/
#define LCD_RS GpioDataRegs.GPADAT.bit.GPIO0
//#define LCD_RW GpioDataRegs.GPADAT.bit.GPIO2 
#define LCD_EN GpioDataRegs.GPADAT.bit.GPIO1
#define LCD_DB GpioDataRegs.GPADAT.all

// 使用前，声明本文件中的相关函数
void Init_Port(void);
void LCD_init(void);                          //初始化函数
void LCD_write_command(uchar command);        //写指令函数
void LCD_write_data(uchar dat);               //写数据函数
void show(char *cont);                       //显示子函数


void main(void)
{
// 步骤 1. 初始化系统控制:
// 设置PLL, WatchDog, 使能外设时钟
// 下面这个函数可以从DSP280x_SysCtrl.c文件中找到.
   InitSysCtrl();

// 步骤 2. 初始化通用输入输出多路复用器GPIO: 
// 这个函数在DSP280x_Gpio.c源文件中被定义了
// 这个函数使GPIO控制类寄存器初始化到默认状态
// InitGpio();  // Skipped for this example  

//本例仅初始化液晶对应的GPIO引脚；
//这些函数在DSP280x_EPwm.c文件里面
   Init_Port();


// 步骤 3. 清除所有中断,初始化中断向量表:
// 禁止CPU全局中断
   /*DINT;*/

// 初始化PIE控制寄存器到他们的默认状态.
// 这个默认状态就是禁止PIE中断及清除所有PIE中断标志 
// 这个函数放在DSP280x_PieCtrl.c源文件里
   /*InitPieCtrl();*/
   
// 禁止CPU中断和清除所有CPU中断标志 
   /*IER = 0x0000;*/
   /*IFR = 0x0000;*/

//初始化PIE中断向量表，并使其指向中断服务子程序（ISR）
// 这些中断服务子程序被放在了DSP280x_DefaultIsr.c源文件中
// 这个函数放在了DSP280x_PieVect.c源文件里面.
   /*InitPieVectTable();*/

// 步骤 4.初始化片内外设:
// InitPeripherals();  // Not required for this example


   
// 步骤 5. 以下是用户编写的指定代码, 使能相关中断:


// 步骤 6. 循环函数，显示字符串:
   for(;;)
   {  
      LCD_init();
	  	 
	 	LCD_write_command(0x80);	//第一行数据指针地址
        show(" YH Technology  ");

		LCD_write_command(0xc0);	//第二行数据指针的地址
	    show(" Welcome to use!  ");
      while(1);
    
   }

} 
/*-------------------------------------- 
;模块名称:show(char *cont)
;功    能:LCD1602显示子函数
;参数说明:cont为显示字符串指针参数
;-------------------------------------*/ 
void show(char *cont)
{
 while(*cont!=0)
 {
  LCD_write_data(*cont);
  DELAY_US(500);  //延时500us
  cont++;
 }
}
/*------------------------------------------*/
/*形式参数：void                            */
/*返回值:void				                */
/*函数描述:初始化电机控制端口               */
/*------------------------------------------*/ 
void Init_Port(void)
{
	/*EALLOW;*/   
// 用户根据需要可以使能内部上拉或禁止. 
// 其他不需要的行注释掉；
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;    // 使能GPIO0 引脚内部上拉
    //GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // 禁止GPIO2 引脚内部上拉
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;   // 禁止GPIO1 引脚内部上拉
   
/* 通过I/O功能选择寄存器配置通用I/O功能*/
    GpioCtrlRegs.GPAMUX1.all = 0x000000;   // 配置GPIO0-GPIO15为通用I/O口


/* 设置I/O口为输入引脚*/
    GpioCtrlRegs.GPADIR.all = 0x00FFF;// 配置GPIO0-GPIO11为输出引脚

// 每个输入口可以有不同的输入限定
   // a) 输入与系统时钟 SYSCLKOUT同步
   // b) 输入被指定的采样窗口限定
   // c) 输入异步 (仅对外设输入有效)
   GpioCtrlRegs.GPAQSEL1.all = 0x0000;    // GPIO0-GPIO15与系统时钟SYSCLKOUT 同步
//输出数据LCD_RW和LCD_EN清零  
   GpioDataRegs.GPADAT.bit.GPIO0 = 1;
   //GpioDataRegs.GPADAT.bit.GPIO2 = 0;
   GpioDataRegs.GPADAT.bit.GPIO1 = 0;
    /*EDIS;*/
} 

/*-------------------------------------- 
;模块名称:LCD_write_command(); 
;功    能:LCD1602写指令函数 
;参数说明:command为写命令参数
;-------------------------------------*/ 
void LCD_write_command(uchar command)
{
LCD_EN=0;
LCD_RS=0;         //指令
//LCD_RW=0;         //写入
LCD_DB=(command<<2)|0x0000;//赋值给D0-D7对应的GPIO2-GPIO9引脚
DELAY_US(500); 
LCD_EN=1;          //允许
DELAY_US(1000);  //
LCD_EN=0;

}

/*-------------------------------------- 
;模块名称:LCD_write_data(); 
;功    能:LCD1602写数据函数 
;占用资源: P2.0--RS(LCD_RS),P2.1--RW(LCD_RW),P2.2--E(LCD_E).
;参数说明:dat为写数据参数
;-------------------------------------*/
void LCD_write_data(uchar dat)
{
LCD_EN=0;
LCD_RS=1;          //数据
//LCD_RW=0;          //写入
LCD_DB=(dat<<2)|0x0001;//赋值给D0-D7对应的GPIO2-GPIO9引脚
DELAY_US(500); 
LCD_EN=1;           //允许
DELAY_US(1000);
LCD_EN=0;
}
/*-------------------------------------- 
;模块名称:LCD_init(); 
;功    能:初始化LCD1602 
;占用资源:--
;参数说明:--
;-------------------------------------*/ 
void LCD_init(void)
{
DELAY_US(15000);//延迟15ms
LCD_write_command(0x38);//设置8位格式，2行，5x7
DELAY_US(5000);//延迟5ms
LCD_write_command(0x38);//设置8位格式，2行，5x7
DELAY_US(5000);
LCD_write_command(0x38);//设置8位格式，2行，5x7
LCD_write_command(0x38);//设置8位格式，2行，5x7
LCD_write_command(0x08);//关显示，不显示光标，光标不闪烁；
LCD_write_command(0x01);//清除屏幕显示：数据指针清零，所有显示清零；
LCD_write_command(0x06);//设定输入方式，增量不移位
LCD_write_command(0x0c);//整体显示，关光标，不闪烁

}



//===========================================================================
// No more.
//===========================================================================
