#ifndef _GAME_LOOP_H
#define _GAME_LOOP_H


//#include <Windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <list>

//SYSTEM INCLUDES///////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Socket.h"
#include "globals.h"

//GAME INCLUDES/////////////////////////
#include "Ray.h"
#include "Camera.h"
#include "Player.h"
#include "Ball.h"
#include "PlayerController.h"
#include "MapGrid.h"


class GameLoop
{
//METHODS
public:
	GameLoop()
		:aiThread(&GameLoop::AIUpdate,this)
	{
		mInGame = false;
		mGameReady = false;
		gameTime = prevGameTime = gameTime.Zero;
		timeScale = 1.0f;
		paused = false;//TODO//13-09-15//
		deltaTimeReal = 0.0f;
		deltaTime = 0.0f;
		frameCount = 0;
		debugMode = 0;

		mMenuState = 0;
		mClient = false;
		mServer = false;
		mIPReady = false;
		mServerIP = "";
		
		mNetworkData = new char;
		memset(mNetworkData,'-',MESSAGESIZE);
		mNetworkData[MESSAGESIZE-1] = '\0';
		
		mSocket = new Socket();
		
		LoadTextures();
	}
	//~GameLoop();

	void GameInitialise();
	void SetWindow(sf::RenderWindow* tempWind);
	void SetClock(sf::Clock* tempClock);
	void Update();
	void MenuUpdate();
	void GameUpdate();
	void AIUpdate();
	//void Draw();
private:
	void LoadTextures();
	void GameEventLoop();
	void MenuEventLoop();

	void ShowDebug();
	sf::Text DisplayText(Vector2 positionIn,std::string phraseIn, float valueIn);
	sf::Text DisplayText(Vector2 positionIn,std::string phraseIn);
	void ResolveBulletPlayerCollision();
	void Collision();
	void MenuDraw();
	void Draw();

	void Reset();
	void Pause();

	void SpawnPlayer(PlayerController* ptrPlayerControllerIn);

//VARIABLES
public:

private:
	bool mInGame;
	bool mGameReady;
	bool mIPReady;
	int mMenuState;
	
	//NETWORKING//////////
	bool mClient;
	bool mServer;
	std::string mServerIP;
	Socket* mSocket;
	char* mNetworkData;//Networking data in/out


	int mMaxPlayers;

	sf::Clock* mPtrClock;
	sf::RenderWindow* mPtrWindow;

	sf::Thread aiThread;
	sf::Time gameTime;
	sf::Time prevGameTime;
	float deltaTimeReal;//actual time between frames
	float deltaTime;//timescale modifier
	float timeScale;//game seconds per real second
	
	bool paused;

	//Fonts
	sf::Font verdana;

	//Textures
	sf::Texture debugPointerTexture;
	sf::Texture mShadowTexture;
	sf::Texture mPlayertexture;
	sf::Texture debugCameraTexture;
	sf::Texture tilesetTexture;
	sf::Texture mBallTexture;
	

	//Debug variables
	int debugMode;
	//Debug mode 0//None
	//Debug mode 1//General
	//Debug mode 3//Aiming
	//Debug mode 4//Rays
	Ray testRay;

	int frameCount;
	float fps;
	float prevFps[FRAME_COUNT_MAX];
	float fpsAverage;


	//Camera
	Camera mainCamera;
	GameObjectIso cameraTargetIcon;
	Ball* mBall;

	//SF::CAMERA
	sf::View mainView;

	//Terrain
	std::vector<GameObjectIso> EnvironmentTiles;//TODO//20-10-2015//Change to Mapgrid instead of gameobject vector
	MapGrid EnvironmentTileGrid;//TODO//23-10-2015//make exist
		
	
	//MainPlayer
	Player* mPlayer;
	PlayerController* mPlayerController;


	//Players
	std::list<Player*> mPlayers;
	std::list<PlayerController*> mControllers;

	/*
	Player* mPlayerRed;
	PlayerController mPlayerControllerRed;
	*/
	

	
};

#endif // _GAME_LOOP_H