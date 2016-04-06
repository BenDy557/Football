#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <math.h>

class Vector2
{
public:
	Vector2();
	Vector2(float x_pos,float y_pos);
	Vector2 operator + (float);
	Vector2 operator - (float);
	Vector2 operator / (float);
	Vector2 operator * (float);
	void operator = (float);
	
	Vector2 operator + (Vector2);
	Vector2 operator - (Vector2);
	float operator * (Vector2);//Dot product
	void operator = (Vector2);
	float x,y;

	float Get_magnitude();
};

#endif