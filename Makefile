CCFLAGS=-g

all: clean client server

client:
	gcc $(CCFLAGS) source/client/*.c -o bin/client

server:
	gcc $(CCFLAGS) source/server/*.c -o bin/server

clean:
	rm -f bin/client
	rm -f bin/server
