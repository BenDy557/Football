#include "PlayerController.h"

PlayerController::PlayerController()
{
	for(int i=0; i<=9; i++)
	{
		state.buttons[i] = false;
	}

	state.leftStick.x = 0;
	state.leftStick.y = 0;
	state.rightStick.x = 0;
	state.rightStick.y = 0;

	ResolveButtons();

	deadZone = 0.2f;

	inputToSend.aimingBearing = 0.0f;
	inputToSend.movementBearing = 0.0f;
}

PlayerInput PlayerController::SendInput()
{
	//inputToSend.aimingBearing = 0.0f;
	//inputToSend.aimingIntesity= 0.0f;
	//inputToSend.movementBearing = 0.0f;
	//inputToSend.movementIntensity = 0.0f;

	ResolveJoystickIntensity();
	ResolveJoystickBearing();
	ResolveButtons();
	return inputToSend;
}

void PlayerController::ResolveJoystickIntensity()
{
	float tempRawMovIntensity = state.leftStick.Get_magnitude()/100.0f;
	if(tempRawMovIntensity>deadZone)
	{

		float tempMovIntensity = (tempRawMovIntensity-deadZone)/(1.0f-deadZone);
		inputToSend.movementIntensity = tempMovIntensity;
		if(inputToSend.movementIntensity > 1.0f)
		{
			inputToSend.movementIntensity = 1.0f;
		}

	}
	else
	{
		inputToSend.movementIntensity = 0.0f;
	}


	float tempRawAimIntensity = state.rightStick.Get_magnitude()/100.0f;
	if(tempRawAimIntensity>deadZone)
	{
		float tempAimIntensity = (tempRawAimIntensity-deadZone)/(1.0f-deadZone);
		inputToSend.aimingIntesity = tempAimIntensity;
		if(inputToSend.aimingIntesity>1.0f)
		{
			inputToSend.aimingIntesity = 1.0f;
		}
	}
	else
	{
		inputToSend.aimingIntesity = 0.0f;
	}
}

void PlayerController::ResolveJoystickBearing()
{
	inputToSend.movementBearing = 0.0f;
	//Left Stick
	if(state.leftStick.x >= 0)//TODO
	{
		if(state.leftStick.x == 0 && state.leftStick.y == 0)
		{
			inputToSend.movementBearing == 0.0f;
		}
		else if(state.leftStick.y <= 0)//0 <= x < 90
		{
			inputToSend.movementBearing = 0 + ((180/M_PI)*(asin(state.leftStick.x/state.leftStick.Get_magnitude())));	
		}
		else if(state.leftStick.y > 0)
		{
			inputToSend.movementBearing = 90 + ((180/M_PI)*(asin(state.leftStick.y/state.leftStick.Get_magnitude())));
		}
	}
	else if(state.leftStick.x < 0)
	{
		if(state.leftStick.y >= 0)
		{
			inputToSend.movementBearing = 180 + ((180/M_PI)*(asin(-state.leftStick.x/state.leftStick.Get_magnitude())));
		}
		else if(state.leftStick.y < 0)
		{
			inputToSend.movementBearing = 270 + ((180/M_PI)*(asin(-state.leftStick.y/state.leftStick.Get_magnitude())));
		}
	}

	//Right Stick
	if(state.rightStick.x >= 0)//TODO
	{
		if(state.rightStick.x == 0 && state.rightStick.y == 0)
		{
			inputToSend.aimingBearing == 0.0f;
		}
		else if(state.rightStick.y <= 0)//0 <= x < 90
		{
			inputToSend.aimingBearing = 0 + ((180/M_PI)*(asin(state.rightStick.x/state.rightStick.Get_magnitude())));	
		}
		else if(state.rightStick.y > 0)
		{
			inputToSend.aimingBearing = 90 + ((180/M_PI)*(asin(state.rightStick.y/state.rightStick.Get_magnitude())));
		}
	}
	else if(state.rightStick.x < 0)
	{
		if(state.rightStick.y >= 0)
		{
			inputToSend.aimingBearing = 180 + ((180/M_PI)*(asin(-state.rightStick.x/state.rightStick.Get_magnitude())));
		}
		else if(state.rightStick.y < 0)
		{
			inputToSend.aimingBearing = 270 + ((180/M_PI)*(asin(-state.rightStick.y/state.rightStick.Get_magnitude())));
		}
	}

	//To account for ISOMETRIC
	inputToSend.movementBearing+=45.0f;
}

void PlayerController::ResolveButtons()
{//Sets button values
	inputToSend.actions[0] = state.buttons[ctrlLB];
	inputToSend.actions[1] = state.buttons[ctrlA];
	inputToSend.actions[2] = state.buttons[ctrlRB];
	inputToSend.actions[3] = state.buttons[ctrlX];
	inputToSend.actions[4] = state.buttons[ctrlStart];

}