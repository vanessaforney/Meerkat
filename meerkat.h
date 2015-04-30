#ifndef MEERKAT_H_
#define MEERKAT_H_

#include <vector>
#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

using namespace std;

class Meerkat;

class Meerkat {
public:
  Meerkat(struct sockaddr_in ip_address, Meerkat buddy, string callback);

private:
  /* The IP address of the server running the current meerket. */
  struct sockaddr_in ip_address;

  /* The buddy meerkat to send packets to when loss is experienced. */
  Meerkat *buddy;

  /* The list of meerkats that will send a panic message to the current meerkat
  when they experience loss. */
  vector<Meerkat> clan;

  /* The name of the program to run. */
  string callback;

  /* Receives a message from a meerkat in the clan that has initiated loss mode.
  Calls the user defined callpack function. */
  void assist_meerkat();
};

#endif
