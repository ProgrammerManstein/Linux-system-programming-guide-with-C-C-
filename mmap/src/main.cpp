#include<unistd.h>
#include<stdio.h>
#include<iostream>
#include<fcntl.h>
#include<sys/mman.h>
#include <sys/wait.h>
#include<string.h>

using namespace std;

int main(int argc, char **argv){
    
    int fd=open("txt",O_RDWR|O_CREAT|O_TRUNC,"0644");
    truncate("txt",4096);
    char *p;
    if(fd==-1){
        perror("open failed");
        exit(1);
    }

    p=(char*)mmap(nullptr,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    close(fd);
    if(p==MAP_FAILED){
        perror("map failed");
        exit(1);
    }
    pid_t pid=fork();
    if(pid==0){
        sleep(1);
        cout<<p<<endl;
    }
    
    else if(pid>0){
        const char* buff="hello world";
        memcpy(p,buff,strlen(buff));
        wait(nullptr);
    }
    munmap(p, 4096);
}