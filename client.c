#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <arpa/inet.h>


int quit(char message[100]) {
    return tolower(message[0]) == 'q' && tolower(message[1]) == 'u' && tolower(message[2]) == 'i' && tolower(message[3]) == 't';
}

int main(int argc, char const *argv[])
{
    char IPbuffer[20];
    in_port_t port;

    printf("Enter the IP address of the server: ");
    fgets(IPbuffer, 20, stdin);
    printf("Enter the port number of the server: ");
    scanf("%d", &port);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_aton(IPbuffer, (struct in_addr *)&serv_addr.sin_addr.s_addr);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof serv_addr) == -1) {
        perror("Connection Failed\n");
        exit(1);
    }
    
    printf("\nConnected to the server\n");
    sleep(2);
    while(1) {
        printf("Enter a message (< 100): ");
        char message[100];
        fgets(message, 100, stdin);
        write(sock, &message, sizeof message);
        if (quit(message)) break;
    }

    close(sock);
    return 0;
}
