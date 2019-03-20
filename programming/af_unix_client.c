//af_unix_client.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#define BUFFER_SIZE 1024

int main(){
  int skt = 0;
  int send_size = 0;
  int recv_size = 0;
  char buf[BUFFER_SIZE];
  if((skt = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
    perror("socket_error");
    exit(1);
  }
  
  struct sockaddr_un addr_client = {0};
  addr_client.sun_family = AF_UNIX;
  strcpy(addr_client.sun_path, "/tmp/unix-domain-socket");

  if (connect(skt, (struct sockaddr*) &addr_client, sizeof(struct sockaddr_un)) < 0){
    perror("connect_error");
    close(skt);
    exit(2);
  }

  while(1){
    if(fgets(buf, BUFFER_SIZE, stdin) == NULL){
      perror("fgets_error");
      close(skt);
      exit(3);
    }
    if((send_size = send(skt, buf, strnlen(buf, BUFFER_SIZE), 0)) < 0){
      perror("send_error");
      close(skt);
      exit(4);
    }
    if((recv_size = recv(skt, buf, sizeof(buf) -1, 0 )) < 0){
      perror("recv_error");
      close(skt);
      exit(5);
    }
    printf("%s\n", buf);    
  }
  printf("a\n");
  close(skt);
  return 0;
}
