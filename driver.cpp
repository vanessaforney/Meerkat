#include "driver.h"

int32_t udp_configure(uint16_t port) {
  int socket_descriptor = 0; // Socket descriptor.
  struct sockaddr_in local; // Socket address for us.
  uint32_t len = sizeof local; // Length of local address.

  // Create the socket.
  if ((socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    printf("Could not create socket.\n"); 
    exit(1);
  }

  // Set up the socket.
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = INADDR_ANY;
  local.sin_port = htons(port);

  // Bind the IP address to a port.
  if (bind(socket_descriptor, (struct sockaddr *) &local, sizeof local) < 0) {
    printf("Error on bind.\n");
    exit(-1);
  }

  return socket_descriptor;
}

void send_packet_werr(int socket_descriptor, struct sockaddr_in *addr, STATUS status) {
  if (send_packet(socket_descriptor, addr, status) < 0) {
    cerr << "Error sending packet" << endl;
    cerr << strerror(errno) <<endl;
  }
}

int send_packet(int socket_descriptor, struct sockaddr_in *addr, STATUS status) {
  if (status == BUDDY) {
    cout << "Sending buddy packet" << endl;
  } else if (status == BUDDY_OK) {
    cout << "Sending buddy ok packet" << endl;
  } else {
    cout << "Sending loss packet" << endl;
  }

  printf("Using family: %d\n", addr->sin_family);
  cout << "Using port: ";
  cout << ntohs(addr->sin_port) << endl;
  char ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(addr->sin_addr), ip, INET_ADDRSTRLEN);
  printf("Using IP address: %s\n", ip);

  return sendto(socket_descriptor, &status, MESSAGE_SIZE, 0, (struct sockaddr *) addr, sizeof(*addr));
}

uint8_t recv_packet(int socket_descriptor, sockaddr_in *from, int timeout, int *result) {
  cout << "Receiving packet" << endl;
  uint8_t message[MESSAGE_SIZE];
  socklen_t len = sizeof from;
  struct timeval tv;

  if (timeout != NO_TIMEOUT) {
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    setsockopt(socket_descriptor, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
  }
  *result = recvfrom(socket_descriptor, message, MESSAGE_SIZE, 0, (struct sockaddr *) from, &len);

  return *message;
}