#include "game.h"

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
