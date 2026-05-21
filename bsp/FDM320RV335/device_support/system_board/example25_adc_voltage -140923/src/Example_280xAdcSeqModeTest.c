//###########################################################################
//
// FILE:   Example_280xAdcSeqModeTest.c
//
// TITLE:  电压表采样实验
//
// ASSUMPTIONS:
//
//    此程序需要 DSP2833x头文件.
//
//    编译此例程前，在DSP2833x_Examples.h文件中确保CPU时钟速
//    度被合适地定义
//
//    连接信号到转换通道A0
//
//    根据在RAM中调试的需要，这个项目配置成"boot to SARAM".2833x引导模式
//    表如下显示. 常用的还有"boot to Flash"模式，当程序在RAM调试完善后就
//    可以将代码烧进Flash中并使用"boot to Flash"引导模式. 
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
//
// 功能描述:
//
//    通过ADC通道A0采集模拟电压值，然后通过SPI传输给数码管
//    通过数码管显示电压值
//


#include "RV335_Device.h"     // DSP2833x Headerfile Include File
#include "RV335_Examples.h"   // DSP2833x Examples Include File

/********************************宏定义数码管位选 IO 接口*******************************************/
#define  SET_BIT4	GpioDataRegs.GPCSET.bit.GPIO70	 = 1 		//与外设板 8_LEDS 端子的 IO70 对应
#define  RST_BIT4	GpioDataRegs.GPCCLEAR.bit.GPIO70 = 1		//与外设板 8_LEDS 端子的 IO70 对应
#define  SET_BIT3   GpioDataRegs.GPCSET.bit.GPIO71	 = 1		//与外设板 8_LEDS 端子的 IO71 对应
#define  RST_BIT3	GpioDataRegs.GPCCLEAR.bit.GPIO71 = 1		//与外设板 8_LEDS 端子的 IO71 对应
#define  SET_BIT2   GpioDataRegs.GPCSET.bit.GPIO72	 = 1		//与外设板 8_LEDS 端子的 IO72 对应
#define  RST_BIT2	GpioDataRegs.GPCCLEAR.bit.GPIO72 = 1		//与外设板 8_LEDS 端子的 IO72 对应
#define  SET_BIT1   GpioDataRegs.GPCSET.bit.GPIO73	 = 1		//与外设板 8_LEDS 端子的 IO73 对应
#define  RST_BIT1	GpioDataRegs.GPCCLEAR.bit.GPIO73 = 1		//与外设板 8_LEDS 端子的 IO73 对应
/*****************************************************************************************************/

/*********************************************函数申明************************************************/
void delay(Uint32 t);
void DisData_Trans(Uint16 data);
void Sellect_Bit(Uint16 i);
void Init_LEDS_Gpio(void);

//void delay(Uint32 t);
/*****************************************************************************************************/

/************************************定义相关变量*********************************************/
unsigned char msg[10]={0xC0,0xf9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};	//段码：0~9
unsigned char DisData_Bit[4] = {0};											//存放拆分后的四位数字
Uint16 DisData = 0;															//显示的数字
Uint16 Loop = 0;

/****************函数声明*******************/ 
void delay_loop(void);
void spi_xmit(Uint16 a);
void spi_fifo_init(void);
void spi_init(void);
//void error(void);
//extern void CharDisplay(Uint16 s, Uint16 LedBuffer[]);

/***************全局变量定义****************/
Uint16 LedBuffer[2];
Uint16 showdata;

// ADC start parameters
#define ADC_MODCLK 0x5   // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 150/(2*4)             = 15MHz
                         //    for 60 MHz devices:    HSPCLK =  60/(2*4)             = 7.5 MHz
#define ADC_CKPS   0x1   // ADC module clock = HSPCLK/2*ADC_CKPS   = 15MHz/(1*2)   = 7.5MHz
                         
#define ADC_SHCLK  0xf   // S/H width in ADC module periods                          = 16 ADC clocks
#define AVG        100  // Average sample limit
#define ZOFFSET    0x00  // Average Zero offset
#define BUF_SIZE   2048  // Sample buffer size

// Global variable for this example
Uint16 SampleTable[BUF_SIZE];

main()
{
   Uint16 i;
   Uint32 Sum=0;
   Uint32 Vin;
   Uint16 sdata;  // 发送数据

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP280x_SysCtrl.c file.
   InitSysCtrl();

// Specific clock setting for this example:
   /*EALLOW;*/
   SysCtrlRegs.HISPCP.all = ADC_MODCLK;	// HSPCLK = SYSCLKOUT/（2*ADC_MODCLK）=15MHZ
   /*EDIS;*/

// Step 2. Initialize GPIO:
// This example function is found in the DSP280x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example

// 次函数在DSP280x_Spi.c文件里面
   InitSpiaGpio();  //开发板使用SPIa
   Init_LEDS_Gpio();

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   /*DINT;*/

// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the DSP280x_PieCtrl.c file.
   /*InitPieCtrl();*/

// Disable CPU interrupts and clear all CPU interrupt flags:
   /*IER = 0x0000;*/
   /*IFR = 0x0000;*/

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP280x_DefaultIsr.c.
// This function is found in DSP280x_PieVect.c.
   /*InitPieVectTable();*/

// 步骤 4.初始化片内外设:
   InitAdc();  // For this example, init the ADC
   
   spi_fifo_init();	  // 初始化Spi的FIFO
   spi_init();		  // 初始化SPI

// Specific ADC setup for this example:
   AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;//设置采样窗口时间：（15+1）*ADCCLK
   AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;//ADC内核时钟分频：HSPCLK/2=6.25MHZ
   AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;        // 1选择级联模式
   AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;  //通道选择ADCAIN0
   AdcRegs.ADCTRL1.bit.CONT_RUN = 1;       // 设置为连续运行


// Step 5.用户指定代码，使能中断:


// 采样表清0
   for (i=0; i<BUF_SIZE; i++)
   {
     SampleTable[i] = 0;
   }
   //关数码管；
   spi_xmit(0xffff);
     //延迟
      delay_loop();

   // 软件启动SEQ1
   AdcRegs.ADCTRL2.all = 0x2000;

   // 取ADC数据并写入采样数据表
   for(;;)
   {
     for (i=0; i<AVG; i++)
     {
        while (AdcRegs.ADCST.bit.INT_SEQ1== 0) {} // 等待中断
        AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
        SampleTable[i] =((AdcRegs.ADCRESULT0>>4) );
     }
	 for (i=0;i<AVG;i++)
	 {
      Sum+=SampleTable[i];
	  Sum=Sum/2;
	 }
	 //输入电压和AD值之间的关系Vin/Sum=3/4096；
	 Vin=Sum*3*10000/4096;      //将输入电压放大100倍，以便于第2位有效小数的四舍五入计算；
	 if(Vin%10>=5)//最后一位整数>=5时，要五入；
	 showdata=Vin/10+1;
	 else
	 showdata=Vin/10;//要四舍；

	 for(i=0;i<100;i++)
	 	{

	 		DisData_Trans(showdata);									//拆分四位数
	 		for(Loop=0;Loop<4;Loop++)								//分别显示四位
	 		{
	 			Sellect_Bit(Loop);	                                //选择要扫描的数码管位
	 			if(Loop==3)
	 				spi_xmit(msg[DisData_Bit[Loop]]+0x80);
	 			else
	 			spi_xmit(msg[DisData_Bit[Loop]]);					//串行输出要显示的数字
	 			delay(2500000);										//延时配合人眼反应时间
	 		}


	 	}


   }
}

//延迟函数	

void delay_loop()
{
    long      i;
    for (i = 0; i < 4500000; i++) {} 
}


//初始化SPI函数
void spi_init()
{    
	SpiaRegs.SPICCR.all =0x004F;// SPI软件复位, 极性位为1（下降沿发送数据）, 每次移
	                            //进和移出16位字长度；禁止SPI内部回送（LOOKBACK）功能；
	SpiaRegs.SPICTL.all =0x0006; // 使能主机模式，正常相位，使能主机发送，禁止接收
	                            //溢出中断，禁止SPI中断；
                                   
	SpiaRegs.SPIBRR =0x007F;	//SPI波特率=25M/128	=195.3KHZ；							
    SpiaRegs.SPICCR.all =0x00CF;//停止SPI软件复位准备接收或发送；禁止回送模式； 
    SpiaRegs.SPIPRI.bit.FREE = 1;  // 自由运行     
}
//发送SPI数据
void spi_xmit(Uint16 a)
{
    SpiaRegs.SPITXBUF=a;
}    
//初始化SPI FIFO
void spi_fifo_init()										
{
    SpiaRegs.SPIFFTX.all=0xE040;//使能FIFO;清除发送中断标志位；禁止FIFO发送中断；
                                //发送中断级别定义为0；
    SpiaRegs.SPIFFRX.all=0x204f;//清除FF溢出标志位；清除溢出接受中断标志位；禁止
	                            //FF接受中断；接受中断级别为16；
    SpiaRegs.SPIFFCT.all=0x0;//SPITXBUF到移位寄存器传送不延迟；
}  
/******************************数码管位选 IO 接口初始化*******************************************/

void Init_LEDS_Gpio(void)
{
    /*EALLOW;*/


	GpioCtrlRegs.GPCPUD.bit.GPIO70= 0;   					// Enable pullup on GPIO11
    GpioDataRegs.GPCSET.bit.GPIO70 = 1;   					// Load output latch
    GpioCtrlRegs.GPCMUX1.bit.GPIO70 = 0;  					// GPIO16 = GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO70 = 1;   					// GPIO16 = output

	GpioCtrlRegs.GPCPUD.bit.GPIO71= 0;   					// Enable pullup on GPIO11
    GpioDataRegs.GPCSET.bit.GPIO71 = 1;   					// Load output latch
    GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 0;  					// GPIO16 = GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO71 = 1;   					// GPIO16 = output

	GpioCtrlRegs.GPCPUD.bit.GPIO72 = 0;   					// Enable pullup on GPIO11
    GpioDataRegs.GPCSET.bit.GPIO72 = 1;   					// Load output latch
    GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 0;  					// GPIO17 = GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO72 = 1;   					// GPIO17 = output

	GpioCtrlRegs.GPCPUD.bit.GPIO73 = 0;   					// Enable pullup on GPIO11
    GpioDataRegs.GPCSET.bit.GPIO73 = 1;   					// Load output latch
    GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 0;  					// GPIO19 = GPIO
    GpioCtrlRegs.GPCDIR.bit.GPIO73 = 1;   					// GPIO19 = output

    /*EDIS;*/

}
/******************************数码管位选函数（从低位到高位扫描）***************************************************/
void Sellect_Bit(Uint16 i)
{
	switch(i)
	{
		case 0:
			RST_BIT4;									//关断数码管第四位
			SET_BIT1;									//选通数码管第一位
			break;

		case 1:
			RST_BIT1;									//关断数码管第一位
			SET_BIT2;									//选通数码管第二位
			break;

		case 2:
			RST_BIT2;									//关断数码管第二位
			SET_BIT3;									//选通数码管第三位
			break;

		case 3:
			RST_BIT3;									//关断数码管第三位
			SET_BIT4;									//选通数码管第四位
			break;

		default:
			break;
	}
}
/*****************************************************************************************************/

/************************** 拆分要显示的四位数保存到数组DisData_Trans【】*****************************/
void DisData_Trans(Uint16 data)
{
	DisData_Bit[3] = data / 1000;						//千位数
	DisData_Bit[2] = data % 1000 / 100 ;				//百位数
	DisData_Bit[1] = data % 100 / 10;					//十位数
	DisData_Bit[0] = data % 10;							//个位数
}
/*****************************************************************************************************/
/*********************************************延时函数************************************************/
void delay(Uint32 t)
{
	Uint32 i = 0;
	for (i = 0; i < t; i++);
}
/*****************************************************************************************************/

//===========================================================================
// No more.
//===========================================================================

