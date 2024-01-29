#ifndef COSIMIF_H
#define COSIMIF_H

#ifndef DONT_USE_VERILATOR 
#error !!! bu header testbench tarafindan kullanilmak niyetiyle yazilmamistir!!
#endif

void init();

void step();

#endif // COSIMIF_H