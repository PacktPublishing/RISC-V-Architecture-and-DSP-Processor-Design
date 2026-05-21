#ifndef RV335_PROJECT_H
#define RV335_PROJECT_H

#include "intexc.h"

//
// For Portability, User Is Recommended To Use Following Data Type Size
// Definitions For 16-bit and 32-Bit Signed/Unsigned Integers:
//
typedef int16_t			int16;
typedef int32_t         int32;
typedef int64_t         int64;
typedef uint16_t       	Uint16;
typedef uint32_t      	Uint32;
typedef uint64_t 		Uint64;
typedef float           float32;
typedef double     		float64;


//
// Defines
//
#define   TARGET      1
#define   CPU_RATE    6.667L   // for a 150MHz CPU clock speed (SYSCLKOUT)
//
// User To Select Target Device
//
#define   DSP_RV335  TARGET    // Selects 'RV335

#define   interrupt __attribute__((interrupt ))

// Common CPU Definitions
#define  EINT   asm("csrsi mstatus, 0x8")
#define  DINT   asm("csrci mstatus, 0x8")
#define  ERTM   asm(" NOP")
#define  DRTM   asm(" NOP")
#define  EALLOW asm(" NOP")
#define  EDIS   asm(" NOP")
#define  ESTOP0 asm("EBREAK")


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


#define SPRINGCORE_2204_EPWM1  1
#define SPRINGCORE_2204_EPWM2  1
#define SPRINGCORE_2204_EPWM3  1
#define SPRINGCORE_2204_EPWM4  1
#define SPRINGCORE_2204_EPWM5  1
#define SPRINGCORE_2204_EPWM6  1
#define SPRINGCORE_2204_ECAP1  1
#define SPRINGCORE_2204_ECAP2  1
#define SPRINGCORE_2204_ECAP3  1
#define SPRINGCORE_2204_ECAP4  1
#define SPRINGCORE_2204_ECAP5  1
#define SPRINGCORE_2204_ECAP6  1
#define SPRINGCORE_2204_EQEP1  1
#define SPRINGCORE_2204_EQEP2  1
#define SPRINGCORE_2204_ECANA  1
#define SPRINGCORE_2204_ECANB  1
#define SPRINGCORE_2204_MCBSPA 1
#define SPRINGCORE_2204_MCBSPB 1
#define SPRINGCORE_2204_SPIA   1
#define SPRINGCORE_2204_SCIA   1
#define SPRINGCORE_2204_SCIB   1
#define SPRINGCORE_2204_SCIC   1
#define SPRINGCORE_2204_I2CA   1


//
// DO NOT MODIFY THIS LINE.
//
#define DELAY_US(A)  RV335_usDelay(((((long double) A * 1000.0L) / \
        (long double)CPU_RATE) - 9.0L) / 5.0L)

#endif  // end of RV335_PROJECT_H definition

