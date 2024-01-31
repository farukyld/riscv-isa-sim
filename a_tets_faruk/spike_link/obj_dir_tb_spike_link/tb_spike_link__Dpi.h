// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Prototypes for DPI import and export functions.
//
// Verilator includes this file in all generated .cpp files that use DPI functions.
// Manually include this file where DPI .c import functions are declared to ensure
// the C functions match the expectations of the DPI imports.

#ifndef VERILATED_TB_SPIKE_LINK__DPI_H_
#define VERILATED_TB_SPIKE_LINK__DPI_H_  // guard

#include "svdpi.h"

#ifdef __cplusplus
extern "C" {
#endif


    // DPI IMPORTS
    // DPI import at src/rtl/pkg/cosim_pkg.sv:39:32
    extern void get_log_mem_read(svLogicVecVal* log_mem_read_o, int* inserted_elements_o);
    // DPI import at src/rtl/pkg/cosim_pkg.sv:44:32
    extern void get_log_mem_write(svLogicVecVal* log_mem_write_o, int* inserted_elements_o);
    // DPI import at src/rtl/pkg/cosim_pkg.sv:34:32
    extern void get_log_reg_write(svLogicVecVal* log_reg_write_o, int* inserted_elements_o);
    // DPI import at src/rtl/pkg/cosim_pkg.sv:28:32
    extern void init();
    // DPI import at src/rtl/pkg/cosim_pkg.sv:32:31
    extern svBit simulation_completed();
    // DPI import at src/rtl/pkg/cosim_pkg.sv:30:32
    extern void step();

#ifdef __cplusplus
}
#endif

#endif  // guard
