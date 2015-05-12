#include "meerkat.h"

Meerkat::Meerkat(uint16_t my_port, uint16_t buddy_port, char *buddy_ip, char *callback) {
  this->my_port = my_port;
  this->buddy_port = buddy_port;
  this->callback = callback;

  (this->buddy_ip).sin_family = AF_INET;
  (this->buddy_ip).sin_port = htons(this->buddy_port);
  inet_pton(AF_INET, buddy_ip, &((this->buddy_ip).sin_addr));
}

void Meerkat::set_socket_descriptor(int32_t socket_descriptor) {
  this->socket_descriptor = socket_descriptor;
}

void Meerkat::configure() {
  uint32_t socket_descriptor = udp_configure(this->my_port);
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

  while (true) {
    switch (state) {
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

void Meerkat::add_to_clan(sockaddr_in *addr) {
  if ((this->clan).count((addr->sin_addr).s_addr) == 0) {
    (this->clan)[(addr->sin_addr).s_addr] = *addr;
  }
}

STATE Meerkat::wait_on_buddy() {
  sockaddr_in from;
  int result;
  uint8_t status = recv_packet(this->socket_descriptor, &from, MSG_DONTWAIT, &result);

  if (result == NO_MESSAGES) {
    send_packet_werr(this->socket_descriptor, &(this->buddy_ip), BUDDY);
  } else {
    if (status == BUDDY) {
      send_packet_werr(this->socket_descriptor, &(this->buddy_ip), BUDDY_OK);
      this->add_to_clan(&from);
    } else if (status == BUDDY_OK) {
      return WAIT_ON_DATA;
    } else if (status == LOSS) {
      if ((this->clan).count(from.sin_addr.s_addr) > 0) {
        this->assist_meerkat();
      } else {
        cerr << "Received loss packet from meerkat not in clan" << endl;
      }
    }
  }

  return WAIT_ON_BUDDY;
}

STATE Meerkat::wait_on_data() {
  sockaddr_in from;
  int result;
  uint8_t status = recv_packet(this->socket_descriptor, &from, 0, &result);

  if (status == BUDDY) {
    send_packet_werr(this->socket_descriptor, &(this->buddy_ip), BUDDY_OK);
    this->add_to_clan(&from);
  } else if (status == LOSS) {
    if ((this->clan).count(from.sin_addr.s_addr) > 0) {
      this->assist_meerkat();
    } else {
      cerr << "Received loss packet from meerkat not in clan" << endl;
    }
  }

  return WAIT_ON_DATA;
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

ports_set check_args(int argc, char **argv) {
  ports_set set(DEFAULT_PORT, DEFAULT_PORT);
  struct sockaddr_in sa;
  struct stat s;
  bool p_set = false, b_set = false;
  char help[] = "-h";
  
  if (argc != HELP && argc != NO_PORTS && argc != ONE_PORT && argc != BOTH_PORTS) {
    cerr << "Wrong number of args. Type ./meerkat -h for usage." << endl;
    exit(-1);
  }
  
  if (strcmp(argv[1], help) == 0) {
    cerr << "Usage: ./meerkat buddy_ipv4_address callback_binary_name " 
            "-p my_port -b buddy_port" << endl;
    exit(-1);
  }
  
  for (int i = 0; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if (argv[i][1] == 'p') {
        if (i+1 < argc) {
          if (p_set) {
            cerr << "Self port entered more than once." << endl;
            exit(-1);
          } else {
            set.my_port = strtol(argv[i+1], NULL, 10);
            p_set = true;
          }
        } else {
          cerr << "-p and -b should go before port number." << endl;
          exit(-1);
        }
      }
      if (argv[i][1] == 'b') {
        if (i+1 < argc) {
          if (b_set) {
            cerr << "Buddy port entered more than once." << endl;
            exit(-1);
          } else {
            set.buddy_port = strtol(argv[i+1], NULL, 10);
            b_set = true;
          }
        } else {
          cerr << "-p and -b should go before port number." << endl;
          exit(-1);
        }
      }
    }
  }
  
  if (set.my_port < LOWEST_PORT || set.my_port > HIGHEST_PORT) {
    cerr << "Self port out of bounds (10k to 50k)." << endl;
    exit(-1);
  }
  
  if (set.buddy_port < LOWEST_PORT || set.buddy_port > HIGHEST_PORT) {
    cerr << "Buddy port out of bounds (10k to 50k)." << endl;
    exit(-1);
  }
  
  // inet_pton returns -1 on error, 0 on invalid IP address.
  if (inet_pton(AF_INET, argv[1], &(sa.sin_addr)) != 1) {
    cerr << "Bad ipv4 address for buddy." << endl;
    exit(-1);
  }
  
  if (stat(argv[2], &s) == 0) {
    if (s.st_mode & S_IFDIR) {
      cerr << "Callback expected binary (given a directory)." << endl;
      exit(-1);
    } else if (s.st_mode & S_IFREG && (access(argv[2], X_OK)) == 0) {
      // This is a file with execute permissions
    } else {
      // something else?
      cerr << "Unknown error reading binary name." << endl;
      exit(-1);
    }
  } else {
    cerr << "Stat error while looking for callback file." << endl;
    exit(-1);
  }
  
  return set;
}

int main(int argc, char **argv) {
  char *buddy_ip, *callback;
  uint16_t my_port, buddy_port;
  
  initialize_signal_handler();
  ports_set set = check_args(argc, argv);

  my_port = set.my_port;
  buddy_port = set.buddy_port;
  buddy_ip = argv[1];
  callback = argv[2];

  Meerkat *meerkat = new Meerkat(my_port, buddy_port, buddy_ip, callback);
  meerkat->process();

  return 0;
}