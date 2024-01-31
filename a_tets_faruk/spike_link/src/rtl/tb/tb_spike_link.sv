
module tb_spike_link;
  import cosim_pkg::*;
  localparam DPI_W   = DPI_WORD_WIDTH;
  localparam KEY_W   = XREG_W;
  localparam VALUE_W = FREG_W;


  freg_t verilog_side_data [reg_t];

  commit_log_reg_item_t log_reg_write_from_c [0:CommitLogRegEntries-1];
  int num_elements_inserted_from_c_side;
  int num_elems_to_insert;
  reg_t temp_key;
  freg_t temp_value;
  initial begin: cosimulation
    $display("cosimulation started");
    init();

    for (int ii = 0;; ii = ii + 1) begin: simulation_loop
      if (simulation_completed()) begin
        $display("simulation completed");
        break;
      end
      
      verilog_side_data.delete();
      
      step();

      // get log_reg_write from c side
      get_log_reg_write(
        log_reg_write_from_c,
        num_elements_inserted_from_c_side
      );

      // add random numbers and random keys in verilog side data for at most 16 times
      num_elems_to_insert = $urandom_range(0, CommitLogRegEntries);
      for (int jj = 0; jj < num_elems_to_insert; jj = jj + 1) begin: random_number_insertion
        temp_key = {$urandom, $urandom};
        temp_value = {$urandom, $urandom, $urandom, $urandom};
        verilog_side_data[temp_key] = temp_value;
      end

      compare_log_reg_write(
        log_reg_write_from_c,
        num_elements_inserted_from_c_side,
        verilog_side_data
      );
    end
    $finish;
  end
  
endmodule

