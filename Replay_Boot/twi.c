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

#include "twi.h"
#include "hardware/twi.h"
#include "messaging.h"

#define TWI_TIMEOUTMAX 50000

void TWI_Wait_Tx(void)
{
    uint32_t timeout = 0;

    while ( !TWI_ByteSent() && (++timeout < TWI_TIMEOUTMAX) );

    if (timeout == TWI_TIMEOUTMAX) {
        ERROR("TWI:Tx timeout.");
    }
}

void TWI_Wait_Rx(void)
{
    uint32_t timeout = 0;

    while ( !TWI_ByteReceived() && (++timeout < TWI_TIMEOUTMAX) );

    if (timeout == TWI_TIMEOUTMAX) {
        ERROR("TWI:Rx timeout.");
    }
}

void TWI_Wait_Cmpl(void)
{
    uint32_t timeout = 0;

    while ( !TWI_TransferComplete() && (++timeout < TWI_TIMEOUTMAX) );

    if (timeout == TWI_TIMEOUTMAX) {
        ERROR("TWI:Completion timeout.");    // note this is debug only, no error handler
    }
}

//
uint8_t Read_THS7353(uint8_t address)
{
    DEBUG(2, "TWI:THS7353 Read");
    uint8_t  readData;
    // device address is 0101100 = x2C
    // assume idle on entry
    // phase1
    TWI_StartWrite(0x2C, 0, 0, address);
    TWI_Stop();  // Set Stop
    TWI_Wait_Tx();
    TWI_Wait_Cmpl();
    // phase2
    TWI_StartRead(0x2C, 0, 0);
    TWI_Stop(); // set stop bit

    TWI_Wait_Rx();
    readData = TWI_ReadByte();
    TWI_Wait_Cmpl();

    return readData;
}

void Write_THS7353(uint8_t address, uint8_t data)
{
    DEBUG(2, "TWI:THS7353 Write");
    TWI_StartWrite(0x2C, 1, address, data);
    TWI_Wait_Tx();

    TWI_Stop();  // Set Stop
    TWI_Wait_Tx();
    TWI_Wait_Cmpl();
}

uint8_t Read_CDCE906(uint8_t address)
{
    DEBUG(2, "TWI:CDCE906 Read");

    // device address is 1101001 = x69
    // comand = 128 + address
    uint8_t readData;
    uint8_t command = 0x80 | (address & 0x7F);

    TWI_StartWrite(0x69, 0, 0, command);

    TWI_Wait_Tx();
    TWI_Wait_Cmpl();

    TWI_StartRead(0x69, 0, 0);
    TWI_Stop(); // set stop bit

    TWI_Wait_Rx();
    readData = TWI_ReadByte();
    TWI_Wait_Cmpl();
    return readData;
}

void Write_CDCE906(uint8_t address, uint8_t data)
{
    DEBUG(2, "TWI:CDCE906 Write");

    uint8_t command = 0x80 | (address & 0x7F);

    TWI_StartWrite(0x69, 0, 0, command);
    TWI_Wait_Tx();

    TWI_WriteByte(data);
    TWI_Stop();  // Set Stop
    TWI_Wait_Tx();
    TWI_Wait_Cmpl();
}
//
//
//
void Configure_VidBuf(uint8_t chan, uint8_t stc, uint8_t lpf, uint8_t mode)
{
    // 7..6 AC-STC filter (0=9Mhz, 1 = 15MHz, 2 = 35MHz, 3 = 35MHz)
    // 5    chan select, 1 = chan B (not used)
    // 4..3 LPF (0=9MHz SD PAL/NSTC, 1 = 16MHz, 480/576P, 2 = 35MHz 720P, 3= bypass 1080P)
    // 2..0 7,6,5 STC with high,mid,low bias
    //      4     AC bias
    //      3     DC biac + 250mV
    //      2     DC biac
    //      1     Mute
    //      0     Off
    DEBUG(2, "AC-STC: ");

    switch (stc) {
        case 0:
            DEBUG(2, "9MHz");
            break;

        case 1:
            DEBUG(2, "15MHz");
            break;

        case 2:
            DEBUG(2, "35MHz");
            break;

        case 3:
            DEBUG(2, "35MHz");
            break;

        default:
            DEBUG(2, "unknown");
            break;
    }

    DEBUG(2, "LPF: ");

    switch (lpf) {
        case 0:
            DEBUG(2, "9MHz");
            break;

        case 1:
            DEBUG(2, "16MHz");
            break;

        case 2:
            DEBUG(2, "35MHz");
            break;

        case 3:
            DEBUG(2, "bypass");
            break;

        default:
            DEBUG(2, "unknown");
            break;
    }

    DEBUG(2, "BIAS: ");

    switch (mode) {
        case 0:
            DEBUG(2, "off");
            break;

        case 1:
            DEBUG(2, "mute");
            break;

        case 2:
            DEBUG(2, "DC");
            break;

        case 3:
            DEBUG(2, "DC+250mV");
            break;

        case 4:
            DEBUG(2, "AC");
            break;

        default:
            DEBUG(2, "unknown");
            break;
    }

    uint8_t command = ((stc & 0x03) << 6 ) | ((lpf & 0x03) << 3) | (mode & 0x07);


    Write_THS7353(chan, command);

    if (Read_THS7353(chan) != command) {
        ERROR("TWI:THS7353 chan:%d Config failure.", chan);

    } else {
        DEBUG(2, "TWI:THS7353 chan:%d Config OK.", chan);
    }
}
//
//

void Configure_ClockGen(const clockconfig_t* config)
{
    uint32_t addr;
    uint8_t  ok = TRUE;
    uint8_t  mem[24];
    // repack from lightly packed ClockConfig struct into registers
    // see datasheet for register details
    //  register -1
    uint8_t  powerdown = 0;  // 0:powerdown
    uint8_t  pll_conf[3] = {0, 0, 0}; // 1:PLL1_bypass 0:PLL1_VFO
    uint8_t  y_sel[6]    = {0, 1, 2, 3, 4, 5}; // static divider selection

    // calculate VFO
    uint32_t vco_freq[3];
    uint32_t op_freq;

    for (addr = 0; addr < 3; addr++) {
        switch (addr) {
            case 0  :
                vco_freq[addr] = (27000 * config->pll1_n) / config->pll1_m;
                break;

            case 1  :
                vco_freq[addr] = (27000 * config->pll2_n) / config->pll2_m;
                break;

            case 2  :
                vco_freq[addr] = (27000 * config->pll3_n) / config->pll3_m;
                break;

            default :
                vco_freq[addr] = 0;
        }

        DEBUG(2, "PLL%d VCO Freq ~ %6d KHz", addr + 1, vco_freq[addr]);

        if (vco_freq[addr] > 190000) {
            pll_conf[addr] |= 1;
        }

        if (vco_freq[addr] > 300000) {
            ERROR(" !! OUT OF RANGE !!");
        }

        if (vco_freq[addr] <  80000) {
            ERROR(" !! OUT OF RANGE !!");
        }
    }

    // px sel (divider source selection)
    // px div (divider) 0-127 divider value
    // yx sel (0= divider0 .. 5= divider5) 0x1x = on
    DEBUG(0, "PLL clock outputs :");

    for (addr = 0; addr < 6; addr++) {
        switch (config->p_sel[addr]) {
            case 0  :
                op_freq = 27000;
                break;

            case 1  :
                op_freq = vco_freq[0];
                break;

            case 2  :
                op_freq = vco_freq[1];
                break;

            case 4  :
                op_freq = vco_freq[2];
                break;

            default :
                op_freq = 0;
        }

        op_freq = op_freq / config->p_div[addr];

        if (config->y_sel[addr] == 0) {
            DEBUG(0, " %d :      OFF", addr);

        } else {
            DEBUG(0, " %d : ~ %6d KHz", addr, op_freq);
        }
    }

    DEBUG(3, "PLL config %d %d %d", pll_conf[0], pll_conf[1], pll_conf[2]);

    mem[ 1 - 1] = (EBITS(config->pll1_m, 0, 8));
    mem[ 2 - 1] = (EBITS(config->pll1_n, 0, 8));
    mem[ 3 - 1] = (EBITS(pll_conf[0], CLOCKCONFIG_PLL1_BYPASS, 1) << 7) |
                  (EBITS(pll_conf[1], CLOCKCONFIG_PLL2_BYPASS, 1) << 6) |
                  (EBITS(pll_conf[2], CLOCKCONFIG_PLL3_BYPASS, 1) << 5) |
                  (EBITS(config->pll1_n, 8, 4) << 1) |
                  (EBITS(config->pll1_m, 8, 1));
    mem[ 4 - 1] = (EBITS(config->pll2_m, 0, 8));
    mem[ 5 - 1] = (EBITS(config->pll2_n, 0, 8));
    mem[ 6 - 1] = (EBITS(pll_conf[0], CLOCKCONFIG_PLL1_VFO, 1) << 7) |
                  (EBITS(pll_conf[1], CLOCKCONFIG_PLL2_VFO, 1) << 6) |
                  (EBITS(pll_conf[2], CLOCKCONFIG_PLL3_VFO, 1) << 5) |
                  (EBITS(config->pll2_n, 8, 4) << 1) |
                  (EBITS(config->pll2_m, 8, 1));
    mem[ 7 - 1] = (EBITS(config->pll3_m, 0, 8));
    mem[ 8 - 1] = (EBITS(config->pll3_n, 0, 8));
    mem[ 9 - 1] = (EBITS(config->p_sel[0], 0, 3) << 5) |
                  (EBITS(config->pll3_n, 8, 4) << 1) |
                  (EBITS(config->pll3_m, 8, 1));
    mem[10 - 1] = (EBITS(config->p_sel[1], 0, 3) << 5);

    mem[11 - 1] = (EBITS(config->p_sel[3], 0, 3) << 3) |
                  (EBITS(config->p_sel[2], 0, 3) << 0);
    mem[12 - 1] = (EBITS(powerdown, CLOCKCONFIG_PD, 1) << 6) |
                  (EBITS(config->p_sel[5], 0, 3) << 3) |
                  (EBITS(config->p_sel[4], 0, 3) << 0);

    mem[13 - 1] = (EBITS(config->p_div[0], 0, 7));
    mem[14 - 1] = (EBITS(config->p_div[1], 0, 7));
    mem[15 - 1] = (EBITS(config->p_div[2], 0, 7));
    mem[16 - 1] = (EBITS(config->p_div[3], 0, 7));
    mem[17 - 1] = (EBITS(config->p_div[4], 0, 7));
    mem[18 - 1] = (EBITS(config->p_div[5], 0, 7));

    mem[19 - 1] = (0x03 << 4) |
                  (EBITS(config->y_sel[0], 0, 1) << 3) |
                  (EBITS(y_sel[0], 0, 3));
    mem[20 - 1] = (0x03 << 4) |
                  (EBITS(config->y_sel[1], 0, 1) << 3) |
                  (EBITS(y_sel[1], 0, 3));
    mem[21 - 1] = (0x03 << 4) |
                  (EBITS(config->y_sel[2], 0, 1) << 3) |
                  (EBITS(y_sel[2], 0, 3));
    mem[22 - 1] = (0x03 << 4) |
                  (EBITS(config->y_sel[3], 0, 1) << 3) |
                  (EBITS(y_sel[3], 0, 3));
    mem[23 - 1] = (0x03 << 4) |
                  (EBITS(config->y_sel[4], 0, 1) << 3) |
                  (EBITS(y_sel[4], 0, 3));
    mem[24 - 1] = (0x03 << 4) |
                  (EBITS(config->y_sel[5], 0, 1) << 3) |
                  (EBITS(y_sel[5], 0, 3));

    for (addr = 0; addr < 24; addr++) {
        Write_CDCE906(addr + 1, mem[addr]);
    }

    // debug, read back and check
    for (addr = 0; addr < 24; addr++) {
        if (Read_CDCE906(addr + 1) != mem[addr]) {
            ok = FALSE;
        }
    }

    if (!ok) {
        ERROR("TWI:Clock Configure failure.");

    } else {
        DEBUG(1, "TWI:Clock Configure OK.");
    }

    //
    // M divider 1..511  N divider 1..4095  m <= n
    //  80 <= fvco <= 200 vf0=0
    // 180 <= fvco <= 300 vfo=1
    //
    //  input clock = 27MHz.
    //  output = (27 * n / m) / p. example m=375 n=2048 p=12 output = 12.288MHz. VCO = 147.456 MHz
    //
    // px sel (divider source selection)
    // 0 = bypass(input clock) 1 = pll1 2 = pll2 4 = pll3
    //
    // px div (divider) 0-127 divider value
    //
    // yx sel (0= divider0 .. 5= divider5) 0x1x = on
    //
    // y0 A0 FPGA DRAM        PLL1
    // y1 A1 Codec (pal/ntsc) 14.318180 NTSC/ 17.734475 PLL1 (off for HD)
    // y2 B0 FPGA sysclk      PLL2
    // y3 B1 Expansion Main -- off
    // y4 C0 FPGA video 27MHz/74.25MHz bypass SD/ PLL3 HD
    // y5 C1 Expansion Small
    //

    // PAL  17.73447                M= 46 N= 423 Fvco=248   p=14 output = 17.73447
    // PAL  28.37516 x 2 = 56.75032 M=481 N=4044 Fvco=227   p=4  output = 56.75052 error = 3.5ppm
    // NTSC 14.31818                M= 33 N= 280 Fvco=229   p=16 output = 14.31818
    // NTSC 28.63636 x 2 = 57.27272 M= 33 N= 280 Fvco=229   p=4  output = 57.27272
    // 50                           M= 27 N= 250 Fvco=250   p=5  output = 50
    // 133                          M= 27 N= 133 Fvco=133   p=1  output = 133    (vfo=0)
    // 74.25                        M= 2  N=11   Fvco=148.5 p=2  output = 74.25  (vfo=0)
}

void Write_CH7301(uint8_t address, uint8_t data)
{
    uint8_t rab = 0x80 | (address & 0x7F);

    // device address is 1110110 = x76
    TWI_StartWrite(0x76, 1, rab, data);
    TWI_Wait_Tx();

    TWI_Stop();  // Set Stop
    TWI_Wait_Tx();
    TWI_Wait_Cmpl();
}

uint8_t Read_CH7301(uint8_t address)
{
    uint8_t rab = 0x80 | (address & 0x7F);
    uint8_t readData;

    TWI_StartWrite(0x76, 0, 0, rab);

    TWI_Wait_Tx();
    TWI_Wait_Cmpl();

    TWI_StartRead(0x76, 0, 0);
    TWI_Stop(); // set stop bit

    TWI_Wait_Rx();
    readData = TWI_ReadByte();
    TWI_Wait_Cmpl();
    return readData;
}

void Configure_CH7301(const vidconfig_t* config)
{
    // first of all, read ID register to make sure device is present
    uint8_t readData = Read_CH7301(0x4B);

    if (readData == 0x17) {
        DEBUG(2, "TWI:CH7301 found OK.");

    } else {
        DEBUG(2, "TWI:**** CH7301 not found. ****");
        return;
    }

    Write_CH7301(0x1C, config->reg1C);
    Write_CH7301(0x1D, config->reg1D);
    Write_CH7301(0x1E, config->reg1E);
    Write_CH7301(0x1F, config->reg1F);

    Write_CH7301(0x20, config->reg20);
    Write_CH7301(0x21, config->reg21);
    Write_CH7301(0x23, config->reg23);

    Write_CH7301(0x31, config->reg31);
    Write_CH7301(0x33, config->reg33);
    Write_CH7301(0x34, config->reg34);
    Write_CH7301(0x35, config->reg35);
    Write_CH7301(0x36, config->reg36);
    Write_CH7301(0x37, config->reg37);

    Write_CH7301(0x48, config->reg48);
    Write_CH7301(0x49, config->reg49);
    Write_CH7301(0x56, config->reg56);

}

