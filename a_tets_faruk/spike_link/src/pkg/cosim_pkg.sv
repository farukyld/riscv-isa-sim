package cosim_pkg;

  parameter int unsigned XREG_W = 64; // spike'tan dolayi sabit
  parameter int unsigned FREG_W = 128; // spike'tan dolayi sabit
  parameter DPI_WORD_WIDTH = 32; // svdpi.h'den dolayi sabit


  // -verilator asagidaki hatayi verdigi icin CommitLogRegEntries tanimladim
  // -verilator: passing dynamic arrays to c function as argument not yet supported.

  parameter int unsigned CommitLogRegEntries = 16; // degistirilebilir. 16'yi gecmeyecegini dusundum

  typedef logic [XREG_W-1:0] reg_t;
  typedef logic [FREG_W-1:0] freg_t;

  typedef struct packed {
    reg_t   key;
    freg_t value;
  } commit_log_reg_item_t;


  typedef struct packed {
    reg_t addr;
    reg_t wdata;
    byte  len;
  } commit_log_mem_item_t;

  import "DPI-C" function void init();

  import "DPI-C" function void step();

  import "DPI-C" function bit simulation_completed();

  import "DPI-C" function void get_log_reg_write(
    output commit_log_reg_item_t log_reg_write_o[],
    output int inserted_elements_o
  );

  import "DPI-C" function void get_log_mem_read(
    output commit_log_mem_item_t log_mem_read_o[],
    output int inserted_elements_o
  );

  import "DPI-C" function void get_log_mem_write(
    output commit_log_mem_item_t log_mem_write_o[],
    output int inserted_elements_o
  );


  // automatic -> defined variables are auto
  // default (static) -> defined variables are shared among
  // concurrent calls of that function. 13.4.2
  function automatic logic [63:0] pack_2x32_to64 (
    logic [31:0] parts[0:1]
  );
    pack_2x32_to64 = {parts[1], parts[0]};
  endfunction

  function automatic logic [127:0] pack_4x32_to128 (
    logic [31:0] parts[0:3]
  );
    pack_4x32_to128 = {parts[3], parts[2], parts[1], parts[0]};
  endfunction


  task automatic compare_log_reg_write(
    input commit_log_reg_item_t correct_log_reg_write [0:CommitLogRegEntries-1],
    input int entry_count,
    input freg_t verilog_map [reg_t]
  );
    int ii; 
    for (ii = 0; ii < entry_count; ii = ii + 1) begin: comparison_itr
      
    end
  endtask

endpackage
