//af_unix_server.c

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
  int fd = 0;
  int recv_size = 0;
  int send_size = 0;
  char buf[BUFFER_SIZE];
  if((skt = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
    perror("socket_error");
    exit(1);
  }
  struct sockaddr_un addr_server = {0};
  addr_server.sun_family = AF_UNIX;
  strcpy(addr_server.sun_path, "/tmp/unix-domain-socket");
  
  // 既に同一ファイルが存在していたら削除
  remove(addr_server.sun_path);

  if(bind(skt, (struct sockaddr *) &addr_server, sizeof(struct sockaddr_un)) < 0){
    perror("bind_error"); 
    close(skt);
    exit(2);
  }
  if(listen(skt, 5) < 0){
    perror("listen_error");
    close(skt);
    exit(3);
  }
  while(1){
    if((fd = accept(skt, NULL, NULL)) < 0){
      perror("accept_error");
      close(skt);
      exit(4);
    }
    while(1){
      if((recv_size = recv(fd, buf, sizeof(buf)-1, 0)) < 0){
        perror("recv_error");
        close(skt);
        exit(5);
      }
      else if(recv_size == 0){
        break;
      }
      //printf("%d\n",recv_size);
      buf[recv_size] = '\0';
      printf("message: %s\n", buf);
      if((send_size = send(fd, buf, strnlen(buf, BUFFER_SIZE), 0)) < 0){
        perror("send_error");
        close(skt);
        exit(7);
      }
    }
    if(close(fd) == 1){
      perror("close_error");
      close(skt);
      exit(6);
    }
  }
  close(skt);
  return 0;
}
