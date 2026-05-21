//###########################################################################
//
// 文件名:    Example_Scia_FFDLB.c
//
// 标题:   RV335片内SCI的FIFO数字回扫测试
//
// ASSUMPTIONS:
//
//    此程序需要 RV335头文件.
//
//    此程序使用外设的内部回扫测试模式。
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
//    本测试使用SCI模块的回扫测试模式来发送参数：从0x00到0xFF，测试中将
//    发送一个参数并检查接受缓存器中是否接受到该参数。
//
//          观察变量:
//                LoopCount     已发送参数的数量
//                ErrorCount    被检测到的错误数量
//                SendChar       发送的参数
//                ReceivedChar   接受到的参数
//###########################################################################
// 释放日期：2013.11.26
//###########################################################################

#include "RV335_Device.h"
#include "RV335_Examples.h"

// Prototype statements for functions found within this file.
void scia_loopback_init(void);
void scia_fifo_init(void);	
void scia_xmit(int a);
void error(int);
 void scia_rx_isr(void);
 void scia_tx_isr(void);

// Global counts used in this example
Uint16 LoopCount;
Uint16 ErrorCount; 

void main(void)
{
    Uint16 SendChar;
    Uint16 ReceivedChar;
    
// Step 1. Initialize System Control registers, PLL, WatchDog, Clocks to default state:
// This function is found in the DSP280x_SysCtrl.c file.
	InitSysCtrl();

// Step 2. Select GPIO for the device or for the specific application:
// This function is found in the DSP280x_Gpio.c file.
// InitGpio(); skip this as this is example selects the I/O
// for SCI-A in this file itself
   InitSciGpio();


	
// Step 5. User specific functions, Reassign vectors (optional), Enable Interrupts:

    LoopCount = 0;
    ErrorCount = 0;
    
    scia_fifo_init();	   // Initialize the SCI FIFO
    scia_loopback_init();  // Initalize SCI for digital loop back 

    // Note: Autobaud lock is not required for this example
    
    // Send a character starting with 0 
    SendChar = 0;								

// Step 6. Send Characters forever starting with 0x00 and going through
// 0xFF.  After sending each, check the recieve buffer for the correct value

	for(;;)
    { 
       scia_xmit(SendChar);
       while(SciaRegs.SCIFFRX.bit.RXFFST !=1) { } // wait for XRDY =1 for empty state
  
       // Check received character
       ReceivedChar = SciaRegs.SCIRXBUF.all;			
       if(ReceivedChar != SendChar) error(1);

       // Move to the next character and repeat the test
       SendChar++;
       // Limit the character to 8-bits
       SendChar &= 0x00FF;
       LoopCount++;
    }

} 	


// Step 7. Insert all local Interrupt Service Routines (ISRs) and functions here:	

void error(int ErrorFlag)
{
      Uint16 Error = ErrorFlag;
      ErrorCount++;
//    asm("     ESTOP0");  // Uncomment to stop the test here
//    for (;;);

}

// Test 1,SCIA  DLB, 8-bit word, baud rate 0x000F, default, 1 STOP bit, no parity 
void scia_loopback_init()
{    
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function
    
 	SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback 
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
	SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK, 
                                   // Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCICTL2.all =0x0003; 
	SciaRegs.SCICTL2.bit.TXINTENA =1;
	SciaRegs.SCICTL2.bit.RXBKINTENA =1;
    SciaRegs.SCIHBAUD    =0x0000;
    SciaRegs.SCILBAUD    =0x000F;
	SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back  
	SciaRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset 
}

// Transmit a character from the SCI'
void scia_xmit(int a)
{
    SciaRegs.SCITXBUF=a;
}    

// Initalize the SCI FIFO
void scia_fifo_init()										
{
    SciaRegs.SCIFFTX.all=0xE040;
    SciaRegs.SCIFFRX.all=0x204f;
    SciaRegs.SCIFFCT.all=0x0;
    
}  

                							
 
    


//===========================================================================
// No more.
//===========================================================================

