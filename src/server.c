#include "game.h"
#include <stdio.h>

#include <stdlib.h>
#include <string.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void usage(int argc, char** argv) {
  printf("usage: %s <v4/v6> <port>\n", argv[0]);
  exit(EXIT_FAILURE);
}

int server_init(const char *proto, const char *portstr, struct sockaddr_storage *storage){
  uint16_t port = (uint16_t)atoi(portstr); 
    if (port == 0) {
        return -1;
    }
    port = htons(port); 

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

int main(int argc, char** argv) {
  if (argc < 3)
    usage(argc, argv);
  struct sockaddr_storage storage;
  if (0 != server_init(argv[1], argv[2], &storage))
    usage(argc, argv);

  int s;
  s = socket(storage.ss_family, SOCK_STREAM, 0);
  if (s == -1)
    logexit("socket");

  int enable = 1;
  if (0 != setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)))
    logexit("setsockopt");

  struct sockaddr *addr = (struct sockaddr *)(&storage);
  if (0 != bind(s, addr, sizeof(storage))) 
    logexit("bind");

  if (0 != listen(s, 10)) 
    logexit("listen");
  
printf("Servidor iniciado em modo IP%s na porta %s Aguardando conexão...\n", argv[1], argv[2]);
  while (1) {
    struct sockaddr_storage cstorage;
    struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
    socklen_t caddrlen = sizeof(cstorage);

    int csock = accept(s, caddr, &caddrlen);
    if (csock == -1) 
      logexit("accept");

    printf("Cliente conectado\n");

    char buf[MSG_SIZE];
    memset(buf, 0, MSG_SIZE);
    size_t count = recv(csock, buf, MSG_SIZE - 1, 0);
    printf("[msg] , %d bytes: %s\n", (int)count, buf);

    sprintf(buf, "recebido\n");
    count = send(csock, buf, strlen(buf) + 1, 0);
    if (count != strlen(buf) + 1) 
        logexit("send");
    close(csock);
  }

  exit(EXIT_SUCCESS);
}
