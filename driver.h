#ifndef DRIVER_H_
#define DRIVER_H_

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* Constructs the remote socket address. */
struct addrinfo *construct_remote_socket();

/* Create the UDP socket. */
void create_udp_socket();

/* Send the datagram. */
void send_datagram();

#endif