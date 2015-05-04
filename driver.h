#ifndef DRIVER_H_
#define DRIVER_H_

#include <iostream>

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGE_SIZE 1
#define STATUS_SIZE 1

using namespace std;

enum STATUS {
  LOSS, BUDDY, BUDDY_OK
};

int32_t udp_configure(uint16_t port);

void send_packet_werr(int socket_descriptor, struct sockaddr_in *addr, uint8_t status);

int send_packet(int socket_descriptor, struct sockaddr_in *addr, uint8_t status);

uint8_t recv_packet(int socket_descriptor, struct sockaddr_in *addr);

#endif