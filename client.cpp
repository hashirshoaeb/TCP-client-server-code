#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "pthread.h"
#include "iostream"
#include "unistd.h"
// we didn't bind the client,
//struct sockaddr_in { uint8_t sin_len; sa_family_t sin_family; in_port_t sin_port; struct in_addr sin_addr; char sin_pad[16];

// pehly server run krain phir client
void* recieveData(void *fd)
{
    long handler = (long) fd;
    while (1) {
        char rbuffer[100];
        recv(handler, rbuffer, 100, 0);
        printf("\n Client Recieved message: %s ", rbuffer);
        printf("\n");
    }
    
}

int main()

{
    long fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("SOCKET Creation Failed \n");
        exit(1);
    }
    
    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(80);
    
    // we put server ip here
    inet_aton("127.0.0.1",&s_addr.sin_addr);
    // 127.0.0.1 is self address
    

    if(connect(fd, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1)
    {
        perror("Connect failed \n");
        exit(1);
    }
    // connection established
    
    pthread_t th;
    pthread_create(&th, NULL, recieveData, (void *)fd);
    
    while (1)
    {
    
        char buffer[100];
        scanf("%s", buffer);
        printf("\n Client sending data to server: %s", buffer);
        send(fd, buffer, strlen(buffer),0);
            //send(socket, data, sizeofdata, )
        
        
        /*char rbuffer[100];
        recv(fd, rbuffer, 100, 0);
        printf("\n Client Recieved message: %s ", rbuffer);
        printf("\n");*/
    }

       printf("\n Client Exiting \n");
    return 0;
}
