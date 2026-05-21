//###########################################################################
//
// 文件名:   Example_ExternalInterrupt.c
//
// 功能:  DSP280x外部中断测试程序.
//
//    应用时，这个工程被配置为"boot to SARAM"（从RAM启动）操作
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
//    XINT1外部中断1输入与系统时钟SYSCLKOUT同步；
//    XINT2外部中断2输入有个很长的输入限定 - 6个采样窗口，每个窗口周期
//    为510*SYSCLKOUT；
//###########################################################################
// 释放日期: 2012.12.12
//###########################################################################


#include "RV335_Device.h"     // Headerfile Include File
#include "RV335_Examples.h"   // Examples Include File

// 中断服务子程序在主程序下面时，需要在主程序之前要先申明才能使用.
 void xint1_isr(void);
 void xint2_isr(void);

// 本例中用到的全局变量；
volatile Uint32 Xint1Count; //发生外部中断1的次数；
volatile Uint32 Xint2Count; //发生外部中断2的次数；
volatile Uint32 Xint2Loop; //给外部中断2的for循环变量

#define keyon1 GpioDataRegs.GPADAT.bit.GPIO12
#define keyon2 GpioDataRegs.GPADAT.bit.GPIO13

#define DELAY 35.700L

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

   EALLOW;

      	//////////////////////////////以下IO口设置为输出，作为列扫描////////////////////////////

          //初始化行3的GPIO50
          GpioCtrlRegs.GPBPUD.bit.GPIO50 = 0;   									// Enable pullup on GPIO50
          GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1;   									// Load output latch
          GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;  									// GPIO50 = GPIO
          GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;   									// GPIO50 = output
   EDIS;

// 步骤 3. 清除所有中断,初始化中断向量表:
// 禁止CPU全局中断
   /*DINT;*/



//  本例中用到的中断重新映射到本件的中断服务子程序；
   EALLOW;	// 修改被保护的寄存器，修改前应添加EALLOW语句
   // init plic default setting
    init_plic();
    // config isr
    IntVectTable.st.XINT1 = xint1_isr;
    // config isr
    IntVectTable.st.XINT2 = xint2_isr;
    // enable interrupt
    enable_interrupt( GET_INT_NUM(IntVectTable.st.XINT2));
    // enable interrupt
    enable_interrupt( GET_INT_NUM(IntVectTable.st.XINT1));

    // enable trap
 	trap_init();

   EDIS;   // EDIS的意思是不允许修改被保护的寄存器

// 步骤 4. 初始化片内外设:
// 这个函数可以在DSP280x_CpuTimers.c源文件中找到
// InitPeripherals(); // 本例中不需要

// 步骤 5. 用户特定的代码来允许需要的中断

// 清除计数器
   Xint1Count = 0; // 计数外部中断1（XINT1）
   Xint2Count = 0; // 计数外部中断2（XINT2）

// 在外设中断扩展模块中断使能寄存器中允许XINT1和XINT2: （组1中断4 & 5）
// 使能CPU中断1（INT1）:
   /*PieCtrlRegs.PIECTRL.bit.ENPIE = 1;*/          // 使能外设中断扩展PIE模块
   /*PieCtrlRegs.PIEIER1.bit.INTx4 = 1;*/          // 使能PIE组1的INT4
   /*PieCtrlRegs.PIEIER1.bit.INTx5 = 1;*/          // 使能PIE组1的INT5
   /*IER |= M_INT1;*/                              // 使能CPU中断1（INT1）
   /*EINT;*/                                       // 开全局中断

// GPIO60-61,GPIO64-68 功能选择为通用I/O口，并输出高电；
   EALLOW;
   GpioDataRegs.GPBSET.bit.GPIO60 = 1;         // 输出高电平
   GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;        // 选择为通用I/O口
   GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;         // 方向定义为输出

   GpioDataRegs.GPBSET.bit.GPIO61 = 1;         // 输出高电平
   GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;        // 选择为通用I/O口
   GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;         // 方向定义为输出

   GpioDataRegs.GPCSET.all = 0x001F;          // GPIO64-66输出高电平
   GpioCtrlRegs.GPCMUX1.all = 0x0000;        // GPIO64-68选择为通用I/O口
   GpioCtrlRegs.GPCDIR.all = 0x0001F;         // GPIO64-68方向定义为输出
   EDIS;

// GPIO12和GPIO13配置为输入引脚通用I/O口
   EALLOW;
   GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;         // 选择为通用I/O口
   GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;          // 方向定义为输入
   GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 0;        // 外部中断1（XINT1）与系统时钟SYSCLKOUT同步

   GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;         // 选择为通用I/O口
   GpioCtrlRegs.GPADIR.bit.GPIO13 = 0;          // 方向定义为输入
   GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 2;        // 外部中断2（XINT2）输入限定6个采样窗口
   GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = 0xFF;   // 每个采样窗口的周期为510*SYSCLKOUT
   EDIS;

// 通过GPIO外部中断选择寄存器，选择GPIO12为外部中断1，选择GPIO13为外部中断2
   EALLOW;
   GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 0x0C;   // XINT1是GPIO12
   GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 0x0D;   // XINT2是GPIO13
   EDIS;

// 配置外部中断1和2的中断控制寄存器
   XIntruptRegs.XINT1CR.bit.POLARITY = 0;      // 下降沿触发中断
   XIntruptRegs.XINT2CR.bit.POLARITY = 0;      // 下降沿触发中断

// 使能外部中断1和外部中断2
   XIntruptRegs.XINT1CR.bit.ENABLE = 1;        // 使能XINT1
   XIntruptRegs.XINT2CR.bit.ENABLE = 1;        // 使能XINT2


// 步骤6. 无限循环:
   for(;;);
	
}

//步骤7.在这里插入中断服务子程序，如果需要使用中断服务子程序，必须在步骤5中重新定义中断
//向量表中对应地地址
 void xint1_isr(void) {
	Uint32 i;
	for(i=0;i<1000000;i++);    //键盘消抖动
	while(keyon1==0);
	GpioDataRegs.GPBTOGGLE.all = 0x30000000;   // GPIO60-GPIO61输出电平取反
    GpioDataRegs.GPCTOGGLE.all = 0x001F;   // GPIO64-GPIO68输出电平取反
	Xint1Count++;

	// 应答寄存器的位1清0，以响应同组内其他中断；
	/*PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;*/
}

 void xint2_isr(void) {
	Uint32 i;
	for(Xint2Loop=1;Xint2Loop<=8;Xint2Loop++)
	{
	  GpioDataRegs.GPBTOGGLE.all = 0x30000000;   // GPIO60-GPIO61输出电平取反
      GpioDataRegs.GPCTOGGLE.all = 0x001F;   // GPIO64-GPIO68输出电平取反
      DELAY_US(100000);  //延时0.1s
    }

	Xint2Count++;
		for(i=0;i<1000000;i++);    //键盘消抖动
	while(keyon2==0);

	// 应答寄存器的位1清0，以响应同组内其他中断；
	/*PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;*/
}


//===========================================================================
// No more.
//===========================================================================
