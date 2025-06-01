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

  if (0 != listen(s, 1)) 
    logexit("listen");
  
printf("Servidor iniciado em modo IP%s na porta %s Aguardando conexão...\n", argv[1], argv[2]);
  while (1) {
    struct sockaddr_storage cstorage;
    struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
    socklen_t caddrlen = sizeof(cstorage);

    int c = accept(s, caddr, &caddrlen);
    if (c == -1) 
      logexit("accept");

    printf("Cliente conectado\n");
	// plataforma para a troca de mensagens
    GameMessage gmsg;
    //gamestatus
    int c_attack = 0, s_attack = 0, result = 0, v_s = 0, v_c = 0;
    // loop principal do game
    while(1) {
      server_msg(1, gmsg.message);
      gmsg.type = MSG_REQUEST;
      if (0 != send_gm(c, &gmsg))
          logexit("MSG_REQUEST");
      printf("Apresentando as opções para o cliente.\n");
    
      gmsg = receive_gm(c);
      c_attack = gmsg.client_action;

      printf("Cliente escolheu %s\n", gmsg.message);
      if (c_attack < 0 || c_attack > 4) {
        printf("Erro: escolha inválida de jogada\n");
        gmsg.type = MSG_ERROR;
        server_msg(3, gmsg.message);
        if (0 != send_gm(c, &gmsg))
            logexit("MSG_ERROR");
        continue;
      }
      s_attack = generate_response();
      printf("Server escolheu aleatoriamente %d \n", s_attack);
      result = battle(s_attack, c_attack);
      snprintf(gmsg.message, sizeof(gmsg.message), 
               "Você escolheu %s\nServidor escolheu: %s\nResultado: %s\n", 
               attack_name(c_attack), attack_name(s_attack), result_name(result));
      switch(result) {
        case -1:
          v_s += 1;
          printf("Cliente perdeu\n");
          break;
        case 0:
          printf("Empate\n");
          break;
        case 1:
          v_c += 1;
          printf("Cliente ganhou\n");
          break;
      }
      printf("Placar atualizado: Cliente %d x %d Servidor\n", v_c, v_s);
      gmsg.client_action = c_attack;
      gmsg.server_action = s_attack;
      gmsg.result = result;
      gmsg.type = MSG_RESULT;
      if (0 != send_gm(c, &gmsg))
        logexit("MSG_RESULT");
      // Empate
      if (0 == result)
        continue;
      while(1) {
        server_msg(2, gmsg.message);
        gmsg.type = MSG_PLAY_AGAIN_REQUEST;
        printf("Perguntando se o cliente deseja jogar novamente.\n");
        if (0 != send_gm(c, &gmsg))
          logexit("MSG_PLAY_AGAIN_REQUEST");
        gmsg = receive_gm(c);
        if (MSG_PLAY_AGAIN_RESPONSE != gmsg.type)
          logexit("MSG_PLAY_AGAIN_RESPONSE");
        if (gmsg.client_action < 0 || gmsg.client_action > 1){
          gmsg.type = MSG_ERROR;
          server_msg(4, gmsg.message);
          if (0 != send_gm(c, &gmsg))
            logexit("MSG_ERROR");
        }
        else
          break;
      }
      // encerramento
      if (0 == gmsg.client_action) 
        break;
      printf("Cliente deseja jogar novamente.\n");
      // caso contrário continua

    
      /*memset(buf, 0, MSG_SIZE);
      size_t count = recv(c, buf, MSG_SIZE - 1, 0);
      printf("[msg] , %d bytes: %s\n", (int)count, buf);

      sprintf(buf, "recebido\n");
      count = send(c, buf, strlen(buf) + 1, 0);
      if (count != strlen(buf) + 1) 
          logexit("send");
    }*/
    }
    printf("Cliente não deseja jogar novamente\n");
    gmsg.type = MSG_END;
    gmsg.client_wins = v_c;
    gmsg.server_wins = v_s;
    printf("Enviando placar final.\n");
    snprintf(gmsg.message, sizeof(gmsg.message),
              "Fim de jogo!\nPlacar final: Você %d x %d Servidor\nObrigado por jogar!\n",
               v_c, v_s);
    if (0 != send_gm(c, &gmsg))
      logexit("MSG_END");
    printf("Encerrando conexão.\n");
    close(c);
    printf("Cliente desconectado.\n");
  }

  exit(EXIT_SUCCESS);
}
