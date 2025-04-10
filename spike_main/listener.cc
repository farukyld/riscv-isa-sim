#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <signal.h>

#define SOCK_PATH_BASE "/home/faruk/tmp/spike_socket"

std::string final_path;

// Signal handler to clean up the socket file on exit
void signal_handler(int signum)
{
  if (!final_path.empty())
  {
    std::cout << "Cleaning up socket: " << final_path << std::endl;
    unlink(final_path.c_str()); // Remove the socket file
  }
  exit(0); // Exit cleanly after cleanup
}

int try_bind(int server_fd, sockaddr_un &addr)
{
  for (int i = 0; i < 10; ++i)
  {
    std::string sock_path = SOCK_PATH_BASE;
    sock_path += std::to_string(i);

    std::strncpy(addr.sun_path, sock_path.c_str(), sock_path.length() + 1);

    // unlink(sock_path.c_str()); // Remove any existing file

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == 0)
    {
      final_path = sock_path;
      return 0;
    }
  }
  return -1;
}

int main()
{
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (server_fd == -1)
  {
    perror("socket creation failed");
    return 1;
  }

  sockaddr_un addr{};
  addr.sun_family = AF_UNIX;

  if (try_bind(server_fd, addr) == -1)
  {
    perror("All bind attempts failed");
    close(server_fd);
    return 1;
  }

  std::cout << "Bound to socket: " << final_path << std::endl;

  if (listen(server_fd, 0) == -1)
  {
    perror("Listen failed");
    close(server_fd);
    unlink(final_path.c_str());
    return 1;
  }

  char buffer[256];
  std::cout << "Listening for connections...\n";

  while (true)
  {
    std::cout << "before accept\n";
    int client_fd = accept(server_fd, nullptr, nullptr);
    if (client_fd == -1)
    {
      perror("Accept failed");
      continue;
    }
    std::cout << "after accept\n";
    size_t number_of_reads = 0;
    size_t number_of_bytes_read = 0;
    while (true)
    {
      ssize_t bytes_received = read(client_fd, buffer, sizeof(buffer) - 1);
      number_of_bytes_read += bytes_received;
      number_of_reads++;
      if (bytes_received > 0)
      {
        buffer[bytes_received] = '\0';
        // process buffer if needed
      }
      else
      {
        printf("listener: sender ended the connection.\n"
               "number_of_reads: %ld\n"
               "number_of_bytes_read: %ld\n",
               number_of_reads, number_of_bytes_read);
        break;
      }
    }
    close(client_fd);
  }

  close(server_fd);
  unlink(final_path.c_str());
  return 0;
}
