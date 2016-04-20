#include "GameLoop.h"

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
	mBall->setWorldPosition(5.0f,5.0f);
	mBall->mShadow->setTexture(mShadowTexture,true);
	mBall->setDeltaTime(&deltaTime);
	

	//PLAYER///////////////////////////////////
	mPlayerController = new PlayerController();

	mPlayer = new Player(true);
	mPlayer->Initialise(sf::Vector2f(100.0f,100.0f),sf::Color::Color(255.0f,0.0f,255.0f,255.0f));//TODO//21-10-15//MagicNumbers
	mPlayer->setTexture(mPlayertexture,false);
	mPlayer->setOrigin((TILE_SCALE/2),(TILE_SCALE*3/4));//TODO//21-10-15//MagicNumbers
	mPlayer->mShadow->setTexture(mShadowTexture,true);
	mPlayer->setDeltaTime(&deltaTime);
	mPlayer->SetInput(mPlayerController->inputToSend);
	mPlayer->SetBall(mBall);
	//player.setCameraPos(mainCamera.position);
	
	if(mServer)
	{
		mPlayers.push_back(mPlayer);
	}
	
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
	mPlayerController->Update();
	
	if(mServer)
	{
		//mPlayer->Input(mPlayerController->SendInput());//Change to initialising input instead of doing it everyframe
		//mPlayerRed->Input(mNetworkPlayerController.SendInput());
	}
	else if(mClient)
	{
		PlayerController tempController;
		tempController.state.leftStick.x=60.0f;
		tempController.state.leftStick.y=0.0f;

		tempController.state.rightStick.x=100.0f;
		tempController.state.rightStick.y=0.0f;

		//mPlayerRed->Input(tempController.SendInput());

		//mPlayer->Input(mNetworkPlayerController.SendInput());
	}

	//mainCamera.Input(mPlayerController->SendInput());
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

	mainCamera.SetTargetWorld(mBall->getWorldPosition());
	mainCamera.Update();


	mainView.setCenter(ConvertVector2(mainCamera.position));

	
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
	case 0://Client or server
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

	case 1://Server
		
		if(mSocket->Receive(mNetworkData)==-1)
		{
			//Would block
		}
		else
		{
			//read message
			//mNetworkData
			
		}

		if(mGameReady)
		{
			mInGame = true;
		}
		break;
	case 2://Client enter serverIP
		if(mIPReady)
		{
			//CLIENT SOCKET SETUP
			mSocket->Initialise();
			mSocket->SetTargetAddress((char*)mServerIP.c_str());
			mSocket->ClearBuffers();

			mSocket->SetBuffer("hello there");
			
			
			if(mSocket->Send()==-1)
			{
				MessageBox(NULL,L"Error Sending Message to Server",L"Error",MB_OK);
			}
			
			
			//Sleep(5000);

			/*
			if(mSocket->Receive(mNetworkData)==-1)
			{
				MessageBox(NULL,L"Error Receiving Message from Server",L"Error",MB_OK);
			}
			*/


			mMenuState = 3;
		}
		break;
	case 3://Client

		//Connect to server

		if(mGameReady)
		{
			mInGame = true;
		}
		break;
	}


	MenuDraw();

	//DisplayText(Vector2(0.0f,0.0f),"Hello");
	//transition
	
	if(mInGame)
	{
		GameInitialise();
		//mInGame = true;
	}
	
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
		mPtrWindow->draw(DisplayText(Vector2(80.0f,128.0f),"Press ENTER to begin the game"));
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

void GameLoop::SpawnPlayer(PlayerController* ptrPlayerControllerIn)
{
	Player* tempPlayer;
	tempPlayer = new Player(true);

	tempPlayer->Initialise(sf::Vector2f(100.0f,100.0f),sf::Color::White);//TODO//21-10-15//MagicNumbers	
	tempPlayer->setTexture(mPlayertexture,false);
	tempPlayer->setOrigin((TILE_SCALE/2),(TILE_SCALE*3/4));//TODO//21-10-15//MagicNumbers
	tempPlayer->mShadow->setTexture(mShadowTexture,true);
	tempPlayer->setDeltaTime(&deltaTime);
	tempPlayer->SetInput(ptrPlayerControllerIn->inputToSend);
	tempPlayer->SetBall(mBall);


	mPlayers.push_back(tempPlayer);
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