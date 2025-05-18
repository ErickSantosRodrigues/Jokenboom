#include "game.h"
#include <stdio.h>

#include <stdlib.h>
#include <string.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void usage(int argc, char** argv) {
  printf("usage: %s <v4/v6> <port>", argv[0]);
  exit(EXIT_FAILURE);
}



int main(int argc, char** argv) {
  if (argc < 3)
    usage(argc, argv);
  struct sockaddr_storage storage;
  if (0 != server_init(argv[1], argv[2], &storage))
    usage(argc, argv);

  int s;
  s = socket(storage.ss_family, SOCK_STREAM, 0);
  
}
int server_init(const char *proto, const char *portstr, struct sockaddr_storage *storage) {
    uint16_t port = (uint16_t)atoi(portstr); 
    if (port == 0) {
        return -1;
    }
    port = htons(port); // host to network short

    memset(storage, 0, sizeof(*storage));
    if (0 == strcmp(proto, "v4")) {
        struct sockaddr_in *addr4 = (struct sockaddr_in *)storage;
        addr4->sin_family = AF_INET;
        addr4->sin_addr.s_addr = INADDR_ANY;
        addr4->sin_port = port;
        return 0;
    } else if (0 == strcmp(proto, "v6")) {
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)storage;
        addr6->sin6_family = AF_INET6;
        addr6->sin6_addr = in6addr_any;
        addr6->sin6_port = port;
        return 0;
    } else {
        return -1;
    }
}
