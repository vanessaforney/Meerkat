#include "driver.h"

int32_t udp_configure() {
  char ip[INET_ADDRSTRLEN];
  int socket_descriptor = 0; // Socket descriptor.
  struct sockaddr_in local; // Socket address for us.
  uint32_t len = sizeof(local); // Length of local address.

  // Create the socket.
  if ((socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    printf("Could not create socket.\n"); 
    exit(1);
  }

  // Set up the socket.
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = INADDR_ANY;
  local.sin_port = htons(0);

  // Bind the IP address to a port.
  if (bind(socket_descriptor, (struct sockaddr *) &local, sizeof(local)) < 0) {
    printf("Error on bind.\n");
    exit(-1);
  }

  // Get the assigned port number.
  getsockname(socket_descriptor, (struct sockaddr *) &local, &len);
  inet_ntop(AF_INET, &(local.sin_addr), ip, INET_ADDRSTRLEN);
  printf("Using port number %d on IP address %s\n", ntohs(local.sin_port), ip);

  return socket_descriptor;
}

int32_t send_packet() {
  return 0;
}

int32_t recv_packet() {
  return 0;
}

/*void send_packet(char *msg, int socket_descriptor, char *destination_ip, unsigned short destination_port) {
  if (sendto(socket_descriptor, msg, strlen(msg), 0, (struct sockaddr *) &destination), sizeof(destination)) < 0) {
    printf("Could not send data to the server.\n");
    exit(1);
  }
}*/