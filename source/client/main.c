#include <assert.h>
#include <stdio.h>

int main(int argc, char *args[]) {
	assert(argc >= 2);
	printf("This is the client!!!\n");

	char *server = args[1];
	char *port = args[2];

	printf("Server: %s, port: %s\n", server, port);
	//args[1] address/name of server to connect to
	//args[2] port on server to connect to
	//args[3...] data for this node
}