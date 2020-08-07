
#ifndef __REG_LN_QSPI_H__
#define __REG_LN_QSPI_H__

typedef union
{
    uint32_t val;
    struct
    {
        uint32_t    bytes               : 9  ; // 8  : 0
        uint32_t    dat_mod             : 2  ; // 10 : 9
        uint32_t    dummy               : 4  ; // 14 : 11
        uint32_t    mod_mod             : 2  ; // 16 : 15
        uint32_t    add_mod             : 2  ; // 18 : 17
        uint32_t    cmd_mod             : 2  ; // 20 : 19
        uint32_t    reserved_0          : 11 ; // 31 : 21
    } bit_field;
} t_ln_qspi_qsip_ctrl0;

typedef union
{
    uint32_t val;
    struct
    {
        uint32_t    wr_cmd              : 8  ; // 7  : 0
        uint32_t    rd_cmd              : 8  ; // 15 : 8
        uint32_t    mod_bits            : 8  ; // 23 : 16
        uint32_t    reserved_0          : 8  ; // 31 : 24
    } bit_field;
} t_ln_qspi_qsip_ctrl1;

typedef union
{
    uint32_t val;
    struct
    {
        uint32_t    fifo_rst_sw         : 1  ; // 0  : 0
        uint32_t    rd_dly              : 1  ; // 1  : 1
        uint32_t    div                 : 5  ; // 6  : 2
        uint32_t    reserved_0          : 25 ; // 31 : 7
    } bit_field;
} t_ln_qspi_qsip_ctrl2;

typedef union
{
    uint32_t val;
    struct
    {
        uint32_t    under               : 1  ; // 0  : 0
        uint32_t    over                : 1  ; // 1  : 1
        uint32_t    empty               : 1  ; // 2  : 2
        uint32_t    full                : 1  ; // 3  : 3
        uint32_t    afull               : 1  ; // 4  : 4
        uint32_t    idle                : 1  ; // 5  : 5
        uint32_t    cur_st              : 3  ; // 8  : 6
        uint32_t    reserved_0          : 23 ; // 31 : 9
    } bit_field;
} t_ln_qspi_qspi_status;

typedef union
{
    uint32_t val;
    struct
    {
        uint32_t    ar                  : 24 ; // 23 : 0
        uint32_t    reserved_0          : 8  ; // 31 : 24
    } bit_field;
} t_ln_qspi_qspi_addr_set;

typedef union
{
    uint32_t val;
    struct
    {
        uint32_t    dr                  : 32 ; // 31 : 0
    } bit_field;
} t_ln_qspi_qspi_data;


//--------------------------------------------------------------------------------

typedef struct
{
    volatile        t_ln_qspi_qsip_ctrl0                     qsip_ctrl0                             ; // 0x0
    volatile        t_ln_qspi_qsip_ctrl1                     qsip_ctrl1                             ; // 0x4
    volatile        t_ln_qspi_qsip_ctrl2                     qsip_ctrl2                             ; // 0x8
    volatile        t_ln_qspi_qspi_status                    qspi_status                            ; // 0xc
    volatile        t_ln_qspi_qspi_addr_set                  qspi_addr_set                          ; // 0x10
    volatile        t_ln_qspi_qspi_data                      qspi_data                              ; // 0x14
} t_hwp_ln_qspi_t;


//--------------------------------------------------------------------------------

static inline void ln_qspi_qsip_ctrl0_set(uint32_t base, uint32_t value)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl0.val = value;
}

static inline void ln_qspi_cmd_mod_setf(uint32_t base, uint8_t cmd_mod)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl0.bit_field.cmd_mod = cmd_mod;
}

static inline void ln_qspi_add_mod_setf(uint32_t base, uint8_t add_mod)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl0.bit_field.add_mod = add_mod;
}

static inline void ln_qspi_mod_mod_setf(uint32_t base, uint8_t mod_mod)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl0.bit_field.mod_mod = mod_mod;
}

static inline void ln_qspi_dummy_setf(uint32_t base, uint8_t dummy)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl0.bit_field.dummy = dummy;
}

static inline void ln_qspi_dat_mod_setf(uint32_t base, uint8_t dat_mod)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl0.bit_field.dat_mod = dat_mod;
}

static inline void ln_qspi_bytes_setf(uint32_t base, uint16_t bytes)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl0.bit_field.bytes = bytes;
}

static inline void ln_qspi_qsip_ctrl1_set(uint32_t base, uint32_t value)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl1.val = value;
}

static inline void ln_qspi_mod_bits_setf(uint32_t base, uint8_t mod_bits)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl1.bit_field.mod_bits = mod_bits;
}

static inline void ln_qspi_rd_cmd_setf(uint32_t base, uint8_t rd_cmd)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl1.bit_field.rd_cmd = rd_cmd;
}

static inline void ln_qspi_wr_cmd_setf(uint32_t base, uint8_t wr_cmd)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl1.bit_field.wr_cmd = wr_cmd;
}

static inline void ln_qspi_qsip_ctrl2_set(uint32_t base, uint32_t value)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl2.val = value;
}

static inline void ln_qspi_div_setf(uint32_t base, uint8_t div)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl2.bit_field.div = div;
}

static inline void ln_qspi_rd_dly_setf(uint32_t base, uint8_t rd_dly)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl2.bit_field.rd_dly = rd_dly;
}

static inline void ln_qspi_fifo_rst_sw_setf(uint32_t base, uint8_t fifo_rst_sw)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl2.bit_field.fifo_rst_sw = fifo_rst_sw;
}

static inline void ln_qspi_qspi_data_set(uint32_t base, uint32_t value)
{
    ((t_hwp_ln_qspi_t*)(base))->qspi_data.val = value;
}

static inline void ln_qspi_dr_set(uint32_t base, uint32_t dr)
{
    ((t_hwp_ln_qspi_t*)(base))->qspi_data.val = dr;
}
static inline void ln_qspi_ar_setf(uint32_t base, uint32_t ar)
{
    ((t_hwp_ln_qspi_t*)(base))->qspi_addr_set.bit_field.ar = ar;
}
static inline void ln_qspi_ar_set(uint32_t base, uint32_t ar)
{
    ((t_hwp_ln_qspi_t*)(base))->qspi_addr_set.val = ar;
}


//--------------------------------------------------------------------------------

static inline uint32_t ln_qspi_qsip_ctrl0_get(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl0.val;
}

static inline uint8_t ln_qspi_cmd_mod_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl0.bit_field.cmd_mod;
}

static inline uint8_t ln_qspi_add_mod_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl0.bit_field.add_mod;
}

static inline uint8_t ln_qspi_mod_mod_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl0.bit_field.mod_mod;
}

static inline uint8_t ln_qspi_dummy_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl0.bit_field.dummy;
}

static inline uint8_t ln_qspi_dat_mod_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl0.bit_field.dat_mod;
}

static inline uint16_t ln_qspi_bytes_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl0.bit_field.bytes;
}

static inline uint32_t ln_qspi_qsip_ctrl1_get(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl1.val;
}

static inline uint8_t ln_qspi_mod_bits_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl1.bit_field.mod_bits;
}

static inline uint8_t ln_qspi_rd_cmd_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl1.bit_field.rd_cmd;
}

static inline uint8_t ln_qspi_wr_cmd_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl1.bit_field.wr_cmd;
}

static inline uint32_t ln_qspi_qsip_ctrl2_get(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl2.val;
}

static inline uint8_t ln_qspi_div_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl2.bit_field.div;
}

static inline uint8_t ln_qspi_rd_dly_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl2.bit_field.rd_dly;
}

static inline uint8_t ln_qspi_fifo_rst_sw_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl2.bit_field.fifo_rst_sw;
}

static inline uint32_t ln_qspi_qspi_status_get(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qspi_status.val;
}

static inline uint8_t ln_qspi_cur_st_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qspi_status.bit_field.cur_st;
}

static inline uint8_t ln_qspi_idle_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qspi_status.bit_field.idle;
}

static inline uint8_t ln_qspi_full_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qspi_status.bit_field.full;
}

static inline uint8_t ln_qspi_empty_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qspi_status.bit_field.empty;
}

static inline uint8_t ln_qspi_over_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qspi_status.bit_field.over;
}

static inline uint8_t ln_qspi_under_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qspi_status.bit_field.under;
}

static inline uint32_t ln_qspi_qspi_data_get(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qspi_data.val;
}

static inline uint32_t ln_qspi_dr_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qspi_data.bit_field.dr;
}
static inline uint32_t ln_qspi_ar_getf(uint32_t base)
{
    return ((t_hwp_ln_qspi_t*)(base))->qspi_addr_set.bit_field.ar;
}


//--------------------------------------------------------------------------------

static inline void ln_qspi_qsip_ctrl0_pack(uint32_t base, uint8_t cmd_mod, uint8_t add_mod, uint8_t mod_mod, uint8_t dummy, uint8_t dat_mod, uint16_t bytes)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl0.val = ( \
                                    ((uint32_t )cmd_mod           << 19) \
                                  | ((uint32_t )add_mod           << 17) \
                                  | ((uint32_t )mod_mod           << 15) \
                                  | ((uint32_t )dummy             << 11) \
                                  | ((uint32_t )dat_mod           << 9) \
                                  | ((uint32_t )bytes) \
                                  );
}

static inline void ln_qspi_qsip_ctrl1_pack(uint32_t base, uint8_t mod_bits, uint8_t rd_cmd, uint8_t wr_cmd)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl1.val = ( \
                                    ((uint32_t )mod_bits          << 16) \
                                  | ((uint32_t )rd_cmd            << 8) \
                                  | ((uint32_t )wr_cmd) \
                                  );
}

static inline void ln_qspi_qsip_ctrl2_pack(uint32_t base, uint8_t div, uint8_t rd_dly, uint8_t fifo_rst_sw)
{
    ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl2.val = ( \
                                    ((uint32_t )div               << 2) \
                                  | ((uint32_t )rd_dly            << 1) \
                                  | ((uint32_t )fifo_rst_sw) \
                                  );
}


//--------------------------------------------------------------------------------

static inline void ln_qspi_qsip_ctrl0_unpack(uint32_t base, uint8_t * cmd_mod, uint8_t * add_mod, uint8_t * mod_mod, uint8_t * dummy, uint8_t * dat_mod, uint16_t * bytes)
{
    t_ln_qspi_qsip_ctrl0 local_val = ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl0;

    *cmd_mod            = local_val.bit_field.cmd_mod;
    *add_mod            = local_val.bit_field.add_mod;
    *mod_mod            = local_val.bit_field.mod_mod;
    *dummy              = local_val.bit_field.dummy;
    *dat_mod            = local_val.bit_field.dat_mod;
    *bytes              = local_val.bit_field.bytes;
}

static inline void ln_qspi_qsip_ctrl1_unpack(uint32_t base, uint8_t * mod_bits, uint8_t * rd_cmd, uint8_t * wr_cmd)
{
    t_ln_qspi_qsip_ctrl1 local_val = ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl1;

    *mod_bits           = local_val.bit_field.mod_bits;
    *rd_cmd             = local_val.bit_field.rd_cmd;
    *wr_cmd             = local_val.bit_field.wr_cmd;
}

static inline void ln_qspi_qsip_ctrl2_unpack(uint32_t base, uint8_t * div, uint8_t * rd_dly, uint8_t * fifo_rst_sw)
{
    t_ln_qspi_qsip_ctrl2 local_val = ((t_hwp_ln_qspi_t*)(base))->qsip_ctrl2;

    *div                = local_val.bit_field.div;
    *rd_dly             = local_val.bit_field.rd_dly;
    *fifo_rst_sw        = local_val.bit_field.fifo_rst_sw;
}

static inline void ln_qspi_qspi_status_unpack(uint32_t base, uint8_t * cur_st, uint8_t * idle, uint8_t * full, uint8_t * empty, uint8_t * over, uint8_t * under)
{
    t_ln_qspi_qspi_status local_val = ((t_hwp_ln_qspi_t*)(base))->qspi_status;

    *cur_st             = local_val.bit_field.cur_st;
    *idle               = local_val.bit_field.idle;
    *full               = local_val.bit_field.full;
    *empty              = local_val.bit_field.empty;
    *over               = local_val.bit_field.over;
    *under              = local_val.bit_field.under;
}

#endif

