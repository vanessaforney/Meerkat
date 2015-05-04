#include "meerkat.h"

Meerkat::Meerkat(char *my_port, char *buddy_port, char *buddy_ip, char *callback) {
  this->my_port = strtol(my_port, NULL, 10);
  this->buddy_port = strtol(buddy_port, NULL, 10);
  this->callback = callback;

  (this->buddy_ip).sin_family = AF_INET;
  (this->buddy_ip).sin_port = htons(this->buddy_port);
  inet_pton(AF_INET, buddy_port, &((this->buddy_ip).sin_addr)); 
}

void Meerkat::set_socket_descriptor(int32_t socket_descriptor) {
  this->socket_descriptor = socket_descriptor;
}

void Meerkat::configure() {
  uint32_t socket_descriptor = udp_configure();
  this->set_socket_descriptor(socket_descriptor);
}

void Meerkat::assist_meerkat() {
  string s(this->callback);
  s = "./" + s;
  execl(s.c_str(), s.c_str(), NULL);
}

void Meerkat::process() {
  STATE state = WAIT_ON_BUDDY;

  this->configure();
  // this->send_buddy_ok();

  while (true) {
    switch(state) {
      case WAIT_ON_BUDDY:
        state = this->wait_on_buddy();
        break;

      case WAIT_ON_DATA:
        state = this->wait_on_data();
        break;

      default:
        cerr << "Default case should not be reached." << endl;
        exit(-1);
    }
  }
}

STATE Meerkat::wait_on_buddy() {
  // recvfrom
    // parse flag in packet

    // if flag == BUDDY
        // this->send_buddy_ok();
        // add to clan (if not in clan)
    // else if flag == BUDDY_OK
        // return WAIT_ON_DATA
    // else if flag == LOSS
        // this->assist_meerkat()

  return WAIT_ON_BUDDY;
}

STATE Meerkat::wait_on_data() {
    // recvfrom
    // parse flag in packet

    // if flag == BUDDY
        // this->send_buddy_ok();
        // add to clan (if not in clan)
    // else if flag == LOSS
        // this->assist_meerkat()

  return WAIT_ON_DATA;
}

void Meerkat::send_to_buddy() {

}

void sigint_handler(int sig) {
  if (SIGINT == sig) {
    cerr << "Meerkat program terminating." << endl;
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

void check_args(int argc, char **argv) {
  // TODO: complete function. Checks args i.e. port number between 10k and 50k, callback
  // exists, and ip is in the correct form.
  if (argc != NUM_ARGS) {

  }
}

int main(int argc, char **argv) {
  char *my_port, *buddy_ip, *buddy_port, *callback;
  
  initialize_signal_handler();
  check_args(argc, argv);

  my_port = argv[1];
  buddy_port = argv[2];
  buddy_ip = argv[3];
  callback = argv[4];

  Meerkat *meerkat = new Meerkat(my_port, buddy_port, buddy_ip, callback);
  meerkat->process();

  return 0;
}