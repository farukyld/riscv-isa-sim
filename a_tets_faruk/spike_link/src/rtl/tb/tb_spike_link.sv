
`include "src/rtl/tb/common_header.svh"


module tb_spike_link;

  localparam DPI_WIDTH       = `DPI_WIDTH;
  localparam KEY_WIDTH       = `KEY_WIDTH;
  localparam VALUE_WIDTH     = `VALUE_WIDTH;

  // -verilator asagidaki hatayi verdigi icin MAX_ENTRY_COUNT tanimladim
  // -verilator: passing dynamic arrays to c function as argument not yet supported.
  localparam MAX_ENTRY_COUNT = 16;
  localparam CLK_PERIOD      = 10;
  always #(CLK_PERIOD/2) clk_i = ~clk_i;


  reg                     clk_i;
  reg                     rst_ni;
  wire  [KEY_WIDTH-1:0]   last_wa1_o;
  wire  [VALUE_WIDTH-1:0] last_wd1_o;
  wire                    last_we1_o;
  wire  [KEY_WIDTH-1:0]   last_wa2_o;
  wire  [VALUE_WIDTH-1:0] last_wd2_o;
  wire                    last_we2_o;

  rng #(
    .DPI_WIDTH   (DPI_WIDTH),
    .KEY_WIDTH   (KEY_WIDTH),
    .VALUE_WIDTH (VALUE_WIDTH),
  ) rng_inst (
    .clk_i       (clk_i),
    .rst_ni      (rst_ni),
    .last_wa1_o  (last_wa1_o),
    .last_wd1_o  (last_wd1_o),
    .last_we1_o  (last_we1_o),
    .last_wa2_o  (last_wa2_o),
    .last_wd2_o  (last_wd2_o),
    .last_we2_o  (last_we2_o)
  );

  value_t verilog_side_data [key_t];

  //     packed dimension size (dim0)              num entries (dim1)   entry size in packets (dim2)
  logic [DPI_WIDTH-1:0]   key_array_from_c_side   [0:MAX_ENTRY_COUNT-1][0:KEY_WIDTH/DPI_WIDTH-1];
  logic [DPI_WIDTH-1:0]   value_array_from_c_side [0:MAX_ENTRY_COUNT-1][0:VALUE_WIDTH/DPI_WIDTH-1];
  int num_elements_inserted_from_c_side;

  initial begin: cosimulation
  
    $display("==========================================");
    $display("=                                        =");
    $display("==========================================");
    rst_ni = 0;
    #CLK_PERIOD;
    clk_i = 0;
    rst_ni = 1;
    init();

    for (int ii = 0;; ii = ii + 1) begin: simulation_loop
      if (simulation_completed()) begin
        $display("simulation completed");
        break;
      end
      #CLK_PERIOD;
      verilog_side_data.delete();
      step();
      get_log_reg_write(
        key_array_from_c_side,
        value_array_from_c_side,
        num_elements_inserted_from_c_side
      );
      if (last_we1_o) begin
        verilog_side_data[last_wa1_o] = last_wd1_o;
      end
      if (last_we2_o) begin
        verilog_side_data[last_wa2_o] = last_wd2_o;
      end

      compare_single_step(
        key_array_from_c_side,
        value_array_from_c_side,
        num_elements_inserted_from_c_side,
        verilog_side_data
      );
    end
    $finish;
  end
  
  

endmodule


task automatic compare_single_step(
  input logic [`DPI_WIDTH-1:0] correct_key_array [0:`MAX_ENTRY_COUNT-1][0:`KEY_WIDTH/`DPI_WIDTH-1],
  input logic [`DPI_WIDTH-1:0] correct_value_array [0:`MAX_ENTRY_COUNT-1][0:`VALUE_WIDTH/`DPI_WIDTH-1],
  input int entry_count,
  input value_t verilog_map [key_t]
);
  int ii; 
  for (ii = 0; ii < entry_count; ii = ii + 1) begin: comparison_itr
      key_t key = pack_2x32_to64(correct_key_array[ii]);
      /* verilator lint_off WIDTHTRUNC */
      if ((!(verilog_map.exists(key)))) begin
      /* verilator lint_on WIDTHTRUNC */

        // $display(
        //   "key: %0d inserted from c side not in verilog side\n", key
        // );
      // key exists, but values are not equal
      end else if (verilog_map[key] != pack_4x32_to128(correct_value_array[ii])) begin
        // $display(
        //   "key-value-pair: (%0d, %0d) inserted from c side",
        //   key,
        //   pack_4x32_to128(correct_value_array[ii]),
        // );
        // $display("not equal to the one in verilog");
        // $display(
        //   "side (verilog value = %0d)\n",verilog_map[key]
        // );

      // key exists, values are equal
      end else begin
        // $display(
        //   "success on key-value-pair: (%0d, %0d)\n",
        //   key,
        //   verilog_map[key]
        // );
      end
    end
endtask

// automatic -> defined variables are auto
// default (static) -> defined variables are shared among
// concurrent calls of that function. 13.4.2
function automatic logic [63:0] pack_2x32_to64 (
  input  logic [31:0] parts[0:1]
);
  pack_2x32_to64 = {parts[1], parts[0]};
endfunction

function automatic logic [127:0] pack_4x32_to128 (
  input  logic [31:0] parts[0:3]
);
  pack_4x32_to128 = {parts[3], parts[2], parts[1], parts[0]};
endfunction
