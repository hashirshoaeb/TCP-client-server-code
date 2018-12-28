#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "pthread.h"
#include "iostream"
#include "unistd.h"

//struct sockaddr_in { uint8_t sin_len; sa_family_t sin_family; in_port_t sin_port; struct in_addr sin_addr; char sin_pad[16];

// we made socket
// we bind it
// we started listening // listening state
// accept, is a blocking call
void* recieveData(void *connfd)
{
    long handler = (long) connfd;
    while (1) {
        char buffer[100];
        recv(handler, buffer, 100, 0);
        printf("\n Server Recieved message: %s ", buffer);
        printf("\n");
    }
    
}

int main()
{
    //SOCK_STREAM tells that it is tcp
    // if we write DGRAM instead of SOCK_STREAM, it will works as udp then
    // TODO: terminal ..> man socket
    int fd = socket(AF_INET, SOCK_STREAM, 0); //?? why AF_INET, SOCK_STREAM, is not defined  anywhere??
    // socket returns a handler in fd. if fd is positive its fine if negative
    if(fd == -1)
    {
        perror("SOCKET Creation Failed \n");
        exit(1);
    }
    
    struct sockaddr_in addr; //?? why we write struct here??
    
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80); // we can write any port number inplace of 80
    
    // well known py bind hota hai,
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        // an error occurred
        perror("BINDING failed on socket \n");
        exit(1);
    }
    
    // kitny log queue mai a sakty hain
    int backlog = 10;
    
    if (listen(fd, backlog)== -1)
    {
        perror("Listening failed on socket \n");
        exit(1);
    }
    
    long connfd;
    struct sockaddr_in cliaddr; // client address
    socklen_t cliaddr_len = sizeof(cliaddr); //lenght of client address
    
    //blocking function, like cin in c++, program  yaha aa k ruck jata hai or wait krta hai..
    connfd = accept(fd, (struct sockaddr *) &cliaddr, &cliaddr_len);
    
    if(connfd == -1)
    {
        perror("ACCEPT failed on socket \n");
        exit(1);
    }
    
    pthread_t th;
    pthread_create(&th, NULL, recieveData, (void *)connfd);
    
    while (1)
    {
        /*char buffer[100];
        recv(connfd, buffer, 100, 0);
        printf("\n Server Recieved message: %s ", buffer);
        printf("\n");*/
        
        char sbuffer[100];
        scanf("%s", sbuffer);
        printf(" \n Server sending data to client: %s", sbuffer);
        send(connfd, sbuffer, strlen(sbuffer),0);
        
    }

    
    
    printf("\n Server Exiting \n");
    return 0;
}
