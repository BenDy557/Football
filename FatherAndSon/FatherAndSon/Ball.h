#ifndef _BALL_H_
#define _BALL_H_

#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <math.h>

#include "vector2.h"
#include "Ray.h"

#include "globals.h"

#include "GameObjectIso.h"

////////////////////////////PLAYER	
class Ball : public GameObjectIso
{
//METHODS
private://?
		void Movement();
		void Animate();
		void UpdatePosition();

public:
	
	Ball(bool hasShadowIn);

	void Update();

	void Initialise(sf::Vector2f positionIn,sf::Color colorIn);//TODO//OLD


	bool CheckBoxCollide(sf::RectangleShape boundingBoxIn);//TODO//OLD

	float getMovementBearing();
	void setMovementBearing(float degreesIn);

	float getMovementIntensity();
	void setMovementIntensity(float intensityScaleIn);


	Vector2 GetVelocity();
	float GetVelocityZ();
	void SetVelocity(Vector2 velocityIn);
	void SetVelocity(Vector2 velocityIn,float zVelocityIn);
	void IncrementVelocity(Vector2 velocityIn);
	void IncrementVelocity(Vector2 velocityIn, float zVelocityIn);
	float getVelocityMagnitude();

	void setDeltaTime(float *deltaTimeIn);

	Ray getPositionVelocityRay();

	void setTileX(int xIn);
	int getTileX();

	void setTileY(int yIn);
	int getTileY();

	void setColor(sf::Color colorIn);

	float getDeadZone();
//VARIABLES
private:
	float deadZone;//0.0f -> 0.1f;

	float* ptrDeltaTime;
	PlayerInput inputState;
	
	Vector2 velocity;
	float mZVelocity;
	float mGravity;
	Vector2 acceleration;
	Ray positionVelocityRay;

	bool mGrounded;
	bool mPrevGrounded;
	float mRestitutionCoefficient;
	float frictionCoefficient;
	float mMinZVelocity;

	int tileX;
	int tileY;

	float lastMovementBearing;
	float lastAimingBearing;

	

	
public:
	sf::Vector2f prevPosition;
	sf::RectangleShape lastCollisionMarker;//TODO//OLD
};

#endif