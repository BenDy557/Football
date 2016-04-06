#include "GameLoop.h"


GameLoop::GameLoop()
{
	m_Server = false;
	m_Client = false;

	data = 0;
	data = new char[100];

	printf("Client or Server\n");
	printf("C for client, S for server\n");
	
	while(!m_Client && !m_Server)
	{	
		

		std::string line;
		std::getline(std::cin, line);
		

		if(line == "c")
		{
			m_Client = true;
			printf("Client\n");

			printf("target IP Address...\n");
			printf("d for default settings...\n");
			std::string lineIP;
			std::getline(std::cin, lineIP);
			if(lineIP == "d")
			{
				m_Socket.Initialise();
				m_Socket.SetTargetAddress(SERVERIP);
			}
			else
			{
				//m_Socket.Initialise((char*) lineIP.c_str(),SOCK_DGRAM,5555);//TODO//Need to make overloaded function actually function
				//m_Socket.SetTargetAddress((char*) lineIP.c_str());
			}

		}
		else if(line == "s")
		{
			m_Server = true;
			printf("Server\n");
			m_Socket.InitialiseServerSocket();
			//m_Socket.SetTargetAddress(TARGETIP);
		}
		
	}

}

void GameLoop::Update()
{
	

	if(m_Client)
	{
		printf("I am a Client...\n");
		printf("Type Message to Send...\n");
		
		fflush(stdout);
		
		std::string line;
		std::getline(std::cin, line);

		//TODO: line could be larger than MESSAGESIZE. need to protect against this.
		memset(data, '-', MESSAGESIZE);
		memcpy(data, line.c_str(), line.size());
		data[MESSAGESIZE-1] = '\0';
		
		m_Socket.SetBuffer(data);
		if(m_Socket.Send()==-1)
		{
			printf("SendError %d",WSAGetLastError());
		}

		m_Socket.Receive(data);
	}
	else if(m_Server)
	{
		printf("Waiting for a message...\n");

		m_Socket.Receive(data);

		m_Socket.SetBuffer("ECHO MESSAGE");
		m_Socket.SendEcho();
	}


	
}