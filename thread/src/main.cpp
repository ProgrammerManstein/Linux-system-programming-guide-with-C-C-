#include<pthread.h>
#include<iostream>
#include<unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

using namespace std;

pthread_mutex_t mutex;
pthread_cond_t con;

void err(int ret,char *str){
    if(ret!=0){
        fprintf(stderr,"%s: %s\n",str,strerror(ret));
        pthread_exit(nullptr);
    }
}

class node{
    public:
    node(int v){
        val=v;
        next=nullptr;
    }
    int val;
    node *next;

};

node *head=nullptr;

void *produce(void *arg){
    int *item=(int*)arg;
    node* newnode=new node(*item);
    sleep(random()%3);
    pthread_mutex_lock(&mutex);
    
    newnode->next=head;
    head=newnode;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&con);
    sleep(random()%3);
    delete item;
    return nullptr;
}

void *consume(void *arg){
    pthread_mutex_lock(&mutex);
    while (!head)
    {
        pthread_cond_wait(&con,&mutex);
    }
    int out=head->val;

    cout<<"receive data: "<<head->val<<endl;

    node* temp = head;
    head = head->next;
    delete temp;
    pthread_mutex_unlock(&mutex);
    sleep(random()%5);

    char *ret = new char('a' + out);
    return (void*)ret;
}

int main(int argc,char** argv){
    int ret;
    mutex=PTHREAD_MUTEX_INITIALIZER;
    con=PTHREAD_COND_INITIALIZER;
    
    pthread_t pid[10];
    pthread_t cid[10];
    
    for(int i=0;i<10;i++){
        int *item=new int(i);
        ret=pthread_create(&pid[i],nullptr,produce,(void*)item);
        if (ret != 0) {
            err(ret, "pthread_create(produce)");
        }
    }
    for(int i=0;i<10;i++){
        ret=pthread_create(&cid[i],nullptr,consume,nullptr);
        if (ret != 0) {
            err(ret, "pthread_create(consume)");
        }
    }

    for(int i = 0; i < 10; i++){
        ret=pthread_join(pid[i],nullptr);
        if (ret != 0) {
            err(ret, "pthread_join(produce)");
        }
    }
    
    for (int i = 0; i < 10; i++)
    {   
        char *ch=nullptr;
        ret=pthread_join(cid[i],(void**)&ch);
        if (ret != 0) {
            err(ret, "pthread_join(consume)");
        }
        cout<<"result char: "<<(*ch)<<endl;
        delete ch;
    }


    while(head){
        node *temp=head;
        head=head->next;
        delete temp;
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&con);
}