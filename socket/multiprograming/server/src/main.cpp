#include<sys/socket.h>
#include<netinet/in.h>
#include<iostream>
#include<fstream>
#include <unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>
#include <signal.h> 
#include<sys/types.h>
#include<sys/wait.h> 
using namespace std;

void catch_child(int signum){
    pid_t p;
    while((p=waitpid(0,nullptr,WNOHANG))>0){
        cout<<"catch child: "<<p<<endl;
    }
    return;
}

int main(int argc,char** argv){
    int pid;

    char IP_buf[1024];
    int lfd=socket(AF_INET,SOCK_STREAM,0);
    int cfd=0;
    
    struct sockaddr_in addrin;
    addrin.sin_family=AF_INET;
    addrin.sin_port=htons(8888);
    addrin.sin_addr.s_addr=htonl(INADDR_ANY);
    
    bind(lfd,(struct sockaddr*) &addrin,sizeof(addrin));
    listen(lfd,128);

    struct sockaddr_in cliaddr;
    socklen_t len=sizeof(cliaddr);
    socklen_t IP_len;
    
    sigset_t set;
    sigset_t newset;
    sigemptyset(&set);
    sigaddset(&set,SIGCHLD);
    sigprocmask(SIG_BLOCK,&set,&newset);

    while(1){       
        cfd=accept(lfd,(struct sockaddr*) &cliaddr,&len);
        if(cfd==-1){
            perror("accept error");
            continue;
        }
        pid=fork();
        if(pid==0){
            close(lfd);
            break;
        }else if(pid>0){
            struct sigaction act;
            act.sa_handler=catch_child;
            sigemptyset(&act.sa_mask);
            act.sa_flags=0;
            sigprocmask(SIG_UNBLOCK,&set,nullptr);
            int sigret=sigaction(SIGCHLD,&act,nullptr);
            if(sigret==-1){
                perror("sigaction error");
                return 1;
            }
            close(cfd);
            continue;
        }
    }

    if(pid==0){
        while(1){    
            int size;
            char buf[BUFSIZ];
            size=read(cfd,buf,sizeof(buf));
            if (size == 0) {
                cout<<"the other side has been closed.\n";
                break;
            }
            for(int i=0;i<size;i++){
                buf[i]=toupper(buf[i]);
            }
            write(cfd,buf,size);
            cout<<"ip:"<<inet_ntop(AF_INET,&cliaddr.sin_addr.s_addr,IP_buf,sizeof(IP_buf))<<endl<<"port:"<<ntohs(addrin.sin_port)<<endl<<"cfd:"<<cfd<<endl;
        }
        close(cfd);
    }
    
}