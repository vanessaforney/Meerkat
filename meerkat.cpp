#include "meerkat.h"

Meerkat::Meerkat(char *my_ip, char *buddy_ip, char *callback) {
  inet_pton(AF_INET, my_ip, &((this->my_ip).sin_addr));
  inet_pton(AF_INET, my_ip, &((this->buddy_ip).sin_addr));
  this->callback = callback;
}

void Meerkat::assist_meerkat() {
  string s(this->callback);
  s = "./" + s;
  execl(s.c_str(), s.c_str(), NULL);
}

void sigint_handler(int sig) {
  if (SIGINT == sig) {
    cerr << "Meerkat program terminating." << endl;
  }
}

int main(int argc, char **argv) {
  char *my_ip, *buddy_ip, *callback;
  struct sigaction sa;
  
  sa.sa_handler = sigint_handler;
  sigfillset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (-1 == sigaction(SIGINT, &sa, NULL))
  {
    cerr << "Couldn't set signal handler." << endl;
    return 2;
  }

  if (argc == NUM_ARGS) {
    my_ip = argv[1];
    buddy_ip = argv[2];
    callback = argv[3];

    Meerkat *meerkat = new Meerkat(my_ip, buddy_ip, callback);
    
    // loop to monitor network
        // hit
            // call
    pause();
    
    meerkat->assist_meerkat();
  }

  return 0;
}