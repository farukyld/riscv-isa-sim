
module tb_spike_link;
  import cosim_pkg::*;
  localparam DPI_W   = DPI_WORD_WIDTH;
  localparam KEY_W   = XREG_W;
  localparam VALUE_W = FREG_W;


  freg_t verilog_side_data [reg_t];

  commit_log_reg_item_t log_reg_write_from_c [0:CommitLogRegEntries-1];
  commit_log_mem_item_t log_mem_read_from_c [0:CommitLogRegEntries-1];
  int num_elements_inserted_from_c_side;
  int num_elems_to_insert;
  reg_t temp_key;
  freg_t temp_value;


  import "DPI-C" function void look_array(int array[]);
  
  typedef struct { // !!! !!! packed yazinca tersten pack'liyor !!!
    int a;
    int d;
  } nonstd_type_t;
  import "DPI-C" function void look_array_of_nonstd_type(nonstd_type_t array[]);

  nonstd_type_t array_of_nonstd_type [0:3];
  int array[0:3];

  initial begin: cosimulation


    $display("cosimulation started");
    $display("***************");
    array[0] = 32'd123;
    $display("verilog side: calling look_array");
    look_array(array);
    $display("vevrilog side: array element 0 after calling look_array: %0d", array[0]);

    array_of_nonstd_type[0].a = 32'd123;
    $display("verilog side: calling look_array_of_nonstd_type");
    look_array_of_nonstd_type(array_of_nonstd_type);
    $display("verilog side: array_of_nonstd_type element 0 after calling look_array_of_nonstd_type: %0d", array_of_nonstd_type[0].a);
    $display("***************");



    init();

    for (int ii = 0;ii < 1; ii = ii + 1) begin: simulation_loop
      if (simulation_completed()) begin
        $display("simulation completed");
        break;
      end
      
      // verilog_side_data.delete();
      
      step();

      // get log_reg_write from c side
      log_reg_write_from_c[0].key = 64'b11 << 32 | 64'b11;
      log_reg_write_from_c[0].value = 128'b11 << 96 | 128'b11 << 64 | 128'b11 << 32 | 128'b11;
      $display("array[0].key %13d", log_reg_write_from_c[0].key);
      $display("array[0].value %13d", log_reg_write_from_c[0].value);
      get_log_reg_write(
        log_reg_write_from_c,
        num_elements_inserted_from_c_side
      );

      // // add random numbers and random keys in verilog side data for at most 16 times
      // num_elems_to_insert = $urandom_range(0, CommitLogRegEntries);
      // for (int jj = 0; jj < num_elems_to_insert; jj = jj + 1) begin: random_number_insertion
      //   temp_key = {$urandom, $urandom};
      //   temp_value = {$urandom, $urandom, $urandom, $urandom};
      //   verilog_side_data[temp_key] = temp_value;
      // end

      // compare_log_reg_write(
      //   log_reg_write_from_c,
      //   num_elements_inserted_from_c_side,
      //   verilog_side_data
      // );
      
      temp_key = log_reg_write_from_c[0].key;
      temp_value = log_reg_write_from_c[0].value;
      $display("temp_key: %d", temp_key);
      $display("temp_value: %d", temp_value);

    end
    $finish;
  end
  
endmodule

