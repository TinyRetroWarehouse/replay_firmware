#ifndef FILEIO_FDD_H_INCLUDED
#define FILEIO_FDD_H_INCLUDED

#include "board.h"
#include "fullfat.h"

// FILE buffer size = FDD sector size
#define FDD_BUF_SIZE 512

// floppy disk interface defs
#define CMD_RDTRK 0x01
#define CMD_WRTRK 0x02

// floppy status
#define DSK_INSERTED 0x01 /*disk is inserted*/
#define DSK_WRITABLE 0x02 /*disk is writable*/
#define MAX_DISPLAY_FILENAME 23

// constants
#define MAX_TRACKS (83*2)
//#define TRACK_SIZE 12668
//#define HEADER_SIZE 0x40
//#define DATA_SIZE 0x400
#define SECTOR_SIZE  512
#define SECTOR_COUNT 11
//#define LAST_SECTOR (SECTOR_COUNT - 1)
//#define GAP_SIZE (TRACK_SIZE - SECTOR_COUNT * SECTOR_SIZE)

typedef struct
{
    FF_FILE *fSource;
    uint8_t status; /*status of floppy*/
    uint8_t tracks; /*number of tracks*/
    //uint8_t sector_offset; /*sector offset to handle tricky loaders*/
    //uint8_t track; /*current track*/
    //uint8_t track_prev; /*previous track*/
    char    name[MAX_DISPLAY_FILENAME]; /*floppy name*/
} adfTYPE;

void FDD_AmigaHeader(adfTYPE *drive,  uint8_t track, uint8_t sector, uint16_t dsksync);
void FDD_Read(adfTYPE *drive, uint32_t offset, uint16_t size);
//uint8_t FDD_FindSync(adfTYPE *drive);
//uint8_t FDD_GetHeader(uint8_t *pTrack, uint8_t *pSector);
//uint8_t FDD_GetData(void);
//void FDD_WriteTrack(adfTYPE *drive);
void FDD_UpdateDriveStatus(void);
void FDD_Handle(void);
void FDD_Init(void);


#endif
