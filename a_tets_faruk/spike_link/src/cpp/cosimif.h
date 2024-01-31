#include "../../obj_dir_tb_spike_link/tb_spike_link__Dpi.h"
#include "decode.h"

typedef union {
  struct {
    reg_t key;
    freg_t value;
    } key_value_pair;
#define KVP_WIDTH_IN_INT32 (sizeof(reg_t)/sizeof(int32_t)+sizeof(freg_t)/sizeof(int32_t))
  int32_t packed_key_value_pair[KVP_WIDTH_IN_INT32];
#undef KVP_WIDTH_IN_INT32
} commit_log_reg_item_t;

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
