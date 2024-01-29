#include "config.h"
#define COSIMIF
#include "sim.h"
#include "cosim.h"
#include "mmu.h"
#include "dts.h"
#include "remote_bitbang.h"
#include "byteorder.h"
#include "platform.h"
#include "libfdt.h"
#include "socketif.h"
#include <fstream>
#include <map>
#include <iostream>
#include <sstream>
#include <climits>
#include <cstdlib>
#include <cassert>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

// bunlari yapabilmek icin sim_t'nin constructor'unu da degistirmek gerekecek.
// static void cosim_handle_signal(int sig)
// {
//   if (ctrlc_pressed)
//     exit(-1);
//   ctrlc_pressed = true;
//   signal(sig, &cosim_handle_signal);
// }

// bu asagidakini cfg degisiyor mu diye bakmak icin koymustum
// start_pc yanlis deger hatasi icin

// reg_t sim_t::htif_get_entry_point()
// {
//   return htif_t::get_entry_point();
// }

void sim_t::prerun()
{
  if (!debug && log)
    set_procs_debug(true);

  htif_t::set_expected_xlen(isa.get_max_xlen());
}

// void sim_t::htif_start()
// {
//   htif_t::start();
// }

cosim_t::cosim_t(const cfg_t *cfg, bool halted,
                  std::vector<std::pair<reg_t, abstract_mem_t *>> mems,
                  std::vector<const device_factory_t *> plugin_device_factories,
                  const std::vector<std::string> &args,
                  const debug_module_config_t &dm_config, const char *log_path,
                  bool dtb_enabled, const char *dtb_file,
                  bool socket_enabled,
                  FILE *cmd_file)
    : sim_t(cfg, halted, mems, plugin_device_factories, args, dm_config, log_path, 
            dtb_enabled, dtb_file, socket_enabled, cmd_file)
{
}

void cosim_t::idle_single_step()
{
  std::cout << "cosim_t::idle_single_step" << std::endl;
  if (sim_t::done())
    return;

  // once cosim_ctrlc_pressed kismini duzeltmem gerekiyor.
  if (sim_t::debug /*|| ctrlc_pressed*/)
    sim_t::interactive();
  else
  {
    sim_t::step(1);
  }

  if (sim_t::remote_bitbang)
    sim_t::remote_bitbang->tick();
  // polimorfizm hatasi icin
  // std::cout << "sim_t::idle_single_step" << std::endl;
}

bool sim_t::communication_available()
{
  return htif_t::communication_available();
}

void sim_t::single_step_without_communication()
{
  std::cout << "sim_t::single_step_without_communication. object at: " << this << std::endl;
  htif_t::single_step_without_communication();
}

void sim_t::single_step_with_communication(std::queue<reg_t> *fromhost_queue, std::function<void(reg_t)> fromhost_callback)
{
  htif_t::single_step_with_communication(fromhost_queue, fromhost_callback);
}
