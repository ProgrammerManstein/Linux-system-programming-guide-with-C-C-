#include<iostream>
#include<string>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include <sys/wait.h>
#include<errno.h>

using namespace std;

void catchsignal(int signo){
    pid_t p;
    while((p=waitpid(0,nullptr,WNOHANG))!=0){
        if (p==-1)
        {
            perror("wait error: ");
            return;
        }
        else{
            cout<<"wait child: "<<p<<endl;
        }
        
    }
}

int main(int argc, char **argv){
    pid_t pid;
    int i;
    for(i=0;i<10;i++){
        pid=fork();
        if(pid==0){
            break;
        }
    }
    if(pid==0){
        cout<<"child: "<<getpid()<<endl;
    }
    else if(pid>0){
        struct sigaction act, oldact;
        act.sa_flags=0;
        sigemptyset(&act.sa_mask);
        act.sa_handler=catchsignal;
        sigaction(SIGCHLD,&act,&oldact);
        cout<<"parent: "<<getpid()<<endl;
        while(1){
        }
    }
}