/*
 * RV335_Flash_API.c
 *
 *  Created on: 2022-8-1
 *      Author: fug
 */
#include "RV335_Project.h"     // Device Headerfile and Examples Include File

#include "RV335_Flash_API.h"


volatile Uint16 *FCR      =(volatile Uint16 *)(0xA88<<1);
volatile Uint32 *FADR     =(volatile Uint32 *)(0xA8A<<1);
volatile Uint16 *FDAT     =(volatile Uint16 *)(0xA8C<<1);
volatile Uint16 *FREGPRO  =(volatile Uint16 *)(0xA8E<<1);

Uint16 Flash_Program(Uint16 *FlashAddr, Uint16 *BufAddr, Uint32 Length, FLASH_ST *FProgStatus)
{
	    Uint16 i;
	    Uint16 rdata;
	    Uint16 wdata;
	    Uint32 addr;
	    Uint16 flash;
	    Uint32 xaddr;
		Uint32 flash_start = 0x300000<<1;
		Uint32 flash_end   = 0x33FFFF<<1;
		Uint32 usr_start   = 0x380400<<1;
		Uint32 usr_end     = 0x3807FF<<1;
		FProgStatus->ActualData=0x0;
		FProgStatus->ExpectedData=0x0;
		FProgStatus->FirstFailAddr=0x0;
	    DisableDog();
	    /*DINT;*/
	    /*InitPieCtrl();*/
	    /*IER = 0x0000;*/
	    /*IFR = 0x0000;*/
	    XIntruptRegs.XINT1CR.bit.ENABLE =0;
	    XIntruptRegs.XINT2CR.bit.ENABLE =0;
	    XIntruptRegs.XINT3CR.bit.ENABLE =0;
	    XIntruptRegs.XINT4CR.bit.ENABLE =0;
	    XIntruptRegs.XINT5CR.bit.ENABLE =0;
	    XIntruptRegs.XINT6CR.bit.ENABLE =0;
	    XIntruptRegs.XINT7CR.bit.ENABLE =0;
	    XIntruptRegs.XNMICR.bit.ENABLE =0;
	    if(CsmRegs.CSMSCR.bit.SECURE == 1)
	    	return STATUS_FAIL_CSM_LOCKED;
        xaddr=(Uint32)FlashAddr;
	    for(i=0;i<Length;i++)
	    {
         if((xaddr+i)<=flash_end && (xaddr+i)>=flash_start)
         {
       	     addr = xaddr+i-flash_start;
             flash = 1;
         }
         else
         {
       	  if((xaddr+i)>=usr_start && (xaddr+i)<=usr_end)
       	  {
       		  addr = xaddr+i-usr_start;
              flash = 0;
       	  }
       	  else
       		  return STATUS_FAIL_ADDR_INVALID;
         }

	      rdata=*(FlashAddr+i);
	      wdata=*(BufAddr+i);
	      if(rdata != 0xFFFF)
	      {
	  		  FProgStatus->ActualData=rdata;
	  		  FProgStatus->ExpectedData=0xFFFF;
	  		  FProgStatus->FirstFailAddr=(Uint32) (FlashAddr+i);
	    	  return STATUS_FAIL_ZERO_BIT_ERROR;
	      }
	 	  EALLOW;
	 	  *FREGPRO=0x55AA;
	 	  if(!flash)
		 	*FCR=0x800;
	 	  *FADR=addr;
	 	  *FDAT=wdata;
	 	  if(!flash)
		 	*FCR=0x891;
	 	  else
	 	    *FCR=0x91;
	 	   DELAY_US(10);//Tnvs
	 	  if(!flash)
	 	    *FCR=0x991;
	 	  else
	 	    *FCR=0x191;
	 	   DELAY_US(15);//Tpgs
	 	  if(!flash)
		    *FCR=0x9B1;
	 	  else
	 	    *FCR=0x1B1;
	 	  DELAY_US(30);//Tprog
	 	  if(!flash)
		    *FCR=0x991;
	 	  else
	 	    *FCR=0x191;
	 	  DELAY_US(1);//Tpgh
	 	  if(!flash)
		    *FCR=0x911;
	 	  else
	 	    *FCR=0x111;
	 	  DELAY_US(10);//Tnvh
	 	  if(!flash)
		    *FCR=0x811;
	 	  else
	 	    *FCR=0x11;
		  DELAY_US(1);//Twhd
	 	  *FCR=0x0;
	 	  *FREGPRO=0x0;
	 	  DELAY_US(4);//Trcv
	      EDIS;
	      rdata=*(FlashAddr+i);
	      if(rdata != wdata)
	      {
	  		  FProgStatus->ActualData=rdata;
	  		  FProgStatus->ExpectedData=wdata;
	  		  FProgStatus->FirstFailAddr=(Uint32) (FlashAddr+i);
	    	  return STATUS_FAIL_PROGRAM;
	      }
	    }
	    return STATUS_SUCCESS;
}


Uint16 Tiotp_Program(Uint16 *FlashAddr, Uint16 *BufAddr, Uint32 Length, FLASH_ST *FProgStatus)
  {
 	    Uint16 i;
 	    Uint16 rdata;
 	    Uint16 wdata;
 	    Uint32 addr;
 	    Uint32 xaddr;
 		Uint32 ti_start   = 0x380000<<1;
 		Uint32 ti_end     = 0x3803FF<<1;
		FProgStatus->ActualData=0x0;
		FProgStatus->ExpectedData=0x0;
		FProgStatus->FirstFailAddr=0x0;
 	    DisableDog();
 	    /*DINT;*/
 	    /*InitPieCtrl();*/
 	    /*IER = 0x0000;*/
 	    /*IFR = 0x0000;*/
 	    XIntruptRegs.XINT1CR.bit.ENABLE =0;
 	    XIntruptRegs.XINT2CR.bit.ENABLE =0;
 	    XIntruptRegs.XINT3CR.bit.ENABLE =0;
 	    XIntruptRegs.XINT4CR.bit.ENABLE =0;
 	    XIntruptRegs.XINT5CR.bit.ENABLE =0;
 	    XIntruptRegs.XINT6CR.bit.ENABLE =0;
 	    XIntruptRegs.XINT7CR.bit.ENABLE =0;
 	    XIntruptRegs.XNMICR.bit.ENABLE =0;
 	    if(CsmRegs.CSMSCR.bit.SECURE == 1)
 	    	return STATUS_FAIL_CSM_LOCKED;
         xaddr=(Uint32)FlashAddr;
 	    for(i=0;i<Length;i++)
 	    {
           if((xaddr+i)<=ti_end && (xaddr+i)>=ti_start)
           {
         	   addr = xaddr+i-ti_start;
           }
           else
        	   return STATUS_FAIL_ADDR_INVALID;

 	      rdata=*(FlashAddr+i);
 	      wdata=*(BufAddr+i);
	      if(rdata != 0xFFFF)
	      {
	  		  FProgStatus->ActualData=rdata;
	  		  FProgStatus->ExpectedData=0xFFFF;
	  		  FProgStatus->FirstFailAddr=(Uint32) (FlashAddr+i);
	    	  return STATUS_FAIL_ZERO_BIT_ERROR;
	      }
	 	  EALLOW;
	 	  *FREGPRO=0x55AA;
		  *FCR=0x800;
	 	  *FADR=addr;
	 	  *FDAT=wdata;
		  *FCR=0x891;
	 	   DELAY_US(10);//Tnvs
	 	  *FCR=0x991;
	 	   DELAY_US(15);//Tpgs
		  *FCR=0x9B1;
	 	   DELAY_US(30);//Tprog
		  *FCR=0x991;
	 	   DELAY_US(1);//Tpgh
		  *FCR=0x911;
	 	   DELAY_US(10);//Tnvh
		  *FCR=0x811;
		   DELAY_US(1);//Twhd
	 	  *FCR=0x0;
	 	  *FREGPRO=0x0;
	 	   DELAY_US(4);//Trcv
	      EDIS;
	      rdata=*(FlashAddr+i);
	      if(rdata != wdata){
	  		  FProgStatus->ActualData=rdata;
	  		  FProgStatus->ExpectedData=wdata;
	  		  FProgStatus->FirstFailAddr=(Uint32) (FlashAddr+i);
	    	  return STATUS_FAIL_PROGRAM;
	      }

	    }
	    return STATUS_SUCCESS;
  }

Uint16  Flash_Erase(Uint16 SectorMask, FLASH_ST *FEraseStat)
 {
	        DisableDog();
	  	    /*DINT;*/
	  	    /*InitPieCtrl();*/
	  	    FEraseStat->ActualData=0x0;
	  	    FEraseStat->ExpectedData=0x0;
	  	    FEraseStat->FirstFailAddr=0x0;
	  	    /*IER = 0x0000;*/
	  	    /*IFR = 0x0000;*/
	  	    XIntruptRegs.XINT1CR.bit.ENABLE =0;
	  	    XIntruptRegs.XINT2CR.bit.ENABLE =0;
	  	    XIntruptRegs.XINT3CR.bit.ENABLE =0;
	  	    XIntruptRegs.XINT4CR.bit.ENABLE =0;
	  	    XIntruptRegs.XINT5CR.bit.ENABLE =0;
	  	    XIntruptRegs.XINT6CR.bit.ENABLE =0;
	  	    XIntruptRegs.XINT7CR.bit.ENABLE =0;
	  	    XIntruptRegs.XNMICR.bit.ENABLE =0;
	  	    if(CsmRegs.CSMSCR.bit.SECURE == 1)
	  	    	return STATUS_FAIL_CSM_LOCKED;
	  	    if(SectorMask!=SECTORA &&
	  	       SectorMask!=SECTORB &&
	  	       SectorMask!=SECTORC &&
	  	       SectorMask!=SECTORD &&
	  	       SectorMask!=SECTORE &&
	  	       SectorMask!=SECTORF &&
	  	       SectorMask!=SECTORG &&
	  	       SectorMask!=SECTORH &&
	  	       SectorMask!=SECTOR_F28335
	  	       )
	  	    	return STATUS_FAIL_ADDR_INVALID;

             switch(SectorMask)
             {
             case SECTORA:Er_sector(0x38000);break;
             case SECTORB:Er_sector(0x30000);break;
             case SECTORC:Er_sector(0x28000);break;
             case SECTORD:Er_sector(0x20000);break;
             case SECTORE:Er_sector(0x18000);break;
             case SECTORF:Er_sector(0x10000);break;
             case SECTORG:Er_sector(0x8000);break;
             case SECTORH:Er_sector(0x0000);break;
             case SECTOR_F28335: Er_all();break;
             default:break;
             }
             return STATUS_SUCCESS;

 }

void Er_sector(Uint32 FlashAddr)
 {
     int i;
     Uint32 addr;
     int fast=1;
     for(i=0;i<64;i++)
     {
      addr = FlashAddr + i*0x80;
	  EALLOW;
 	  *FREGPRO=0x55AA;
 	  *FADR=addr;
 	  *FCR=0x212;
	   DELAY_US(10);//Tnvs
	  *FCR=0x312;
	  if(!fast)
	    DELAY_US(30000);//Terase
	  else
	    DELAY_US(30);//Terase - fast sim
	  *FCR=0x112;
	   DELAY_US(10);//Tnvh
	  *FCR=0x12;
	   DELAY_US(1);//Twhd
	  *FCR=0x0;
	  *FREGPRO=0x0;
	   DELAY_US(4);//Trcv
      EDIS;
     }
 }
 void Er_all()
 {
     int fast=1;
	  EALLOW;
 	  *FREGPRO=0x55AA;
 	  *FCR=0x614;
	   DELAY_US(10);//Tnvs
 	  *FCR=0x714;
	  if(!fast)
	    DELAY_US(30000);//Tme
	  else
	    DELAY_US(30);//Tme - fast sim
 	  *FCR=0x514;
	  DELAY_US(110);//Tnvh1
 	  *FCR=0x414;
	  DELAY_US(1);//Twhd
	  *FCR=0x0;
	  *FREGPRO=0x0;
	  DELAY_US(4);//Trcv
      EDIS;
 }

 float32 Flash_APIVersion()
 {
	 float a=2.10;
	 return a;

 }
 Uint16  Flash_APIVersionHex()
 {
	 Uint16 a=0x210;
	 return a;
 }

 Uint16  Flash_DepRecover()
 {
	 DELAY_US(100);
	 return STATUS_SUCCESS;
 }

 void    Flash_ToggleTest(volatile Uint32 *ToggleReg, Uint32 Mask)
 {
     while(1)
     {
	 EALLOW;
	*(ToggleReg)=(Uint32)Mask;
 	 EDIS;
	 DELAY_US(50);
     }
 }

 Uint16  Flash_Verify(Uint16 *StartAddr, Uint16 *BufAddr, Uint32 Length, FLASH_ST *FVerifyStat)
  {
 	        Uint16 i;
 		    Uint16 rdata;
 		    Uint16 wdata;
 		    Uint32 addr;
 		    Uint32 xaddr;
 		    Uint32 flash_start = 0x300000<<1;
 		    Uint32 flash_end   = 0x33FFFF<<1;
 		    Uint32 usr_start   = 0x380400<<1;
 		    Uint32 usr_end     = 0x3807FF<<1;
 		    Uint32 ti_start    = 0x380000<<1;
 		    Uint32 ti_end      = 0x3803FF<<1;
 			FVerifyStat->ActualData=0x0;
 			FVerifyStat->ExpectedData=0x0;
 			FVerifyStat->FirstFailAddr=0x0;
 		    DisableDog();
 		    /*DINT;*/
 		    /*InitPieCtrl();*/
 		    /*IER = 0x0000;*/
 		    /*IFR = 0x0000;*/
 		    XIntruptRegs.XINT1CR.bit.ENABLE =0;
 		    XIntruptRegs.XINT2CR.bit.ENABLE =0;
 		    XIntruptRegs.XINT3CR.bit.ENABLE =0;
 		    XIntruptRegs.XINT4CR.bit.ENABLE =0;
 		    XIntruptRegs.XINT5CR.bit.ENABLE =0;
 		    XIntruptRegs.XINT6CR.bit.ENABLE =0;
 		    XIntruptRegs.XINT7CR.bit.ENABLE =0;
 		    XIntruptRegs.XNMICR.bit.ENABLE =0;
 		    if(CsmRegs.CSMSCR.bit.SECURE == 1)
 		    	return STATUS_FAIL_CSM_LOCKED;
 	        xaddr=(Uint32)StartAddr;
 		    for(i=0;i<Length;i++)
 		    {
 	          if((xaddr+i)<=flash_end && (xaddr+i)>=flash_start)
 	          {
 	        	  addr = xaddr+i-flash_start;
 	          }
 	          else {
 	        	  if((xaddr+i)>=usr_start && (xaddr+i)<=usr_end)
 	        	  {
 	        		  addr = xaddr+i-usr_start;
 	        	  }
 	        	  else
 	        	  {
 	            	  addr = xaddr+i-ti_start;
 	        	  }
 	          }

 		      rdata=*(StartAddr+i);
 		      wdata=*(BufAddr+i);

 		      if(rdata != wdata)
 		      {
 		    	  FVerifyStat->ActualData=rdata;
 		    	  FVerifyStat->ExpectedData=wdata;
 		    	  FVerifyStat->FirstFailAddr=(Uint32) (StartAddr+i);
 		    	  return STATUS_FAIL_VERIFY;
 		      }
 		    }
 		    return STATUS_SUCCESS;

  }
