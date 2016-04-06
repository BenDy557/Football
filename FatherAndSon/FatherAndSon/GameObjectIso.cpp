#include "GameObjectIso.h"

GameObjectIso::GameObjectIso()
{
	//setScale(sf::Vector2f(TILE_SCALE,TILE_SCALE));
	//setSize(sf::Vector2f(TILE_SCALE,TILE_SCALE));
	mHasShadow = false;
	worldPositionZ = 0.0f;
	mShadow = 0;
}

GameObjectIso::GameObjectIso(bool hasShadowIn)
{
	//setScale(sf::Vector2f(TILE_SCALE,TILE_SCALE));
	//setSize(sf::Vector2f(TILE_SCALE,TILE_SCALE));
	mHasShadow = hasShadowIn;
	worldPositionZ = 0.0f;
	if(mHasShadow)
	{
		mShadow = new GameObjectIso(false);
		//mShadow->setSize(sf::Vector2f(TILE_SCALE,TILE_SCALE));
		//mShadow->setScale(sf::Vector2f(TILE_SCALE,TILE_SCALE));
		mShadow->setOrigin((TILE_SCALE/2),(TILE_SCALE*3/4));
	}
}

void GameObjectIso::ConvertToWorldCoord()
{
	getPosition();

}

Vector2 GameObjectIso::getWorldPosition()
{
	return worldPosition;
}

void GameObjectIso::setWorldPosition(float xIn, float yIn)
{
	worldPosition.x = xIn;
	worldPosition.y = yIn;
}

void GameObjectIso::moveWorldPosition(float xIn, float yIn)
{
	worldPosition.x += xIn;
	worldPosition.y += yIn;
}

void GameObjectIso::moveWorldPosition(float xIn, float yIn, float zIn)
{
	worldPosition.x += xIn;
	worldPosition.y += yIn;

	worldPositionZ += zIn;
}



float GameObjectIso::getWorldPositionZ()
{
	return worldPositionZ;
}

void GameObjectIso::setWorldPositionZ(float worldPositionZIn)
{
	worldPositionZ = worldPositionZIn;
}

void GameObjectIso::Update()//TODO//11-09-15//DeltaTime//Might overwrite sf::rectangle shape
{
	//setPosition(worldPosition.x-cameraPos->x,worldPosition.y-cameraPos->y);




	setPosition(((worldPosition.x*(TILE_SCALE/2))+(worldPosition.y*(TILE_SCALE/2)))
		,((-worldPosition.x*(TILE_SCALE/4))+(worldPosition.y*(TILE_SCALE/4))-(worldPositionZ*(TILE_SCALE/2))));

	if(mHasShadow)
	{
		mShadow->setPosition(((worldPosition.x*(TILE_SCALE/2))+(worldPosition.y*(TILE_SCALE/2)))
						   ,((-worldPosition.x*(TILE_SCALE/4))+(worldPosition.y*(TILE_SCALE/4))));
		
		//float tempShadowScale = 1.0f+worldPositionZ/2;
		//mShadow->setScale(1.0f-(1.0f*worldPositionZ),1.0f-(1.0f*worldPositionZ));
		//mShadow->setScale(tempShadowScale,tempShadowScale);
		//mShadow->setColor(sf::Color(255.0f,255.0f,255.0f,25(255.0f*(tempShadowScale))));
	}
	//setPosition(((worldPosition.x*1)+(worldPosition.y*0.5))-cameraPos->x
	//		  ,(((worldPosition.y*0.5)-(worldPosition.x*1)))-cameraPos->y);
		
}

void GameObjectIso::setDeltaTime(float &deltaTimeIn)
{
	deltaTime = &deltaTimeIn;
}





