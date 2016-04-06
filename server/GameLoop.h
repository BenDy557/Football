#ifndef _GAMELOOP_H_
#define _GAMELOOP_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "Socket.h"


class GameLoop
{
	//Funtions
public:
	GameLoop();

	void Update();
private:

	//Variables
public:

private:
	Socket m_Socket;

	bool m_Client;
	bool m_Server;

	char* data;//Networking data in/out

};


#endif