#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void usage(int argc, char** argv) {
  printf("usage: %s <ipv4/ipv6> <port>\n", argv[0]);
  exit(EXIT_FAILURE);
}
int addrparse(const char *addrstr, const char *portstr, struct sockaddr_storage *storage) {
    if (addrstr == NULL || portstr == NULL)
      return -1;
    
    uint16_t port = (uint16_t)atoi(portstr); 
    if (port == 0) 
      return -1;
    
    port = htons(port);

    // IPv4
    struct in_addr inaddr4;  
    if (inet_pton(AF_INET, addrstr, &inaddr4)) {
        struct sockaddr_in *addr4 = (struct sockaddr_in *)storage;
        addr4->sin_family = AF_INET;
        addr4->sin_port = port;
        addr4->sin_addr = inaddr4;
        return 0;
    }

    // IPv6
    struct in6_addr inaddr6;  
    if (inet_pton(AF_INET6, addrstr, &inaddr6)) {
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)storage;
        addr6->sin6_family = AF_INET6;
        addr6->sin6_port = port;
        memcpy(&(addr6->sin6_addr), &inaddr6, sizeof(inaddr6));
        return 0;
    }

    return -1;
}
int main(int argc, char** argv) {
if (argc < 3) 
		usage(argc, argv);
	
	struct sockaddr_storage storage;
	if (0 != addrparse(argv[1], argv[2], &storage))
		usage(argc, argv);
	
	int s;
	s = socket(storage.ss_family, SOCK_STREAM, 0);
	if (s == -1) 
		logexit("socket");
	
	struct sockaddr *addr = (struct sockaddr *)(&storage);
	if (0 != connect(s, addr, sizeof(storage))) 
		logexit("connect");
	
	printf("Conectado ao Servidor\n");
	// plataforma para a troca de mensagens
  GameMessage gmsg;
  while(1) {
  	gmsg = receive_gm(s);
  	switch(gmsg.type) {
  	  case MSG_REQUEST:
      	printf("%s", gmsg.message);
      	fgets(gmsg.message, MSG_SIZE-1, stdin);
      	gmsg.client_action = atoi(gmsg.message);
      	gmsg.type = MSG_RESPONSE;
      	send_gm(s, &gmsg);
      	break;
    	case MSG_ERROR:
    	  printf("%s", gmsg.message);
    	  break;
    	case MSG_RESULT:
    	  printf("%s", gmsg.message);
    	  break;
    	case MSG_PLAY_AGAIN_REQUEST:
    	  printf("%s", gmsg.message);
      	fgets(gmsg.message, MSG_SIZE-1, stdin);
      	gmsg.client_action = atoi(gmsg.message);
      	gmsg.type = MSG_PLAY_AGAIN_RESPONSE;
      	send_gm(s, &gmsg);
      	break;
    	case MSG_END:
    	  printf("%s", gmsg.message);
        break;    	  
  	}
  	// solicitação para finalizar conexão com o servidor
    if (gmsg.type  == MSG_END)
      break;
  	/*size_t count = send(s, buf, strlen(buf)+1, 0);
  	if (count != strlen(buf)+1)
  		logexit("send");
	
  	memset(buf, 0, MSG_SIZE);
  	unsigned total = 0;
  	while(1) {
  		count = recv(s, buf + total, MSG_SIZE - total, 0);
  		if (count == 0)
  			break;
  		total += count;
  	}*/
	}
	close(s);

	exit(EXIT_SUCCESS);  
}
