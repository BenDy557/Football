#include "Camera.h"


Camera::Camera()
{
	position = 0.0f;
	worldPosition = 0.0f;
	targetPosition = 0.0f;
	targetWorldPosition = 0.0f;

	velocity = 0.0f;
	zoom = 1.0f;//TODO//11-09-15//Give a use

	cameraMode = CameraMode::camModeFollowLoose;
	cameraLean = 0.0f;
	cameraLeanDistance = 2.0f;

	camModeControlSpeed = 0.5f;


	//*ptrTargetPosition = 0;
	
	//*ptrTargetWorldPosition = 0;

}

/*
Vector2 Camera::getPositionAddress()//TODO//13-09-15//Maybe shouldnt do this
{
	return ;
}
*/

void Camera::Input(PlayerInput tempInput)
{
	//tempInput.aimingBearing

	//atan2(0.5, 0.5)*180/math.pi;

	if(cameraMode == CameraMode::camModeControl || cameraMode == CameraMode::camModeControlLoose)
	{
		Vector2 leanAmount;
	
		float LeanModifier = tempInput.aimingIntesity * cameraLeanDistance;
		
		leanAmount.x =-LeanModifier*sin((tempInput.aimingBearing+225)*(M_PI/180.0f));
		leanAmount.y = LeanModifier*cos((tempInput.aimingBearing+225)*(M_PI/180.0f));
		
		Lean(leanAmount.x,leanAmount.y);
	}
}

void Camera::Update()
{
	//position=(position+velocity);


	Move();
}

void Camera::Move()
{
	switch(cameraMode)
	{
	case camModeControl:
		//Right Analog stick moves
		targetWorldPosition.x += cameraLean.x;
		targetWorldPosition.y += cameraLean.y;

		targetPosition.x = ((targetWorldPosition.x*(TILE_SCALE/2))+(targetWorldPosition.y*(TILE_SCALE/2)));
		targetPosition.y = -((targetWorldPosition.x*(TILE_SCALE/4))-(targetWorldPosition.y*(TILE_SCALE/4)));
		position.x = targetPosition.x;
		position.y = targetPosition.y;
		break;

	case camModeFollowHard:

		targetPosition.x = ((targetWorldPosition.x*(TILE_SCALE/2))+(targetWorldPosition.y*(TILE_SCALE/2)));//TODO//21-10-15//Try and find a way to not duplicate position formula
		targetPosition.y = -((targetWorldPosition.x*(TILE_SCALE/4))-(targetWorldPosition.y*(TILE_SCALE/4)));//TODO//21-10-15//Try and find a way to not duplicate position formula	
		position.x = targetPosition.x;
		position.y = targetPosition.y;
		break;

	case camModeFollowLoose:
		targetPosition.x = ((targetWorldPosition.x*(TILE_SCALE/2))+(targetWorldPosition.y*(TILE_SCALE/2)));//TODO//21-10-15//Try and find a way to not duplicate position formula
		targetPosition.y = -((targetWorldPosition.x*(TILE_SCALE/4))-(targetWorldPosition.y*(TILE_SCALE/4)));//TODO//21-10-15//Try and find a way to not duplicate position formula
		//AccelerateTowards(targetPosition);//TODO//21-10-15//Make function exist
		position = position + (targetPosition - position)*0.1;//TODO//23-10-15//Convert to deltaTime
		break;
	
	case camModeControlLoose:
		//Right Analog stick moves
		targetWorldPosition.x += cameraLean.x;
		targetWorldPosition.y += cameraLean.y;

		targetPosition.x = ((targetWorldPosition.x*(TILE_SCALE/2))+(targetWorldPosition.y*(TILE_SCALE/2)));
		targetPosition.y = -((targetWorldPosition.x*(TILE_SCALE/4))-(targetWorldPosition.y*(TILE_SCALE/4)));

		//position.x = targetPosition.x;
		//position.y = targetPosition.y;
		position = (position + (targetPosition - position)*0.1f);//TODO//23-10-15//Convert to deltaTime
		break;

	}


	


	
}

void Camera::SetCameraMode(CameraMode cameraModeIn)
{
	cameraMode = cameraModeIn;
}

void Camera::Lean(float xIn,float yIn)
{
	cameraLean.x = xIn;
	cameraLean.y = yIn;
}

void Camera::SetTargetWorldReference(Vector2 &cameraTargetIn)
{
	*ptrTargetWorldPosition = cameraTargetIn;
}

void Camera::SetTargetWorld(Vector2 cameraTargetIn)
{
	targetWorldPosition = cameraTargetIn;
}

Vector2 Camera::GetTargetPosition()
{
	return targetPosition;
}

Vector2 Camera::GetTargetWorldPosition()
{
	return targetWorldPosition;
}