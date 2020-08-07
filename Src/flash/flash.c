#include "flash.h"
#include "reg_ln_qspi.h"

#define REG_QSPI_BASE  0x23004000
#define USE_FULL_ASSERT
#define MAX_READ_WORD 0x80
#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
  while (1)
  {
  }
}
#endif

#ifdef  USE_FULL_ASSERT
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif 

void flash_read_status_reg0(uint8_t *data)
{
    ln_qspi_qsip_ctrl0_pack(REG_QSPI_BASE, 0x0, 0x3, 0x3, 0xF, 0x0, 0x0);
    ln_qspi_qsip_ctrl1_pack(REG_QSPI_BASE, 0x0, FLASH_READ_STATUS_REG_0_7, 0x32);

    *data = ln_qspi_qspi_data_get(REG_QSPI_BASE);
}

void flash_read_status_reg1(uint8_t *data)
{
    ln_qspi_qsip_ctrl0_pack(REG_QSPI_BASE, 0x0, 0x3, 0x3, 0xF, 0x0, 0x0);
    ln_qspi_qsip_ctrl1_pack(REG_QSPI_BASE, 0x0, FLASH_READ_STATUS_REG_8_15, 0x32);

    *data = ln_qspi_qspi_data_get(REG_QSPI_BASE);
}
static void wait_qspi_bus_idle(void)
{
    while (0 == ln_qspi_idle_getf(REG_QSPI_BASE)) {};
}

static void flash_wait_qspi_flash_idle(void)
{
    uint8_t status = 0;
    wait_qspi_bus_idle();

    do {        
        flash_read_status_reg0(&status);     
    } while(status & 0x1);
}

static void flsh_internal_sector_erase(uint32_t offset)
{
    ln_qspi_qsip_ctrl0_pack(REG_QSPI_BASE, 0x0, 0x3, 0x3, 0xF, 0x3, 0x0);
    ln_qspi_qsip_ctrl1_pack(REG_QSPI_BASE, 0x0, 0x0, FLASH_WRITE_ENABLE); //write enable
    ln_qspi_dr_set(REG_QSPI_BASE, 0);
    wait_qspi_bus_idle();

    ln_qspi_qsip_ctrl0_pack(REG_QSPI_BASE, 0x0, 0x0, 0x3, 0xF, 0x3, 0x1ff);
    ln_qspi_qsip_ctrl1_pack(REG_QSPI_BASE, 0x0, 0x0, FLASH_SECTOR_ERASE);
    ln_qspi_ar_set(REG_QSPI_BASE, offset);
    ln_qspi_dr_set(REG_QSPI_BASE, 0);

    flash_wait_qspi_flash_idle();
}


//standard write byte
static void flash_internal_standard_write_byte(uint32_t offset, uint8_t byte)
{
    //set wel
    ln_qspi_qsip_ctrl0_pack(REG_QSPI_BASE, 0x0, 0x3, 0x3, 0xF, 0x3, 0x0);
    ln_qspi_qsip_ctrl1_pack(REG_QSPI_BASE, 0x0, 0x0, FLASH_WRITE_ENABLE); //write enable
    ln_qspi_dr_set(REG_QSPI_BASE, 0);
    flash_wait_qspi_flash_idle();

    ln_qspi_qsip_ctrl0_pack(REG_QSPI_BASE, 0x0, 0x0, 0x3, 0xF, 0x0, 0);
    ln_qspi_qsip_ctrl1_pack(REG_QSPI_BASE, 0x0, 0x0, FLASH_PAGE_PROGRAM);
    ln_qspi_ar_set(REG_QSPI_BASE, offset);
    ln_qspi_dr_set(REG_QSPI_BASE, byte);

    flash_wait_qspi_flash_idle();
}
// write words
static void flash_internal_write_word(uint32_t offset,uint32_t* buf,uint32_t len)
{
     uint32_t rd_byte_length = len * 4 - 1;
    //set wel
    ln_qspi_qsip_ctrl0_pack(REG_QSPI_BASE, 0x0, 0x3, 0x3, 0xF, 0x3, 0x0);
    ln_qspi_qsip_ctrl1_pack(REG_QSPI_BASE, 0x0, 0x0, FLASH_WRITE_ENABLE); //write enable
    ln_qspi_dr_set(REG_QSPI_BASE, 0);
    flash_wait_qspi_flash_idle();

    ln_qspi_qsip_ctrl0_pack(REG_QSPI_BASE, 0x0, 0x0, 0x3, 0xF, 0x0, rd_byte_length);
    ln_qspi_qsip_ctrl1_pack(REG_QSPI_BASE, 0x0, 0x0, FLASH_PAGE_PROGRAM);
    ln_qspi_ar_set(REG_QSPI_BASE, offset);

    for(int i = 0; i < len; i++)
    {
        ln_qspi_dr_set(REG_QSPI_BASE, *buf++);
    }
    flash_wait_qspi_flash_idle();

}
//standard read byte
static void flash_internal_standard_read_byte(uint32_t offset, uint8_t *buf, uint32_t len)
{
    uint32_t rd_byte_length = len - 1;
    uint32_t len_in_words = len / sizeof(uint32_t);
    uint8_t  remainder    = len % sizeof(uint32_t);
    uint32_t tempdata = 0;
    
    ln_qspi_qsip_ctrl0_pack(REG_QSPI_BASE, 0x0, 0x0, 0x3, 15, 0x0, rd_byte_length);
    ln_qspi_qsip_ctrl1_pack(REG_QSPI_BASE, 0x0, FLASH_STANDARD_READ, 0x0);

    ln_qspi_ar_set(REG_QSPI_BASE, offset);

    for (uint32_t i = 0; i < len_in_words; i++) {
        tempdata = ln_qspi_qspi_data_get(REG_QSPI_BASE);
        *buf++ = (uint8_t)(tempdata);
        *buf++ = (uint8_t)(tempdata >> 8);
        *buf++ = (uint8_t)(tempdata >> 16);
        *buf++ = (uint8_t)(tempdata >> 24);
    }

    if (remainder) {
        tempdata = ln_qspi_qspi_data_get(REG_QSPI_BASE);
    }
    
    for (; remainder > 0; remainder--) {
        *buf++ = (uint8_t)(tempdata);
        tempdata >>= 8;
    }

    flash_wait_qspi_flash_idle();
}

//dual mode
static void flash_internal_dual_read_word(uint32_t offset, uint32_t *buf, uint32_t len)
{
    uint32_t rd_byte_length = len * 4 - 1;

    ln_qspi_qsip_ctrl0_pack(REG_QSPI_BASE, 0x0, 0x1, 0x3, 0x3, 0x1, rd_byte_length);
    ln_qspi_qsip_ctrl1_pack(REG_QSPI_BASE, 0x0, FLASH_DUAL_IO_READ, 0x0);

    ln_qspi_ar_set(REG_QSPI_BASE, offset);

    for (int i = 0; i < len; i++) {
        *buf++ = ln_qspi_qspi_data_get(REG_QSPI_BASE);
    }
    flash_wait_qspi_flash_idle();
}

void flash_chip_ease(void)
{
    //set wel
    ln_qspi_qsip_ctrl0_pack(REG_QSPI_BASE, 0x0, 0x3, 0x3, 0xF, 0x3, 0x0);
    ln_qspi_qsip_ctrl1_pack(REG_QSPI_BASE, 0x0, 0x0, 0x6); //write enable
    ln_qspi_dr_set(REG_QSPI_BASE, 0);

    flash_wait_qspi_flash_idle();

    ln_qspi_qsip_ctrl0_pack(REG_QSPI_BASE, 0x0, 0x3, 0x3, 0xF, 0x3, 0x0);
    ln_qspi_qsip_ctrl1_pack(REG_QSPI_BASE, 0x0, 0x0, 0x60); //chip erase
    ln_qspi_dr_set(REG_QSPI_BASE, 0);

    flash_wait_qspi_flash_idle();
}
/////////////////////////////// flash API //////////////////////////////// 
void flash_erase(uint32_t offset, uint32_t len)
{
    assert_param(offset < FALSH_SIZE_MAX);
    uint32_t num_4k = len / FLASH_SECTOR_SIZE + ((len % FLASH_SECTOR_SIZE) ? 1 : 0);
    
    // sector erase
    for (uint32_t i = 0; i < num_4k; i++) {
        flsh_internal_sector_erase(offset);
        offset += FLASH_SECTOR_SIZE;
    }
    
    //TODO: block erace
    
}

void flash_read(uint32_t offset, uint8_t *buf, uint32_t len)
{
    assert_param(len >= 1);
    assert_param( (offset + len) <= FALSH_SIZE_MAX );
    
    uint8_t shift = ((uint32_t)buf & 0x03) ? (4 - (uint32_t)buf & 0x03) : 0;
    uint32_t length_in_words = (len - shift) / sizeof(uint32_t);
    uint8_t remainder = (len - shift) % sizeof(uint32_t);
    uint32_t cycles = (length_in_words / MAX_READ_WORD) + ((length_in_words % MAX_READ_WORD) ? 1 : 0);
    uint32_t i = 0;
    
    if (shift) {
        flash_internal_standard_read_byte(offset,buf,shift);
        offset += shift;
        buf += shift;
    }
    
    for(i = 0;i < cycles; i++)
    {
        uint32_t read_word = ((length_in_words < MAX_READ_WORD) ? length_in_words : MAX_READ_WORD);
        length_in_words -= read_word;
        flash_internal_dual_read_word(offset,(uint32_t *)buf,read_word);
        buf += sizeof(uint32_t)*read_word;
        offset += sizeof(uint32_t)*read_word;
    }
    if (remainder) {
        flash_internal_standard_read_byte(offset,buf,remainder);
    }
}

void flash_write(uint32_t offset, uint8_t *buf, uint32_t len)
{
    assert_param(offset < FALSH_SIZE_MAX);
    
    uint32_t page_base = offset&(~(FLASH_PAGE_SIZE-1));
    uint32_t tailing_length = (page_base + FLASH_PAGE_SIZE) - offset;
    uint32_t page_num = (len > tailing_length) ? ((len - tailing_length)/FLASH_PAGE_SIZE + ((len - tailing_length)%FLASH_PAGE_SIZE ? 1 : 0) + 1):1;
    uint8_t  shift;
    int i,j;
    
    for(i = 0 ; i < page_num; i++)
    {
        page_base = offset&(~(FLASH_PAGE_SIZE-1));
        tailing_length = (page_base + FLASH_PAGE_SIZE) - offset;
        tailing_length = (len < tailing_length) ? len : tailing_length;
        uint8_t word_length;
        uint8_t remain_length;
        if(tailing_length < 4)
        {
            for(j = 0; j < tailing_length; j++)
            {
                flash_internal_standard_write_byte(offset++, *(buf++));
            }
        }
        else
        {
            shift = ((uint32_t)buf & 0x03) ? (4 - (uint32_t)buf & 0x03) : 0;
            for(j = 0; j < shift; j++)
            {
                flash_internal_standard_write_byte(offset++, *(buf++));
            }
            word_length = (tailing_length - shift)/sizeof(uint32_t);
            remain_length = (tailing_length - shift)%sizeof(uint32_t);
            flash_internal_write_word(offset, (uint32_t *)buf, word_length);
            offset += (word_length << 2);
            buf += (word_length << 2);
            for(j = 0; j < remain_length; j++)
            {
                flash_internal_standard_write_byte(offset++, *(buf++));
            }
        }
        len -= tailing_length;
    }
}


void flash_init(void)
{
    //TODO: clock cfg？ FLASH QE enable？
}




#define BUF_SIZE  515
uint8_t rbuf[BUF_SIZE] = {0};
uint8_t wbuf[BUF_SIZE] = {0};
int ii, jj, zz;
void flash_test(void)
{

//    uint32_t sector_addr = 0;     
    uint32_t sector_addr = 0x100000; //1M 后面开始
    
    for (ii = 0; ii < BUF_SIZE; ii++)
    {
        wbuf[ii] = ii;
    }

    for (jj = 0; jj < 20; jj++) // erase 20次
    {
        flash_erase(sector_addr, 4*1024);
        
        flash_read(sector_addr, rbuf, BUF_SIZE);
        for (ii = 0; ii < BUF_SIZE; ii++)
            if (rbuf[ii] != 0xFF)
                 while (1);
            
        flash_write(sector_addr+10, wbuf+1, BUF_SIZE-1);

        flash_read(sector_addr+10, rbuf+1, BUF_SIZE-1);
        for (ii = 1; ii < BUF_SIZE; ii++)
            if (rbuf[ii] != (uint8_t)ii)
                while (1) ;

        sector_addr += 0x1000;
    }
}




