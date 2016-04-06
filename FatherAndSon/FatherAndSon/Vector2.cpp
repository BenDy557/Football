#include "Vector2.h"


Vector2::Vector2 ()
{
	x = 0;
	y = 0;
}

Vector2::Vector2 (float x_pos,float y_pos)
{
	x = x_pos;
	y = y_pos;
}

//Floating point operator overloading
Vector2 Vector2::operator+ (float param)
{
	Vector2 temp;
	temp.x = x + param;
	temp.y = y + param;
	return (temp);
}

Vector2 Vector2::operator- (float param)
{
	Vector2 temp;
	temp.x = x - param;
	temp.y = y - param;
	return (temp);
}

Vector2 Vector2::operator/ (float param)
{
	Vector2 temp;
	temp.x = x / param;
	temp.y = y / param;
	return (temp);
}

Vector2 Vector2::operator* (float param)
{
	Vector2 temp;
	temp.x = x * param;
	temp.y = y * param;
	return (temp);
}

void Vector2::operator= (float param)
{
	x = y = param;
}

//vector operator overloading
Vector2 Vector2::operator+ (Vector2 param)
{
	Vector2 temp;
	temp.x = x + param.x;
	temp.y = y + param.y;
	return (temp);
}

Vector2 Vector2::operator- (Vector2 param)
{
	Vector2 temp;
	temp.x = x - param.x;
	temp.y = y - param.y;
	return (temp);	
}

float Vector2::operator* (Vector2 param)//DOT PRODUCT
{
	float temp_f;
	Vector2 temp;

	temp.x = x * param.x;
	temp.y = y * param.y;

	temp_f= temp.x+temp.y;
	return (temp_f);
}
/*
float vector2::operator**(vector2 param)//CROSS PRODUCT
{

}
*/
void Vector2::operator= (Vector2 param)
{
	x = param.x;
	y = param.y;
}

float Vector2::Get_magnitude()//Returns magnitude
{
	float Magnitude_squared = (x*x)+(y*y);
	return sqrt(Magnitude_squared);
}