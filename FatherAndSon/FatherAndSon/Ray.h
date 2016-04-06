#ifndef _RAY_H
#define _RAY_H

#define _USE_MATH_DEFINES

#include <math.h>

#include <SFML/Graphics.hpp>

#include "Vector2.h"


class Ray : public sf::RectangleShape
{
	//METHODS
private:
	void setShape();//sets the shape component of the Ray
	bool ClipVector(int axis,sf::RectangleShape& boxIn);
	void UpdateDirectionVector();
public:
	Ray();
	Ray(Vector2 startPosIn, float lengthIn, float bearingIn);
	Ray(Vector2 startPosIn, Vector2 endPosIn);
	void Update(Vector2 startPosIn, Vector2 endPosIn);
	void Update(Vector2 startPosIn, float lengthIn, float bearingIn);
	
	float GetMagnitude();

	bool CollideWith(Vector2 PosIn);
	bool CollideWithRadius(Vector2 posIn, float radius,Vector2& collisionPoint, float&  vectorFraction);
	//Vector2 CollideWith(sf::Shape.boundingBox);//SFML bounding box
	bool CollideWithAABB(sf::RectangleShape& boxIn, Vector2& collisionPointIn, float&  vectorFraction);
	bool CollideWithAABB(sf::RectangleShape& boxIn);

	bool GetDebugDrawToggle();
	void SetDebugDrawToggle(bool toggleIn);
	void setDebugDrawScale(float scaleIn);
	//VARIABLES
private:
	Vector2 startPos;
	Vector2 endPos;

	Vector2 directionVector;//Directional vector
	
	float length;
	float bearing;

	bool debugDrawToggle;
	float debugDrawScale;
	//Collision
	float clipFractionLow;
	float clipFractionHigh;
public:

};

#endif //_RAY_H