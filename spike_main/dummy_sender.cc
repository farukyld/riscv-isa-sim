#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <signal.h>
#include <fcntl.h>
#include <cerrno>

int unix_socket_fd;
#define SOCK_PATH_BASE "/home/faruk/tmp/spike_socket"

int try_connect()
{
  for (int i = 0; i < 10; ++i)
  {
    std::string sock_path = SOCK_PATH_BASE + std::to_string(i);

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1)
    {
      printf("socket creation failed");
      continue;
    }

    // Set the socket to non-blocking
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
      printf("fcntl(F_GETFL) failed");
      close(fd);
      continue;
    }
    flags |= O_NONBLOCK; // Set O_NONBLOCK flag
    if (fcntl(fd, F_SETFL, flags) == -1)
    {
      printf("fcntl(F_SETFL) failed");
      close(fd);
      continue;
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, sock_path.c_str(), sizeof(addr.sun_path) - 1);

    int stat = connect(fd, (sockaddr *)&addr, sizeof(addr));
    if (stat == 0)
    {
      std::cout << "Connected to: " << sock_path << std::endl;
      return fd; // Success
    }
    else
    {
      // Check if the error is EINPROGRESS or EAGAIN
      if (errno == EINPROGRESS || errno == EAGAIN)
      {
        // Connection in progress or would block, try the next one
        std::cout << "Connection to " << sock_path << " in progress or would block. Trying next socket..." << std::endl;
      }
      else
      {
        // Some other error, print it and try the next one
        printf("connect failed for %s\n"
               "with errno: %d",
               sock_path.c_str(), errno);
      }
    }

    close(fd); // If connect failed, close and try next
  }

  std::cout << "No available spike socket to connect to\n";
  return -1;
}

int main(int argc, char **argv)
{
  // log commits via unix domain socket
  // unix_socket_fd = try_connect();
  // if (unix_socket_fd == -1)
  // {
  //   exit(1);
  // }
  int n = 500000000;
  if (argc == 2)
  {
    n = atoi(argv[1]);
  }
  for (int i = 0; i < n; i++)
  {
    uint64_t a[4];
    a[0]=i;
    a[1]=i*i;
    a[2]=i*(i-1);
    a[3]=i*(i+1);

    if (i % (n / 10) == 0)
      printf("before writing %dth message\n", i);
    // write(unix_socket_fd, &(i), sizeof(int));
    write(2, &(a[0]), 32);
    if (i % (n / 10) == 0)
      printf("after writing %dth message\n", i);
  }
}