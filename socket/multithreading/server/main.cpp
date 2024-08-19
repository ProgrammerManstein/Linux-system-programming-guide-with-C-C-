#include<iostream>
#include<string>
#include<string.h>
#include<pthread.h>
#include<errno.h>
#include<sys/socket.h>
#include<wait.h>
#include<unistd.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>

using namespace std;

void write_client(){

}

int main(int argc, char **argv){
    int ldf=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_port=htons(8888);
    int ret=bind(ldf,(struct sockaddr*)&addr,sizeof(addr));

    ret=listen(ldf,128);
    struct sockaddr_in cliaddr;
    socklen_t socklen=sizeof(cliaddr);
    pthread_t thread[128];
    int no=0;
    while (1)
    {
        int cfd=accept(ldf,(sockaddr*)&cliaddr,&socklen);
        
        pthread_create(&thread[no],nullptr,);

    }
    
}