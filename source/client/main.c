#include <assert.h>
#include <stdio.h>

int main(int argc, char *args[]) {
	// assert(argc >= 2);
	// printf("This is the client!!!\n");

	// char *server = args[1];
	// char *port = args[2];

	// printf("Server: %s, port: %s\n", server, port);
	// //args[1] address/name of server to connect to
	// //args[2] port on server to connect to
	// //args[3...] data for this node

	// char *text = "Hei på deg!";
	// char *otherText = text + 4;

	// printf("%s\n", text);
	// printf("%s\n", otherText);

	char buffer[] = "A\0\0\0B\0\0\0C\0\0\0D\0\0\0";
	// int tall[] = {10,20,30,40,50};
	// int *noenTall = tall + 2;
	int *tall = (int*)buffer;

	printf("%d\n", tall[0]);
	printf("%d\n", tall[1]);
	printf("%d\n", tall[2]);
	printf("%d\n", tall[3]);

}