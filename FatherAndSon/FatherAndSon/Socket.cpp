#include "Socket.h"

Socket::Socket()
{
	//m_Socket = 0;
	m_AddressFamilySpecification = 0;
	m_Type = 0;
	m_Protocol = 0;

	m_MessageSize = MESSAGESIZE;

	m_SendToAddress = nullptr;
	m_SentFromAddress = nullptr;

	m_BufferToSend = new char[MESSAGESIZE];
	m_BufferReceived = new char[MESSAGESIZE];

	
	m_SentFromAddress = new sockaddr_in();
	m_SendToAddress = new sockaddr_in();
}

Socket::~Socket()
{
	delete m_SentFromAddress;
	m_SentFromAddress = nullptr;

	delete m_SendToAddress;
	m_SendToAddress = nullptr;

	delete m_BufferToSend;
	m_BufferToSend = nullptr;

	delete m_BufferReceived;
	m_BufferReceived = nullptr;
}

void Socket::Initialise()
{
	int tempPortIncrement = 0; 
	while(!SocketSetUp(SOCK_DGRAM,CURRENTIP,PORTNUMBERMIN,tempPortIncrement))
	{
		tempPortIncrement++;
		if((PORTNUMBERMIN+tempPortIncrement) > PORTNUMBERMAX)
		{
			//die("Ports unavailable");//TODO//ReplaceDie
			MessageBox(NULL,L"Error Ports unavailable",L"Error",MB_OK);
			return;
		}
	}
	
}

void Socket::InitialiseServerSocket()
{
	int tempPortIncrement = 0; 
	if(!SocketSetUp(SOCK_DGRAM,SERVERIP,SERVERPORT,tempPortIncrement))
	{
		//die("Ports unavailable");//TODO//ReplaceDie
		MessageBox(NULL,L"Error Ports unavailable",L"Error",MB_OK);
	}
}
void Socket::Initialise(char* currentIPIn)
{
	SocketSetUp(SOCK_DGRAM,currentIPIn,PORTNUMBERMIN,PORTNUMBERMAX);
}
void Socket::Initialise(char* currentIPIn, int socketTypeIn)
{
	SocketSetUp(socketTypeIn,currentIPIn,PORTNUMBERMIN,PORTNUMBERMAX);
}
void Socket::Initialise(char* currentIPIn,int socketTypeIn,int serverPort)
{
	SocketSetUp(socketTypeIn,currentIPIn,serverPort,PORTNUMBERMAX);
}

bool Socket::SocketSetUp(int socketTypeIn,char* currentIPIn,int serverPortMin,int serverPortModifier)
{
	// Initialise the WinSock library -- we want version 2.2.
	int error = WSAStartup(0x0202, &w);
	if (error != 0)
	{
		fprintf(stderr, "Error: %s\n", "WSAStartup failed");
		exit(1);
	}
	if (w.wVersion != 0x0202)
	{
		fprintf(stderr, "Error: %s\n", "Wrong WinSock version");
		exit(1);
	}

	//Create The Server Socket
	m_Socket = socket(AF_INET, socketTypeIn, 0);
	
	


	bool nonBlockingIn = true;
	if(nonBlockingIn)
	{
		u_long value = 1;//0 for blocking !0 for non-blocking
		ioctlsocket(m_Socket, FIONBIO, &value);
	}

	// Fill out a sockaddr_in structure to describe the address we'll listen on.
	sockaddr_in tempSockAddr;
	tempSockAddr.sin_family = AF_INET;
	tempSockAddr.sin_addr.s_addr = inet_addr(currentIPIn);
	// htons converts the port number to network byte order (big-endian).
	tempSockAddr.sin_port = htons(serverPortMin+serverPortModifier);//TODO//make manager assign port?

	// Bind the socket to that address.
	if (bind(m_Socket, (LPSOCKADDR) &tempSockAddr, sizeof(tempSockAddr)) != 0)
	{
		fprintf(stderr, "Error: %s\n", "bind failed");
		return false;
	}
	

	// ntohs does the opposite of htons.
	printf("socket bound to port %d\n", ntohs(tempSockAddr.sin_port));

	m_BufferToSend = new char;
	m_BufferReceived = new char;


}

void Socket::SetTargetAddress(char* TargetIPIn)
{
	// Fill out a sockaddr_in structure with the address that
	// we want to send to.
	m_SendToAddress->sin_family = AF_INET;

	m_SendToAddress->sin_port = htons(SERVERPORT);
	m_SendToAddress->sin_addr.s_addr = inet_addr(SERVERIP);
}

int Socket::Receive(char*& dataOut)
{
	
	int fromAddrSize = sizeof(*m_SentFromAddress);
	
	int count = recvfrom(m_Socket, m_BufferReceived, m_MessageSize, 0,
						(sockaddr *) m_SentFromAddress, &fromAddrSize);

	if (count == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();
		
		if (errorCode == WSAEWOULDBLOCK)
		{    
			// recv would have blocked,    
			//MessageBox(NULL,L"Error WSAEWOULDBLOCK",L"Error",MB_OK);
		} 
		else 
		{  
			MessageBox(NULL,L"FatalError networking things went wrong",L"Error",MB_OK);
			exit(1);
		}	
	}
	else 
	{  
		// data read successfully
	}

	
	/*
	printf("Received %d bytes from address %s port %d: '",
		count, inet_ntoa(m_SentFromAddress->sin_addr), ntohs(m_SentFromAddress->sin_port));
		fwrite(m_BufferReceived, 1, count, stdout);
		printf("'\n");
		*/

	dataOut = m_BufferReceived;

	return count;
}

int Socket::Send()
{
	//m_SendToAddress = m_SentFromAddress;

	//printf("Sending message to %s %d \n",inet_ntoa(m_SendToAddress->sin_addr),ntohs(m_SendToAddress->sin_port));
	int count = sendto(m_Socket, m_BufferToSend, m_MessageSize, 0,
		   (const sockaddr*) m_SendToAddress, sizeof(*m_SendToAddress));
		   
	return count;
}

int Socket::SendEcho()
{
	//printf("Sending Echo message to %s %d \n",inet_ntoa(m_SentFromAddress->sin_addr),ntohs(m_SentFromAddress->sin_port));
	int count = sendto(m_Socket, m_BufferToSend, m_MessageSize, 0,
		   (const sockaddr *) m_SentFromAddress, sizeof(*m_SentFromAddress));
		   
	return count;
}


void Socket::SetBuffer(char* bufferIn)
{
	//m_Buffer = bufferIn;
	memset(m_BufferToSend, '-', MESSAGESIZE);
	m_BufferToSend[MESSAGESIZE-1] = '\0';
	
	for(int i = 0; bufferIn[i] != '\0';i++)
	{
		m_BufferToSend[i] = bufferIn[i];
	}
	
}

void Socket::ClearBuffers()
{
	memset(m_BufferReceived,'-',MESSAGESIZE);
	m_BufferReceived[MESSAGESIZE-1] = '\0';

	memset(m_BufferToSend,'-',MESSAGESIZE);
	m_BufferToSend[MESSAGESIZE-1] = '\0';
		
}



