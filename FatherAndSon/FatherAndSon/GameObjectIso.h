#ifndef _GAME_OBJECT_ISO_H_
#define _GAME_OBJECT_ISO_H_

#include <SFML\Graphics.hpp>

#include "Vector2.h"
//#include "Vector3.h"
#include "globals.h"
//ISOMETRIC GAME OBJECT
class GameObjectIso : public sf::RectangleShape
{
	//METHODS
private:
	void ConvertToWorldCoord();//TODO//11-09-15//Work on name and functionality

	
public:
	GameObjectIso();
	GameObjectIso(bool hasShadownIn);
	void setDeltaTime(float &deltaTimeIn);
	void setCameraPos(Vector2 &cameraPosIn);

	void Update();//TODO//13-09-15//Dont know if this overrides rectangle shape update
	void setWorldPosition(float xIn,float yIn);
	void moveWorldPosition(float xIn,float yIn);
	void moveWorldPosition(float xIn, float yIn, float zIn);


	Vector2 getWorldPosition();
	float getWorldPositionZ();
	void setWorldPositionZ(float worldPositionZIn);

	//VARIABLES
private:
	

	float *deltaTime;
	Vector2 *cameraPos;

	Vector2 worldPosition;
	float worldPositionZ;
	
public:
	bool mHasShadow;
	sf::RectangleShape *mShadow;

protected:
	
	

};


#endif