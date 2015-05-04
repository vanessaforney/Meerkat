#ifndef DRIVER_H_
#define DRIVER_H_

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGE_SIZE 8
#define STATUS_SIZE 8

enum STATUS {
  LOSS, BUDDY, BUDDY_OK
};

void configure_message(uint8_t status, uint8_t *message);

int32_t udp_configure();

int32_t send_packet();

int32_t recv_packet();

#endif