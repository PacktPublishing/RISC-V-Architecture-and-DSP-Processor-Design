#ifndef RV335_DEVICE_H
#define RV335_DEVICE_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// Defines
//
#define   TARGET   1

//
// User To Select Target Device
//
#define   DSP28_28335   TARGET    // Selects '28335/'28235
#define   DSP28_28334   0         // Selects '28334/'28234
#define   DSP28_28333   0         // Selects '28333/'
#define   DSP28_28332   0         // Selects '28332/'28232


// add for rv
#define interrupt __attribute__((interrupt ))
//#define cregister __attribute__((cregister ))

// add for rv end

//
// Common CPU Definitions
//

// change for rv
//extern cregister volatile unsigned int IFR;
//extern cregister volatile unsigned int IER;
// change for rv end

// change for rv

//#define  EINT   asm(" clrc INTM")    // Enable Maskable Interrupts (Clear INTM Bit)
//#define  DINT   asm(" setc INTM")    // Disable Maskable Interrupts (Set INTM Bit)
//#define  ERTM   asm(" clrc DBGM")    // enable debug events
//#define  DRTM   asm(" setc DBGM")    // disable debug events
//#define  EALLOW asm(" EALLOW")    // Enable Write Access to Protected Space
//#define  EDIS   asm(" EDIS")    // Disable Write Access to Protected Registers
//#define  ESTOP0 asm(" ESTOP0")    // perform like a software breakpoint when connect emulator

#define  EINT   asm("csrsi mstatus, 0x8")
#define  DINT   asm("csrci mstatus, 0x8")
#define  ERTM   asm(" NOP")
#define  DRTM   asm(" NOP")
#define  EALLOW asm(" NOP")  // ok
#define  EDIS   asm(" NOP")  // ok
#define  ESTOP0 asm("EBREAK")

// change for rv end

#define M_INT1  0x0001
#define M_INT2  0x0002
#define M_INT3  0x0004
#define M_INT4  0x0008
#define M_INT5  0x0010
#define M_INT6  0x0020
#define M_INT7  0x0040
#define M_INT8  0x0080
#define M_INT9  0x0100
#define M_INT10 0x0200
#define M_INT11 0x0400
#define M_INT12 0x0800
#define M_INT13 0x1000
#define M_INT14 0x2000
#define M_DLOG  0x4000
#define M_RTOS  0x8000

#define BIT0    0x0001
#define BIT1    0x0002
#define BIT2    0x0004
#define BIT3    0x0008
#define BIT4    0x0010
#define BIT5    0x0020
#define BIT6    0x0040
#define BIT7    0x0080
#define BIT8    0x0100
#define BIT9    0x0200
#define BIT10   0x0400
#define BIT11   0x0800
#define BIT12   0x1000
#define BIT13   0x2000
#define BIT14   0x4000
#define BIT15   0x8000

//
// For Portability, User Is Recommended To Use Following Data Type Size
// Definitions For 16-bit and 32-Bit Signed/Unsigned Integers:
//
#ifndef DSP28_DATA_TYPES
#define DSP28_DATA_TYPES
//typedef int                int16;
//typedef long               int32;
//typedef long long          int64;
//typedef unsigned int       Uint16;
//typedef unsigned long      Uint32;
//typedef unsigned long long Uint64;
//typedef float              float32;
//typedef long double        float64;
typedef int16_t			int16;
typedef int32_t         int32;
typedef int64_t         int64;
typedef uint16_t       	Uint16;
typedef uint32_t      	Uint32;
typedef uint64_t 		Uint64;
typedef float           float32;
typedef double     		float64;
#endif

//
// Included Peripheral Header Files
//
#include "RV335_Adc.h"                // ADC Registers
#include "RV335_DevEmu.h"             // Device Emulation Registers
#include "RV335_CpuTimers.h"          // 32-bit CPU Timers
#include "RV335_ECan.h"               // Enhanced eCAN Registers
#include "RV335_ECap.h"               // Enhanced Capture
#include "RV335_DMA.h"                // DMA Registers
#include "RV335_EPwm.h"               // Enhanced PWM
#include "RV335_EQep.h"               // Enhanced QEP
#include "RV335_Gpio.h"               // General Purpose I/O Registers
#include "RV335_I2c.h"                // I2C Registers
#include "RV335_Mcbsp.h"              // McBSP
#include "RV335_PieCtrl.h"            // PIE Control Registers
#include "RV335_PieVect.h"            // PIE Vector Table
#include "RV335_Spi.h"                // SPI Registers
#include "RV335_Sci.h"                // SCI Registers
#include "RV335_SysCtrl.h"            // System Control/Power Modes
#include "RV335_XIntrupt.h"           // External Interrupts
#include "RV335_Xintf.h"              // XINTF External Interface

#if DSP28_28335 || DSP28_28333
#define DSP28_EPWM1  1
#define DSP28_EPWM2  1
#define DSP28_EPWM3  1
#define DSP28_EPWM4  1
#define DSP28_EPWM5  1
#define DSP28_EPWM6  1
#define DSP28_ECAP1  1
#define DSP28_ECAP2  1
#define DSP28_ECAP3  1
#define DSP28_ECAP4  1
#define DSP28_ECAP5  1
#define DSP28_ECAP6  1
#define DSP28_EQEP1  1
#define DSP28_EQEP2  1
#define DSP28_ECANA  1
#define DSP28_ECANB  1
#define DSP28_MCBSPA 1
#define DSP28_MCBSPB 1
#define DSP28_SPIA   1
#define DSP28_SCIA   1
#define DSP28_SCIB   1
#define DSP28_SCIC   1
#define DSP28_I2CA   1
#endif  // end DSP28_28335 || DSP28_28333

#if DSP28_28334
#define DSP28_EPWM1  1
#define DSP28_EPWM2  1
#define DSP28_EPWM3  1
#define DSP28_EPWM4  1
#define DSP28_EPWM5  1
#define DSP28_EPWM6  1
#define DSP28_ECAP1  1
#define DSP28_ECAP2  1
#define DSP28_ECAP3  1
#define DSP28_ECAP4  1
#define DSP28_ECAP5  0
#define DSP28_ECAP6  0
#define DSP28_EQEP1  1
#define DSP28_EQEP2  1
#define DSP28_ECANA  1
#define DSP28_ECANB  1
#define DSP28_MCBSPA 1
#define DSP28_MCBSPB 1
#define DSP28_SPIA   1
#define DSP28_SCIA   1
#define DSP28_SCIB   1
#define DSP28_SCIC   1
#define DSP28_I2CA   1
#endif  // end DSP28_28334

#if DSP28_28332
#define DSP28_EPWM1  1
#define DSP28_EPWM2  1
#define DSP28_EPWM3  1
#define DSP28_EPWM4  1
#define DSP28_EPWM5  1
#define DSP28_EPWM6  1
#define DSP28_ECAP1  1
#define DSP28_ECAP2  1
#define DSP28_ECAP3  1
#define DSP28_ECAP4  1
#define DSP28_ECAP5  0
#define DSP28_ECAP6  0
#define DSP28_EQEP1  1
#define DSP28_EQEP2  1
#define DSP28_ECANA  1
#define DSP28_ECANB  1
#define DSP28_MCBSPA 1
#define DSP28_MCBSPB 0
#define DSP28_SPIA   1
#define DSP28_SCIA   1
#define DSP28_SCIB   1
#define DSP28_SCIC   0
#define DSP28_I2CA   1
#endif  // end DSP28_28332

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of RV335_DEVICE_H definition

//
// End of file
//

