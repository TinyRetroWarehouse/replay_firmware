/*
 WWW.FPGAArcade.COM

 REPLAY Retro Gaming Platform
 No Emulation No Compromise

 All rights reserved
 Mike Johnson Wolfgang Scherr

 SVN: $Id:

--------------------------------------------------------------------

 Redistribution and use in source and synthezised forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.

 Redistributions in synthesized form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

 Neither the name of the author nor the names of other contributors may
 be used to endorse or promote products derived from this software without
 specific prior written permission.

 THIS CODE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.

 You are responsible for any legal issues arising from your use of this code.

 The latest version of this file can be found at: www.FPGAArcade.com

 Email support@fpgaarcade.com
*/
#ifndef HARDWARE_TIMER_H_INCLUDED
#define HARDWARE_TIMER_H_INCLUDED

#include "board.h"
#include "irq.h"

static volatile uint32_t s_milliseconds = 0;
static void ISR_System(void)
{
    if (AT91C_BASE_PITC->PITC_PISR & AT91C_PITC_PITS)
    {
        s_milliseconds += (AT91C_BASE_PITC->PITC_PIVR & AT91C_PITC_PICNT) >> 20;
    }
    AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_SYS);
    AT91C_BASE_AIC->AIC_EOICR = 0;
}

//
// Timer
//
void Timer_Init(void)
{
    void (*vector)(void) = &ISR_System;

    disableIRQ();   // is this really necessary?
    AT91C_BASE_AIC->AIC_IDCR = (1 << AT91C_ID_SYS);

    AT91C_BASE_PITC->PITC_PIMR = AT91C_PITC_PITEN | ((BOARD_MCK / 16 / 1000 - 1) & AT91C_PITC_PIV) | AT91C_PITC_PITIEN; //counting period 1ms

    AT91C_BASE_AIC->AIC_SMR[AT91C_ID_SYS] = 0;
    AT91C_BASE_AIC->AIC_SVR[AT91C_ID_SYS] = (int32_t)vector;

    AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_SYS);
    AT91C_BASE_AIC->AIC_IECR = (1 << AT91C_ID_SYS);
    enableIRQ();
}

uint32_t Timer_Get(uint32_t offset)
{
    uint32_t systimer = s_milliseconds;
    systimer += offset;
    return (systimer);
}

uint32_t Timer_Check(uint32_t time)
{
    uint32_t systimer = s_milliseconds;
    /*calculate difference*/
    time -= systimer;
    /*check if <t> has passed*/
    return (time > (1 << 31));
}

void Timer_Wait(uint32_t time)
{
    time = Timer_Get(time);

    while (!Timer_Check(time));
}

#endif
