/*
 * flash_api.h
 *
 *  Created on: 2022-8-1
 *      Author: fug
 */

#define SECTORA   (Uint16)0x0001
#define SECTORB   (Uint16)0x0002
#define SECTORC   (Uint16)0x0004
#define SECTORD   (Uint16)0x0008
#define SECTORE   (Uint16)0x0010
#define SECTORF   (Uint16)0x0020
#define SECTORG   (Uint16)0x0040
#define SECTORH   (Uint16)0x0080
#define SECTOR_F28335 (SECTORA|SECTORB|SECTORC|\
                       SECTORD|SECTORE|SECTORF|\
                       SECTORG|SECTORH)
#define STATUS_SUCCESS                        0
#define STATUS_FAIL_CSM_LOCKED               10
#define STATUS_FAIL_REVID_INVALID            11
#define STATUS_FAIL_ADDR_INVALID             12
#define STATUS_FAIL_INCORRECT_PARTID         13
#define STATUS_FAIL_API_SILICON_MISMATCH     14
#define STATUS_FAIL_NO_SECTOR_SPECIFIED      20
#define STATUS_FAIL_PRECONDITION             21
#define STATUS_FAIL_ERASE                    22
#define STATUS_FAIL_COMPACT                  23
#define STATUS_FAIL_PRECOMPACT               24
#define STATUS_FAIL_PROGRAM                  30
#define STATUS_FAIL_ZERO_BIT_ERROR           31
#define STATUS_FAIL_VERIFY                   40

typedef struct {
    Uint32  FirstFailAddr;
    Uint16  ExpectedData;
    Uint16  ActualData;
}FLASH_ST;

extern Uint16  Flash_Program(Uint16 *FlashAddr, Uint16 *BufAddr, Uint32 Length, FLASH_ST *FProgStatus);
extern Uint16  Tiotp_Program(Uint16 *FlashAddr, Uint16 *BufAddr, Uint32 Length, FLASH_ST *FProgStatus);
extern Uint16  Flash_Erase(Uint16 SectorMask, FLASH_ST *FEraseStat);
extern void    Er_sector(Uint32 FlashAddr);
extern void    Er_all();
extern float32 Flash_APIVersion();
extern Uint16  Flash_APIVersionHex();
extern Uint16  Flash_DepRecover();
extern Uint16  Flash_Verify(Uint16 *StartAddr, Uint16 *BufAddr, Uint32 Length, FLASH_ST *FVerifyStat);
extern void    Flash_ToggleTest(volatile Uint32 *ToggleReg, Uint32 Mask);
