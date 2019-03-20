//af_unix_server.c

#include <stdio.h>
#include <sys/type.h>
#include <sys/socket.h>
#include <errno.h>

int main(){
  int s = 0;
  if(s = socket(AF_UNIX, SOCK_STREAM, 0)==-1){
    printf("errno : %d",errno);
    exit(1);
  }
  if(bind(s, )==-1){
    printf("errno : %d",errno);
    exit(2);
  }
  

}
