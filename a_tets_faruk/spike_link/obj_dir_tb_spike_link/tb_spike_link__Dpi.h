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
    // DPI import at src/rtl/tb/common_header.svh:21:30
    extern void get_log_reg_write(const svOpenArrayHandle key_array, const svOpenArrayHandle value_array, int* num_elements_inserted);
    // DPI import at src/rtl/tb/common_header.svh:13:30
    extern void init();
    // DPI import at src/rtl/tb/common_header.svh:19:29
    extern svBit simulation_completed();
    // DPI import at src/rtl/tb/common_header.svh:17:30
    extern void step();

#ifdef __cplusplus
}
#endif

#endif  // guard
