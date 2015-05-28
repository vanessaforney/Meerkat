#include "meerkat.h"

void sigint_handler(int sig) {
  if (SIGINT == sig) {
    cerr << "Meerkat program terminating." << endl;
    exit(0);
  }
}

void initialize_signal_handler() {
  struct sigaction sa;

  sa.sa_handler = sigint_handler;
  sigfillset(&sa.sa_mask);
  sa.sa_flags = 0;

  if (ERROR == sigaction(SIGINT, &sa, NULL)) {
    cerr << "Couldn't set signal handler." << endl;
    exit(ERROR);
  }
}