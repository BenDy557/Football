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
		gameTime = prevGameTime = gameTime.Zero;
		timeScale = 1.0f;
		paused = false;//TODO//13-09-15//
		deltaTimeReal = 0.0f;
		deltaTime = 0.0f;
		frameCount = 0;
		debugMode = 0;

		mNetUpdateRate = 10;
		mNetUpdateTimer = 1.0f/mNetUpdateRate;
		mMenuState = 0;
		mInGame = false;
		mGameReady = false;
		playersSetUp = 0;
		mClient = false;
		mServer = false;
		mIPReady = false;
		mServerIP = "";
		mNetworkData = new char[MESSAGESIZE];
		memset(mNetworkData,'p',MESSAGESIZE);
		mNetworkData[MESSAGESIZE-1] = '\0';
		mSocket = new Socket();

		mPlayer = nullptr;
		mBall = nullptr;
		mPlayerController = nullptr;

		mCurrentPlayersAmount = 1;

		
		LoadTextures();
	}
	~GameLoop();

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

	Player* SpawnPlayer(PlayerController* ptrPlayerControllerIn);


	//Networking
	void Serialise(PacketType packetTypeIn, char* dataIn,char* bufferOut);
	PacketType DeSerialise(void*& dataOut, char* bufferIn);


//VARIABLES
public:

private:
	bool mInGame;
	bool mGameReady;
	bool mIPReady;
	int mMenuState;
	int playersSetUp;
	
	//NETWORKING//////////
	bool mClient;
	bool mServer;
	
	std::string mServerIP;
	Socket* mSocket;
	char* mNetworkData;//Networking data in/out

	float mNetUpdateRate;
	float mNetUpdateTimer;

	int mCurrentPlayersAmount;
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


	std::list<Socket*> mClients;
	/*
	Player* mPlayerRed;
	PlayerController mPlayerControllerRed;
	*/
	

	
};

#endif // _GAME_LOOP_H