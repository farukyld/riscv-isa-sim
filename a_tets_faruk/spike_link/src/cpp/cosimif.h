#include "../../obj_dir_tb_spike_link/tb_spike_link__Dpi.h"
#include "decode.h"

typedef 
  struct {
    reg_t key;
    freg_t value;
} commit_log_reg_item_t;
#define CMT_LOG_REG_ITEM_DPI_WORDS (sizeof(commit_log_reg_item_t)/sizeof(svBitVecVal))

typedef union {
  struct {
    reg_t addr;
    reg_t wdata;
    char len;
    } addr_wdata_len_tuple;
#define AWL_WIDTH_IN_CHAR (sizeof(reg_t)/sizeof(char)+sizeof(reg_t)/sizeof(char)+sizeof(char)/sizeof(char))
  char packed_addr_wdata_len_tuple[AWL_WIDTH_IN_CHAR];
#undef AWL_WIDTH_IN_CHAR
} commit_log_mem_item_t;
