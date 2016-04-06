#ifndef SOCKET_H_
#define SOCKET_H_

// The IP address for the server
#define SERVERIP "127.0.0.1"
#define CURRENTIP "127.0.0.1"


// The UDP port number for the server
#define SERVERPORT 5555
#define PORTNUMBERMIN 5556
#define PORTNUMBERMAX 6000

// The (fixed) size of message that we send between the two programs
#define MESSAGESIZE_MAX 65507//MAX message size
#define MESSAGESIZE 16


#include <WinSock2.h>
#include <stdio.h>

class Socket
{
	//Function
public:
	Socket();
   ~Socket();

	void Initialise();
	void InitialiseServerSocket();
	void Initialise(char* serverIPIn);	
	void Initialise(char* serverIPIn,int socketTypeIn);
	void Initialise(char* serverIPIn,int socketTypeIn,int serverPort);
	bool SocketSetUp(int socketTypeIn,char* serverIPIn,int serverPortMin,int serverPortModifier);

	void SetTargetAddress(char* TargetIPIn);

	void Shutdown();

	void SetBuffer(char* bufferIn);

	int Send();
	int SendEcho();

	int Receive(char*& dataOut);
private:
	
	//Variables
public:

private:
	WSADATA w;

	SOCKET m_Socket;
	int *m_AddressFamilySpecification, *m_Type, *m_Protocol;

	sockaddr_in *m_SendToAddress;
	sockaddr_in *m_SentFromAddress;

	char* m_BufferReceived;
	char* m_BufferToSend;

	int m_MessageSize;
};


#endif