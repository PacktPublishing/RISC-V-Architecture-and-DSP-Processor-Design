/********************************************************************
* 文件名： 	按键扫描程序
* 描述:  	运行程序，按下按键，观测变量Key的值（该值从从1~9)。				 

**********************************************************************/
/********************************************************************
程序说明：按键扫描原理：1、3列对应的IO设置为输出,3行对应的IO设置为输入。其实
					2、若无按键按下，3行输入IO，均为高电平（因为有外部上拉电阻）
					3、若按键按下，对应行IO被拉低（假设为第X行），检测为低电平。按键所在行（X行），被鉴别出来。
					4、此时，依次改变3列的输出为高，当遇到按键所在列时，第X行电平重新变为高。按键所在列。被鉴别出来。
			观测变量Key的值来判断哪个按键按下了；
********************************************************************/
#include "RV335_Device.h"     // RV335 Headerfile Include File
#include "RV335_Examples.h"   // RV335 Examples Include File

/**************************************宏定义************************************************/

#define SET_KY1			GpioDataRegs.GPBSET.bit.GPIO48 = 1						//Y1拉高
#define RST_KY1			GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1						//Y1拉低
#define SET_KY2			GpioDataRegs.GPBSET.bit.GPIO49 = 1						//Y2拉高
#define RST_KY2			GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1						//Y2拉低
#define SET_KY3 		GpioDataRegs.GPBSET.bit.GPIO50 = 1						//Y3拉高
#define RST_KY3			GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1						//Y3拉低


#define KX1_STATUS	 	GpioDataRegs.GPADAT.bit.GPIO12							//X1状态
#define KX2_STATUS	 	GpioDataRegs.GPADAT.bit.GPIO13							//X2状态
#define KX3_STATUS	 	GpioDataRegs.GPADAT.bit.GPIO14							//X3状态
/*****************************************************************************************************/

/**************************************函数声明************************************************/
void Init_KeyGpio(void);							//初始化按键IO
void delay(Uint32 t);								//延时函数
void ResetAllKY(void);								//3列全部输出低电平
void KX_AllStatus(void);							//读取3行IO电平状态
void Read_KX(Uint16 x);								//读取按键所在行
void Set_KY(Uint16 x);								//设置任意列输出高电平
void Rst_KY(Uint16 x);								//设置任意列输出低电平
void Read_KY(Uint16 x);								//读取按键所在列
/****************************************************************************************************/

/**************************************变量定义************************************************/
Uint16 Keys[3][3] = {1,2,3,4,5,6,7,8,9};          //数据表，与9个按键对应{SW1,SW2,SW3,SW4,SW5,SW6,SW7,SW8,SW9}
Uint16 Key = 0;									  //实时按键信息变量
Uint16 KX_On = 0;
Uint16 KX_Tim[5] = {0};
Uint16 KX_Status[5]={0};
Uint16 KY_On = 0;
/*****************************************************************************************************/
/*********************************************延时函数************************************************/
void delay(Uint32 t)
{
	Uint32 i = 0;
	for (i = 0; i < t; i++);
}
/*****************************************************************************************************/

/****************************************4列全部输出低电平************************************************/
void ResetAllKY(void)
{

	RST_KY3	;
	RST_KY2	;
	RST_KY1	;
}
/*****************************************************************************************************/

/****************************************读取4行IO电平状态************************************************/
void KX_AllStatus(void)
{
	KX_Status[1] = KX1_STATUS;
	KX_Status[2] = KX2_STATUS;
	KX_Status[3] = KX3_STATUS;

}
/*****************************************************************************************************/

/**************************************读取按键行位置**************************************/
void Read_KX(Uint16 x)
{
	KX_AllStatus();
	if(KX_Status[x] == 0)
	{
		 KX_Tim[x]++;
		 if(KX_Tim[x] >= 6000)
		 {
			 KX_On = x;	
			 KX_Tim[1]= 0;
			 KX_Tim[2]=0;
			 KX_Tim[3]=0;

		 }
	}
}
/*****************************************************************************************************/

/*******************************指定列输出高电平******************************************/
void Set_KY(Uint16 x)
{
	if(x==1){SET_KY1;}
	if(x==2){SET_KY2;}
	if(x==3){SET_KY3;}

}
/*****************************************************************************************************/

/*******************************指定列输出低电平**********************************************/
void Rst_KY(Uint16 x)
{
	if(x==1){RST_KY1;}
	if(x==2){RST_KY2;}
	if(x==3){RST_KY3;}

}
/*****************************************************************************************************/

/*******************************读取按键列位置**********************************************/
void Read_KY(Uint16 x)
{
	
	if(!KX_Status[KX_On] && KX_On)
	{
		Set_KY(x);
		delay(200);
		KX_AllStatus();
		if(KX_Status[KX_On])
		{
			KY_On = x;
			// set breakpoint here to debug,
			// watch Key value
			Key = Keys[KX_On-1][KY_On-1];
			KY_On = 0;
			KX_On = 0;
			
		}
		Rst_KY(x);
	}
}
/*****************************************************************************************************/

/*******************************按键IO初始化***********************************/
void Init_KeyGpio(void)
{

	EALLOW; 
	 
	//////////////////////////////以下3个IO口设置为输出，作为列扫描////////////////////////////

	//初始化行1的GPIO48
	GpioCtrlRegs.GPBPUD.bit.GPIO48 = 0;   									// Enable pullup on GPIO48
    GpioDataRegs.GPBSET.bit.GPIO48 = 1;   									// Load output latch
    GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0;  									// GPIO48 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO48 = 1;   									// GPIO48 = output

    //初始化行2的GPIO49
	GpioCtrlRegs.GPBPUD.bit.GPIO49 = 0;   									// Enable pullup on GPIO49
    GpioDataRegs.GPBSET.bit.GPIO49 = 1;   									// Load output latch
    GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;  									// GPIO49 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO49 = 1;   									// GPIO49 = output

    //初始化行3的GPIO50
    GpioCtrlRegs.GPBPUD.bit.GPIO50 = 0;   									// Enable pullup on GPIO50
    GpioDataRegs.GPBSET.bit.GPIO50 = 1;   									// Load output latch
    GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;  									// GPIO50 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;   									// GPIO50 = output


	//////////////////////////////以下3个IO口设置为输入，作为行扫描////////////////////////////



	//初始化列1的GPIO12
    GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;   									// Enable pullup on GPIO12
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;  									// 设置为一般IO口
    GpioCtrlRegs.GPADIR.bit.GPIO12  = 0;   									// IO口方向为输入

 	//初始化列2的GPIO13
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;   									// Enable pullup on GPIO13
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;  									// 设置为一般IO口
    GpioCtrlRegs.GPADIR.bit.GPIO13  = 0;   									// IO口方向为输入

	//初始化列3的GPIO14
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;   									// Enable pullup on GPIO14
    GpioCtrlRegs.GPAMUX1.bit.GPIO14  = 0;  									// 设置为一般IO口
    GpioCtrlRegs.GPADIR.bit.GPIO14   = 0;   									// IO口方向为输入
 
    EDIS;  
 	ResetAllKY();
}
/*****************************************************************************************************/



int main(void)
{

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP280x_SysCtrl.c file.
   InitSysCtrl();
   
// Step 2. Initalize GPIO: 
// This example function is found in the DSP280x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example
 
// For this example use the following configuration:
   Init_KeyGpio();											//初始化拨码开关IO  
//Buzz_Gpio_Init();
// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts 
   /*DINT;*/

// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.  
// This function is found in the DSP280x_PieCtrl.c file.
   /*InitPieCtrl();*/

// Disable CPU interrupts and clear all CPU interrupt flags:
   /*IER = 0x0000;*/
   /*IFR = 0x0000;*/


	while(1)												//死循环
	{
		Read_KX(1);
		Read_KX(2);
		Read_KX(3);
		Read_KY(1);
		Read_KY(2);
		Read_KY(3);
		
	}

	return 0;
} 	


//===========================================================================
// No more.
//===========================================================================

