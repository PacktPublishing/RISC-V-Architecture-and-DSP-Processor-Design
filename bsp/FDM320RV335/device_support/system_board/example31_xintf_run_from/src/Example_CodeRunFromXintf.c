//###########################################################################
//
// 文件名:    Example_2833xCodeRunFromXintf.c
//
// 标题:   代码在片外SRAM运行的测试
//
// ASSUMPTIONS:
//
//     此程序需要 RV335头文件.
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
//          本例配置CPU定时器0，通过定时中断每次增加相应计数器
//          代码下载到SARAM中，外部存储区域7被配置为16位数据总线，
//          一部分代码被拷贝到外部存储器中执行
//
//       观察以下变量
//          CpuTimer0.InterruptCount
//          CpuTimer1.InterruptCount
//          CpuTimer2.InterruptCount
//###########################################################################
// 释放日期: 13.9.26
//###########################################################################

#include <string.h>
#include "../include/RV335_Project.h"
#include "../routines/include/RV335_Device.h"

#include "../include/RV335_SysCtrl.h"

// 这两个函数将被加载到SARAM中，并被复制到外部存储区域7中执行
void init_zone7(void);
void cpu_timer0_isr(void) __attribute__ ((section("xintffuncs")));
void cpu_timer1_isr(void) __attribute__ ((section("xintffuncs")));
void cpu_timer2_isr(void) __attribute__ ((section("xintffuncs")));

// 使用前，声明本文件中的相关函数；
void init_zone7(void);
 void cpu_timer0_isr(void);
 void cpu_timer1_isr(void);
 void cpu_timer2_isr(void);

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


   // 步骤 3. 清除所有中断，初始化中断向量表:
   // 禁止CPU全局中断
   /*DINT;*/

   // 初始化PIE控制寄存器到他们的默认状态.
   // 这个默认状态就是禁止PIE中断及清除所有PIE中断标志
   // 这个函数放在RV335_PieCtrl.c源文件里
   /*InitPieCtrl();*/

   // 禁止CPU中断和清除所有CPU中断标志
   /*IER = 0x0000;*/
   /*IFR = 0x0000;*/

   // 初始化XINTF区域7
      init_zone7();

  //
  // Copy non-time critical code to XINTF
  // This includes the following ISR functions: cpu_timer0_isr(),
  // cpu_timer1_isr(). The  _xintffuncs_load_start, _xintffuncs_run_start,
  // and _xintffuncs_run_end symbols are created by the linker.
  extern char _xintffuncs_load_start;
  void* p_xintffuncs_load_start = &_xintffuncs_load_start;
  extern char _xintffuncs_run_start;
  void* p_xintffuncs_run_start = &_xintffuncs_run_start;
  extern char _xintffuncs_run_end;
  void* p_xintffuncs_run_end = &_xintffuncs_run_end;

  size_t xintffuncs_size = p_xintffuncs_run_end - p_xintffuncs_run_start;
  memcpy((void *)p_xintffuncs_run_start, (void *)p_xintffuncs_load_start, xintffuncs_size);


   //  本例中用到的中断重新映射到本件的中断服务子程序；
   EALLOW;  // 修改被保护的寄存器，修改前应添加EALLOW语句
   // init plic default setting
    init_plic();
    // config isr
    IntVectTable.st.TINT0 = cpu_timer0_isr;
    // enable interrupt


    // config isr
    IntVectTable.st.TIMER1 = cpu_timer1_isr;

    // enable interrupt

   // init plic default setting

    IntVectTable.st.TIME2 = cpu_timer2_isr;


    // enable trap
    trap_init();
   EDIS;    // EDIS的意思是不允许修改被保护的寄存器

   // 步骤 4. 初始化片内外设:
//         本函数可以在 RV335_CpuTimers.c源文件中找到
   InitCpuTimers();   // 本例仅初始化CPU定时器

// 配置CPU 0,1,2定时器，每秒中断一次:
// 100MHz CPU 频率, 1秒周期(单位是微秒)

   ConfigCpuTimer(&CpuTimer0, 100, 1000000);
   ConfigCpuTimer(&CpuTimer1, 100, 1000000);
   ConfigCpuTimer(&CpuTimer2, 100, 1000000);


   CpuTimer0Regs.TCR.all = 0x4001; // 该指令设置TSS位 = 0，则启动定时器
   CpuTimer1Regs.TCR.all = 0x4001; // 该指令设置TSS位 = 0，则启动定时器
   CpuTimer2Regs.TCR.all = 0x4001; // 该指令设置TSS位 = 0，则启动定时器

   enable_interrupt( GET_INT_NUM(IntVectTable.st.TINT0));
   enable_interrupt( GET_INT_NUM(IntVectTable.st.TIMER1));
   enable_interrupt( GET_INT_NUM(IntVectTable.st.TIME2));



// 使能CPU中断1、13、14，它们分别连接到CPU定时器0、1、2

   /*IER |= M_INT1;*/
   /*IER |= M_INT13;*/
   /*IER |= M_INT14;*/

// 在PIE中使能定时器0：组1中断7
   /*PieCtrlRegs.PIEIER1.bit.INTx7 = 1;*/

// 使能全局中断和更高优先级的实时调试事件
   /*EINT;*/   // 使能全局中断 INTM
   ERTM;   //  使能全局实时中断DBGM

// Step 6. 进入无限循环
   for(;;);

}


 void cpu_timer0_isr(void) {
   CpuTimer0.InterruptCount++;
   // 对相应组的应答寄存器清零以便相应改组的更多中断；
   /*PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;*/
}

 void cpu_timer1_isr(void) {
   CpuTimer1.InterruptCount++;
   // The CPU acknowledges the interrupt.
   EDIS;
}

 void cpu_timer2_isr(void) {
   EALLOW;
   CpuTimer2.InterruptCount++;
   // The CPU acknowledges the interrupt.
   EDIS;
}

// 对外部存储区域7进行实时配置参数
// 注意
//    该函数不能在外部存储器中执行
//    依据数据手册和片外RAM需求调整定时
void init_zone7(void)
{

    // 确保外部存储器时钟被使能
	SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1;

	// 配置GPIO作为外部存储器16位的数据总线
	// 该函数在RV335_Xintf.c中
	InitXintf16Gpio();

    // 所有的外部区域---------------------------------
    // 设置所有外部区域 XTIMCLK = SYSCLKOUT
    XintfRegs.XINTCNF2.bit.XTIMCLK = 0;
    // 3个写缓冲
    XintfRegs.XINTCNF2.bit.WRBUFF = 3;
    // XCLKOUT被使能
    XintfRegs.XINTCNF2.bit.CLKOFF = 0;
    // XCLKOUT = XTIMCLK 
    XintfRegs.XINTCNF2.bit.CLKMODE = 0;   

    // 区域7------------------------------------
    // When using ready, ACTIVE must be 1 or greater
    // Lead must always be 1 or greater
    // 区域的写周期
    XintfRegs.XTIMING7.bit.XWRLEAD = 1;//写建立等待状态周期=1XTIMCLK
    XintfRegs.XTIMING7.bit.XWRACTIVE = 2;//写激活等待周期=2XTIMCLK
    XintfRegs.XTIMING7.bit.XWRTRAIL = 1;//写追踪等待周期=1XTIMCLK
    // 区域的读周期
    XintfRegs.XTIMING7.bit.XRDLEAD = 1;//读建立等待状态周期=1XTIMCLK
    XintfRegs.XTIMING7.bit.XRDACTIVE = 3;//读激活等待周期=3XTIMCLK
    XintfRegs.XTIMING7.bit.XRDTRAIL = 0;//读追踪等待周期=0XTIMCLK
    
    // 不设置读写的建立/激活/追踪周期为原来的2倍；
    XintfRegs.XTIMING7.bit.X2TIMING = 0;
 
    // 区域不采样XREADY信号
    XintfRegs.XTIMING7.bit.USEREADY = 0;
    XintfRegs.XTIMING7.bit.READYMODE = 0;
 
    // 1,1 = x16 数据线（采用）
    // 0,1 = x32 数据线
    XintfRegs.XTIMING7.bit.XSIZE = 3;
    
   //占用N+1个指令周期
   __asm__(			
    	" NOP\n"		
    	" NOP\n"		
    	" NOP\n"		
    	" NOP\n"		
    	" NOP\n"		
    	" NOP\n"		
    	" NOP\n"		
    	" NOP\n"		
    			
    );			 
}

//===========================================================================
// No more.
//===========================================================================
