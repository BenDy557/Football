#include "Ray.h"

using std::swap;
using std::min;
using std::max;


Ray::Ray()
{
	startPos.x = startPos.y = 1.0f;
	endPos = startPos;
	length = 10.0f;
	bearing = 0.0f;

	debugDrawToggle = false;
	debugDrawScale = 1.0f;

	setFillColor(sf::Color::Green);

	setSize(sf::Vector2f(-length,1));
	setRotation(bearing);
}

Ray::Ray(Vector2 startPosIn, Vector2 endPosIn)
{
	startPos = startPosIn;
	endPos = endPosIn;

	//finds the length and bearing when given start and end positions
	length = abs(startPos.Get_magnitude()-endPos.Get_magnitude());
	bearing = (M_PI/180) * atan2(startPos.y - endPos.y, startPos.x - endPos.x);
	setShape();

}

Ray::Ray(Vector2 startPosIn, float lengthIn, float bearingIn)
{
	startPos = startPosIn;
	length = lengthIn;
	bearing = bearingIn;

	if(bearing>=0 
	&& bearing<90)
	{
		endPos.x = length * (float)sin((M_PI/180.0f)*bearing);
		endPos.y = -sqrt((length*length)-(endPos.x*endPos.x));
	}
	else
	if(bearing>=90
	&& bearing<180)
	{
		endPos.y = length * (float)sin((M_PI/180.0f)*(bearing-90));
		endPos.x = sqrt((length*length)-(endPos.y*endPos.y));
	}
	else
	if(bearing>=180
	&& bearing<270)
	{
		endPos.x = -length * (float)sin((M_PI/180.0f)*(bearing-180));
		endPos.y = sqrt((length*length)-(endPos.x*endPos.x));
	}
	else
	if(bearing>=270
	&& bearing<360)
	{
		endPos.y = -length * (float)sin((M_PI/180.0f)*(bearing-270));
		endPos.x = -sqrt((length*length)-(endPos.y*endPos.y));
	}
	else
	{
		//error
	}

	setShape();
}

void Ray::Update(Vector2 startPosIn, Vector2 endPosIn)
{
	startPos = startPosIn;
	endPos = endPosIn;

	//finds the length and bearing when given start and end positions
	UpdateDirectionVector();
	length = directionVector.Get_magnitude();
	
	bearing = -90 + (180/M_PI) * atan2(startPos.y - endPos.y, startPos.x - endPos.x);//TODO//see if you can get rid of the -90

	setShape();
}

void Ray::Update(Vector2 startPosIn, float lengthIn, float bearingIn)
{
	startPos = startPosIn;
	length = lengthIn;
	bearing = bearingIn;

	if(bearing>=0 
	&& bearing<90)
	{
		endPos.x = length * (float)sin((M_PI/180.0f)*bearing);
		endPos.y = -sqrt((length*length)-(endPos.x*endPos.x));
	}
	else
	if(bearing>=90
	&& bearing<180)
	{
		endPos.y = length * (float)sin((M_PI/180.0f)*(bearing-90));
		endPos.x = sqrt((length*length)-(endPos.y*endPos.y));
	}
	else
	if(bearing>=180
	&& bearing<270)
	{
		endPos.x = -length * (float)sin((M_PI/180.0f)*(bearing-180));
		endPos.y = sqrt((length*length)-(endPos.x*endPos.x));
	}
	else
	if(bearing>=270
	&& bearing<360)
	{
		endPos.y = -length * (float)sin((M_PI/180.0f)*(bearing-270));
		endPos.x = -sqrt((length*length)-(endPos.y*endPos.y));
	}
	else
	{
		//error
	}

	setShape();
	//TODO//?
}

void Ray::UpdateDirectionVector()
{
	directionVector = startPos - endPos;
}

float Ray::GetMagnitude()
{
	return directionVector.Get_magnitude();
}

bool Ray::CollideWithRadius(Vector2 posIn, float radius,Vector2& collisionPoint, float&  vectorFraction)
{
	return false;//TODO//DOESNT WORK
	Vector2 startPosDiff = (startPos - posIn);
	if((startPosDiff.Get_magnitude() - radius) < length)
	{
	//POTENTIAL COLLISION
	}
	return false;
}

bool Ray::CollideWithAABB(sf::RectangleShape& boxIn)
{
	clipFractionLow = 0;
	clipFractionHigh = 1;

	if(!ClipVector(0,boxIn))//x axis
	{
		return false;
	}

	if(!ClipVector(1,boxIn))//y axis
	{
		return false;
	}

	return true;
}

bool Ray::CollideWithAABB(sf::RectangleShape& boxIn, Vector2& collisionPointIn, float&  vectorFraction)
{
	UpdateDirectionVector();

	clipFractionLow = 0;
	clipFractionHigh = 1;//Fractions!

	if(!ClipVector(0,boxIn))//x axis
	{
		return false;
	}

	if(!ClipVector(1,boxIn))//y axis
	{
		return false;
	}


	collisionPointIn = startPos + directionVector*clipFractionLow*-1;
	vectorFraction = clipFractionLow;
	
	return true;
}

bool Ray::ClipVector(int axis, sf::RectangleShape& boxIn)
{
	float tempClipFractionLow = 0;
	float tempClipFractionHigh = 1;//temp Fractions
	

	//gets fraction of vector that fits inside of bounding box
	if(axis == 0)//x axis
	{
		tempClipFractionLow = (((boxIn.getPosition().x - boxIn.getGlobalBounds().width/2) - startPos.x)/(endPos.x - startPos.x));
		tempClipFractionHigh = (((boxIn.getPosition().x + boxIn.getGlobalBounds().width/2) - startPos.x)/(endPos.x - startPos.x));
	}
	else if(axis == 1)//y axis
	{
		tempClipFractionLow = (((boxIn.getPosition().y + boxIn.getGlobalBounds().height/2) - startPos.y)/(endPos.y - startPos.y));
		tempClipFractionHigh = (((boxIn.getPosition().y - boxIn.getGlobalBounds().height/2) - startPos.y)/(endPos.y - startPos.y));
	}
	else
	{
		return false;
	}

	if(tempClipFractionHigh < tempClipFractionLow)
	{
		swap(tempClipFractionHigh,tempClipFractionLow);
	}

	if(tempClipFractionHigh < clipFractionLow)
	{
		return false;
	}
	else
	if(tempClipFractionLow > clipFractionHigh)
	{
		return false;
	}

	clipFractionLow = max(tempClipFractionLow,clipFractionLow);
	clipFractionHigh = min(tempClipFractionHigh,clipFractionHigh);

	if(clipFractionLow > clipFractionHigh)
	{
		return false;
	}

	return true;
}

void Ray::setShape()
{
	setFillColor(sf::Color::Green);
	setSize(sf::Vector2f(2,-length*debugDrawScale));
	setRotation(bearing);
	setPosition(startPos.x,startPos.y);
}

bool Ray::GetDebugDrawToggle()
{
	return debugDrawToggle;
}

void Ray::SetDebugDrawToggle(bool toggleIn)
{
	debugDrawToggle = toggleIn;
}

void Ray::setDebugDrawScale(float scaleIn)
{
	debugDrawScale = scaleIn;
}