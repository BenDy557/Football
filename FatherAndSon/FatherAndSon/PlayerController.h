#ifndef _PLAYER_CONTROLLER_H_
#define _PLAYER_CONTROLLER_H_

#define _USE_MATH_DEFINES
#include <math.h>

#include "globals.h"
#include <SFML/Graphics.hpp>

//PLAYER CONTROLLER/////////////////////
class PlayerController
{
//METHODS
public:
	PlayerController();
	void Update();
	
private:
	void ResolveJoystickIntensity();
	void ResolveJoystickBearing();
	void ResolveButtons();

//VARIABLES
public:
	//PlayerInput SendInput();
	ControllerState state;
	PlayerInput* inputToSend;
private:
	//int controller_number;
	float deadZone;
	

};

#endif