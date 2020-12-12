all : cli serv
	
cli : client.c
	gcc -Wall -Wextra client.c semaphors.h -o client
serv : server.c
	gcc -Wall -Wextra server.c semaphors.h -o server
