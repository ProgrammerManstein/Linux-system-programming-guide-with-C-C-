#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

using namespace std;

int main(int argc,char** argv){
    int fd[2];
    int ret=pipe(fd);
    if(ret==-1){
        perror("pipe error");
        exit(1);
    }
    pid_t pid;
    pid=fork();
    if(pid>0){
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        execlp("ls","ls",nullptr);
    }
    else if(pid==0){
        close(fd[1]);
        dup2(fd[0],STDIN_FILENO);
        execlp("wc","wc","-l",nullptr);
    }
}