#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>


void usage(int argc, char** argv) {
  printf("usage: %s <ipv4/ipv6> <port>", argv[0]);
  exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
  exit(EXIT_SUCCESS);
}
