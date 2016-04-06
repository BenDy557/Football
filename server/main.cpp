/*	UDP server example - by Henry Fortuna and Adam Sampson

	When the user types a message, the client sends it to the server
	as a UDP packet. The server then sends a packet back to the
	client, and the client prints it out.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#include "GameLoop.h"
//#include "Socket.h"


// Prototypes
void die(const char *message);


int main()
{
	printf("Echo Server\n");

	GameLoop m_GameLoop;

	while(true)
	{
		m_GameLoop.Update();
	}

	
	// We won't actually get here, but if we did then we'd want to clean up...
	printf("Quitting\n");
	//closesocket(sock);//TODO
	WSACleanup();
	return 0;
}


// Print an error message and exit.
void die(const char *message) {
	fprintf(stderr, "Error: %s\n", message);
	exit(1);
}