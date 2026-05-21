/*******************************************************************
 *
 *    DESCRIPTION:	LED Funtcion File
 *
 *    AUTHOR:	ZhangYang
 *
 *    HISTORY:	2011-3-4
 *
 *******************************************************************/
#include "RV335_Device.h"     // DSP280x Headerfile Include File
#include "RV335_Examples.h"   // DSP280x Examples Include File 
#include "LED.h"
Uint16 CharToLed(Uint16 s)
{
	switch(s)
	{
		case 0:
			return LED0;
		case 1:
			return LED1;
		case 2:
			return LED2;
		case 3:
			return LED3;
		case 4:
			return LED4;
		case 5:
			return LED5;
		case 6:
			return LED6;
		case 7:
			return LED7;
		case 8:
			return LED8;
		case 9:
			return LED9;
		default:
			return LEDNULL;
	}
}

void CharDisplay(Uint16 s, Uint16 LedBuffer[])		
{	
	
	if(s<=99)
	{
		LedBuffer[1]=CharToLed(s/10)+LEDDOT;
		LedBuffer[0]=CharToLed(s%10);
    }
	else if(s<=990)
	{
	    s=s/10;
        LedBuffer[1]=CharToLed(s/10);
		LedBuffer[0]=CharToLed(s%10);
	}
	else
	{
	    LedBuffer[1]=CharToLed(9);
		LedBuffer[0]=CharToLed(9);
	}


}




