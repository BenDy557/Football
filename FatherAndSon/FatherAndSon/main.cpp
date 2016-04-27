


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>

//Winsock
//#include <winsock2.h>
//#pragma comment(lib, "ws2_32.lib")

//SFML
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

//Game Includes
#include "GameLoop.h"

//#include "PlayerController.h"//TODO

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int nCmdShow)
{
	//Create Main Window
	sf::RenderWindow window(sf::VideoMode(GLOBAL_S_WIDTH, GLOBAL_S_HEIGHT), "Game Window",sf::Style::Default);
	//window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(60);

	//Loading
	GameLoop gameLoop;
	//gameLoop.SetWindow(window);

	//load music
	sf::Music music;
	if(!music.openFromFile("resources/orchestral.ogg"))
	{
		return EXIT_FAILURE;
	}
	
	//Play Music
	//music.play();

	sf::Clock clock;
	sf::Time time;
	//float fps=1000000/time.asMicroseconds();
	//elapsed microseconds/1,000,000 = delta time


	gameLoop.SetClock(&clock);
	gameLoop.SetWindow(&window);

	
	////////////////////////////////////////
	//WINDOW OPEN///////////////////////////
    while (window.isOpen())
    {
		window.clear(/*sf::Color::Color(40.0f,94.0f,91.0f,255.0f)*/);
		
		gameLoop.Update();
        window.display();
    }
    return 0;
}