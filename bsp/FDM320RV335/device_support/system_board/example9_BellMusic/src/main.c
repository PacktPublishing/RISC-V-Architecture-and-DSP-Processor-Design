#include "RV335_Project.h"     // Device Headerfile and Examples Include File
#include "RV335_SysCtrl.h"
#include "RV335_GlobalVariableDefs.h"
#include "../routines/include/RV335_I2c_defines.h"

/***************全局变量定义****************/
#define uchar unsigned char

/****************端口宏定义*****************/
#define BUZZ_CLK_GENER	GpioDataRegs.GPATOGGLE.bit.GPIO7 = 1;	//蜂鸣器控制IO，IO电平翻转，产生控制脉冲

// 使用前，声明本文件中的相关函数
void cpu_timer0_isr(void);
extern void Show(void);
extern void Buzz_Gpio_Init(void);

int main(void)
{
	// Initialize trap
	trap_init();
	init_priority();
	// Initialize System Control
	InitSysCtrl();
	/* --- add your code below ---*/
	Buzz_Gpio_Init();

	IntVectTable.st.TINT0 = cpu_timer0_isr;


	InitCpuTimers();   // 本例仅需要初始化CPU定时器

	ConfigCpuTimer(&CpuTimer0, 150, 1000000);
	StartCpuTimer0();
	enable_interrupt( GET_INT_NUM(IntVectTable.st.TINT0));

   while(1)
	 {

	Show();
	 }

	return 0;
}
void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;
   BUZZ_CLK_GENER;
   // Acknowledge this interrupt to receive more interrupts from group 1
//   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
