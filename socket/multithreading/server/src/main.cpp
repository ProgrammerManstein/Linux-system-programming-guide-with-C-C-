#include <iostream>
#include <string>
#include <cstring>
#include <pthread.h>
#include <errno.h>
#include <sys/socket.h>
#include <wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <memory>

using namespace std;

struct client_data
{
    int cfd;
    sockaddr_in cliaddr;
    client_data(int fd, sockaddr_in addr) : cfd(fd), cliaddr(addr) { }
};

void* write_client(void* data)
{
    // Transfer ownership to unique_ptr for automatic cleanup
    //std::unique_ptr<client_data> d(static_castc);
    client_data *d=(client_data*) data;
    pthread_detach(pthread_self());
    
    char buf[BUFSIZ];
    while (true) {
        memset(buf, 0, sizeof(buf));
        int ret = read(d->cfd, buf, sizeof(buf));
        if (ret <= 0) {
            if (ret == 0) {
                cout << "The other side has closed the connection.\n";
            } else {
                perror("read error");
            }
            break;
        }
        for (int i = 0; i < ret; ++i) {
            buf[i] = toupper(buf[i]);
        }
        write(d->cfd, buf, ret);
    }
    close(d->cfd);
    return nullptr;
}

int main(int argc, char** argv)
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8888);

    int ret = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0) {
        perror("Bind failed");
        close(lfd);
        return 1;
    }

    ret = listen(lfd, 128);
    if (ret < 0) {
        perror("Listen failed");
        close(lfd);
        return 1;
    }

    struct sockaddr_in cliaddr;
    socklen_t socklen = sizeof(cliaddr);
    pthread_t thread;

    while (true) {
        int cfd = accept(lfd, (sockaddr*)&cliaddr, &socklen);
        if (cfd < 0) {
            perror("Accept failed");
            continue;
        }

        client_data* data = new client_data(cfd, cliaddr);
        ret = pthread_create(&thread, nullptr, write_client, static_cast<void*>(data));
        if (ret != 0) {
            cerr << "Failed to create thread: " << strerror(ret) << endl;
            close(cfd);
            delete data;
        }
    }

    close(lfd);
    return 0;
}
