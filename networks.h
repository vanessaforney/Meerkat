#ifndef DRIVER_H_
#define DRIVER_H_

#include <iostream>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define NO_TIMEOUT -1
#define TIMEOUT 1
#define MESSAGE_SIZE 4
#define STATUS_SIZE 1
#define NO_MESSAGES -1

using namespace std;

enum STATUS {
  LOSS, BUDDY, BUDDY_OK
};

int32_t udp_configure(uint16_t port);

void send_packet_werr(int socket_descriptor, struct sockaddr_in *addr, STATUS status);

int send_packet(int socket_descriptor, struct sockaddr_in *addr, STATUS status);

uint8_t recv_packet(int socket_descriptor, sockaddr_in *from, int timeout, int *result);

#endif