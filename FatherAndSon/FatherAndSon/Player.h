#ifndef _PLAYER_H_
#define _PLAYER_H_

#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <math.h>

#include "vector2.h"
#include "Ray.h"

#include "globals.h"

#include "GameObjectIso.h"
#include "Ball.h"


using std::min_element;
using std::abs;
////////////////////////////PLAYER	
class Player : public GameObjectIso
{
//METHODS
private://?
	void Actions();
	void Movement();
	void Collision();
	void Aiming();
	
	void UpdatePosition();
	
	void Jump();
	void Kick();
	void Chip();
	void SpriteAnimation();

public:
	Player(bool hasShadowIn);//TODO//OLD
	void Initialise(sf::Vector2f positionIn,sf::Color colorIn);//TODO//OLD
	void Update();//TODO//OLD
	void SetInput(PlayerInput* playerInputIn);
	PlayerInput GetPlayerInput();
	bool CheckBoxCollide(sf::RectangleShape boundingBoxIn);//TODO//OLD

	float getMovementBearing();
	void setMovementBearing(float degreesIn);

	float getMovementIntensity();
	void setMovementIntensity(float intensityScaleIn);

	float getAimingBearing();
	void setAimingBearing(float degreesIn);

	float getAimingIntensity();
	void setAimingIntensity(float intensityScaleIn);

	Vector2 getVelocity();
	float GetVelocityZ();
	void SetVelocity(float xVelIn, float yVelIn, float zVelIn);
	float getVelocityMagnitude();

	void setDeltaTime(float *deltaTimeIn);
	void SetBall(Ball *ballIn);

	Ray getPositionVelocityRay();

	void setTileX(int xIn);
	int getTileX();

	void setTileY(int yIn);
	int getTileY();

	//void setColor(sf::Color colorIn);

	float getDeadZone();
//VARIABLES
private:
	float deadZone;//0.0f -> 0.1f;

	float* ptrDeltaTime;
	PlayerInput* ptrInputState;
	
	float lastMovementBearing;
	float lastAimingBearing;

	int tileX;
	int tileY;

	Vector2 velocity;
	Vector2 acceleration;

	float mZVelocity;
	float mGravity;

	bool mGrounded;
	bool mKickReady;
	float mKickDistance;
	float mVerticalKickDistance;
	Ball *mPtrBall;

	sf::Vector2i mSpriteCoord;
	sf::Vector2i mSpriteSheetMax;
	sf::Vector2f mTopLeftSpriteCoord;
	float mFrameTimer;
	float mFrameTimeMax;

	float minVelocity;//TODO//OLD
	float maxVelocity;//TODO//OLD
	float accelerationForce;//TODO//OLD
	const float accelerationForceDefault;//TODO//OLD
	float frictionCoefficient;//TODO//OLD
	const float frictionCoefficientDefault;//TODO//OLD
	
	Ray positionVelocityRay;
	
public:
	int mPlayerNumber;
	int mTeam;
	bool mBallKicked;

	sf::Vector2f prevPosition;
	sf::RectangleShape lastCollisionMarker;//TODO//OLD
};

#endif