/*--------------------------------------------------------------------
 *                          loaderline
 *                      www.fpgaarcade.com
 *                     All rights reserved.
 *
 *                     admin@fpgaarcade.com
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *--------------------------------------------------------------------
 *
 * Checksum Generator for Replay Apps / INI lines
 * 
 * Copyright (c) 2020, W. Scherr, ws_arcade <at> pin4.at (www.pin4.at)
 *
 */

#if defined(__linux) || defined(__APPLE__)
#else
#include<Windows.h>
#endif
#include<stdio.h>
#include<stdint.h>

int main() {
    FILE *rAppFile;
    uint32_t sum=0;
    uint32_t len=0;
    uint32_t val;

    rAppFile=fopen("rApp_ARM.bin","rb");
    if (!rAppFile) {
        printf("# file not readable - generate dummy line\n");
        printf("LAUNCH = 0x00000000,0x0000,0x00000000\n");
    } else {
      while (!feof(rAppFile)) {
        if (fread(&val,sizeof(val),1,rAppFile)) {
          len+=4;
          sum+=val;
        };
      }
      fclose(rAppFile);
      printf("# file has %d byte\n",len);
      printf("ROM = rApp_ARM.bin,0x%04lx,0x00000000\n",len);
      printf("LAUNCH = 0x00000000,0x%04lx,0x%08lx\n",len,sum);
    }
    return 0;
}
