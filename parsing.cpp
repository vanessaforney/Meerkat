#include "meerkat.h"

ports_set check_args(int argc, char **argv) {
  ports_set set(DEFAULT_PORT, DEFAULT_PORT);
  struct sockaddr_in sa;
  struct stat s;
  bool p_set = false, b_set = false, t_set = false;
  char help[] = "-h";
  char life[] = "life";
  char death[] = "death";
  
  if (argc != HELP && argc != NO_PORTS && argc != ONE_PORT 
          && argc != BOTH_PORTS && argc != LIFE_AND_DEATH) {
    cerr << "Wrong number of args. Type ./meerkat -h for usage." << endl;
    exit(-1);
  }
  
  if (strcmp(argv[1], help) == 0) {
    cerr << "Usage: ./meerkat buddy_ipv4_address callback_binary_name " 
            "-p my_port -b buddy_port -t life_or_death" << endl;
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
      if (argv[i][1] == 't') {
         if (i+1 < argc) {
            if (t_set) {
               cerr << "Life or death option entered more than once." << endl;
               exit(-1);
            } else {
               if (strcmp(argv[i+1], life) == 0) {
                 set.type = LIFE;
               } else if (strcmp(argv[i+1], death) == 0) {
                 set.type = DEATH;
               } else {
                 cerr << "Wrong input to life/death." << endl;
                 exit(-1);
               }
               t_set = true;
            }
         } else {
            cerr << "-t should go before life or death option." << endl;
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
      cerr << "Unknown error reading binary name." << endl;
      exit(-1);
    }
  } else {
    cerr << "Stat error while looking for callback file." << endl;
    exit(-1);
  }
  
  return set;
}