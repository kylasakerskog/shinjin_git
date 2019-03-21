#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* L2 プロトコル */
#include <arpa/inet.h>
#include <sys/ioctl.h> 
#include <net/if.h>  
#include <linux/if.h>
#define BUFFER_SIZE 1024

int main(){
  //Socketの取得
  int pd = 0;
  if((pd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0){ //全てのプロトコル受信
    perror("socket_error");
    exit(1);
  }

  //Interfaceの取得
  //sockaddr_llにはインターフェースのインデックス番号が必要
  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);
  ioctl(pd, SIOCGIFINDEX, &ifr);
  int interface_index = ifr.ifr_ifindex;

  //特定のInterfaceからのPacketのみを受信
  struct sockaddr_ll sll;
  memset(&sll, 0xff, sizeof(sll));
  sll.sll_family = AF_PACKET;
  sll.sll_protocol = htons(ETH_P_ALL);
  sll.sll_ifindex = interface_index;

  if(bind(pd, (struct sockaddr *) &sll, sizeof(sll)) < 0){
    perror("bind_error"); 
    close(pd);
    exit(2);
  }

  int i = 1;
  char buf[BUFFER_SIZE];
  int recv_size = 0;
  while(1){
    fd_set fds;
    struct timeval t;
    FD_ZERO(&fds);
    FD_SET(pd, &fds);
    memset(&t, 0, sizeof(t));
    i = select(FD_SETSIZE, &fds, NULL, NULL, &t);
    if(i < 0){
      perror("select_error");
      close(pd);
      exit(3);
    }
    else if (i > 0){
      if((recv_size = recv(pd, buf, sizeof(buf)-1, 0)) < 0){
        perror("recv_error");
        close(pd);
        exit(4);
      }
      else if(recv_size > 0){
        buf[recv_size] = '\0';
        printf("message: %s;\n", buf);
      }
    }
  }

  close(pd);
  return 0;
  
}
