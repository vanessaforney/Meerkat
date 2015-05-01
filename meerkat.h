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

#include "driver.h"

#define NUM_ARGS 4
#define ERROR -1

using namespace std;

//typedef enum State STATE;

enum STATE {
  WAIT_ON_BUDDY, WAIT_ON_DATA
};

class Meerkat;

class Meerkat {
public:
  Meerkat(char *my_ip, char *buddy_ip, char *buddy_port, char *callback);

  // Set the socket descriptor of the current meerkat.
  void set_socket_descriptor(int32_t socket_descriptor);

  // Begins the meerkat process to handle incoming connections and initialize
  // the buddy meerkat.
  void process();

private:
  // The IP address of the server running the current meerket.
  char *my_ip;

  // The IP address of the buddy meerkat.
  char *buddy_ip;

  // The port number of the buddy meerket.
  char *buddy_port;

  // The name of the program to run.
  char *callback;

  // The list of meerkats that will send a panic message to the current meerkat
  // when they experience loss.
  // TODO: decide how to store these. We do not have the meerkat here.
  // Idea: class for IP linking to the port number? Call it Connection?
  vector<Meerkat> clan;

  // The socket descriptor for the current meerkat.
  // TODO: decide if this is needed.
  int32_t socket_descriptor;

  // Configure the meerkat and bind to its associated port number.
  void configure();

  // Receives a message from a meerkat in the clan that has initiated loss mode.
  // Calls the user defined callpack function.
  void assist_meerkat();

  // The meerkat is waiting for a BUDDY_OK signal for the buddy meerkat.
  STATE wait_on_buddy();

  // The meerkat has received a BUDDY_OK and is waiting to assist other meerkats.
  STATE wait_on_data();

  // Send packet to buddy.
  void send_to_buddy();
};

#endif
