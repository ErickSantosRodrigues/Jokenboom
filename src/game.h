#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>

#define MSG_SIZE 256

typedef enum {
MSG_REQUEST,
MSG_RESPONSE,
MSG_RESULT,
MSG_PLAY_AGAIN_REQUEST,
MSG_PLAY_AGAIN_RESPONSE,
MSG_ERROR,
MSG_END
} MessageType;

typedef struct {
int type; // Tipo da mensagem
int client_action;
int server_action;
int result;
int client_wins;
int server_wins;
char message[MSG_SIZE];
} GameMessage;

#define NUCLEAR_ATTACK 0 
#define INTERCEPT_ATTACK 1
#define CYBER_ATTACK 2
#define DRONE_STRIKE 3
#define BIO_ATTACK 4


int generate_response(); 
int battle(int s_attack, int c_attack);
char* attack_name(int attack);
char* result_name(int result);


// Common functions
void logexit(const char *msg);
int send_gm(int sockfd, const GameMessage* gmsg);
GameMessage receive_gm(int sockfd);
void server_msg(int id, char* msg);
