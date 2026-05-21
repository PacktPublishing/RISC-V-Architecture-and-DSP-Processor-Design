#ifndef RV335_DEFAULT_ISR_H
#define RV335_DEFAULT_ISR_H

#ifdef __cplusplus
extern "C" {
#endif

//
// Default Interrupt Service Routine Declarations:
// 
//
// Non-Peripheral Interrupts
//
interrupt void INT13_ISR(void);     // XINT13 or CPU-Timer 1
interrupt void INT14_ISR(void);     // CPU-Timer2
interrupt void DATALOG_ISR(void);   // Datalogging interrupt
interrupt void RTOSINT_ISR(void);   // RTOS interrupt
interrupt void EMUINT_ISR(void);    // Emulation interrupt
interrupt void NMI_ISR(void);       // Non-maskable interrupt
interrupt void ILLEGAL_ISR(void);   // Illegal operation TRAP
interrupt void USER1_ISR(void);     // User Defined trap 1
interrupt void USER2_ISR(void);     // User Defined trap 2
interrupt void USER3_ISR(void);     // User Defined trap 3
interrupt void USER4_ISR(void);     // User Defined trap 4
interrupt void USER5_ISR(void);     // User Defined trap 5
interrupt void USER6_ISR(void);     // User Defined trap 6
interrupt void USER7_ISR(void);     // User Defined trap 7
interrupt void USER8_ISR(void);     // User Defined trap 8
interrupt void USER9_ISR(void);     // User Defined trap 9
interrupt void USER10_ISR(void);    // User Defined trap 10
interrupt void USER11_ISR(void);    // User Defined trap 11
interrupt void USER12_ISR(void);    // User Defined trap 12


//
// Catch-all for Reserved Locations For testing purposes
//
interrupt void rsvd_ISR(void);           // for test
interrupt void INT_NOTUSED_ISR(void);    // for unused interrupts

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif    // end of RV335_DEFAULT_ISR_H definition

//
// End of file
//

