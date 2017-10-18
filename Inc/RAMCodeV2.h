/*********************************************************************
*                SEGGER MICROCONTROLLER GmbH & Co KG                 *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*           (C) 2010 SEGGER Microcontroller GmbH & Co. KG            *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File    : RAMCodeV2.h
Purpose : Header file which contains a types and function prototypes
          used by the RAMCode
Notes:
          1. pContext can be freely used by the specific part of the RAMCode.
             pContext points to an area of 64 bytes.
---------------------------END-OF-HEADER------------------------------
*/

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define U8  unsigned char
#define U16 unsigned short
#define U32 unsigned long

#define I8  signed char
#define I16 signed short
#define I32 signed long

#define MIN(a,b)    (((a) < (b)) ? (a) : (b))

#define RAMCODE_CMD_CHECKRAM    (0x01)      // Generic
#define RAMCODE_CMD_CHECKBLANK  (0x02)      // Generic
#define RAMCODE_CMD_CALCCRC     (0x03)      // Generic
#define RAMCODE_CMD_GETDESC     (0x04)      // Specific
#define RAMCODE_CMD_READ        (0x05)      // Specific
#define RAMCODE_CMD_PROGRAM     (0x06)      // Specific
#define RAMCODE_CMD_ERASE       (0x07)      // Specific
#define RAMCODE_CMD_PREPARE     (0x08)      // Specific
#define RAMCODE_CMD_RESTORE     (0x09)      // Specific

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

typedef struct {
  U16     NumExtraBytes;              // Amount of additional RAM reserved for RAMCode
  U8      MinAlign;                   // Some flash algos can only program units of 512 bytes or more. Alignment is 2^n bytes
  U8      AutoErase;                  // Erase is automatically done when programming
  U8      SupportRead;                // RAMCode supports read command
  U8      SupportEraseSector;         // RAMCode supports erase sector command
  U8      SupportEraseChip;           // RAMCode supports erase chip command
  U8      SupportMultiSectorErase;    // RAMCode supports multi sector erasing
  U8      SupportMultiSectorProg;     // RAMCode supports multi sector programming
  U8      AlgoRequiresClockSpeed;
  U8      HaltIfPCLeavesRAMCode;
  U8      Dummy[5];
} RAMCODE_ALGO_DESC;

typedef struct {
  U32     Offset;
  U32     NumSectors;
  U32     SectorSize;
} RAMCODE_FLASH_BLOCK;

typedef struct {
  U32                 NumBlocks;
  RAMCODE_FLASH_BLOCK BlockInfo[2];      // This is the first block of an array of flash blocks
} RAMCODE_FLASH_DESC;

typedef struct {
  RAMCODE_ALGO_DESC   AlgoDesc;
  RAMCODE_FLASH_DESC  FlashDesc;
} RAMCODE_DESC;

typedef struct {
  U32     Cmd;
  void*   pBuffer;
  U32     BufferSize;
  U32     BaseAddr;
  U32     Offset;
  union {
    U32   NumBytes;
    U32   NumSectors;
  };
  union {
    U32   Para0;
    U32   CRC;
  };
  union {
    U32   Para1;
    U32   Polynom;
  };
  U32     Para2;
} RAMCODE_CMD_INFO;

typedef struct {
  int     ErrorCode;
  union {
    U32   CRC;
    U32   NumItemsRem;
    U32   NumBytes;
    U32   NumFlashBlocks;
    U32   FailAddr;
  };
  U32     FailData;
} RAMCODE_RESULT_INFO;

/*********************************************************************
*
*       API
*
**********************************************************************
*/

void  RAMCODE_BuildCRCTable (void* pCRCTable, U32 Polynom);
void  RAMCODE_CalcCRC       (void* pContext, RAMCODE_RESULT_INFO* pResult, const RAMCODE_CMD_INFO* pInfo, void* pCRCTable);
void  RAMCODE_CheckBlank    (void* pContext, RAMCODE_RESULT_INFO* pResult, const RAMCODE_CMD_INFO* pInfo);
void  RAMCODE_CheckRAM      (void* pContext, RAMCODE_RESULT_INFO* pResult, const RAMCODE_CMD_INFO* pInfo);
void  RAMCODE_Start         (void* pContext, RAMCODE_RESULT_INFO* pResult,       RAMCODE_CMD_INFO* pInfo);

void  FLASH_CallFunc        (void* pContext, RAMCODE_RESULT_INFO* pResult, const RAMCODE_CMD_INFO* pInfo, int Offset);

void  FLASH_GetDesc         (void* pContext, RAMCODE_RESULT_INFO* pResult, const RAMCODE_CMD_INFO* pInfo);
void  FLASH_Read            (void* pContext, RAMCODE_RESULT_INFO* pResult, const RAMCODE_CMD_INFO* pInfo);
void  FLASH_Program         (void* pContext, RAMCODE_RESULT_INFO* pResult, const RAMCODE_CMD_INFO* pInfo);
void  FLASH_Erase           (void* pContext, RAMCODE_RESULT_INFO* pResult, const RAMCODE_CMD_INFO* pInfo);
void  FLASH_Prepare         (void* pContext);
void  FLASH_Restore         (void* pContext);
void  FLASH_FeedWatchdog    (void* pContext);

/**************************** End of file ***************************/
