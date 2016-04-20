#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <SFML/Graphics.hpp>
#include "Vector2.h"

const int GLOBAL_S_WIDTH	=	1280;		//client area resolution
const int GLOBAL_S_HEIGHT	=	720;

const int GLOBAL_S_XPOS =	200;		//client position
const int GLOBAL_S_YPOS =	100;

//FAST SQRT//source http://bytes.com/topic/c/answers/722158-sqrt-fastsqrt
inline float fastsqrt(float val) {
int tmp = *(int *)&val;
tmp -= 1<<23; 
tmp = tmp >> 1; 
tmp += 1<<29; 
return *(float *)&tmp;
}

//VECTOR CONVERSION
inline Vector2 ConvertSFVector2(sf::Vector2f sfVector2)
{
	Vector2 convertedVector;
	convertedVector.x = sfVector2.x;
	convertedVector.y = sfVector2.y;

	return convertedVector;
}

inline sf::Vector2f ConvertVector2(Vector2 benVector2)
{
	sf::Vector2f convertedVector;
	convertedVector.x = benVector2.x;
	convertedVector.y = benVector2.y;

	return convertedVector;
}

//TIMER
struct game_time_struct
{
	int curr_time;
	int prev_time;
	float time_delta;
};

const int FRAME_COUNT_MAX = 30;

//GAME
struct ControllerState
{
	bool buttons[10];
	
	Vector2 leftStick;
	Vector2 rightStick;
	//TODO//float d-pad[2]
	//TODO//float leftTrigger
	//TODO//float rightTrigger
};

struct PlayerInput//TODO//10-09-15//Change to new Control Scheme
{
	bool actions[5];//0=x,1=x,2=rifle

	float movementBearing;
	float movementIntensity;

	float aimingBearing;
	float aimingIntesity;
};

enum ControllerButtons{ctrlA,ctrlB,ctrlX,ctrlY,ctrlLB,ctrlRB,ctrlBack,ctrlStart,ctrlLS,ctrlRS};


enum CameraMode{camModeControl,camModeFollowHard,camModeFollowLoose,camModeControlLoose};

const int MAP_GRID_X_MAX = 24;
const int MAP_GRID_Y_MAX = 20;

const int TILE_SCALE = 64.0f;

const Vector2 RESET_POSITION(380.0f,280.0f);

//COLOURS (colors)
const sf::Color blue(0.0f,0.0f,255.0f);
const sf::Color brightGreen(64.0f,255.0f,64.0f);
const sf::Color fullGreen(0.0f,255.0f,0.0f);
const sf::Color normalGreen(0.0f,192.0f,0.0f);
const sf::Color darkGreen(0.0f,128.0f,0.0f);

const sf::Color playerLOS(0.0f,255.0f,255.0f);
const sf::Color playerOOS(0.0f,164.0f,164.0f);

const sf::Color floorColor(100.0f,100.0f,100.0f);
const sf::Color wallColor(36.0f,82.0f,36.0f);

enum PacketType{playerInput,playerData,ballData};

struct LocomotionData
{
	Vector2 mPosition;
	float mPositionZ;
	Vector2 mVelocity;
	float mVelocityZ;
};

#endif