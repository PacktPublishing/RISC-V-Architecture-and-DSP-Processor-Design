/********************************************************************
* 文件名： 	蜂鸣器音乐程序
* 描述:  	执行该程序，蜂鸣器演奏《生日快乐》或《小小星星亮晶晶》
* 创建人： 	Tony from YH Technology
* 版本号： 	V1.0
* 淘宝： 	http://YH-007.taobao.com
***********************************************************************/
/********************************************************************
程序说明：1.调用Buzz_Gpio_Init()函数，初始化与蜂鸣器相关的IO
		  2.调用Show()音乐播放函数。
		  3.定时器中断程序中写入BUZZ_CLK_GENER，产生蜂鸣器驱动脉冲。
********************************************************************/
#include "RV335_Project.h"     // Device Headerfile and Examples Include File
#include "RV335_SysCtrl.h"
#include "RV335_GlobalVariableDefs.h"
#include "../routines/include/RV335_I2c_defines.h"

#define BUZZ_OFF				GpioDataRegs.GPASET.bit.GPIO7 = 1;
#define BUZZ_ON

void Show(void);
void Buzz_Gpio_Init(void);
void Delay(Uint16 t);

Uint16 Musi[23]={		//单位 us，不同频率下，蜂鸣器发出不同音调的声音
				0,
				3816,	//L_do
				3496,	//L_re
				3215,	//L_mi
				2865,	//L_fa
				2551,	//L_so
				2272,	//L_la
				2024,	//L_xi
				1912,	//do
				1703,	//re
				1517,	//mi
				1432,	//fa
				1275,	//so
				1136,	//la
				1013,	//xi
				956,	//H_do
				851,	//H_re
				758,	//H_mi
				716,	//H_fa
				638,	//H_so
				568,	//H_la
				506,	//H_xi
				0xFF	//STOP
				};
				/*
Uint16 Song[]={1,1,5,5,6,6,5,4,4,3,3,2,2,1,5,5,4,4,3,3,2,5,5,4,4,3,3,2,22}; //《小小星星亮晶晶》简谱
Uint16 DT[]={2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4};		//节拍
*/
Uint16 Song[]=	{5,5,6,5,8,7,5,5,6,5,8,7,5,5,5,10,8,7,6,11,10,8,9,8,22	};	//《祝你生日快乐》简谱
Uint16 DT[]={2,2,5,4,6,8,2,2,5,6,6,9,4,2,5,4,6,12,2,2,5,6,8,12 };			//节拍

void Delay(Uint16 t)														//延时函数
{
	Uint32 i=0;
	Uint32 gain = 300000;													//延时增益
	Uint32 base=0;
	base=gain*t;
	for(i=0;i<=base;i++);
}

void Show(void)
{
	Uint16 addr=0;
	while(1)
	{
		if(Musi[Song[addr]]==0xFF)
		{
			return;															//音乐播放结束
		}
		else
		{
			StopCpuTimer0();												//停止计数
			disable_interrupt(GET_INT_NUM(IntVectTable.st.TIMER1));												//不使能定时中断
			ConfigCpuTimer(&CpuTimer0, 150, Musi[Song[addr]+8]/2);			//设置定时时间
			StartCpuTimer0();												//重启定时器
			enable_interrupt( GET_INT_NUM(IntVectTable.st.TIMER1));												//使能定时中断
			Delay(DT[addr]);												//音乐节拍延时
			StopCpuTimer0();												//停止计数
			disable_interrupt(GET_INT_NUM(IntVectTable.st.TIMER1));			//不使能定时中断
			BUZZ_OFF;														//关闭蜂鸣器
			Delay(2);														//音乐停顿
			addr++;
		}
	}

}

void Buzz_Gpio_Init(void)
{
	EALLOW;
/*	GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;   									// Enable pullup on GPIO11
    GpioDataRegs.GPASET.bit.GPIO11 = 1;   									// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;  									// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;   									// GPIO11 = output

	*/
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;   									// Enable pullup on GPIO0
    GpioDataRegs.GPASET.bit.GPIO7 = 1;  									 // Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;  									// GPIO0 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;   									// GPIO0 = output
    EDIS;
}
