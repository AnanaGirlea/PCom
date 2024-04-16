CC=gcc -g
CFLAGS=-I.

client: client.c requests.c helpers.c buffer.c parson.c
	$(CC) -o client client.c requests.c helpers.c buffer.c parson.c -Wall

run: client
	valgrind --track-origins=yes ./client

clean:
	rm -f *.o client
