#include "GameLoop.h"

GameLoop::~GameLoop()
{
	if(mNetworkData)
	{
		delete mNetworkData;
		mNetworkData = nullptr;
	}

	if(mSocket)
	{
		delete mSocket;
		mSocket = nullptr;
	}

	if(mBall)
	{
		delete mBall;
		mBall = nullptr;
	}

	if(mPlayerController)
	{
		delete mPlayerController;
		mPlayerController = nullptr;
	}

	if(mPlayer)
	{
		delete mPlayer;
		mPlayer = nullptr;
	}

	mPlayers.clear();

}
void GameLoop::LoadTextures()
{
	//DEBUG TEXT///////////////////////////////
	if (!verdana.loadFromFile("resources/verdana.ttf"))
	{
		// error...//TODO
		MessageBox(NULL,L"Error loading verdana.ttf",L"Error",MB_OK);
	}
}

void GameLoop::GameInitialise()
{
	//TEXTURE LOADING//////////////////////////
	if(!tilesetTexture.loadFromFile("resources/TerrainTileset.png"))
	{
		MessageBox(NULL,L"Error loading TerrainTileset.png",L"Error",MB_OK);
	}

	if(!debugPointerTexture.loadFromFile("resources/PositionPointer.png"))
	{
		MessageBox(NULL,L"Error loading PositionPointer.png",L"Error",MB_OK);
	}

	if(!mShadowTexture.loadFromFile("resources/Shadow.png"))
	{
		MessageBox(NULL,L"Error loading Shadow.png",L"Error",MB_OK);
	}

	if(!mPlayertexture.loadFromFile("resources/Player.png"))
	{
		MessageBox(NULL,L"Error loading Player.png",L"Error",MB_OK);
	}
	
	if(!debugCameraTexture.loadFromFile("resources/CameraTarget.png"))
	{
		MessageBox(NULL,L"Error loading CameraTarget.png",L"Error",MB_OK);
	}

	if(!mBallTexture.loadFromFile("resources/Ball.png"))
	{
		MessageBox(NULL,L"Error loading Ball.png",L"Error",MB_OK);
	}

	

	//MAP//////////////////////////////////////
	//TODO//11-09-15//Loop and generate map
	for(int mapY=0;mapY<MAP_GRID_Y_MAX;mapY++)
	{
		for(int mapX=0;mapX<MAP_GRID_X_MAX;mapX++)
		{
			GameObjectIso tempTile(false);

			//tempTile.setSize(sf::Vector2f(TILE_SCALE,TILE_SCALE));//TODO//21-10-15//MagicNumbers
			//tempTile.setScale(sf::Vector2f(TILE_SCALE,TILE_SCALE));
			tempTile.setTexture(tilesetTexture,false);
			tempTile.setTextureRect(sf::IntRect(TILE_SCALE*(rand()%10),TILE_SCALE*(rand()%2),TILE_SCALE,TILE_SCALE));//TODO//11-09-15//Set to random floor type
			//tempTile.setCameraPos(mainCamera.position);
			tempTile.setOrigin((TILE_SCALE/2),(TILE_SCALE*3/4));//TODO//21-10-15//MagicNumbers
			tempTile.setWorldPosition(mapX,mapY);

			EnvironmentTiles.push_back(tempTile);//TODO//11-09-15//Externalise
		}
	}


	
	
	
	//GAMEOBJECTS//////////////////////////////
	//////////////////////////////GAMEOBJECTS//
	mBall = new Ball(true);
	//mBall->setSize(sf::Vector2f(TILE_SCALE,TILE_SCALE));
	//mBall->setScale(sf::Vector2f(TILE_SCALE,TILE_SCALE));
	mBall->setTexture(mBallTexture,true);
	mBall->setOrigin(TILE_SCALE/2,(TILE_SCALE*3/4));
	mBall->setWorldPosition(12.0f,9.0f);
	mBall->mShadow->setTexture(mShadowTexture,true);
	mBall->setDeltaTime(&deltaTime);
	

	//PLAYER///////////////////////////////////
	mPlayerController = new PlayerController();

	if(!mPlayer)
	{
		mPlayer = new Player(true);
	}

	for (std::list<Player*>::iterator it = mPlayers.begin(); it != mPlayers.end(); it++)
	{

		if((*it)->mTeam == 1)
		{
			(*it)->setColor(sf::Color::Blue);
		}
		else if((*it)->mTeam == 2)
		{
			(*it)->setColor(sf::Color::Red);
		}
		else
		{
			(*it)->setColor(sf::Color::Magenta);
		}

	}

	mPlayer->setTexture(mPlayertexture,false);
	mPlayer->setOrigin((TILE_SCALE/2),(TILE_SCALE*3/4));//TODO//21-10-15//MagicNumbers
	mPlayer->mShadow->setTexture(mShadowTexture,true);
	mPlayer->setDeltaTime(&deltaTime);
	mPlayer->SetInput(mPlayerController->inputToSend);
	mPlayer->SetBall(mBall);
	//player.setCameraPos(mainCamera.position);
	
	//mPlayers.push_back(mPlayer);
	
	
	//mControllers.push_back(mPlayerController);

	


	//CAMERA///////////////////////////////////
	//mainCamera.position//TODO//13-09-15//Proper initialise function
	mainCamera.SetTargetWorld(ConvertSFVector2(mBall->getPosition()));//SetToReference
	//cameraTargetIcon.setSize(sf::Vector2f(TILE_SCALE,TILE_SCALE));
	//cameraTargetIcon.setScale(sf::Vector2f(TILE_SCALE,TILE_SCALE));
	cameraTargetIcon.setTexture(debugCameraTexture,true);
	//cameraTargetIcon.setCameraPos(mainCamera.position);
	cameraTargetIcon.setOrigin(TILE_SCALE/2,(TILE_SCALE*3/4));//TODO//21-10-15//MagicNumbers
	cameraTargetIcon.setWorldPosition(0.0f,0.0f);

	//SF
	//mainView.setCenter(sf::Vector2f(mainCamera.position.x,mainCamera.position.y));
	mainView.setCenter(0.0f,0.0f);
	mainView.setSize(GLOBAL_S_WIDTH,GLOBAL_S_HEIGHT);
	mainView.zoom(1.0f);
	//mainView.rotate(45);

}

void GameLoop::SetClock(sf::Clock* tempClock)
{
	mPtrClock = tempClock;
}

void GameLoop::SetWindow(sf::RenderWindow* tempWindow)
{
	mPtrWindow = tempWindow;
}

void GameLoop::Update()
{
	if(mInGame)
	{
		GameUpdate();
	}
	else
	{
		MenuUpdate();
	}
}

void GameLoop::GameUpdate()
{	
	//INPUT
	mPlayerController->Update();
	
	if(mServer)
	{
		//mPlayer->Input(mPlayerController->SendInput());//Change to initialising input instead of doing it everyframe
		//mPlayerRed->Input(mNetworkPlayerController.SendInput());
	}
	else if(mClient)
	{
		mPlayerController->inputToSend->movementIntensity/=2;
	}

	mPtrWindow->setView(mainView);

	//DELTA TIME////////////////////////////
	prevGameTime = gameTime;
	gameTime = mPtrClock->getElapsedTime();//amount of microseconds passed

	if(gameTime.asMicroseconds() > 0)
	{
		deltaTimeReal = ((gameTime.asMicroseconds() - prevGameTime.asMicroseconds())/1000000.0f);
		deltaTime = deltaTimeReal * timeScale;
		if(deltaTime>0.2f)
		{
			deltaTime = 0.2f;
		}
		fps = (1/deltaTimeReal);
	}
	else
	{
		deltaTimeReal = 0;
		deltaTime = 0;
		fps = -1;
	}

	
	//FRAMES PER SECOND/////////////////////
	if(frameCount>=FRAME_COUNT_MAX)
	{
		frameCount = 0;
	}
	prevFps[frameCount] = fps;
	
	frameCount++;

	fpsAverage = 0;

	for(int i =0; i< FRAME_COUNT_MAX ; i++)
	{
		fpsAverage += prevFps[i]; 
	}

	fpsAverage /= FRAME_COUNT_MAX;//Average fps over x amount fo frames
	

	//EVENT LOOP////////////////////////////
	GameEventLoop();
   
	//UPDATES///////////////////////////////
	//mBall->moveWorldPosition(0.01f,0.03f);

	for (std::list<Player*>::iterator it = mPlayers.begin(); it != mPlayers.end(); it++)
	{
		(*it)->Update();
	}

	//mPlayer->Update();
	//mPlayerRed->Update();
	
	mBall->Update();


	for(int i=0;i<EnvironmentTiles.size();i++)//TODO//13-09-15//Delete
	{
		EnvironmentTiles[i].Update();
	}

	cameraTargetIcon.setWorldPosition(mainCamera.GetTargetWorldPosition().x,mainCamera.GetTargetWorldPosition().y);//TODO//22-10-15//Classify as debug ui
	cameraTargetIcon.Update();

	mainCamera.SetTargetWorld((mBall->getWorldPosition()+mPlayer->getWorldPosition())/2);
	mainCamera.Update();


	mainView.setCenter(ConvertVector2(mainCamera.position));

	

	//NETWORKING
	//SEND
	mNetUpdateTimer -= deltaTimeReal;
	if(mNetUpdateTimer <= 0.0f)
	{
		mNetUpdateTimer = 1.0f/mNetUpdateRate;
		if(mServer)
		{
			std::list<Socket*>::iterator itSock = mClients.begin();
			for (std::list<Player*>::iterator it = mPlayers.begin(); (it != mPlayers.end())||(itSock != mClients.end()); it++)
			{
				if( *it != mPlayer)
				{
					//BALL
					LocomotionData* tempLocomotionData = new LocomotionData();
					tempLocomotionData->mPosition = mBall->getWorldPosition();
					tempLocomotionData->mPositionZ = mBall->getWorldPositionZ();
					tempLocomotionData->mVelocity = mBall->GetVelocity();
					tempLocomotionData->mVelocityZ = mBall->GetVelocityZ();

					Serialise(PacketType::ballData,(char*)tempLocomotionData,mNetworkData);
				
					(*itSock)->SetBuffer(mNetworkData);
					if((*itSock)->Send()==-1)
					{
						MessageBox(NULL,L"Error Sending Update Message to client",L"Error",MB_OK);
					}
					else
					{
						//message sent
					}


					for(std::list<Player*>::iterator itPlayersToSend = mPlayers.begin(); itPlayersToSend != mPlayers.end(); itPlayersToSend++)
					{
						if(itPlayersToSend != it)
						{
							//LOCOMOTION

							/*
							//INPUT
							PlayerInputData* tempPlayerData = new PlayerInputData();
							tempPlayerData->playerNumber = (*itPlayersToSend)->mPlayerNumber;
							tempPlayerData->playerInput = (*itPlayersToSend)->GetPlayerInput();
							
							Serialise(PacketType::playerInputData,(char*)tempPlayerData,mNetworkData);
						
							(*itSock)->SetBuffer(mNetworkData);
							if((*itSock)->Send()==-1)
							{
								MessageBox(NULL,L"Error Sending Update Message to client",L"Error",MB_OK);
							}
							else
							{
								//message sent
							}
							*/

							//LOCOMOTION
							PlayerLocomotionData* tempPlayerLocomotionData = new PlayerLocomotionData();
							tempPlayerLocomotionData->playerNumber= (*itPlayersToSend)->mPlayerNumber;
							tempPlayerLocomotionData->locomotionData.mPosition = (*itPlayersToSend)->getWorldPosition();
							tempPlayerLocomotionData->locomotionData.mPositionZ = (*itPlayersToSend)->getWorldPositionZ();
							tempPlayerLocomotionData->locomotionData.mVelocity = (*itPlayersToSend)->getVelocity();
							tempPlayerLocomotionData->locomotionData.mVelocityZ = (*itPlayersToSend)->GetVelocityZ();
						
							Serialise(PacketType::playerLocomotionData,(char*)tempPlayerLocomotionData,mNetworkData);
						
							(*itSock)->SetBuffer(mNetworkData);
							if((*itSock)->Send()==-1)
							{
								MessageBox(NULL,L"Error Sending Update Message to client",L"Error",MB_OK);
							}
							else
							{
								//message sent
							}
						}						
					}

					itSock++;
				}
			}
		}
		else if(mClient)
		{
			
			PlayerLocomotionData* tempPlayerLocomotionData = new PlayerLocomotionData();
			tempPlayerLocomotionData->playerNumber= mPlayer->mPlayerNumber;
			tempPlayerLocomotionData->locomotionData.mPosition = mPlayer->getWorldPosition();
			tempPlayerLocomotionData->locomotionData.mPositionZ = mPlayer->getWorldPositionZ();
			tempPlayerLocomotionData->locomotionData.mVelocity = mPlayer->getVelocity();
			tempPlayerLocomotionData->locomotionData.mVelocityZ = mPlayer->GetVelocityZ();
						
			Serialise(PacketType::playerLocomotionData,(char*)tempPlayerLocomotionData,mNetworkData);
						
			mSocket->SetBuffer(mNetworkData);
			if(mSocket->Send()==-1)
			{
				MessageBox(NULL,L"Error Sending Update Message to client",L"Error",MB_OK);
			}
			else
			{
				//message sent
			}
		}
	}
	
	if(mPlayer->mBallKicked && mClient)
	{
		mPlayer->mBallKicked = false;

		LocomotionData* tempLocomotionData = new LocomotionData();
		tempLocomotionData->mPosition = mBall->getWorldPosition();
		tempLocomotionData->mPositionZ = mBall->getWorldPositionZ();
		tempLocomotionData->mVelocity = mBall->GetVelocity();
		tempLocomotionData->mVelocityZ = mBall->GetVelocityZ();

		Serialise(PacketType::ballData,(char*)tempLocomotionData,mNetworkData);
				
		mSocket->SetBuffer(mNetworkData);
		if(mSocket->Send()==-1)
		{
			MessageBox(NULL,L"Error Sending Update Message to server",L"Error",MB_OK);
		}
		else
		{
			//message sent
		}
	}





	//RECEIVE
	if(mServer)
	{
		bool messagesLeft = true;
		//while(messagesLeft)
		{
			if(mSocket->Receive(mNetworkData)==-1)
			{
				//wouldblock
				messagesLeft = false;
			}
			else
			{
				void* data = new char[MESSAGESIZE];
			
				switch(DeSerialise(data,mNetworkData))
				{
					
				case PacketType::ballData:
					{
					LocomotionData* usefulData = new LocomotionData;
					memcpy(usefulData,data,sizeof(LocomotionData));

					mBall->setWorldPosition(usefulData->mPosition.x,usefulData->mPosition.y);
					mBall->setWorldPositionZ(usefulData->mPositionZ);
					mBall->SetVelocity(usefulData->mVelocity,usefulData->mVelocityZ);
					break;
					}
					
					/*
				case PacketType::playerInputData:
					{
					PlayerInputData* usefulData = new PlayerInputData;
					memcpy(usefulData,data,sizeof(PlayerInputData));
					//TODO//////////////////////////////////////////////////////////
					break;
					}
					*/
				case PacketType::playerLocomotionData:
					{
					PlayerLocomotionData* usefulData = new PlayerLocomotionData;
					memcpy(usefulData,data,sizeof(PlayerLocomotionData));

					for(std::list<Player*>::iterator it = mPlayers.begin(); it != mPlayers.end(); it++)
					{
						if(usefulData->playerNumber == (*it)->mPlayerNumber)
						{
							(*it)->setWorldPosition(usefulData->locomotionData.mPosition.x,usefulData->locomotionData.mPosition.y);
							(*it)->setWorldPositionZ(usefulData->locomotionData.mPositionZ);
							(*it)->SetVelocity(usefulData->locomotionData.mVelocity.x,usefulData->locomotionData.mVelocity.y,usefulData->locomotionData.mVelocityZ);

							//it = mPlayers.end();
						}
					}
					
					break;
					}

				}
			}
		}
	}
	else if(mClient)
	{
		bool messagesLeft = true;
		while(messagesLeft)
		{
			if(mSocket->Receive(mNetworkData)==-1)
			{
				//wouldblock
				messagesLeft = false;
			}
			else
			{
				void* data = new char[MESSAGESIZE];
			

				switch(DeSerialise(data,mNetworkData))
				{
				case PacketType::ballData:
					{
					LocomotionData* usefulData = new LocomotionData;
					memcpy(usefulData,data,sizeof(LocomotionData));

					mBall->setWorldPosition(usefulData->mPosition.x,usefulData->mPosition.y);
					mBall->setWorldPositionZ(usefulData->mPositionZ);
					mBall->SetVelocity(usefulData->mVelocity,usefulData->mVelocityZ);
					break;
					}
				case PacketType::playerInputData:
					{
					PlayerInputData* usefulData = new PlayerInputData;
					memcpy(usefulData,data,sizeof(PlayerInputData));
					//TODO//////////////////////////////////////////////////////////
					break;
					}
				case PacketType::playerLocomotionData:
					{
					PlayerLocomotionData* usefulData = new PlayerLocomotionData;
					memcpy(usefulData,data,sizeof(PlayerLocomotionData));

					for(std::list<Player*>::iterator it = mPlayers.begin(); it != mPlayers.end(); it++)
					{
						if(usefulData->playerNumber == (*it)->mPlayerNumber)
						{
							(*it)->setWorldPosition(usefulData->locomotionData.mPosition.x,usefulData->locomotionData.mPosition.y);
							(*it)->setWorldPositionZ(usefulData->locomotionData.mPositionZ);
							(*it)->SetVelocity(usefulData->locomotionData.mVelocity.x,usefulData->locomotionData.mVelocity.y,usefulData->locomotionData.mVelocityZ);

							//it = mPlayers.end();
						}
					}
					
					break;
					}

				}
			}
		}
	}

	//UI////////////////////////////////////
	//sf::Mouse::getPosition(tempWind);

	//DEBUG RAYS////////////////////////////
	

	//COLLISION DETECTION///////////////////
	//Collision();

	//TEST AREA////////
	//END//////////////


	//DRAW//////////////////////////////////
	Draw();

}

void GameLoop::MenuUpdate()
{
	MenuEventLoop();

	switch(mMenuState)
	{
	case 0://///////////CLIENT/SERVER//
		if(mClient)
		{
			mMenuState = 2;
		}
		else if(mServer)
		{
			mMenuState = 1;
			//SERVER SOCKET SETUP
			mSocket->InitialiseServerSocket();
			mSocket->ClearBuffers();
		}
		break;

	case 1:///////////////////SERVER//
		if(mSocket->Receive(mNetworkData)==-1)
		{
			//wouldblock
		}
		else
		{
			//Deserialise
			void* data = new char[MESSAGESIZE];
			switch(DeSerialise(data,mNetworkData))
			{
			case PacketType::joinRequest:
				//Create socket for each client
				Socket* tempClientSocket = new Socket();
				tempClientSocket->Initialise();
				*(tempClientSocket->m_SendToAddress) = *(mSocket->m_SentFromAddress);
				tempClientSocket->ClearBuffers();
				mClients.push_back(tempClientSocket);


				JoinRequest* usefulData = new JoinRequest;
				memcpy(usefulData,data,sizeof(JoinRequest));

				mCurrentPlayersAmount++;

				//Send client acceptance message
				JoinGranted* tempJoinGranted = new JoinGranted();
				tempJoinGranted->playerNumber = mCurrentPlayersAmount;

				if(mCurrentPlayersAmount%2 == 0)
				{
					tempJoinGranted->playerTeam = 2;
				}
				else
				{
					tempJoinGranted->playerTeam = 1;
				}


				Serialise(PacketType::joinGranted,(char*)tempJoinGranted,mNetworkData);
				tempClientSocket->SetBuffer(mNetworkData);
				if(tempClientSocket->Send()==-1)
				{
					MessageBox(NULL,L"Error Sending Message to client",L"Error",MB_OK);
				}



				PlayerController* tempPlayerController = new PlayerController();
				mControllers.push_back(tempPlayerController);

				Player* tempPlayer = SpawnPlayer(tempPlayerController);
				tempPlayer->mPlayerNumber = mCurrentPlayersAmount;
				tempPlayer->mTeam = tempJoinGranted->playerTeam;

				switch(tempJoinGranted->playerTeam)
				{
				case 1:
					tempPlayer->setWorldPosition(4.0f,5.0f + tempJoinGranted->playerNumber);
					tempPlayer->setColor(sf::Color::Blue);
					
					break;
				
				case 2:
					tempPlayer->setWorldPosition(20.0f,5.0f + tempJoinGranted->playerNumber);
					tempPlayer->setColor(sf::Color::Red);
					break;

				default:
					tempPlayer->setWorldPosition(1.0f,1.0f);
					tempPlayer->setColor(sf::Color::Magenta);
					break;
				}
				
				break;
			}
		}


		if(mGameReady)
		{
			mPlayer = new Player(true);
			mPlayer->mPlayerNumber = 1;
			mPlayer->mTeam = 1;
			mPlayer->setWorldPosition(4.0f,6.0f);
			mPlayers.push_back(mPlayer);
			

			int tempPlayerNumberToSend = 1;

			for (std::list<Socket*>::iterator itSock = mClients.begin(); itSock!=mClients.end();itSock++)
			{
				GameStart* tempGameStart = new GameStart();
				tempGameStart->numberOfPlayers = mCurrentPlayersAmount;

				Serialise(PacketType::gameStart,(char*)tempGameStart,mNetworkData);

				(*itSock)->ClearBuffers();
				(*itSock)->SetBuffer(mNetworkData);
				if((*itSock)->Send()==-1)
				{
					MessageBox(NULL,L"Error Sending Message to clients",L"Error",MB_OK);
				}


				for (std::list<Player*>::iterator it = mPlayers.begin(); it != mPlayers.end(); it++)
				{			

					PlayerLocomotionData* tempPlayerLocomotionData = new PlayerLocomotionData;
					tempPlayerLocomotionData->playerNumber = tempPlayerNumberToSend;
					tempPlayerLocomotionData->locomotionData.mPosition =  (*it)->getWorldPosition();
					tempPlayerLocomotionData->locomotionData.mPositionZ = (*it)->getWorldPositionZ();
					tempPlayerLocomotionData->locomotionData.mVelocity = (*it)->getVelocity();
					tempPlayerLocomotionData->locomotionData.mVelocityZ = (*it)->GetVelocityZ();

					Serialise(PacketType::playerLocomotionData,(char*)tempPlayerLocomotionData,mNetworkData);

					(*itSock)->ClearBuffers();
					(*itSock)->SetBuffer(mNetworkData);
					if((*itSock)->Send()==-1)
					{
						MessageBox(NULL,L"Error Sending Message to clients",L"Error",MB_OK);
					}
				
					tempPlayerNumberToSend++;
				}
			}

			mInGame = true;
		}
		break;

	case 2:///////////////////CLIENT//enter serverIP
		if(mIPReady)
		{
			//CLIENT SOCKET SETUP
			mSocket->Initialise();
			mSocket->SetTargetAddress((char*)mServerIP.c_str());
			mSocket->ClearBuffers();
			
			JoinRequest* tempJoinRequest = new JoinRequest();
			tempJoinRequest->partA[0] = 'h';
			tempJoinRequest->partA[1] = 'e';
			tempJoinRequest->partA[2] = 'l';
			tempJoinRequest->partA[3] = 'l';
			tempJoinRequest->partA[4] = 'o';
			tempJoinRequest->partA[5] = '\0';
						   
			tempJoinRequest->partB[0] = 'b';
			tempJoinRequest->partB[1] = 'a';
			tempJoinRequest->partB[2] = 'n';
			tempJoinRequest->partB[3] = 't';
			tempJoinRequest->partB[4] = 's';
			tempJoinRequest->partB[5] = '\0';

			
			//Serialise
			Serialise(PacketType::joinRequest,(char*)tempJoinRequest,mNetworkData);
			mSocket->SetBuffer(mNetworkData);
			if(mSocket->Send()==-1)
			{
				MessageBox(NULL,L"Error Sending Message to Server",L"Error",MB_OK);
			}

			mMenuState = 3;
		}
		break;
	case 3:///////////////////CLIENT//waiting for game start
		
		
		if(mSocket->Receive(mNetworkData)==-1)
		{
			//Would block
		}
		else
		{
			void* data = new char[MESSAGESIZE];
			

			switch(DeSerialise(data,mNetworkData))
			{
			case PacketType::joinGranted:
				{
					JoinGranted* usefulData = new JoinGranted();
					memcpy(usefulData,data,sizeof(JoinGranted));

					mPlayer = new Player(true);
					mPlayer->mPlayerNumber = usefulData->playerNumber;
					mPlayer->mTeam = usefulData->playerTeam;
					
					break;
				}

			case PacketType::gameStart:
				{
					GameStart* usefulData = new GameStart;
					memcpy(usefulData,data,sizeof(GameStart));

					mPlayers.clear();
					
					int iModifier = 0;
					for(int i = 1; i < usefulData->numberOfPlayers; i++)
					{
						PlayerController* tempPlayerController = new PlayerController();
					
						Player* tempPlayer = SpawnPlayer(tempPlayerController);
						
						if(i == mPlayer->mPlayerNumber)
						{
							iModifier = 1;
						}
						
						tempPlayer->mPlayerNumber = i + iModifier;

						mControllers.push_back(tempPlayerController);
					}

					mPlayers.push_back(mPlayer);

					break;
				}

			case PacketType::playerLocomotionData:
				{
					PlayerLocomotionData* usefulData = new PlayerLocomotionData();
					memcpy(usefulData,data,sizeof(PlayerLocomotionData));

					for (std::list<Player*>::iterator it = mPlayers.begin(); it != mPlayers.end(); it++)
					{
						if((*it)->mPlayerNumber == usefulData->playerNumber)
						{
								
							(*it)->setWorldPosition(usefulData->locomotionData.mPosition.x,usefulData->locomotionData.mPosition.y);
							(*it)->setWorldPositionZ(usefulData->locomotionData.mPositionZ);
							(*it)->SetVelocity(usefulData->locomotionData.mVelocity.x,usefulData->locomotionData.mVelocity.y,usefulData->locomotionData.mVelocityZ);


							if((*it)->mPlayerNumber%2 == 0)
							{
								(*it)->mTeam = 2;
							}
							else
							{
								(*it)->mTeam = 1;
							}


							playersSetUp++;
						}
					}

					if(playersSetUp == mPlayers.size())
					{
						mInGame = true;
					}
				
					break;
				}
			}
		}
		
		break;
	}

	MenuDraw();

	if(mInGame)
	{
		GameInitialise();
	}
}

void GameLoop::Serialise(PacketType packetTypeIn, char* dataIn,char* bufferOut)
{

	size_t bufferSize;
	
	PacketType tempPacketType;
	tempPacketType = packetTypeIn;
	
	memcpy(bufferOut, &tempPacketType, sizeof(PacketType));
	
	switch(packetTypeIn)
	{
	case PacketType::joinRequest:
		memcpy(&bufferOut[sizeof(PacketType)], dataIn, sizeof(JoinRequest));
		break;

	case PacketType::ballData:
		memcpy(&bufferOut[sizeof(PacketType)], dataIn, sizeof(LocomotionData));
		break;

	case PacketType::playerLocomotionData:
		memcpy(&bufferOut[sizeof(PacketType)], dataIn, sizeof(PlayerLocomotionData));
		break;

	case PacketType::playerInputData:
		memcpy(&bufferOut[sizeof(PacketType)], dataIn, sizeof(PlayerInputData));
		break;

	case PacketType::joinGranted:
		memcpy(&bufferOut[sizeof(PacketType)], dataIn, sizeof(JoinGranted));
		break;

	case PacketType::gameStart:
		memcpy(&bufferOut[sizeof(PacketType)], dataIn, sizeof(GameStart));
		break;
	}

	
}

PacketType GameLoop::DeSerialise(void*& dataOut, char* bufferIn)
{
	PacketType* tempPacketType = new PacketType();
	*tempPacketType = PacketType::nullPacket;


	//Copying Enum
	char* tempBuffer = new char[16];
	for(int i = 0; i < sizeof(PacketType);i++)
	{
		tempBuffer[i] = bufferIn[i];
	}


	*tempPacketType = (PacketType)*tempBuffer;

	switch(*tempPacketType)
	{
	case PacketType::joinRequest:
		memcpy(dataOut,&bufferIn[sizeof(PacketType)],sizeof(JoinRequest));
		break;
	case PacketType::ballData:
		memcpy(dataOut,&bufferIn[sizeof(PacketType)],sizeof(LocomotionData));
		break;
	case PacketType::playerLocomotionData:
		memcpy(dataOut,&bufferIn[sizeof(PacketType)],sizeof(PlayerLocomotionData));
		break;
	case PacketType::playerInputData:
		memcpy(dataOut,&bufferIn[sizeof(PacketType)],sizeof(PlayerInputData));
		break;
	case PacketType::joinGranted:
		memcpy(dataOut,&bufferIn[sizeof(PacketType)],sizeof(JoinGranted));
		break;
	case PacketType::gameStart:
		memcpy(dataOut,&bufferIn[sizeof(PacketType)],sizeof(GameStart));
		break;
	}


	return *tempPacketType;
}

void GameLoop::Collision()
{

	
}

void GameLoop::MenuDraw()
{
	mPtrWindow->draw(DisplayText(Vector2(50.0f,25.0f),"Fütball"));

	switch(mMenuState)
	{
	case 0:
		mPtrWindow->draw(DisplayText(Vector2(80.0f,100.0f),"Client or Server?"));
		mPtrWindow->draw(DisplayText(Vector2(80.0f,128.0f),"  (C)        (S)"));
		break;

	case 1://Server
		mPtrWindow->draw(DisplayText(Vector2(80.0f,100.0f),"You are the Server"));
		mPtrWindow->draw(DisplayText(Vector2(80.0f,156.0f),"Press ENTER to begin the game"));
		mPtrWindow->draw(DisplayText(Vector2(80.0f,128.0f),"Current Players: ",mCurrentPlayersAmount));
		break;

	case 2://Client IP Enter
		mPtrWindow->draw(DisplayText(Vector2(80.0f,100.0f),"You are a Client"));
		mPtrWindow->draw(DisplayText(Vector2(80.0f,128.0f),"Server IP address (D for default)"));
		mPtrWindow->draw(DisplayText(Vector2(80.0f,156.0f),mServerIP));
		
		break;

	case 3://Client
		mPtrWindow->draw(DisplayText(Vector2(80.0f,100.0f),"You are a Client"));
		mPtrWindow->draw(DisplayText(Vector2(80.0f,128.0f),"Press ENTER to begin the game"));
		break;


	}
	
}

void GameLoop::Draw()
{

	for(int i=0;i<EnvironmentTiles.size();i++)//TODO//11-09-15//Delete
	{
		mPtrWindow->draw(EnvironmentTiles[i]);
	}

	//mPtrWindow->draw(*mPlayer->mShadow);
	//mPtrWindow->draw(*mPlayer);

	for (std::list<Player*>::iterator it = mPlayers.begin(); it != mPlayers.end(); it++)
	{
		mPtrWindow->draw(*(*it)->mShadow);
		mPtrWindow->draw(*(*it));
	}
	
	
	mPtrWindow->draw(*mBall->mShadow);
	mPtrWindow->draw(*mBall);
	

	//UI////////////////////////////////////
	if(paused)
	{
		mPtrWindow->draw(DisplayText(Vector2(50.0f,25.0f),"Game Paused"));
	}


	//DEBUG/////////////////////////////////
	//tempWind.draw(cameraTargetIcon);
	//DEBUG TEXT////////////////////////////
	ShowDebug();
}

//EVENT LOOP////////////////////////////
void GameLoop::GameEventLoop()
{
	sf::Event event;
    while (mPtrWindow->pollEvent(event))
    {
		switch(event.type)
		{
		case sf::Event::Closed:
            mPtrWindow->close();
			break;
			
			//KEYBOARD//////////////////////////////
		case sf::Event::KeyPressed:

			if(event.key.code == sf::Keyboard::Escape)
			{
				mPtrWindow->close();
			}
			if(event.key.code == sf::Keyboard::Space)
			{
				Pause();
			}
			if(event.key.code == sf::Keyboard::R)
			{
				Reset();
			}


			if(event.key.code == sf::Keyboard::P)
			{
				SpawnPlayer(mPlayerController);
			}

			if(event.key.code == sf::Keyboard::Num0)
			{
				debugMode = 0;
			}
			if(event.key.code == sf::Keyboard::Num1)
			{
				debugMode = 1;
			}
			if(event.key.code == sf::Keyboard::Num2)
			{
				debugMode = 2;
			}
			if(event.key.code == sf::Keyboard::Num3)
			{
				debugMode = 3;
			}
			if(event.key.code == sf::Keyboard::Num4)
			{
				debugMode = 4;
			}
			

			break;

			//CONTROLLER INPUT//////////////////////


		case sf::Event::JoystickButtonPressed:
			//Button Numbers
			//A=0,B=1,X=2,Y=3,LB=4,RB=5,Back=6,Start=7,LS=8,RS=9
			if(event.joystickButton.joystickId == 0)
			{
				mPlayerController->state.buttons[event.joystickButton.button] = true;
			}

			if(event.joystickButton.joystickId == 1)
			{
				//mPlayerControllerRed.state.buttons[event.joystickButton.button] = true;
			}

			break;

		case sf::Event::JoystickButtonReleased:
			if(event.joystickButton.joystickId == 0)
			{
				mPlayerController->state.buttons[event.joystickButton.button] = false;
			}

			if(event.joystickButton.joystickId == 1)
			{
				//mPlayerControllerRed.state.buttons[event.joystickButton.button] = false;
			}

			break;
			
			//JOSTICK INPUT/////////////////////////
		case sf::Event::JoystickMoved:
			if(event.joystickMove.joystickId == 0)
			{
				//Left Stick
				if(event.joystickMove.axis == sf::Joystick::X)
				{
					mPlayerController->state.leftStick.x = event.joystickMove.position;		
				}
				if(event.joystickMove.axis == sf::Joystick::Y)
				{
					mPlayerController->state.leftStick.y = event.joystickMove.position;		
				}
				//Right Stick
				if(event.joystickMove.axis == sf::Joystick::U)
				{
					mPlayerController->state.rightStick.x = event.joystickMove.position;
				}
				if(event.joystickMove.axis == sf::Joystick::R)
				{
					mPlayerController->state.rightStick.y = event.joystickMove.position;		
				}
			}

			if(event.joystickMove.joystickId == 1)
			{
				//Left Stick
				if(event.joystickMove.axis == sf::Joystick::X)
				{
					//mPlayerControllerRed.state.leftStick.x = event.joystickMove.position;		
				}
				if(event.joystickMove.axis == sf::Joystick::Y)
				{
					//mPlayerControllerRed.state.leftStick.y = event.joystickMove.position;		
				}
				//Right Stick
				if(event.joystickMove.axis == sf::Joystick::U)
				{
					//mPlayerControllerRed.state.rightStick.x = event.joystickMove.position;
				}
				if(event.joystickMove.axis == sf::Joystick::R)
				{
					//mPlayerControllerRed.state.rightStick.y = event.joystickMove.position;		
				}
			}
			break;

		default:
			break;
		}
    }
}

void GameLoop::MenuEventLoop()
{
	sf::Event event;
    while (mPtrWindow->pollEvent(event))
    {
		switch(event.type)
		{
		case sf::Event::Closed:
            mPtrWindow->close();
			break;
			
			//KEYBOARD//////////////////////////////
		case sf::Event::KeyPressed:

			if(event.key.code == sf::Keyboard::Escape)
			{
				mPtrWindow->close();
			}
			if(event.key.code == sf::Keyboard::Return)
			{
				
				if(!mIPReady)
				{
					mIPReady = true;
				}
				else if(!mGameReady)
				{
					mGameReady = true;
				}

			}
			if(event.key.code == sf::Keyboard::C)
			{
				//mClient = true;
				mClient = true;
			}
			if(event.key.code == sf::Keyboard::S)
			{
				mServer = true;
				mIPReady = true;	
			}
			if(event.key.code == sf::Keyboard::D)
			{
				//DefualtSettings
				mServerIP = "127.0.0.1";
			}

			if(event.key.code == sf::Keyboard::Num0)
			{
				mServerIP += "0";
			}
			if(event.key.code == sf::Keyboard::Num1)
			{
				mServerIP += "1";
			}
			if(event.key.code == sf::Keyboard::Num2)
			{
				mServerIP += "2";
			}
			if(event.key.code == sf::Keyboard::Num3)
			{
				mServerIP += "3";
			}
			if(event.key.code == sf::Keyboard::Num4)
			{
				mServerIP += "4";
			}
			if(event.key.code == sf::Keyboard::Num5)
			{
				mServerIP += "5";
			}
			if(event.key.code == sf::Keyboard::Num6)
			{
				mServerIP += "6";
			}
			if(event.key.code == sf::Keyboard::Num7)
			{
				mServerIP += "7";
			}
			if(event.key.code == sf::Keyboard::Num8)
			{
				mServerIP += "8";
			}
			if(event.key.code == sf::Keyboard::Num9)
			{
				mServerIP += "9";
			}
			if(event.key.code == sf::Keyboard::Period)
			{
				mServerIP += ".";
			}

			if(event.key.code == sf::Keyboard::Left)
			{
				//MenuSelectLeft
			}
			if(event.key.code == sf::Keyboard::Right)
			{
				//MenuSelectLeft
			}
			if(event.key.code == sf::Keyboard::Up)
			{
				//MenuSelectLeft
			}
			if(event.key.code == sf::Keyboard::Down)
			{
				//MenuSelectLeft
			}

			break;
		}
	}
}

void GameLoop::ShowDebug()
{
	if(debugMode == 0)
	{
		//do nothing
	}
	else if(debugMode == 1)//TODO
	{
		mPtrWindow->draw(DisplayText(Vector2(0.0f,0.0f),"PlayerData"));
		mPtrWindow->draw(DisplayText(Vector2(0.0f,20.0f),"PlayerX",mPlayer->getPosition().x));
		mPtrWindow->draw(DisplayText(Vector2(0.0f,40.0f),"PlayerY",mPlayer->getPosition().y));
	}
	else if(debugMode == 2)//curr FPS Debug
	{
		mPtrWindow->draw(DisplayText(Vector2(0.0f,0.0f),"PerformanceData"));
		mPtrWindow->draw(DisplayText(Vector2(0.0f,20.0f),"FPS curr: ", (int)fpsAverage));
		mPtrWindow->draw(DisplayText(Vector2(0.0f,40.0f),"FrameTime: ", deltaTimeReal));
	}
	else if(debugMode == 3)//Controller Information
	{
		mPtrWindow->draw(DisplayText(Vector2(0.0f,0.0f),"ControllerData"));
		mPtrWindow->draw(DisplayText(Vector2(0.0f,20.0f),"JoystickX: ",mPlayerController->state.leftStick.x));
		mPtrWindow->draw(DisplayText(Vector2(0.0f,40.0f),"JoystickY: ",mPlayerController->state.leftStick.y));
	}
	else if(debugMode == 4)//Camera Information
	{
		mPtrWindow->draw(DisplayText(Vector2(0.0f,0.0f),"CameraData"));
		mPtrWindow->draw(DisplayText(Vector2(0.0f,20.0f),"CameraX",mainCamera.position.x));
		mPtrWindow->draw(DisplayText(Vector2(0.0f,40.0f),"CameraY",mainCamera.position.y));
	}
	else
	{
		//error
	}
	
	
	//tempWind.draw(player0.lastCollisionMarker);
	//mapGrid.mapTiles[player0.getTileY()][player0.getTileX()].setRotation(45);
	//tempWind.draw(mapGrid.mapTiles[player0.getTileY()][player0.getTileX()]);
}

void GameLoop::AIUpdate()
{
	//TODO
}

Player* GameLoop::SpawnPlayer(PlayerController* ptrPlayerControllerIn)
{
	Player* tempPlayer;
	tempPlayer = new Player(true);

	tempPlayer->Initialise(sf::Vector2f(100.f,100.f),sf::Color::White);//TODO//21-10-15//MagicNumbers	
	tempPlayer->setTexture(mPlayertexture,false);
	tempPlayer->setOrigin((TILE_SCALE/2),(TILE_SCALE*3/4));//TODO//21-10-15//MagicNumbers
	tempPlayer->mShadow->setTexture(mShadowTexture,true);
	tempPlayer->setDeltaTime(&deltaTime);
	tempPlayer->SetInput(ptrPlayerControllerIn->inputToSend);
	tempPlayer->SetBall(mBall);


	mPlayers.push_back(tempPlayer);

	return tempPlayer;
}

sf::Text GameLoop::DisplayText(Vector2 positionIn, std::string phraseIn, float valueIn)
{
	sf::Text tempMessage;

	tempMessage.setFont(verdana);
	tempMessage.setCharacterSize((TILE_SCALE/4));
	tempMessage.setColor(sf::Color::White);
	tempMessage.setPosition(positionIn.x,positionIn.y);

	std::stringstream tempStringStream(std::stringstream::in | std::stringstream::out);
	tempStringStream << valueIn;
	std::string tempstr = tempStringStream.str();
	tempMessage.setString(phraseIn+tempstr);

	return tempMessage;
}

sf::Text GameLoop::DisplayText(Vector2 positionIn, std::string phraseIn)
{
	sf::Text tempMessage;

	tempMessage.setFont(verdana);
	tempMessage.setCharacterSize((TILE_SCALE/4));
	tempMessage.setColor(sf::Color::White);
	tempMessage.setPosition(positionIn.x,positionIn.y);
	tempMessage.setString(phraseIn);

	return tempMessage;
}

void GameLoop::Reset()//TODO
{
	if(!paused)
	{
		Pause();
	}
}

void GameLoop::Pause()
{
	paused = !paused;

	if(paused)
	{
		timeScale = 0.0f;
	}
	else
	{
		timeScale = 1.0f;
	}
}