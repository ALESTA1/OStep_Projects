#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main(){

  int fd = socket(AF_INET, SOCK_STREAM, 0);

  if(fd==-1){
    std::cerr<<"Cannot create socket"<<std::endl;
  }

  
}