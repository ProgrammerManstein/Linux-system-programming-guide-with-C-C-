#include<iostream>
#include<string>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include <sys/wait.h>
#include<errno.h>
#include<algorithm>

using namespace std;

void catchsignal(int signo){
    pid_t p;
    int stat;
    while((p=waitpid(0,&stat,0))!=-1){
    
        if(WIFEXITED(stat)){
            cout<<"wait child: "<<p<<endl;
        }
    }
}

int main(int argc, char **argv){
    pid_t pid;
    //block sigchld
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGCHLD);
    sigprocmask(SIG_BLOCK,&set,nullptr);

    int i;
    for(i=0;i<10;i++){
        pid=fork();
        if(pid==0){
            break;
        }
    }
    if(pid==0){
        sleep(i/2); //perant exec other codes;
        cout<<"child: "<<getpid()<<endl;
    }
    else if(pid>0){
        sleep(1); //perant exec other codes;
        struct sigaction act, oldact;
        act.sa_flags=0;
        sigemptyset(&act.sa_mask);
        act.sa_handler=catchsignal;
        sigaction(SIGCHLD,&act,&oldact);
        //unblock sigchild
        sigprocmask(SIG_UNBLOCK,&set,nullptr);
        cout<<"parent: "<<getpid()<<endl;
        //while(1); //perant exec other codes;
        
    }
}