#ifndef __HAL_FLASH_H____
#define __HAL_FALSH_H____
#include "stdint.h"
#define FLASH_SECURITY_SIZE_MAX  (4*256)
#define FLASH_SECURITY_PAGE_SIZE (256)
        
#define FLASH_PAGE_SIZE          (256)
#define FLASH_SECTOR_SIZE        (4 *1024)
#define FALSH_SIZE_4K            (4 *1024)
#define FALSH_SIZE_BLOCK_32K     (32*1024)
#define FALSH_SIZE_BLOCK_64K     (64*1024)

#define FALSH_SIZE_MAX		     (4 *1024*1024)


typedef enum
{
    FLASH_WRITE_NON_VOLATILE_SR = 0x01,
    FLASH_WRITE_VOLATILE_SR     = 0x50,
    FLASH_READ_STATUS_REG_0_7   = 0x05,
    FLASH_READ_STATUS_REG_8_15  = 0x35,
                              
    FLASH_WRITE_DISABLE         = 0x04,
    FLASH_WRITE_ENABLE          = 0x06,
                                
    FLASH_STANDARD_READ         = 0x03,
    FLASH_DUAL_READ             = 0x3B,
    FLASH_DUAL_IO_READ          = 0xBB,
    FLASH_QUAD_READ             = 0x6B,
    FLASH_QUAD_IO_READ          = 0xEB,
    FLASH_QUAD_IO_WORD_READ     = 0xE7,
                                
    FLASH_PAGE_PROGRAM          = 0x02,
    FLASH_QUAD_PAGE_PROGRAMM    = 0x32,
                                
    FLASH_SECTOR_ERASE          = 0x20,
    FLASH_32KB_BLOCK_ERASE      = 0x52,
    FLASH_64KB_BLOCK_ERASE      = 0xd8,
    FLASH_CHIP_ERASE            = 0x60,
                                
    FLASH_SUSPEND               = 0x75,//Program/Erase Suspend
    FLASH_RESUME                = 0x7A,//Program/Erase Resume
    FLASH_READ_ID               = 0x9F,//Read Identification
    FLASH_READ_DEVICE_ID        = 0x90,//Read Manufacture ID/Device ID
                                
    FLASH_SECURITY_ERASE        = 0x44,
    FLASH_SECURITY_PROGRAM      = 0x42,
    FLASH_SECURITY_READ         = 0x48,
                                
    FLASH_POWER_DOWN            = 0xB9,
    FLASH_POWER_ON              = 0xAB,
} nor_flash_cmd_t;


/* -----------  FLASH API  -----------------*/
void flash_chip_erase(void);
void flash_erase(uint32_t offset, uint32_t len);//@note: offset,len(Note:4K Aligned)
void flash_chip_ease(void);
void flash_read(uint32_t offset, uint8_t *buf, uint32_t len);
void flash_write(uint32_t offset, uint8_t *buf, uint32_t len);

void flash_init(void);

#endif /* __HAL_FALSH_H__ */
