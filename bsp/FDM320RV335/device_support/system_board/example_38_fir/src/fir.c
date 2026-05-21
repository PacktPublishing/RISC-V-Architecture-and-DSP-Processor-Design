#include "RV335_Device.h"     // RV335 Headerfile Include File
#include "RV335_Examples.h"   // RV335 Examples Include File
//#include "f2812a.h"
#include"math.h"

#define FIRNUMBER 25
#define SIGNAL1F 1000
#define SIGNAL2F 4500
#define SAMPLEF  10000
#define PI 3.1415926

float InputWave();
float FIR();

float fHn[FIRNUMBER]={ 0.0,0.0,0.001,-0.002,-0.002,0.01,-0.009,
                       -0.018,0.049,-0.02,0.11,0.28,0.64,0.28,
                       -0.11,-0.02,0.049,-0.018,-0.009,0.01,
                       -0.002,-0.002,0.001,0.0,0.0
                     };
float fXn[FIRNUMBER]={ 0.0 };
float fInput,fOutput;
float fSignal1,fSignal2;
float fStepSignal1,fStepSignal2;
float f2PI;
int i;
float fIn[256],fOut[256];
int nIn,nOut;

main(void)
{
   
   nIn=0; nOut=0;
	f2PI=2*PI;
	fSignal1=0.0;
	fSignal2=PI*0.1;
	fStepSignal1=2*PI/30;
	fStepSignal2=2*PI*1.4;
	while ( 1 )
	{
		fInput=InputWave();
		fIn[nIn]=fInput;
		nIn++; nIn%=256;
		fOutput=FIR();
		fOut[nOut]=fOutput;
		nOut++;
		if ( nOut>=256 )
		{
			nOut=0;		/* 请在此句上设置软件断点 */
		}
	}
} 	


float InputWave()
{
	for ( i=FIRNUMBER-1;i>0;i-- )
		fXn[i]=fXn[i-1];
	fXn[0]=sin(fSignal1)+cos(fSignal2)/6.0;
	fSignal1+=fStepSignal1; 
	if ( fSignal1>=f2PI )	fSignal1-=f2PI;
	fSignal2+=fStepSignal2;
	if ( fSignal2>=f2PI )	fSignal2-=f2PI;
	return(fXn[0]);
}

float FIR()
{
	float fSum;
	fSum=0;
	for ( i=0;i<FIRNUMBER;i++ )
	{
		fSum+=(fXn[i]*fHn[i]);
	}
	return(fSum);
}
