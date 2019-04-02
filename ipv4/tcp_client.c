#include <stdio.h> //fprintf, perror
#include <stdlib.h> //exit
#include <string.h> //memset
#include <sys/types.h> //inet_addr, connect, read, socket, write
#include <sys/socket.h> //inet_addr, connect, shutdown, socket
#include <sys/uio/h> //read. write
#include <netinet/in.h> //inet_addr
#include <arpa/inet.h> //htons, inet_addr
#include <unistd.h> //close, read. write

#define msg_size 32
#define max_msg_size 1024
#define buff_size
#define server_addr "127.0.0.1"
#define server_port 12345

int main(){
    int s, cc;
    struct sockaddr_in sa;
    char buf[max_msg_size]

    if((s = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket error\n");
        exit(1);
    }
    memset(&sa, 0, sizeof(sa));
    sa.sin_len = sizeof(sa);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(server_port);
    sa.sin_addr.s_addr = inet_addr(server_addr);
    
    /* サーバへ接続 */
    if(connect(s, (struct sockaddr *)&sa, sizeof(sa)) == -1){
        perror("connect error\n");
        exit(2);
    }

    while((cc = read(s, buf, sizeof(buf))) > 0){
        write(1, buf, cc);
    }
    if(cc == -1){
        perror("read error\n");
        exit(3);
    }
    if(shutdown(s, SHUT_RDWR) == -1){
        perror("shut down");
        exit(4);
    }
    if(close(s) == -1){
        perror("close error");
        exit(5);
    }
}