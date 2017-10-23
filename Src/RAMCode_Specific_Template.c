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
File    : RAMCodeV2_Specific_Template.c
Purpose : Template for specific part of custom RAMCode.

Notes when creating a RAMCode:
  - The RAMCode must not use any static variables. Use pContext for cross-function passing of variables
  - The RAMCode must not use any functions from runtime libraries, meaning:
    No memcpy(), memset() etc.
    This includes not indirectly structure variables via StructVar = {0}; etc. which causes an indirect call of memset()
  - The RAMCode must not rely on being running from a specific address. It has to rune position-independent.
  - The main() function will be there in the debug built *only*, for debugging purposes. The RAMCode must not rely on it being there / called.
---------------------------END-OF-HEADER------------------------------
*/
#include "RAMCodeV2.h"
#include "qspi_rom.h"
#include "flash_rom.h"
#include "reg_sysc_cmp.h"

#define CYPRESS_FLASH           0
#define XTX_FALSH               1
/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
#define PAGE_SIZE_SHIFT   8     // 1 page = 2^8 = 256 bytes
#define SECTOR_SIZE_SHIFT 12    // 1 sector = 16 pages (4096 bytes)

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       FLASH_GetDesc
*
*  Function description
*    Called by the host application / Flasher ARM to retrieve information about flash memory & RAMCode.
*    Usually this is the minimum alignment of program data for the RAMCode, sectorization of the flash, features supported by the RAMCode, ...
*
*  Parameters
*    pContext  Pointer to 64-byte area which can be used to store variables accross the RAMCode FLASH_xxx functions.
*              Note that there may be 64 + pDesc->AlgoDesc.NumExtraBytes (FLASH_GetDesc()) available, depending on what FLASH_GetDesc() has requested.
*              The 64-byte area is initialized to 0 before FLASH_Prepare() and before FLASH_GetDesc() is called.
*
*  Notes
*    (1) Must not rely on any other function being called before. Has to be treated totally independent from all other FLASH_ functions.
*    (2) May even be called before FLASH_Prepare()
*    (3) May request via pDesc->AlgoDesc.NumExtraBytes that more than 64 bytes are needed for pContext. However, these 64 bytes are only available *after* this function, so this function should not try to make use of them.
*/
void FLASH_GetDesc(void* pContext, RAMCODE_RESULT_INFO* pResult, const RAMCODE_CMD_INFO* pInfo) {
  (void)pContext;
  (void)pResult;
  RAMCODE_DESC* pDesc = (RAMCODE_DESC*)pInfo->pBuffer;
  sysc_cmp_spif_io_en_setf(1);
  flash_init();
  
  pDesc->AlgoDesc.NumExtraBytes           = 0;                           // Normally not used. If the 64 bytes of the pContext area are not sufficient, additional bytes can be requested by the RAMCode, by filling in this parameter.
  pDesc->AlgoDesc.MinAlign                = PAGE_SIZE_SHIFT;             // This means the calling application always programs a multiple of 2^8 bytes at once. (In this sample we assume, this is the size of one page, so the algorithm can always be sure to get 1 complete page of data, when programming)
  pDesc->AlgoDesc.AutoErase               = 0;                           // 1 = Sector is auto-erased before programmed (Only supported by some flashes)
  pDesc->AlgoDesc.SupportRead             = 1;                           // 1 = Separate read function is necessary to read back content of flash memory (necessary for NAND, SPI-NOR, DataFlash), 0 = No read function is needed. Flash is memory-mapped (Connected to data bus of the CPU. E.g. this is the case when using a parallel NOR flash)
  pDesc->AlgoDesc.SupportEraseSector      = 1;                           // 1 = RAMCode supports sector erase
  pDesc->AlgoDesc.SupportEraseChip        = 1;                           // 1 = RAMCode supports chip erase
  pDesc->AlgoDesc.SupportMultiSectorErase = 0;                           // 1 = RAMCode supports erasure of multiple sectors by one call of FLASH_Erase()
  pDesc->AlgoDesc.SupportMultiSectorProg  = 0;                           // 1 = RAMCode support programming of multiple pages/sectors by one call of FLASH_Program()
  pDesc->AlgoDesc.AlgoRequiresClockSpeed  = 0;                           // 1 = Clock speed is required for flash programming
  pDesc->AlgoDesc.HaltIfPCLeavesRAMCode   = 1;                           // 1 = PC is not allowed to leave the RAMCode during flash programming. Some devices provide a Flash programming code, placed in ROM, which handles the flash programming and which is activated by this RAMCode. In this case the parameter should be 0.

#if (CYPRESS_FLASH == 1)
  pDesc->FlashDesc.NumBlocks              = 2;                           // Number of flash block regions we have. For example, if the flash has 2 sectors which are 4 KBytes in size and 1 sector which is 2 KBytes in size, we have 2 flash block regions
  pDesc->FlashDesc.BlockInfo[0].Offset       = 0x0;                     // Offset of the flash block, relative to the base address of the flash area. Needs only to be filled if there is a gap between the blocks or if we have sectors of different sizes. Otherwise the calling applications assumes that all blocks are in sequential order.
  pDesc->FlashDesc.BlockInfo[0].NumSectors   = 32;                        // Number of sectors (1 sector = smallest erasable unit) in this flash block
  pDesc->FlashDesc.BlockInfo[0].SectorSize   = 1 << SECTOR_SIZE_SHIFT;      // Size of one sector
  pDesc->FlashDesc.BlockInfo[1].Offset       = 0x20000;                     // Offset of the flash block, relative to the base address of the flash area. Needs only to be filled if there is a gap between the blocks or if we have sectors of different sizes. Otherwise the calling applications assumes that all blocks are in sequential order.
  pDesc->FlashDesc.BlockInfo[1].NumSectors   = 510;                        // Number of sectors (1 sector = smallest erasable unit) in this flash block
  pDesc->FlashDesc.BlockInfo[1].SectorSize   = 1 << (SECTOR_SIZE_SHIFT+4);      // Size of one sector
#elif (XTX_FALSH == 1)
  pDesc->FlashDesc.NumBlocks              = 1;                           // Number of flash block regions we have. For example, if the flash has 2 sectors which are 4 KBytes in size and 1 sector which is 2 KBytes in size, we have 2 flash block regions
  pDesc->FlashDesc.BlockInfo[0].Offset       = 0x0;                     // Offset of the flash block, relative to the base address of the flash area. Needs only to be filled if there is a gap between the blocks or if we have sectors of different sizes. Otherwise the calling applications assumes that all blocks are in sequential order.
  pDesc->FlashDesc.BlockInfo[0].NumSectors   = 512;                        // Number of sectors (1 sector = smallest erasable unit) in this flash block
  pDesc->FlashDesc.BlockInfo[0].SectorSize   = 1 << SECTOR_SIZE_SHIFT;      // Size of one sector  
#endif


}

/*********************************************************************
*
*       FLASH_Read
*
*  Purpose:
*    This function reads the flash memory at a given offset.
*    It may be requested to read any number of bytes, starting at any address within the flash memory.
*    For non-memory mapped flashes like NAND flash this means:
*    This function may need to be able to temporarily store a complete page in a buffer,
*    then execute ECC correction and extract the actual bytes that have been requested by the caller.
*    A buffer for the page can be requested via pDesc->AlgoDesc.NumExtraBytes in FLASH_GetDesc(), so an additional xxx bytes will be available in pContext (besides the 64 bytes that are always there).
*
*  Parameters:
*    pInfo->pBuffer:  Pointer to RAM area where data from flash memory shall be stored.
*    pInfo->NumBytes: Number of bytes to read.
*    pInfo->Offset:   Offset from flash base address where reading shall start from.
*    pContext         Pointer to 64-byte area which can be used to store variables accross the RAMCode FLASH_xxx functions.
*                     Note that there may be 64 + pDesc->AlgoDesc.NumExtraBytes (FLASH_GetDesc()) available, depending on what FLASH_GetDesc() has requested.
*                     The 64-byte area is initialized to 0 before FLASH_Prepare() and before FLASH_GetDesc() is called.
*
*  Valid values for pResult->ErrorCode:
*    0 = Operation completed successfully
*    1 = Block verification error (Detected by generic RAMCode, do not use!)
*    2 = Item verification error
*    3 = Timeout occurred
*    4 = Program error
*    5 = Program 1 over 0
*    6 = Sector is locked
*    7 = Erase error
*    8 = No flash memory
*    9 = Generic error
*   >9 = Unspecified error (reserved for further error codes)
*   <0 = Algo depending error code (usually a small integer such as -1, -2, -3) (Do NOT use)
*/
void FLASH_Read(void* pContext, RAMCODE_RESULT_INFO* pResult, const RAMCODE_CMD_INFO* pInfo) {
  volatile U8* pDest;
  U32 NumBytesRem;
  U32 Cycles,Remainder,Offset,i;
  pDest        = pInfo->pBuffer;                                // Specifies where the data read from flash, should be stored
  NumBytesRem  = pInfo->NumBytes;                               // Number of bytes to be read
  Offset       = pInfo->Offset;
  Cycles = NumBytesRem / FLASH_READ_MAX_BYTES;
  Remainder = NumBytesRem % FLASH_READ_MAX_BYTES;
  for(i=0;i<Cycles;i++){
    flash_read(Offset,FLASH_READ_MAX_BYTES,(uint8_t *)pDest);
    Offset += FLASH_READ_MAX_BYTES;
    pDest += FLASH_READ_MAX_BYTES;
  }
  if(Remainder){
    flash_read(Offset,Remainder,(uint8_t *)pDest);
  }
  
  pResult->ErrorCode = 0;                                       // This is the default value, so it is not obligatory to set it to 0
  pResult->NumItemsRem = 0;
}

/*********************************************************************
*
*       FLASH_Program
*
*  Purpose:
*    This function programs the flash memory.
*    pInfo->NumBytes is always a multiple of 2^n bytes (specified by MinAlign in FLASH_GetDesc())
*
*  Parameters:
*    pInfo->pBuffer:  Pointer to RAM area where data which shall be written is present.
*    pInfo->NumBytes: Number of bytes to write.
*    pInfo->Offset:   Offset from flash base address where writing shall start from.
*    pContext         Pointer to 64-byte area which can be used to store variables accross the RAMCode FLASH_xxx functions.
*                     Note that there may be 64 + pDesc->AlgoDesc.NumExtraBytes (FLASH_GetDesc()) available, depending on what FLASH_GetDesc() has requested.
*                     The 64-byte area is initialized to 0 before FLASH_Prepare() and before FLASH_GetDesc() is called.
*
*  Valid values for pResult->ErrorCode:
*    0 = Operation completed successfully
*    1 = Block verification error (Detected by generic RAMCode, do not use!)
*    2 = Item verification error
*    3 = Timeout occurred
*    4 = Program error
*    5 = Program 1 over 0
*    6 = Sector is locked
*    7 = Erase error
*    8 = No flash memory
*    9 = Generic error
*   >9 = Unspecified error (reserved for further error codes)
*   <0 = Algo depending error code (usually a small integer such as -1, -2, -3) (Do NOT use)
*/
void FLASH_Program(void* pContext, RAMCODE_RESULT_INFO* pResult, const RAMCODE_CMD_INFO* pInfo) {
  volatile U8* pSrc;
  U32 Offset;
  U32 NumPages;
  U32 i;
  pSrc               = pInfo->pBuffer;
  NumPages           = pInfo->NumBytes >> PAGE_SIZE_SHIFT;
  Offset             = pInfo->Offset;
  for(i=0; i< NumPages; i++)
  {
    flash_program_jflash(Offset,1<<PAGE_SIZE_SHIFT,(uint8_t *)pSrc);
    Offset += (1<<PAGE_SIZE_SHIFT);
    pSrc += (1<<PAGE_SIZE_SHIFT);
  }
  
  pResult->ErrorCode = 0;    // This is the default value, so it is not obligatory to set it to 0
}

/*********************************************************************
*
*       FLASH_Erase
*
*  Purpose:
*    This function erases one or more flash sectors
*    or performs a chip erase.
*
*  Parameters:
*    pInfo->NumSectors: Number of sectors to erase.
*                       For chip erase (if supported by the RAMCode) this value is 0.
*    pInfo->Offset:     Offset from flash base address where erasing shall start. (Normally this is the address of a sector)
*    pContext           Pointer to 64-byte area which can be used to store variables accross the RAMCode FLASH_xxx functions.
*                       Note that there may be 64 + pDesc->AlgoDesc.NumExtraBytes (FLASH_GetDesc()) available, depending on what FLASH_GetDesc() has requested.
*                       The 64-byte area is initialized to 0 before FLASH_Prepare() and before FLASH_GetDesc() is called.
*
*  Valid values for pResult->ErrorCode:
*    0 = Operation completed successfully
*    1 = Block verification error (Detected by generic RAMCode, do not use!)
*    2 = Item verification error
*    3 = Timeout occurred
*    4 = Program error
*    5 = Program 1 over 0
*    6 = Sector is locked
*    7 = Erase error
*    8 = No flash memory
*    9 = Generic error
*   >9 = Unspecified error (reserved for further error codes)
*   <0 = Algo depending error code (usually a small integer such as -1, -2, -3) (Do NOT use)
*/
void FLASH_Erase(void* pContext, RAMCODE_RESULT_INFO* pResult, const RAMCODE_CMD_INFO* pInfo) {
  U32 Offset;
  U32 NumSectors;

  Offset = pInfo->Offset;
  NumSectors  = pInfo->NumSectors;
  pResult->ErrorCode = 0;  // This is the default value, so it is not obligatory to set it to 0
  //
  // Erase one or more sectors
  //
  if (NumSectors) {
    do {
      flash_erase_block(Offset,Sector_Erase);
      Offset += (1 << SECTOR_SIZE_SHIFT);
    } while (--NumSectors);
  } else {
    //
    // Erase complete flash chip
    //
    flash_erase(Offset,FALSH_SIZE_MAX);
  }
}

/*********************************************************************
*
*       FLASH_Prepare
*
*  Function description
*    Called once per RAMCode download
*
*    Sample RAMCode usage scenario:
*      - RAMCode download
*      - Prepare()
*      - Erase()
*      - Program()
*      - Read()
*      - Erase()
*      - Program()
*      - Restore()
*
*    For example, this function can be used to perform GPIO init
*    which is necessary to connect to the flash
*
*  Parameters
*    pContext  Pointer to 64-byte area which can be used to store variables accross the RAMCode FLASH_xxx functions.
*              Note that there may be 64 + pDesc->AlgoDesc.NumExtraBytes (FLASH_GetDesc()) available, depending on what FLASH_GetDesc() has requested.
*              The 64-byte area is initialized to 0 before FLASH_Prepare() and before FLASH_GetDesc() is called.
*
*  Notes
*    (1) Must not rely on FLASH_DetDesc() being called before.
*        Must assume that it is the very first FLASH_xxx function that is called.
*        So pContext is not already initialized at this point.
*/
void FLASH_Prepare(void* pContext) {
  (void)pContext;
  sysc_cmp_spif_io_en_setf(1);
  flash_init();
}

/*********************************************************************
*
*       FLASH_Restore
*
*  Function description
*    Called once per RAMCode download.
*
*    For example:
*      - RAMCode download
*      - Prepare()
*      - Erase()
*      - Program()
*      - Read()
*      - Erase()
*      - Program()
*      - Restore()
*
*    This function restores whatever has been initialized by FLASH_Prepare()
*    to leave the target in the same state as before the flash programming has been performed.
*
*  Parameters
*    pContext  Pointer to 64-byte area which can be used to store variables accross the RAMCode FLASH_xxx functions.
*              Note that there may be 64 + pDesc->AlgoDesc.NumExtraBytes (FLASH_GetDesc()) available, depending on what FLASH_GetDesc() has requested.
*              The 64-byte area is initialized to 0 before FLASH_Prepare() and before FLASH_GetDesc() is called.
*/
void FLASH_Restore(void* pContext) {
  (void)pContext;
}

/*********************************************************************
*
*       FLASH_FeedWatchdog
*
*  Function description
*    Feeds watchdog if enabled.
*    It is user's responsibility to call this function
*    during execution of the FLASH_Read(), FLASH_Program(),
*    FLASH_Erase() functions.
*
*  Parameters
*    pContext  Pointer to 64-byte area which can be used to store variables accross the RAMCode FLASH_xxx functions.
*              Note that there may be 64 + pDesc->AlgoDesc.NumExtraBytes (FLASH_GetDesc()) available, depending on what FLASH_GetDesc() has requested.
*              The 64-byte area is initialized to 0 before FLASH_Prepare() and before FLASH_GetDesc() is called.
*/
void FLASH_FeedWatchdog(void* pContext) {
  (void)pContext;
}

/**************************** End of file ***************************/
