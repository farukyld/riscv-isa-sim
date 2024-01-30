`ifndef COMMON_HEADER_SVH
`define COMMON_HEADER_SVH

`define KEY_WIDTH 64
`define VALUE_WIDTH 128
`define DPI_WIDTH 32
`define MAX_ENTRY_COUNT 16

typedef logic [`KEY_WIDTH-1:0]   key_t;
typedef logic [`VALUE_WIDTH-1:0] value_t;

// void init()
import "DPI-C" function void init();
// !!! daha sonra bunu daha kullanilabilir hale getirmek icin bu sekile donust
// import "DPI-C" function int init(input int argc, input string argv[]);

import "DPI-C" function void step();

import "DPI-C" function bit simulation_completed();

import "DPI-C" function void get_log_reg_write(
  output logic [`KEY_WIDTH-1:0]   key_array  [][],
  output logic [`VALUE_WIDTH-1:0] value_array[][],
  output int num_elements_inserted
);

`endif
