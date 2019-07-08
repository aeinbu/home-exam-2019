// Based on http://www.linuxhowtos.org/C_C++/socket.htm
#include <assert.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <sys/socket.h>
// #include <sys/types.h>
#include <unistd.h>

void error(const char *msg, int socket) {
    printf("socket %d: %s\n", socket, msg);
    exit(EXIT_FAILURE);
}

int setupServerSocket(int port) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
        error("ERROR opening socket", server_socket);

    struct sockaddr_in server_addr;
    memset((char *)&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    if (bind(server_socket, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        close(server_socket);
        error("ERROR on binding", server_socket);
    }

    return server_socket;
}

int main(int argc, char *argv[]) {
    assert(argc >= 2);
    int port = atoi(argv[1]);

    int server_socket = setupServerSocket(port);
    listen(server_socket, 5);

    fd_set all_fds;
    FD_ZERO(&all_fds);
    FD_SET(server_socket, &all_fds);

    fd_set read_fds;
    int largest_socket = server_socket;

    while (true) {
        read_fds = all_fds;
        if (select(largest_socket + 1, &read_fds, NULL, NULL, NULL) == -1) {
            close(server_socket);
            error("select failed", -1);
        }

        for (int current_socket = 0; current_socket <= largest_socket;
             current_socket++) {
            if (FD_ISSET(current_socket, &read_fds)) {
                printf("SERVER FD-%d: ready for reading!\n", current_socket);

                if (current_socket == server_socket) {
                    // register client socket!
                    printf("receiving from server socket %d\n", current_socket);

                    struct sockaddr_in client_addr;
                    socklen_t clilen = sizeof(client_addr);
                    int incoming_client_socket =
                        accept(server_socket, (struct sockaddr *)&client_addr,
                               &clilen);
                    if (incoming_client_socket < 0) {
                        close(server_socket);
                        error("ERROR on accept", current_socket);
                    }

                    printf("Incoming client on FD-%d is %d.%d.%d.%d:%d\n",
                           incoming_client_socket,
                           ((client_addr.sin_addr.s_addr) >> 0) & 0xFF,
                           (client_addr.sin_addr.s_addr >> 8) & 0xFF,
                           (client_addr.sin_addr.s_addr >> 16) & 0xFF,
                           (client_addr.sin_addr.s_addr >> 24) & 0xFF,
                           client_addr.sin_port);

                    // int client_socket = current_socket;
                    // char buffer[1024];
                    // memset(buffer, '\0', sizeof(buffer));
                    // int n = recv(client_socket, buffer, sizeof(buffer) - 1,
                    // 0); if (n < 0) { 	FD_CLR(client_socket, &all_fds);
                    // 	error("ERROR reading from socket (disconnected?)",
                    // current_socket);
                    // }

                    // if (strcmp("exit\r\n", buffer) == 0) {
                    // 	printf("fd-%d: Closing socket!\n", client_socket);
                    // 	close(client_socket);
                    // 	FD_CLR(client_socket, &all_fds);
                    // 	break;
                    // }

                    // printf("FD-%d RECEIVED: %s\n", client_socket, buffer);

                    printf("opening client socket %d\n", incoming_client_socket);
                    largest_socket = incoming_client_socket;
                    FD_SET(incoming_client_socket, &all_fds);
                } else {

                    // act on client socket traffic!
                    printf("FD-%d READY\n", current_socket);

                    int client_socket = current_socket;
                    char buffer[1024];
                    memset(buffer, '\0', sizeof(buffer));
                    int n = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
                    if (n < 0) {
                        FD_CLR(client_socket, &all_fds);
                        error("ERROR reading from socket (disconnected?)", current_socket);
                    }

                    if (strcmp("exit\r\n", buffer) == 0) {
                        printf("fd-%d: Closing socket!\n", client_socket);
                        close(client_socket);
                        FD_CLR(client_socket, &all_fds);
                        break;
                    }

                    printf("FD-%d RECEIVED:\n%s\n", client_socket, buffer);

                    n = write(client_socket,
                              "HTTP/1.1 200 OK\nContent-Type: "
                              "text/plain\nContent-Length:14\n\nHello Browser!",
                              78);
                    if (n < 0) {
                        close(server_socket);
                        error("ERROR writing to socket", current_socket);
                    }
                }
            }
        }
    }

    close(server_socket);
    return EXIT_SUCCESS;
}