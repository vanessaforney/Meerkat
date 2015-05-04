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

void check_args(int argc, char **argv) {
  // TODO: complete function. Checks args i.e. port number between 10k and 50k, callback
  // exists, and ip is in the correct form.
  struct sockaddr_in sa;
  struct stat s;
  char help[] = "-h\n";
  int my_port = strtol(argv[1], NULL, 10);
  int buddy_port = strtol(argv[2], NULL, 10);
  
  if (strcmp(argv[1], help) == 0) {
    cerr << "Usage: ./meerkat my_port buddy_port buddy_ipv4_address "
            << "callback_binary_name" << endl;
    exit(-1);
  }
  
  if (argc != NUM_ARGS) {
    cerr << "Wrong number of args. Type ./meerkat -h for usage." << endl;
    exit(-1);
  }
  
  if (my_port < LOWEST_PORT || my_port > HIGHEST_PORT) {
    cerr << "Self port out of bounds (10k to 50k)." << endl;
    exit(-1);
  }
  
  if (buddy_port < LOWEST_PORT || buddy_port > HIGHEST_PORT) {
    cerr << "Buddy port out of bounds (10k to 50k)." << endl;
    exit(-1);
  }
  
  // inet_pton returns -1 on error, 0 on invalid IP address.
  if (inet_pton(AF_INET, argv[3], &(sa.sin_addr)) != 1) {
    cerr << "Bad ipv4 address for buddy." << endl;
    exit(-1);
  }
  
  if (stat(argv[4], &s) == 0) {
    if (s.st_mode & S_IFDIR) {
      cerr << "Callback expected binary (given a directory)." << endl;
      exit(-1);
    }
    else if (s.st_mode & S_IFREG) {
      // We're good this is a file.
    }
    else {
      // something else?
      cerr << "Unknown error reading binary name." << endl;
      exit(-1);
    }
  }
  else {
    cerr << "Stat error while looking for file." << endl;
    exit(-1);
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