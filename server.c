#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <linux/if.h>


void * worker(void * arg) {
    int newSocket = *((int *)arg);
    
    close(newSocket);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    int serverPort;
    printf("Enter the port number for server: ");
    scanf("%d", &serverPort);


    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));


    // 8-10 lines below are just to get the IP address
    int n;
    struct ifreq ifr;
    char array[] = "eth0";
    n = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , array , IFNAMSIZ - 1);
    ioctl(n, SIOCGIFADDR, &ifr);
    close(n);

    printf("\nStarting Server...\n");
    sleep(2);
    printf("Server started on %s:%d\n" , inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr), serverPort);
    sleep(1);
    printf("\nWaiting for clients...\n");
    listen(serverSocket, 5);

    // Print the global IP Address of this system
    


    pthread_t threadID;
    while(1) {
        addr_size = sizeof serverStorage;
        newSocket = accept(serverSocket, (struct sockaddr *)&serverStorage, &addr_size);

        int res = pthread_create(&threadID, NULL, worker, &newSocket);
        if (res != 0) {
            printf("Thread not created.\n");
        }
    }
    close(serverSocket);
    return 0;
}
