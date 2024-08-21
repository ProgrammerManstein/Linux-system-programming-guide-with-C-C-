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
#include <fcntl.h>

using namespace std;

struct client_data
{
    int cfd;
    sockaddr_in cliaddr;
    client_data(int fd,sockaddr_in addr){
        cfd=fd;
        cliaddr=addr;
    }
};


void* write_client(void *data){
    
    client_data *d=(client_data*) data;
    pthread_detach(pthread_self());
    char buf[BUFSIZ];
    while (1)
    {        
        memset(buf, 0, sizeof(buf));
        int ret=read(d->cfd,buf,sizeof(buf));
        if (ret == 0) {
            cout<<"the other side has been closed.\n";
            break;
        }
        for(int i=0;i<ret;i++){
            buf[i]=toupper(buf[i]);
        }
        write(d->cfd,buf,ret);
        
    }
    close(d->cfd);
    delete data;
}

int main(int argc, char **argv){
    int lfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_port=htons(8888);
    int ret=bind(lfd,(struct sockaddr*)&addr,sizeof(addr));

    ret=listen(lfd,128);
    struct sockaddr_in cliaddr;
    socklen_t socklen=sizeof(cliaddr);
    pthread_t thread;
    int no=0;
    while (1)
    {
        int cfd=accept(lfd,(sockaddr*)&cliaddr,&socklen);
        client_data *data=new client_data(cfd,cliaddr);
        
        pthread_create(&thread,nullptr,write_client,(void*)data);
    }
    close(lfd);
}