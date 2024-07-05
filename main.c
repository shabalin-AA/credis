#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "value.h"
#include "str.h"
#include "list.h"
#include "set.h"
#include "hashtable.h"
#include "interpreter.h"

#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h> 
#include <unistd.h>
#include <pthread.h>

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

#define MSG_LEN 512

void* handle_client(void* arg)
{
  int client_fd = *(int*)arg;
  char buf[MSG_LEN];
  while (1) {
    int len = read(client_fd, buf, sizeof(buf));
    buf[len-1] = 0;
    if (strcmp(buf, "QUIT") == 0) {
      close(client_fd);
      break;
    }
    else {
      TokenList tokens = tokenize((Str) {.content = buf, .len = len});
      char* response = interpret(tokens);
      send(client_fd, response, strlen(response), 0);
      DA_DEINIT(&tokens);
    }
  }
  return NULL;
}

int main(void)
{
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  assert(sockfd > 0 && "socket() failed");
  
  sockaddr_in server_addr = {0};
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(6399);
  
  int bind_res = bind(sockfd, (sockaddr*)&server_addr, sizeof(server_addr));
  assert(bind_res == 0 && "bind() failed");
  
  int listen_res = listen(sockfd, 4);
  assert(listen_res == 0 && "listen() failed");
  
  sockaddr_in client_addr;
  socklen_t client_addr_len;
  
  while (1) {
    int clientfd = accept(sockfd, (sockaddr*)&client_addr, &client_addr_len);
    assert(clientfd > 0 && "accept() failed");
    printf("%d connected\n", clientfd);
    pthread_t thread;
    pthread_create(&thread, NULL, handle_client, &clientfd);
  }
  
  close(sockfd);
  return 0;
}