//###########################################################################
//
// FILE:   RV335_MemCopy.c
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
#include "../include/RV335_Device.h"

//
// MemCopy -
//
void 
MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    { 
        *DestAddr++ = *SourceAddr++;
    }
    return;
}

//
// End of file
//

