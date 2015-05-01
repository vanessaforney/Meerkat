#ifndef DRIVER_H_
#define DRIVER_H_

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

int32_t udp_configure();

int32_t send_packet();

int32_t recv_packet();

#endif