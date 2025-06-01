#include "game.h"
#include <string.h>

int generate_response() {
  return rand() % 5;
}
void logexit(const char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

// retorna o valor do ponto de vista do cliente
// -1 : Derrota, 0: Empate, 1: Vitoria
int battle(int s_attack, int c_attack) {
  switch(s_attack) {
    case NUCLEAR_ATTACK:
      switch (c_attack) {
        case DRONE_STRIKE:
        case CYBER_ATTACK:
          return -1;
          break;
        case NUCLEAR_ATTACK:
          return 0;
          break;
        default:
          return 1;
          break;
      }
    case INTERCEPT_ATTACK:
      switch (c_attack) {
        case NUCLEAR_ATTACK:
        case BIO_ATTACK:
          return -1;
          break;
        case INTERCEPT_ATTACK:
          return 0;
          break;
        default:
          return 1;
          break;
      }
    case CYBER_ATTACK:
      switch (c_attack) {
        case INTERCEPT_ATTACK:
        case DRONE_STRIKE:
          return -1;
          break;
        case CYBER_ATTACK:
          return 0;
          break;
        default:
          return 1;
          break;
      }
    case DRONE_STRIKE:
      switch (c_attack) {
        case INTERCEPT_ATTACK:
        case BIO_ATTACK:
          return -1;
          break;
        case DRONE_STRIKE:
          return 0;
          break;
        default:
          return 1;
          break;
      }
    case BIO_ATTACK:
      switch (c_attack) {
        case NUCLEAR_ATTACK:
        case CYBER_ATTACK:
          return -1;
          break;
        case BIO_ATTACK:
          return 0;
          break;
        default:
          return 1;
          break;
      }

  }
}
char* attack_name(int attack) {
  switch (attack) {
    case NUCLEAR_ATTACK:
      return "NUCLEAR_ATTACK";
      break;
    case INTERCEPT_ATTACK:
      return "INTERCEPT_ATTACK";
      break;
    case DRONE_STRIKE:
      return "DRONE_STRIKE";
      break;
    case CYBER_ATTACK:
      return "CYBER_ATTACK";
      break;
    case BIO_ATTACK:
      return "BIO_ATTACK";
      break;
    default:
      return "ERROR";
  
  }
}
char* result_name(int result) {
  switch(result) {
    case -1:
      return "Derrota!";
      break;
    case 0:
      return "Empate!";
      break;
    case 1:
      return "Vitória";
      break;
    default:
      return "ERROR";
      break;
  }
}

int send_gm(int sockfd, const GameMessage* gmsg) {
  if (-1 == send(sockfd, gmsg, sizeof(GameMessage), 0))
    return -1;
  return 0;
}
GameMessage receive_gm(int sockfd) {
  GameMessage gmsg;
  if (-1 == recv(sockfd, &gmsg, sizeof(GameMessage), 0))
    gmsg.type = MSG_ERROR;
  return gmsg;
}

//Compila todas as mesagens de resposta do server
//id: numeração da mensagem, msg: endereço onde será armazenada
void server_msg(int id, char* msg) {
  switch (id) {
    case 1:
      strcpy(msg, "Escolha sua jogada:\n\n");
      strcat(msg, "0 - Nuclear Attack\n");
      strcat(msg, "1 - Intercept Attack\n");
      strcat(msg, "2 - Cyber Attack\n");
      strcat(msg, "3 - Drone Strike\n");
      strcat(msg, "4 - Bio Attack\n");
    break;
    case 2:
      strcpy(msg, "Deseja jogar novamente?\n\n");
      strcat(msg, "1 - Sim\n");
      strcat(msg, "0 - Não\n");
      break;
    case 3:
      strcpy(msg, "Por favor, selecione um valor de 0 a 4.\n");
      break;
    case 4:
      strcpy(msg, "Por favor, digite 1 para jogar novamente ou 0 para encerrar.\n");
      break;

    default:
      strcpy(msg, "ID de mensagem não encotrado\n");
    break;
  }
}
// @TODO: server_msg que pode receber parametros

/*int main() {
  int test = 0;
  char* s_attack;
  char* c_attack;
  s_attack = attack_name(DRONE_STRIKE); 
  for (int i=0; i<= 25; i++) {
      test = generate_response();
      c_attack = attack_name(test);
      printf("SERVER_ATTACK: %s\n", s_attack);
      printf("CLIENT_ATTACK: %s\n", c_attack);
      printf("RESULT: %d\n", battle(DRONE_STRIKE, test));
      
  }
}*/
