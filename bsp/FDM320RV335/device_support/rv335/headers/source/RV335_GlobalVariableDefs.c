//###########################################################################
//
// FILE:   RV335_GlobalVariableDefs.c
//
// TITLE:   
//
//###########################################################################
// $FDMTEK Release: $
// $Release Date:  $
// $Copyright:
//  (C) Copyright 2023 QingDao Fundamental Microelectronic .Ltd - http://www.fdmtek.com
//###########################################################################

//
// Included Files
//
#include "RV335_Project.h"
#include "RV335_GlobalVariableDefs.h"     // RV335 Headerfile Include File"

//
// Define Global Peripheral Variables
//
//#ifdef __cplusplus
//#pragma DATA_SECTION("AdcRegsFile")
//#else
//#pragma DATA_SECTION(AdcRegs,"AdcRegsFile");
//#endif
volatile struct ADC_REGS AdcRegs __attribute__ ((section("AdcRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("AdcMirrorFile")
//#else
//#pragma DATA_SECTION(AdcMirror,"AdcMirrorFile");
//#endif
volatile struct ADC_RESULT_MIRROR_REGS AdcMirror __attribute__ ((section("AdcMirrorFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("CpuTimer0RegsFile")
//#else
//#pragma DATA_SECTION(CpuTimer0Regs,"CpuTimer0RegsFile");
//#endif
volatile struct CPUTIMER_REGS CpuTimer0Regs __attribute__ ((section("CpuTimer0RegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("CpuTimer1RegsFile")
//#else
//#pragma DATA_SECTION(CpuTimer1Regs,"CpuTimer1RegsFile");
//#endif
volatile struct CPUTIMER_REGS CpuTimer1Regs __attribute__ ((section("CpuTimer1RegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("CpuTimer2RegsFile")
//#else
//#pragma DATA_SECTION(CpuTimer2Regs,"CpuTimer2RegsFile");
//#endif
volatile struct CPUTIMER_REGS CpuTimer2Regs __attribute__ ((section("CpuTimer2RegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("CsmPwlFile")
//#else
//#pragma DATA_SECTION(CsmPwl,"CsmPwlFile");
//#endif
volatile struct CSM_PWL CsmPwl __attribute__ ((section("CsmPwlFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("CsmRegsFile")
//#else
//#pragma DATA_SECTION(CsmRegs,"CsmRegsFile");
//#endif
volatile struct CSM_REGS CsmRegs __attribute__ ((section("CsmRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("DevEmuRegsFile")
//#else
//#pragma DATA_SECTION(DevEmuRegs,"DevEmuRegsFile");
//#endif
volatile struct DEV_EMU_REGS DevEmuRegs __attribute__ ((section("DevEmuRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("DmaRegsFile")
//#else
//#pragma DATA_SECTION(DmaRegs,"DmaRegsFile");
//#endif
volatile struct DMA_REGS DmaRegs __attribute__ ((section("DmaRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ECanaRegsFile")
//#else
//#pragma DATA_SECTION(ECanaRegs,"ECanaRegsFile");
//#endif
volatile struct ECAN_REGS ECanaRegs __attribute__ ((section("ECanaRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ECanaMboxesFile")
//#else
//#pragma DATA_SECTION(ECanaMboxes,"ECanaMboxesFile");
//#endif
volatile struct ECAN_MBOXES ECanaMboxes __attribute__ ((section("ECanaMboxesFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ECanaLAMRegsFile")
//#else
//#pragma DATA_SECTION(ECanaLAMRegs,"ECanaLAMRegsFile");
//#endif
volatile struct LAM_REGS ECanaLAMRegs __attribute__ ((section("ECanaLAMRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ECanaMOTSRegsFile")
//#else
//#pragma DATA_SECTION(ECanaMOTSRegs,"ECanaMOTSRegsFile");
//#endif
volatile struct MOTS_REGS ECanaMOTSRegs __attribute__ ((section("ECanaMOTSRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ECanaMOTORegsFile")
//#else
//#pragma DATA_SECTION(ECanaMOTORegs,"ECanaMOTORegsFile");
//#endif
volatile struct MOTO_REGS ECanaMOTORegs __attribute__ ((section("ECanaMOTORegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ECanbRegsFile")
//#else
//#pragma DATA_SECTION(ECanbRegs,"ECanbRegsFile");
//#endif
volatile struct ECAN_REGS ECanbRegs __attribute__ ((section("ECanbRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ECanbMboxesFile")
//#else
//#pragma DATA_SECTION(ECanbMboxes,"ECanbMboxesFile");
//#endif
volatile struct ECAN_MBOXES ECanbMboxes __attribute__ ((section("ECanbMboxesFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ECanbLAMRegsFile")
//#else
//#pragma DATA_SECTION(ECanbLAMRegs,"ECanbLAMRegsFile");
//#endif
volatile struct LAM_REGS ECanbLAMRegs __attribute__ ((section("ECanbLAMRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ECanbMOTSRegsFile")
//#else
//#pragma DATA_SECTION(ECanbMOTSRegs,"ECanbMOTSRegsFile");
//#endif
volatile struct MOTS_REGS ECanbMOTSRegs __attribute__ ((section("ECanbMOTSRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ECanbMOTORegsFile")
//#else
//#pragma DATA_SECTION(ECanbMOTORegs,"ECanbMOTORegsFile");
//#endif
volatile struct MOTO_REGS ECanbMOTORegs __attribute__ ((section("ECanbMOTORegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("EPwm1RegsFile")
//#else
//#pragma DATA_SECTION(EPwm1Regs,"EPwm1RegsFile");
//#endif
volatile struct EPWM_REGS EPwm1Regs __attribute__ ((section("EPwm1RegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("EPwm2RegsFile")
//#else
//#pragma DATA_SECTION(EPwm2Regs,"EPwm2RegsFile");
//#endif
volatile struct EPWM_REGS EPwm2Regs __attribute__ ((section("EPwm2RegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("EPwm3RegsFile")
//#else
//#pragma DATA_SECTION(EPwm3Regs,"EPwm3RegsFile");
//#endif
volatile struct EPWM_REGS EPwm3Regs __attribute__ ((section("EPwm3RegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("EPwm4RegsFile")
//#else
//#pragma DATA_SECTION(EPwm4Regs,"EPwm4RegsFile");
//#endif
volatile struct EPWM_REGS EPwm4Regs __attribute__ ((section("EPwm4RegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("EPwm5RegsFile")
//#else
//#pragma DATA_SECTION(EPwm5Regs,"EPwm5RegsFile");
//#endif
volatile struct EPWM_REGS EPwm5Regs __attribute__ ((section("EPwm5RegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("EPwm6RegsFile")
//#else
//#pragma DATA_SECTION(EPwm6Regs,"EPwm6RegsFile");
//#endif
volatile struct EPWM_REGS EPwm6Regs __attribute__ ((section("EPwm6RegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ECap1RegsFile")
//#else
//#pragma DATA_SECTION(ECap1Regs,"ECap1RegsFile");
//#endif
volatile struct ECAP_REGS ECap1Regs __attribute__ ((section("ECap1RegsFile")));


//#ifdef __cplusplus
//#pragma DATA_SECTION("ECap2RegsFile")
//#else
//#pragma DATA_SECTION(ECap2Regs,"ECap2RegsFile");
//#endif
volatile struct ECAP_REGS ECap2Regs __attribute__ ((section("ECap2RegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ECap3RegsFile")
//#else
//#pragma DATA_SECTION(ECap3Regs,"ECap3RegsFile");
//#endif
volatile struct ECAP_REGS ECap3Regs __attribute__ ((section("ECap3RegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ECap4RegsFile")
//#else
//#pragma DATA_SECTION(ECap4Regs,"ECap4RegsFile");
//#endif
volatile struct ECAP_REGS ECap4Regs __attribute__ ((section("ECap4RegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ECap5RegsFile")
//#else
//#pragma DATA_SECTION(ECap5Regs,"ECap5RegsFile");
//#endif
volatile struct ECAP_REGS ECap5Regs __attribute__ ((section("ECap5RegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ECap6RegsFile")
//#else
//#pragma DATA_SECTION(ECap6Regs,"ECap6RegsFile");
//#endif
volatile struct ECAP_REGS ECap6Regs __attribute__ ((section("ECap6RegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("EQep1RegsFile")
//#else
//#pragma DATA_SECTION(EQep1Regs,"EQep1RegsFile");
//#endif
volatile struct EQEP_REGS EQep1Regs __attribute__ ((section("EQep1RegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("EQep2RegsFile")
//#else
//#pragma DATA_SECTION(EQep2Regs,"EQep2RegsFile");
//#endif
volatile struct EQEP_REGS EQep2Regs __attribute__ ((section("EQep2RegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("GpioCtrlRegsFile")
//#else
//#pragma DATA_SECTION(GpioCtrlRegs,"GpioCtrlRegsFile");
//#endif
volatile struct GPIO_CTRL_REGS GpioCtrlRegs __attribute__ ((section("GpioCtrlRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("GpioDataRegsFile")
//#else
//#pragma DATA_SECTION(GpioDataRegs,"GpioDataRegsFile");
//#endif
volatile struct GPIO_DATA_REGS GpioDataRegs __attribute__ ((section("GpioDataRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("GpioIntRegsFile")
//#else
//#pragma DATA_SECTION(GpioIntRegs,"GpioIntRegsFile");
//#endif
volatile struct GPIO_INT_REGS GpioIntRegs __attribute__ ((section("GpioIntRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("I2caRegsFile")
//#else
//#pragma DATA_SECTION(I2caRegs,"I2caRegsFile");
//#endif
volatile struct I2C_REGS I2caRegs __attribute__ ((section("I2caRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("McbspaRegsFile")
//#else
//#pragma DATA_SECTION(McbspaRegs,"McbspaRegsFile");
//#endif
volatile struct MCBSP_REGS McbspaRegs __attribute__ ((section("McbspaRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("McbspbRegsFile")
//#else
//#pragma DATA_SECTION(McbspbRegs,"McbspbRegsFile");
//#endif
volatile struct MCBSP_REGS McbspbRegs __attribute__ ((section("McbspbRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("PartIdRegsFile")
//#else
//#pragma DATA_SECTION(PartIdRegs,"PartIdRegsFile");
//#endif
volatile struct PARTID_REGS PartIdRegs __attribute__ ((section("PartIdRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("SciaRegsFile")
//#else
//#pragma DATA_SECTION(SciaRegs,"SciaRegsFile");
//#endif
volatile struct SCI_REGS SciaRegs __attribute__ ((section("SciaRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ScibRegsFile")
//#else
//#pragma DATA_SECTION(ScibRegs,"ScibRegsFile");
//#endif
volatile struct SCI_REGS ScibRegs __attribute__ ((section("ScibRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("ScicRegsFile")
//#else
//#pragma DATA_SECTION(ScicRegs,"ScicRegsFile");
//#endif
volatile struct SCI_REGS ScicRegs __attribute__ ((section("ScicRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("SpiaRegsFile")
//#else
//#pragma DATA_SECTION(SpiaRegs,"SpiaRegsFile");
//#endif
volatile struct SPI_REGS SpiaRegs __attribute__ ((section("SpiaRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("SysCtrlRegsFile")
//#else
//#pragma DATA_SECTION(SysCtrlRegs,"SysCtrlRegsFile");
//#endif
volatile struct SYS_CTRL_REGS SysCtrlRegs __attribute__ ((section("SysCtrlRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("FlashRegsFile")
//#else
//#pragma DATA_SECTION(FlashRegs,"FlashRegsFile");
//#endif
volatile struct FLASH_REGS FlashRegs __attribute__ ((section("FlashRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("XIntruptRegsFile")
//#else
//#pragma DATA_SECTION(XIntruptRegs,"XIntruptRegsFile");
//#endif
volatile struct XINTRUPT_REGS XIntruptRegs __attribute__ ((section("XIntruptRegsFile")));

//#ifdef __cplusplus
//#pragma DATA_SECTION("XintfRegsFile")
//#else
//#pragma DATA_SECTION(XintfRegs,"XintfRegsFile");
//#endif
volatile struct XINTF_REGS XintfRegs __attribute__ ((section("XintfRegsFile")));

//
// End of file
//

