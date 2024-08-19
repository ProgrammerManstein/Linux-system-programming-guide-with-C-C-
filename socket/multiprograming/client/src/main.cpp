#include<iostream>
#include<fstream>
#include <unistd.h>
#include<arpa/inet.h> 
#include<sys/socket.h>
#include<string.h>
using namespace std;

int main(int argc,char**argv){
    char buf[BUFSIZ];
    int fd=socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in addrin;
    addrin.sin_family=AF_INET;
    addrin.sin_port=htons(8888);
    inet_pton(AF_INET,"127.0.0.1",&addrin.sin_addr.s_addr);
    connect(fd,(struct sockaddr *)&addrin,sizeof(addrin));
    while(1){
        string in;
        cin>>in;
        in.copy(buf,in.length(),0);
        write(fd,buf,sizeof(buf));
        if(in=="close"){
            cout<<"close"<<endl;
            break;
        }
        read(fd,buf,sizeof(buf));
        string str=buf;
        cout<<str<<endl;
    }
    close(fd);
}