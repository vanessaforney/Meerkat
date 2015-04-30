#include "meerkat.h"

Meerkat::Meerkat(char *my_ip, char *buddy_ip, char *callback) {
  inet_pton(AF_INET, my_ip, &((this->my_ip).sin_addr));
  inet_pton(AF_INET, my_ip, &((this->buddy_ip).sin_addr));
  this->callback = callback;
}

void Meerkat::assist_meerkat() {
  char *path = (char *) calloc(3, 1);
  path[0] = '.';
  path[1] = '/';

  execl(strcat(path, this->callback), NULL);
}

int main(int argc, char **argv) {
  char *my_ip, *buddy_ip, *callback;

  if (argc == NUM_ARGS) {
    my_ip = argv[1];
    buddy_ip = argv[2];
    callback = argv[3];

    Meerkat *meerkat = new Meerkat(my_ip, buddy_ip, callback);

    // loop to monitor network
        // hit
            // call
    meerkat->assist_meerkat();
  }

  return 0;
}