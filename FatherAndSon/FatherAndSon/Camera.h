#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <math.h>
#define _USE_MATH_DEFINES

#include "globals.h"

#include "Vector2.h"

class Camera
{
	//METHODS
private:

public:
	Camera();
	void Input(PlayerInput teampInput);
	void Update();
	void Move();
	void SetCameraMode(CameraMode cameraModeIn);
	void Lean(float xIn, float yIn);

	void SetTargetWorldReference(Vector2 &cameraTargetIn);
	void SetTargetWorld(Vector2 cameraTargetIn);
	Vector2 GetPositionAddress();
	Vector2 GetTargetPosition();
	Vector2 GetTargetWorldPosition();
	//VARIABLES
private:
	Vector2 velocity;
	CameraMode cameraMode;
	float zoom;

	Vector2 *ptrTargetPosition;
	Vector2 targetPosition;
	Vector2 *ptrTargetWorldPosition;
	Vector2 targetWorldPosition;

	Vector2 cameraLean;
	float cameraLeanDistance;//TODO//13-10-15//in pixels or world co-ordinates

	float camModeControlSpeed;

	//TODO//23-10-15//Add in delta time for camera movement
public:
	Vector2 position;//Change to private
	Vector2 worldPosition;

};


#endif