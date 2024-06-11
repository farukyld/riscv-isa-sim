#include "devices.h"
#include "mmu.h"
#include <stdexcept>

mmio_device_map_t& mmio_device_map()
{
  // !!! boyle bir seye ne gerek vardi? direkt global bir 
  // degisken olarak tanimlamak yerine?
  static mmio_device_map_t device_map;
  return device_map;
}

void bus_t::add_device(reg_t addr, abstract_device_t* dev)
{
  // Searching devices via lower_bound/upper_bound
  // implicitly relies on the underlying std::map 
  // container to sort the keys and provide ordered
  // iteration over this sort, which it does. (python's
  // SortedDict is a good analogy)
  devices[addr] = dev;
}

bool bus_t::load(reg_t addr, size_t len, uint8_t* bytes)
{
  // Find the device with the base address closest to but
  // less than addr (price-is-right search)
  auto it = devices.upper_bound(addr);
  if (devices.empty() || it == devices.begin()) {
    // Either the bus is empty, or there weren't 
    // any items with a base address <= addr
    // !!! it == begin demek:
    // addr, butun cihazlardan kucuk demek. upper_bound, key'i
    //  addr'den buyuk ilk cihazi donduruyor.
    // ama biz addr'den kucuk en buyuk cihazi istiyoruz, 
    // demek ki oyle bir cihaz yok.
    return false;
  }
  // Found at least one item with base address <= addr
  // The iterator points to the device after this, so
  // go back by one item.
  it--;
  // !!! it yukaridaki satiri calistirmadan once, 
  // addr'den buyuk en kucuk cihazdi, yukaridaki satiri 
  // calistirinca, addr'den kucuk en buyuk cihaz oldu.
  // yani addr'nin hangi cihaza tekabul ettigini bulduk.
  return it->second->load(addr - it->first, len, bytes);
}

bool bus_t::store(reg_t addr, size_t len, const uint8_t* bytes)
{
  // See comments in bus_t::load
  auto it = devices.upper_bound(addr);
  if (devices.empty() || it == devices.begin()) {
    return false;
  }
  it--;
  return it->second->store(addr - it->first, len, bytes);
}

std::pair<reg_t, abstract_device_t*> bus_t::find_device(reg_t addr)
{
  // See comments in bus_t::load
  auto it = devices.upper_bound(addr);
  if (devices.empty() || it == devices.begin()) {
    return std::make_pair((reg_t)0, (abstract_device_t*)NULL);
  }
  it--;
  return std::make_pair(it->first, it->second);
}

mem_t::mem_t(reg_t size)
  : sz(size)
{
  if (size == 0 || size % PGSIZE != 0)
    throw std::runtime_error("memory size must be a positive multiple of 4 KiB");
}

mem_t::~mem_t()
{
  for (auto& entry : sparse_memory_map)
    free(entry.second);
}

bool mem_t::load_store(reg_t addr, size_t len, uint8_t* bytes, bool store)
{
  if (addr + len < addr || addr + len > sz)
    return false;

  while (len > 0) {
    auto n = std::min(PGSIZE - (addr % PGSIZE), reg_t(len));

    if (store)
      memcpy(this->contents(addr), bytes, n);
    else
      memcpy(bytes, this->contents(addr), n);

    addr += n;
    bytes += n;
    len -= n;
  }

  return true;
}

// !!! addr: paddr
// dondurulen adres, host device'daki konum. 
// not: paddr, page'lenmis bir sekilde bulunuyor. 
// yani, void* addr = contents(paddr) dedigimizde,
// hunharca addr'i kullanamayiz. zaten contents metodu
// iki yerde kullaniliyor. (mem_t::load_store ve 
// sim_t::addr_to_mem ) ikisinde de buna dikkat ediliyor.
char* mem_t::contents(reg_t addr) {
  reg_t ppn = addr >> PGSHIFT, pgoff = addr % PGSIZE;
  auto search = sparse_memory_map.find(ppn);
  if (search == sparse_memory_map.end()) {
    auto res = (char*)calloc(PGSIZE, 1);
    if (res == nullptr)
      throw std::bad_alloc();
    sparse_memory_map[ppn] = res;
    return res + pgoff;
  }
  return search->second + pgoff;
}

void mem_t::dump(std::ostream& o) {
  const char empty[PGSIZE] = {0};
  for (reg_t i = 0; i < sz; i += PGSIZE) {
    reg_t ppn = i >> PGSHIFT;
    auto search = sparse_memory_map.find(ppn);
    if (search == sparse_memory_map.end()) {
      o.write(empty, PGSIZE);
    } else {
      o.write(sparse_memory_map[ppn], PGSIZE);
    }
  }
}
