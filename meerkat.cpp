#include "meerkat.h"

Meerkat::Meerkat(uint16_t my_port, uint16_t buddy_port, char *buddy_ip, char *callback, TYPE type) {
  this->my_port = my_port;
  this->buddy_port = buddy_port;
  this->callback = callback;
  this->type = type;

  (this->buddy_ip).sin_family = AF_INET;
  (this->buddy_ip).sin_port = htons(this->buddy_port);
  inet_pton(AF_INET, buddy_ip, &((this->buddy_ip).sin_addr));

  //GPIO *gpio = new GPIO();
  //GPIO_SET_AS_INPUT(gpio, 27);
}

void Meerkat::set_socket_descriptor(int32_t socket_descriptor) {
  this->socket_descriptor = socket_descriptor;
}

void Meerkat::configure() {
  uint32_t socket_descriptor = udp_configure(this->my_port);
  this->set_socket_descriptor(socket_descriptor);
}

void Meerkat::assist_meerkat() {
  // string s(this->callback);
  // s = "./" + s;
  // execl(s.c_str(), s.c_str(), NULL);
  struct timeval end;
  gettimeofday(&end, NULL);
  double timestamp = end.tv_sec + (end.tv_usec/1000000.0);
  printf("End: %.6lf\n", timestamp);
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

      case CHECK_GPIO_STATUS:
        state = this->check_gpio_status();
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
  uint8_t status = recv_packet(this->socket_descriptor, &from, TIMEOUT, &result);

  if (result == NO_MESSAGES) {
    send_packet_werr(this->socket_descriptor, &(this->buddy_ip), BUDDY);
  } else {
    if (status == BUDDY) {
      send_packet_werr(this->socket_descriptor, &(this->buddy_ip), BUDDY_OK);
      this->add_to_clan(&from);
    } else if (status == BUDDY_OK) {
      if (this->type == LIFE) {
        pid_t pid = fork();

        if (pid == 0) {
          //while ((GPIO_READ(gpio, 17)) == ALIVE) {
          //} 
          struct timeval start;
          gettimeofday(&start, NULL);
          double t1 = start.tv_sec + (start.tv_usec/1000000.0);
          printf("Start: %.6lf\n", t1);
          exit(-1);
        }

        return WAIT_ON_DATA;
      } else {
        return CHECK_GPIO_STATUS;
      }
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
  uint8_t status = recv_packet(this->socket_descriptor, &from, NO_TIMEOUT, &result);

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

STATE Meerkat::check_gpio_status() {
  //if ((GPIO_READ(gpio, 27)) != ALIVE) {
    int input, i = 0;
    scanf("%d", &input);

    while (i++ < 10) {
      send_packet(this->socket_descriptor, &(this->buddy_ip), LOSS);
    }
  //}

  return CHECK_GPIO_STATUS;
}

int main(int argc, char **argv) {
  char *buddy_ip, *callback;
  uint16_t my_port, buddy_port;
  TYPE type;

  initialize_signal_handler();
  ports_set set = check_args(argc, argv);

  my_port = set.my_port;
  buddy_port = set.buddy_port;
  buddy_ip = argv[1];
  callback = argv[2];
  type = set.type;

  Meerkat *meerkat = new Meerkat(my_port, buddy_port, buddy_ip, callback, type);
  meerkat->process();

  return 0;
}