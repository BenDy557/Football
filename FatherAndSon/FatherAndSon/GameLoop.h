#ifndef _GAME_LOOP_H
#define _GAME_LOOP_H

#include <Windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <list>

//SYSTEM INCLUDES///////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

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
		GameInitialise();
	}

	void GameInitialise();
	void SetWindow(sf::RenderWindow &tempWind);
	void Update(sf::RenderWindow &tempWind,sf::Clock tempClock);
	void AIUpdate();
	//void Draw();
private:
	void EventLoop(sf::RenderWindow &tempWind);
	void ShowDebug(sf::RenderWindow &tempWind);
	sf::Text DisplayText(Vector2 positionIn,std::string phraseIn, float valueIn);
	sf::Text DisplayText(Vector2 positionIn,std::string phraseIn);
	void ResolveBulletPlayerCollision();
	void Collision();
	void Draw(sf::RenderWindow &tempWind);

	void Reset();
	void Pause();

//VARIABLES
public:

private:
	sf::Thread aiThread;
	sf::Time gameTime;
	sf::Time prevGameTime;
	float deltaTimeReal;//actual time between frames
	float deltaTime;//timescale modifier
	float timeScale;//game seconds per real second
	
	bool paused;
	bool twoPlayer;
	bool aiVersus;

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
		
	//Players
	Player* mPlayer;
	PlayerController mPlayerController;

	Player* mPlayerRed;
	PlayerController mPlayerControllerRed;
	

	
};

#endif // _GAME_LOOP_H