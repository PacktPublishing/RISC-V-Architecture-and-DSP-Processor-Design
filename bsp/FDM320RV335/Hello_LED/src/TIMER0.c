//###########################################################################
//
// TITLE:   用定时器TIME0控制LED闪烁实验
//
//
// 功能描述：
//
//   程序编译下载成功后，运行程序时LED灯组会一直闪烁
//
//###########################################################################

#include "../routines/include/RV335_Device.h"     // RV335 头文件
#include "../routines/include/RV335_Examples.h"   // RV335 例子相关头文件

#define	  LED1	GpioDataRegs.GPCDAT.bit.GPIO64  //宏定义GPA组的GPIO0为LED1
#define	  LED2	GpioDataRegs.GPCDAT.bit.GPIO65  //宏定义GPA组的GPIO1为LED2
#define	  LED3	GpioDataRegs.GPCDAT.bit.GPIO66  //宏定义GPA组的GPIO2为LED3
#define	  LED4	GpioDataRegs.GPCDAT.bit.GPIO67 //宏定义GPA组的GPIO3为LED4
#define	  LED5	GpioDataRegs.GPCDAT.bit.GPIO68  //宏定义GPA组的GPIO4为LED5

 void ISRTimer0(void);  //声明定时器TIME0中断
void configtestled(void);        //声明GPIO配置

void main(void)
{
   InitSysCtrl();


   init_plic();
   // enable trap
   trap_init();
   // config isr
   IntVectTable.st.TINT0 = ISRTimer0;  //将定时器中断添加都中断向量表里
   // enable interrupt
   enable_interrupt( GET_INT_NUM(IntVectTable.st.TINT0));
   InitCpuTimers();   // 定时器初始化

//通过以下面程序就可以让定时器 0 每隔一段时间产生一次中断，这段时间的
//计算公式为： △T= Freq * Period /150000000（s）；（其中 150000000 是
//CPU 的时钟频率，即 150MHz 的 时钟频率）针对此实验，Frep 为 150，Period 为 1000000，那么△T=1s。
   ConfigCpuTimer(&CpuTimer0, 150, 1000000);
   StartCpuTimer0();  //开启定时器

//    ERTM;   // 使能总实时中断

// 本例使用下面的GPIO配置
    configtestled();

	LED1=1;        //初始化LED电平
	DELAY_US(10);  //延时
	LED2=0;        //初始化LED电平
	DELAY_US(10);  //延时
	LED3=1;        //初始化LED电平
	DELAY_US(10);  //延时
	LED4=0;        //初始化LED电平
	DELAY_US(10);  //延时
	LED5=1;        //初始化LED电平
	DELAY_US(10);  //延时
    for(; ;);      //For循环
}


 void ISRTimer0(void) {
   // Acknowledge this interrupt to receive more interrupts from group 1
    /*PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;*/ //0x0001赋给12组中断ACKnowledge寄存器，对其全部清除，不接受其他中断
    CpuTimer0Regs.TCR.bit.TIF=1;  // 定时到了指定时间，标志位置位，清除标志
    CpuTimer0Regs.TCR.bit.TRB=1;  // 重载Timer0的定时数据
        LED1=~LED5; //取反
    	LED2=~LED1; //取反
	    LED3=~LED2; //取反
    	LED4=~LED3; //取反
    	LED5=~LED4; //取反
}

void configtestled(void)
{

   GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 0; // GPIO复用为GPIO功能
   GpioCtrlRegs.GPCDIR.bit.GPIO64 = 1;  // GPIO设置为输出
   GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 0; // GPIO复用为GPIO功能
   GpioCtrlRegs.GPCDIR.bit.GPIO65 = 1;  // GPIO设置为输出
   GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 0; // GPIO复用为GPIO功能
   GpioCtrlRegs.GPCDIR.bit.GPIO66 = 1;  // GPIO设置为输出
   GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 0; // GPIO复用为GPIO功能
   GpioCtrlRegs.GPCDIR.bit.GPIO67 = 1;  // GPIO设置为输出
   GpioCtrlRegs.GPCMUX1.bit.GPIO68 = 0; // GPIO复用为GPIO功能
   GpioCtrlRegs.GPCDIR.bit.GPIO68 = 1;  // GPIO设置为输出
}
