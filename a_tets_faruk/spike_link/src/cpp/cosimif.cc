#include "cosimif.h"
#include "cosim_create_sim.h"
#include "args_reader.h"
#include "debug_header.h"

#ifndef KEY_W
#define KEY_W sizeof(reg_t)*8
#endif

#ifndef VALUE_W
#define VALUE_W sizeof(freg_t)*8
#endif

#ifndef DPI_W
#define DPI_W sizeof(svBitVecVal)*8
#endif

sim_t *simulation_object;

std::function<void()> step_callback;

// !!! bu iki variable her ne kadar init'den baska bir yerde kullanilmiyor gibi gozukse de
// aslinda step_callback'e bind edildigi icin oralarda kullaniliyor.
// dolayisiyla bu iki variable, init local scope'dan disinda da kullanilabilir olmali.
// bu yuzden global scope'ta tanimliyorum.
std::queue<reg_t> fromhost_queue;
// !!! fromhost_calback reg_t alan, void donduren bir std::function
std::function<void(reg_t)> fromhost_callback;

void init()
{
  auto argc_argv = read_args_from_file("/home/usr1/riscv-isa-sim/a_tets_faruk/spike_link/log/args.txt");

  simulation_object = create_sim_with_args(argc_argv->argc, argc_argv->argv);

  #if DEBUG_LEVEL >= DEBUG_WARN
  std::cout <<__FILE__ <<":" <<__LINE__<< " sim.cfg.startpc.hasval: " << simulation_object->get_cfg().start_pc.has_value() << std::endl;
  std::cout << "simulation object created at: " << simulation_object << std::endl;
  #endif
  simulation_object->prerun();

  #if DEBUG_LEVEL >= DEBUG_WARN
  std::cout <<__FILE__ <<":" <<__LINE__<< " sim.cfg.startpc.hasval: " << simulation_object->get_cfg().start_pc.has_value() << std::endl;
  #endif


  ((htif_t*)simulation_object)->start();

  #if DEBUG_LEVEL >= DEBUG_WARN
  std::cout <<__FILE__ <<":" <<__LINE__<< " sim.cfg.startpc.hasval: " << simulation_object->get_cfg().start_pc.has_value() << std::endl;
  #endif


  auto enq_func = [](std::queue<reg_t> *q, uint64_t x)
  { q->push(x); };

  // !!! first argument of enq_func is bind to fromhost_queue.
  // second argument is a placeholder for the reg_t argument of fromhost_callback.
  // caller passes an actual argument to the first (_1) argument of the fromhost_callback.
  fromhost_callback = std::bind(enq_func, &fromhost_queue, std::placeholders::_1);
  // !!! yani fromhost_callback, fromhost_queue'ye
  // reg_t turunden bir seyler push'lamaya yariyor

  if (((htif_t*)simulation_object)->communication_available())
  {
    printf("communication_available() is true\n");
    // htif_t pointer'ine type-cast yapmaya gerek yoktu muhtemelen ama acik acik gostermek istedim
    step_callback = std::bind(&htif_t::single_step_with_communication, (htif_t*)simulation_object, &fromhost_queue, fromhost_callback);
  }
  else
  {
    printf("communication_available() is false\n");
    step_callback = std::bind(&htif_t::single_step_without_communication, (htif_t*)simulation_object);
  }
}

void step()
{
  #if DEBUG_LEVEL >= DEBUG_WARN
  std::cout << "cosimif.cc: step. callback address: " << &step_callback << std::endl;
  #endif
  step_callback();
}

svBit simulation_completed()
{
  return ((htif_t*)simulation_object)->exitcode_not_zero();
}

void look_array(const svOpenArrayHandle array){
  std::cout <<"c side: array.size0: " << svSize(array,0) << std::endl;
  int* arr_elm_ptr = (int*)svGetArrElemPtr1(array,0);
  std::cout << "c side: arr_elm_ptr: " << arr_elm_ptr << std::endl;
  std::cout << "c side: arr_elm_ptr dereferenced: " << *arr_elm_ptr << std::endl;
  std::cout << "c side: changing arr element "<< std::endl;
  *arr_elm_ptr= 33;
}


void look_array_of_nonstd_type(const svOpenArrayHandle array){
  std::cout <<"c side: array.size0: " << svSize(array,0) << std::endl;
  svLogicVec32* arr_elm_ptr = (svLogicVec32*)svGetArrElemPtr1(array,0);
  std::cout << "c side: arr_elm_ptr: " << arr_elm_ptr << std::endl;
  std::cout << "c side: arr_elm_ptr dereferenced: " << (*arr_elm_ptr).c << std::endl;
  std::cout << "c side: changing arr element "<< std::endl;
  (*arr_elm_ptr).c = 33;
}

void get_log_reg_write(const svOpenArrayHandle log_reg_write_o, int* inserted_elements_o)
{
  auto map_from_c_side = simulation_object->get_core(0)->get_state()->log_reg_write;

  DEBUG_PRINT_WARN("burada iki tarafin boyutlari icin asertion konulabilir\n");

  int& num_entries = *inserted_elements_o;
  num_entries = 0;
  // log_reg_write_o'nun her bir elementi 192 bit. ilk 64 bit key, sonraki 128 bit value.
  for (auto x: map_from_c_side){
    void* ptr1 = svGetArrayPtr(log_reg_write_o);
    std::cout << "ptr1: " << ptr1 << std::endl;
    commit_log_reg_item_t& array_entry = *(commit_log_reg_item_t*)svGetArrElemPtr1(log_reg_write_o, 0);
    std::cout << "array_entry_ptr: " << &array_entry << std::endl;
    std::cout << "----------array_entry: " << array_entry.packed_key_value_pair[0] << std::endl;
    
    
    // array_entry'ye yazacagiz.

    // svPutPartselBit(array_entry, 1234, 0, DPI_W); // seg fault.
    // svPutPartselBit(array_entry, key_value_pair.packed_key_value_pair[1], KEY_W, VALUE_W);

    num_entries++;
  }
}

void get_log_mem_read(const svOpenArrayHandle log_mem_read_o, int* inserted_elements_o){
  auto map_from_c_side = simulation_object->get_core(0)->get_state()->log_mem_read;

  DEBUG_PRINT_WARN("burada iki tarafin boyutlari icin asertion konulabilir\n");

  int& num_entries = *inserted_elements_o;


}


void get_log_mem_write(const svOpenArrayHandle log_reg_write_o, int* inserted_elements_o){}
