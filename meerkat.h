#ifndef MEERKAT_H_
#define MEERKAT_H_

#include <vector>
#include <iostream>
#include <cstdlib>

#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>

#define NUM_ARGS 4

using namespace std;

class Meerkat;

class Meerkat {
public:
  Meerkat(char *my_ip, char *buddy_ip, char *callback);

  /* Receives a message from a meerkat in the clan that has initiated loss mode.
  Calls the user defined callpack function. */
  void assist_meerkat();

private:
  /* The IP address of the server running the current meerket. */
  struct sockaddr_in my_ip;

  /* The IP address of the buddy server running meerkat to send packets to when
  loss is experienced. */
  struct sockaddr_in buddy_ip;

  /* The list of meerkats that will send a panic message to the current meerkat
  when they experience loss. */
  vector<Meerkat> clan;

  /* The name of the program to run. */
  char *callback;
};

#endif
