#include "GameLoop.h"


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

	//DEBUG TEXT///////////////////////////////
	if (!verdana.loadFromFile("resources/verdana.ttf"))
	{
		// error...//TODO
		MessageBox(NULL,L"Error loading verdana.ttf",L"Error",MB_OK);
	}


	//MAP//////////////////////////////////////
	//TODO//11-09-15//Loop and generate map
	for(int mapY=0;mapY<MAP_GRID_Y_MAX;mapY++)
	{
		for(int mapX=0;mapX<MAP_GRID_X_MAX;mapX++)
		{
			GameObjectIso tempTile(false);

			tempTile.setSize(sf::Vector2f(TILE_SCALE,TILE_SCALE));//TODO//21-10-15//MagicNumbers
			tempTile.setTexture(&tilesetTexture,false);
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
	mBall->setSize(sf::Vector2f(TILE_SCALE,TILE_SCALE));
	mBall->setTexture(&mBallTexture,true);
	mBall->setOrigin(TILE_SCALE/2,(TILE_SCALE*3/4));
	mBall->setWorldPosition(5.0f,5.0f);
	mBall->mShadow->setTexture(&mShadowTexture,true);
	mBall->setDeltaTime(&deltaTime);
	
	//PLAYER///////////////////////////////////
	player = new Player(true);
	player->Initialise(sf::Vector2f(100.0f,100.0f),sf::Color::White);//TODO//21-10-15//MagicNumbers
	player->setTexture(&mPlayertexture,false);
	player->setOrigin((TILE_SCALE/2),(TILE_SCALE*3/4));//TODO//21-10-15//MagicNumbers
	player->mShadow->setTexture(&mShadowTexture,true);
	player->setDeltaTime(&deltaTime);
	player->SetBall(mBall);
	//player.setCameraPos(mainCamera.position);


	//CAMERA///////////////////////////////////
	//mainCamera.position//TODO//13-09-15//Proper initialise function
	mainCamera.SetTargetWorld(ConvertSFVector2(player->getPosition()));//SetToReference
	cameraTargetIcon.setSize(sf::Vector2f(TILE_SCALE,TILE_SCALE));
	cameraTargetIcon.setTexture(&debugCameraTexture,true);
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

void GameLoop::Update(sf::RenderWindow &tempWind, sf::Clock tempClock)
{
	player->Input(playerController.SendInput());
	mainCamera.Input(playerController.SendInput());
	tempWind.setView(mainView);

	//DELTA TIME////////////////////////////
	prevGameTime = gameTime;
	gameTime = tempClock.getElapsedTime();//amount of microseconds passed

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
	EventLoop(tempWind);
   
	//UPDATES///////////////////////////////
	//mBall->moveWorldPosition(0.01f,0.03f);

	player->Update();
	
	mBall->Update();


	

	for(int i=0;i<EnvironmentTiles.size();i++)//TODO//13-09-15//Delete
	{
		EnvironmentTiles[i].Update();
	}

	cameraTargetIcon.setWorldPosition(mainCamera.GetTargetWorldPosition().x,mainCamera.GetTargetWorldPosition().y);//TODO//22-10-15//Classify as debug ui
	cameraTargetIcon.Update();

	mainCamera.SetTargetWorld(player->getWorldPosition());
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
	Draw(tempWind);
}

void GameLoop::Collision()
{

	
}

void GameLoop::Draw(sf::RenderWindow &tempWind)
{

	for(int i=0;i<EnvironmentTiles.size();i++)//TODO//11-09-15//Delete
	{
		tempWind.draw(EnvironmentTiles[i]);
	}

	tempWind.draw(*player->mShadow);
	tempWind.draw(*player);
	//tempWind.draw(cameraTargetIcon);
	
	tempWind.draw(*mBall->mShadow);
	tempWind.draw(*mBall);
	

	

	//UI////////////////////////////////////
	if(paused)
	{
		tempWind.draw(DisplayText(Vector2(50.0f,25.0f),"Game Paused"));
	}


	//DEBUG RAYS////////////////////////////

	//DEBUG TEXT////////////////////////////
	ShowDebug(tempWind);
}

//EVENT LOOP////////////////////////////
void GameLoop::EventLoop(sf::RenderWindow &tempWind)
{
	 sf::Event event;
    while (tempWind.pollEvent(event))
    {
		switch(event.type)
		{
		case sf::Event::Closed:
            tempWind.close();
			break;
			
			//KEYBOARD//////////////////////////////
		case sf::Event::KeyPressed:

			if(event.key.code == sf::Keyboard::Escape)
			{
				tempWind.close();
			}
			if(event.key.code == sf::Keyboard::Space)
			{
				Pause();
			}
			if(event.key.code == sf::Keyboard::R)
			{
				Reset();
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

			//TODO//11-09-15//Camera controller properly
			if(event.key.code == sf::Keyboard::Up)
			{
				//mainCamera.Move(0);
			}
			if(event.key.code == sf::Keyboard::Right)
			{
				//mainCamera.Move(1);
			}
			if(event.key.code == sf::Keyboard::Down)
			{
				//mainCamera.Move(2);
			}
			if(event.key.code == sf::Keyboard::Left)
			{
				//mainCamera.Move(3);
			}

			if(event.key.code == sf::Keyboard::S)
			{
				playerController.state.leftStick.x = 0.0f;
				playerController.state.leftStick.y = 100.0f;
			}

			if(event.key.code == sf::Keyboard::D)
			{
				playerController.state.leftStick.x = 100.0f;
				playerController.state.leftStick.y = 0.0f;
			}

			if(event.key.code == sf::Keyboard::A)
			{
				playerController.state.leftStick.x = -100.0f;
				playerController.state.leftStick.y = 0.0f;
			}

			if(event.key.code == sf::Keyboard::W)
			{
				playerController.state.leftStick.x = 0.0f;
				playerController.state.leftStick.y = -100.0f;
			}

			if(event.key.code == sf::Keyboard::X)
			{
				playerController.state.leftStick.x = 0.0f;
				playerController.state.leftStick.y = 0.0f;
			}

			break;

			//CONTROLLER INPUT//////////////////////
		case sf::Event::JoystickButtonPressed:
			//Button Numbers
			//A=0,B=1,X=2,Y=3,LB=4,RB=5,Back=6,Start=7,LS=8,RS=9
			if(event.joystickButton.joystickId == 0)
			{
				playerController.state.buttons[event.joystickButton.button] = true;
			}
			break;

		case sf::Event::JoystickButtonReleased:
			if(event.joystickButton.joystickId == 0)
			{
				playerController.state.buttons[event.joystickButton.button] = false;
			}
			break;
			
			//JOSTICK INPUT/////////////////////////
		case sf::Event::JoystickMoved:
			if(event.joystickMove.joystickId == 0)
			{
				//Left Stick
				if(event.joystickMove.axis == sf::Joystick::X)
				{
					playerController.state.leftStick.x = event.joystickMove.position;		
				}
				if(event.joystickMove.axis == sf::Joystick::Y)
				{
					playerController.state.leftStick.y = event.joystickMove.position;		
				}
				//Right Stick
				if(event.joystickMove.axis == sf::Joystick::U)
				{
					playerController.state.rightStick.x = event.joystickMove.position;
				}
				if(event.joystickMove.axis == sf::Joystick::R)
				{
					playerController.state.rightStick.y = event.joystickMove.position;		
				}
			}
			break;

		default:
			break;
		}
    }
}

void GameLoop::ShowDebug(sf::RenderWindow &tempWind)
{
	if(debugMode == 0)
	{
		//do nothing
	}
	else if(debugMode == 1)//TODO
	{
		tempWind.draw(DisplayText(Vector2(0.0f,0.0f),"PlayerData"));
		tempWind.draw(DisplayText(Vector2(0.0f,20.0f),"PlayerX",player->getPosition().x));
		tempWind.draw(DisplayText(Vector2(0.0f,40.0f),"PlayerY",player->getPosition().y));
	}
	else if(debugMode == 2)//curr FPS Debug
	{
		tempWind.draw(DisplayText(Vector2(0.0f,0.0f),"PerformanceData"));
		tempWind.draw(DisplayText(Vector2(0.0f,20.0f),"FPS curr: ", (int)fpsAverage));
		tempWind.draw(DisplayText(Vector2(0.0f,40.0f),"FrameTime: ", deltaTimeReal));
	}
	else if(debugMode == 3)//Controller Information
	{
		tempWind.draw(DisplayText(Vector2(0.0f,0.0f),"ControllerData"));
		tempWind.draw(DisplayText(Vector2(0.0f,20.0f),"JoystickX: ",playerController.state.leftStick.x));
		tempWind.draw(DisplayText(Vector2(0.0f,40.0f),"JoystickY: ",playerController.state.leftStick.y));
	}
	else if(debugMode == 4)//Camera Information
	{
		tempWind.draw(DisplayText(Vector2(0.0f,0.0f),"CameraData"));
		tempWind.draw(DisplayText(Vector2(0.0f,20.0f),"CameraX",mainCamera.position.x));
		tempWind.draw(DisplayText(Vector2(0.0f,40.0f),"CameraY",mainCamera.position.y));
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