package cosim_pkg;

  parameter int unsigned XREG_W = 64; // spike'tan dolayi sabit
  parameter int unsigned FREG_W = 128; // spike'tan dolayi sabit
  parameter DPI_W = 32; // svdpi.h'den dolayi sabit


  // -verilator asagidaki hatayi verdigi icin CommitLogEntries tanimladim
  // -verilator: passing dynamic arrays to c function as argument not yet supported.

  parameter int unsigned CommitLogEntries = 16; // degistirilebilir. 16'yi gecmeyecegini dusundum


  /*verilog tarafinda kullanilacak*/
  typedef bit [XREG_W-1:0] reg_t; // +packed +2-state ?little endian?
  typedef bit [FREG_W-1:0] freg_t;

  typedef struct packed {
    reg_t   key;
    freg_t value;
  } commit_log_reg_item_t;
  
  typedef  struct packed {
    reg_t addr;
    reg_t wdata;
    byte  len;
  } commit_log_mem_item_t;
  /**/


  typedef int dpi_word_t;

  parameter LOG_REG_WRITE_ITEM_DPI_WORDS = XREG_W/DPI_W + FREG_W/DPI_W;

  parameter LOG_MEM_ITEM_INTS_DPI_WORDS = 2*XREG_W/DPI_W + 1;

  import "DPI-C" function void init();

  import "DPI-C" function void step();

  import "DPI-C" function bit simulation_completed();

  import "DPI-C" function void private_get_log_reg_write(
    output int log_reg_write_o [][LOG_REG_WRITE_ITEM_DPI_WORDS],
    output int inserted_elements_o,
    input int processor_i
  );

  import "DPI-C" function void private_get_log_mem_read(
    output commit_log_mem_item_t log_mem_read_o[],
    output int inserted_elements_o,
    input int processor_i
  );

  import "DPI-C" function void private_get_log_mem_write(
    output dpi_word_t log_mem_write_o [][],
    output int inserted_elements_o,
    input int processor_i
  );

  import "DPI-C" function void wait_key();


  function void get_log_reg_write(
    output commit_log_reg_item_t log_reg_write_o[CommitLogEntries],
    output int inserted_elements_o
  );
    // burda private_get_log_reg_write_from_c cagirilacak
    // ciktisi log_reg_write_o'ya yazilacak
    dpi_word_t log_reg_write_from_c [CommitLogEntries][LOG_REG_WRITE_ITEM_DPI_WORDS];
    int n;

    private_get_log_reg_write(log_reg_write_from_c, inserted_elements_o, 0);
    
    for (int ii = 0; ii < inserted_elements_o; ii = ii + 1) begin: log_reg_write_itr
      log_reg_write_o[ii].key = pack_2x32_to64le(log_reg_write_from_c[ii][0:1]);
      log_reg_write_o[ii].value = pack_4x32_to128le(log_reg_write_from_c[ii][2:5]);
    end
  endfunction
  

  function void get_log_mem_read(
    output commit_log_mem_item_t log_mem_read_o[CommitLogEntries],
    output int inserted_elements_o
  );
    // burda private_get_log_mem_read_from_c cagirilacak
    // ciktisi log_mem_read_o'ya yazilacak
    dpi_word_t log_mem_read_from_c [CommitLogEntries][LOG_MEM_ITEM_INTS_DPI_WORDS];
    int n;

    private_get_log_mem_read(log_mem_read_from_c, inserted_elements_o, 0);

    for (int ii = 0; ii < inserted_elements_o; ii = ii + 1) begin: log_mem_read_itr
      log_mem_read_o[ii].addr = pack_2x32_to64le(log_mem_read_from_c[ii][0:1]);
      log_mem_read_o[ii].wdata = pack_2x32_to64le(log_mem_read_from_c[ii][2:3]);
      // !!! truncaiton hatasi verebilir
      log_mem_read_o[ii].len = log_mem_read_from_c[ii][4][7:0];
    end

  endfunction


  function void get_log_mem_write(
    output commit_log_mem_item_t log_mem_write_o[CommitLogEntries],
    output int inserted_elements_o
  ); 
    // burda private_get_log_mem_write_from_c cagirilacak
    // ciktisi log_mem_write_o'ya yazilacak
    dpi_word_t log_mem_write_from_c [CommitLogEntries][LOG_MEM_ITEM_INTS_DPI_WORDS];
    int n;

    private_get_log_mem_write(log_mem_write_from_c, inserted_elements_o, 0);

    for (int ii = 0; ii < inserted_elements_o; ii = ii + 1) begin: log_mem_write_itr
      log_mem_write_o[ii].addr = pack_2x32_to64le(log_mem_write_from_c[ii][0:1]);
      log_mem_write_o[ii].wdata = pack_2x32_to64le(log_mem_write_from_c[ii][2:3]);
      // !!! truncaiton hatasi verebilir
      log_mem_write_o[ii].len = log_mem_write_from_c[ii][4][7:0];
    end

  endfunction


  // automatic -> defined variables are auto
  // default (static) -> defined variables are shared among
  // concurrent calls of that function. 13.4.2
  function automatic logic [63:0] pack_2x32_to64le (
    logic [31:0] parts[0:1]
  );
    pack_2x32_to64le = {parts[1], parts[0]};
  endfunction

  function automatic logic [127:0] pack_4x32_to128le (
    logic [31:0] parts[0:3]
  );
    pack_4x32_to128le = {parts[3], parts[2], parts[1], parts[0]};
  endfunction


  task automatic compare_log_reg_write(
    input commit_log_reg_item_t correct_log_reg_write [CommitLogEntries],
    input int entry_count,
    input freg_t verilog_map [reg_t]
  );
    int ii; 
    for (ii = 0; ii < entry_count; ii = ii + 1) begin: comparison_itr
      
    end
  endtask

endpackage
