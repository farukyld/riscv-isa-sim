
#define COSIMIF
#include "sim.h"

class cosim_t : public sim_t
{
public:
// public:
//   sim_t(const cfg_t *cfg, bool halted,
//         std::vector<std::pair<reg_t, abstract_mem_t*>> mems,
//         std::vector<const device_factory_t*> plugin_device_factories,
//         const std::vector<std::string>& args,
//         const debug_module_config_t &dm_config, const char *log_path,
//         bool dtb_enabled, const char *dtb_file,
//         bool socket_enabled,
//         FILE *cmd_file); // needed for command line option --cmd
//   ~sim_t();
// put those constructors that directly calls sim_t constructor

  cosim_t(const cfg_t *cfg, bool halted,
        std::vector<std::pair<reg_t, abstract_mem_t*>> mems,
        std::vector<const device_factory_t*> plugin_device_factories,
        const std::vector<std::string>& args,
        const debug_module_config_t &dm_config, const char *log_path,
        bool dtb_enabled, const char *dtb_file,
        bool socket_enabled,
        FILE *cmd_file);

private:
  virtual void idle_single_step() override;
};