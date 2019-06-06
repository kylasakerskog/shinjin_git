#include <stdio.h>
#include <unistd.h>
int main(){
  char msg[] = "hello world!\n";
  write(stdout->_fileno, msg, sizeof(msg)); //1はファイルハンドラでstdoutを示す 
  return 0;
}
