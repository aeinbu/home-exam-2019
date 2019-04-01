// Based on http://www.linuxhowtos.org/C_C++/socket.htm
#include <assert.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    assert(argc >= 2);
    int portno = atoi(argv[1]);

    int mainsocketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mainsocketfd < 0)
        error("ERROR opening socket");

    struct sockaddr_in server_addr;
    memset((char *)&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(portno);
    if (bind(mainsocketfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        error("ERROR on binding");

    listen(mainsocketfd, 5);

    struct sockaddr_in client_addr;
    socklen_t clilen = sizeof(client_addr);
    int clientsocketfd = accept(mainsocketfd, (struct sockaddr *)&client_addr, &clilen);
    if (clientsocketfd < 0)
        error("ERROR on accept");

    char buffer[256];
    memset(buffer, '\0', sizeof(buffer));
    int n = read(clientsocketfd, buffer, sizeof(buffer) - 1);
    if (n < 0)
        error("ERROR reading from socket");

    printf("Here is the message: %s\n", buffer);
    n = write(clientsocketfd, "I got your message!\n", 20);
    if (n < 0)
        error("ERROR writing to socket");

    close(clientsocketfd);
    close(mainsocketfd);
    return 0;
}