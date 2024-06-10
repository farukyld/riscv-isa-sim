#ifndef REMOTE_BITBANG_H
#define REMOTE_BITBANG_H

#include <stdint.h>

#include "jtag_dtm.h"

// jtag'e internet uzerinden erisim sagliyor.
class remote_bitbang_t
{
public:
  // Create a new server, listening for connections from localhost on the given
  // port.
  remote_bitbang_t(uint16_t port, jtag_dtm_t *tap);

  // Do a bit of work. // !!! localhost::port'ta bir sey var sa, jtag'a gonderiyor. jtag'den debug module'e gonderiyor. jtag'in host tarafi.
  void tick();

private:
  jtag_dtm_t *tap;

  int socket_fd;
  int client_fd;

  static const ssize_t buf_size = 64 * 1024;
  char recv_buf[buf_size];
  ssize_t recv_start, recv_end;

  // Check for a client connecting, and accept if there is one.
  void accept();
  // Execute any commands the client has for us.
  void execute_commands();
};

#endif
