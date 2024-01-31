
module tb_spike_link;
  import cosim_pkg::*;
  localparam KEY_W   = XREG_W;
  localparam VALUE_W = FREG_W;


  // freg_t verilog_side_data [reg_t];

  commit_log_reg_item_t log_reg_write_from_c [CommitLogRegEntries];
  commit_log_mem_item_t log_mem_read_from_c [CommitLogRegEntries];
  commit_log_mem_item_t log_mem_write_from_c [CommitLogRegEntries];
  int num_elements_inserted_from_c_side;
  // int num_elems_to_insert;
  reg_t temp_key;
  freg_t temp_value;

  initial begin: cosimulation

    init();

    for (int ii = 0;; ii = ii + 1) begin: simulation_loop
      if (simulation_completed()) begin
        $display("simulation completed");
        break;
      end
      
      // verilog_side_data.delete();
      
      step();

      // get log_reg_write from c side
      // log_reg_write_from_c[0].key = 64'b11 << 32 | 64'b11;
      // log_reg_write_from_c[0].value = 128'b11 << 96 | 128'b11 << 64 | 128'b11 << 32 | 128'b11;
      // $display("array[0].key %13d", log_reg_write_from_c[0].key);
      // $display("array[0].value %13d", log_reg_write_from_c[0].value);
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
      
      // temp_key = log_reg_write_from_c[0].key;
      // temp_value = log_reg_write_from_c[0].value;
      // $display("temp_key: %d", temp_key);
      // $display("temp_value: %d", temp_value);

    end
    $finish;
  end
  
endmodule

