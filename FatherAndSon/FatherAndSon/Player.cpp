#include "Player.h"

Player::Player(bool hasShadowIn): 
	accelerationForceDefault(10.0f),
	frictionCoefficientDefault(1.0f),
	GameObjectIso(hasShadowIn)
{
	mPtrBall = nullptr;

	//setSize(sf::Vector2f(20.0f,20.0f));
	mPlayerNumber = 0;
	setColor(sf::Color::Magenta);

	mSpriteSheetMax.x = 4;
	mSpriteSheetMax.y = 4;

	mSpriteCoord.x = 0;
	mSpriteCoord.y = 0;

	mTopLeftSpriteCoord.x = 0;
	mTopLeftSpriteCoord.y = 0;

	mFrameTimeMax = 10.0f;
	mFrameTimer = 0;

	setTextureRect(sf::IntRect(0,0,TILE_SCALE,TILE_SCALE));

	//setPosition(200.0f,200.0f);
	//setOrigin(getRadius(),getRadius());//TODO

	//ptrInputState = PlayerInput();
	ptrInputState = new PlayerInput();
	ptrInputState->actions[0]=false;
	ptrInputState->actions[1]=false;
	ptrInputState->actions[2]=false;
	ptrInputState->actions[3]=false;
	ptrInputState->actions[4]=false;
	ptrInputState->aimingBearing = 0.0f;
	ptrInputState->aimingIntesity = 0.0f;
	ptrInputState->movementBearing = 0.0f;
	ptrInputState->movementIntensity = 0.0f;

	
	accelerationForce = accelerationForceDefault;
	frictionCoefficient = frictionCoefficientDefault;

	deadZone = 0.3f;

	minVelocity = 0.5f;
	maxVelocity = 5.0f;

	setWorldPosition(5.0f,10.0f);

	
	mBallKicked = false;
	mGrounded = true;
	mZVelocity = 0.0f;
	mGravity = 9.8f;
	mKickDistance = 1.0f;
	mVerticalKickDistance = 1.0f;

}

void Player::Initialise(sf::Vector2f positionIn,sf::Color colorIn)
{
	setColor(colorIn);
	setPosition(positionIn); 

	//positionVelocityRay.Update(ConvertSFVector2(getPosition()),ConvertSFVector2(getPosition())+(velocity* *ptrDeltaTime));
	//positionVelocityRay.setDebugDrawScale(10.0f);
}


void Player::Update()//TODO//14-09-15//Overwrites GameObjectIso update
{
	while(ptrInputState->movementBearing>360)
	{
		ptrInputState->movementBearing -= 360;
	}
	
	while(ptrInputState->movementBearing<0)
	{
		ptrInputState->movementBearing += 360;
	}

	Actions();
	Movement();
	
	Collision();

	GameObjectIso::Update();

	Collision();
}



void Player::Movement()
{
	if(ptrInputState->movementIntensity>=deadZone)
	{
		float accelerationModifier = ptrInputState->movementIntensity * accelerationForce;
		
		acceleration.x=-accelerationModifier*sin((ptrInputState->movementBearing+180)*(M_PI/180.0f));
		acceleration.y=accelerationModifier*cos((ptrInputState->movementBearing+180)*(M_PI/180.0f));
		
		lastMovementBearing = getMovementBearing();
	}
	else
	{
		setMovementBearing(lastMovementBearing);
		acceleration = 0;//TODO//setAcceleration(0);

		if(velocity.Get_magnitude() <= minVelocity)
		{
			velocity = 0.0f;
		}
	}
	


	//friction
	velocity = velocity - (velocity*(frictionCoefficient * *ptrDeltaTime));

	if(velocity.Get_magnitude()<maxVelocity)
	{
		velocity = velocity + (acceleration* *ptrDeltaTime);
	}
	else
	{
		//MaxVelocity	
	}

	
	

	

	

	SpriteAnimation();

	UpdatePosition();

	positionVelocityRay.Update(ConvertSFVector2(getPosition()),ConvertSFVector2(getPosition())+(velocity* *ptrDeltaTime));
}

void Player::UpdatePosition()
{
	//move(velocity.x* *ptrDeltaTime,velocity.y* *ptrDeltaTime);

	moveWorldPosition(velocity.x* *ptrDeltaTime,velocity.y* *ptrDeltaTime,mZVelocity* *ptrDeltaTime);
}

void Player::Actions()
{
	//ACTION STATE//////////////////////////
	//set up action triggers

	if(ptrInputState->actions[1])
	{
		Jump();
	}
	if(ptrInputState->actions[3])
	{
		Kick();
	}
	if(ptrInputState->actions[2])
	{
		Chip();
	}
	
}

void Player::Collision()
{
	if(getWorldPositionZ() <= 0.0f)
	{
		setWorldPositionZ(0.0f);
		mGrounded = true;
	}
	else
	{
		mGrounded = false;
	}


	//Gravity
	if(!mGrounded)
	{
		mZVelocity -= mGravity* *ptrDeltaTime;
	}
	else
	{
		mKickReady = true;
	}
}

void Player::Jump()
{
	if(mGrounded)
	{
		mZVelocity = 5.0f;
	}
}

void Player::Kick()
{
	if(mKickReady)
	{
		//BOOT IT!
		if(abs((mPtrBall->getWorldPosition().Get_magnitude() - getWorldPosition().Get_magnitude()))<mKickDistance
		&& abs(mPtrBall->getWorldPositionZ()-getWorldPositionZ())<mVerticalKickDistance)
		{	
			//mPtrBall->IncrementVelocity((acceleration*0.3f)+(velocity*0.25f),mPtrBall->GetVelocityZ()/2.0f);
			mPtrBall->SetVelocity((acceleration*0.6f)+(velocity*0.25f),mPtrBall->GetVelocityZ()/2.0f);
			mBallKicked = true;
			//mPtrBall->IncrementVelocity((acceleration*0.3f)+(velocity*0.25f),0.0f);
		}

		if(mGrounded)
		{
			mZVelocity = 2.5f;
		}
		else//if in air and havent kicked already
		{
			mZVelocity = 1.0f;
		}
		
		mKickReady = false;
	}
	
}

void Player::Chip()
{
	if(mKickReady)
	{
		if(abs((mPtrBall->getWorldPosition().Get_magnitude() - getWorldPosition().Get_magnitude()))<mKickDistance
		&& abs(mPtrBall->getWorldPositionZ()-getWorldPositionZ())<mVerticalKickDistance)
		{
			//mPtrBall->IncrementVelocity((acceleration*0.1f),2.0f);
			mPtrBall->SetVelocity(mPtrBall->GetVelocity()*0.5f,4.0f);
			mBallKicked = true;
			
		}

		if(mGrounded)
		{
			mZVelocity = 2.5f;
			mKickReady = false;

		}
		else
		{	
			mZVelocity = 1.0f;
			mKickReady = false;
		}

		mKickReady = false;
	}
}

void Player::SpriteAnimation()
{
	mFrameTimeMax = 0.15f;
	mFrameTimeMax = 1.0f/(velocity.Get_magnitude()*2.0f);


	mFrameTimer += *ptrDeltaTime;

	if(mGrounded)
	{
		if(velocity.Get_magnitude() < 0.2f)
		{
			mSpriteCoord.x = 0;
		}
		else if(mFrameTimer >= mFrameTimeMax)
		{
			mSpriteCoord.x += 1;

			if(mSpriteCoord.x >= mSpriteSheetMax.x)
			{
				mSpriteCoord.x -= 4;
			
				if(mSpriteCoord.x >= mSpriteSheetMax.x)//There is a reason for this//it's for alternating legs when you change direction
				{
					mSpriteCoord.x = 0;
				}
			}
			mFrameTimer = 0; 
		}
	}
	else
	{
		mSpriteCoord.x = 1;
	}

	if(getMovementBearing()>=-22.5 && getMovementBearing()<22.5)
	{
		//headingupwards
		mSpriteCoord.y = 2;
	}
	else if(getMovementBearing()>=22.5 && getMovementBearing()<67.5)
	{
		mSpriteCoord.y = 3;
	}
	else if(getMovementBearing()>=67.5 && getMovementBearing()<112.5)
	{
		mSpriteCoord.y = 0;
	}
	else if(getMovementBearing()>=112.5 && getMovementBearing()<157.5)
	{
		mSpriteCoord.y = 1;
	}
	else if(getMovementBearing()>=157.5 && getMovementBearing()<202.5)
	{
		mSpriteCoord.y = 2;
	}
	else if(getMovementBearing()>=202.5 && getMovementBearing()<247.5)
	{
		mSpriteCoord.y = 3;
	}
	else if(getMovementBearing()>=247.5 && getMovementBearing()<292.5)
	{
		mSpriteCoord.y = 0;
	}
	else if(getMovementBearing()>=292.5 && getMovementBearing()<337.5)
	{
		mSpriteCoord.y = 1;
	}
	else if(getMovementBearing()>=337.5 && getMovementBearing()<382.5)
	{
		mSpriteCoord.y = 2;
	}


	mTopLeftSpriteCoord.x = mSpriteCoord.x * TILE_SCALE;
	mTopLeftSpriteCoord.y = mSpriteCoord.y * TILE_SCALE;

	setTextureRect(sf::IntRect(mTopLeftSpriteCoord.x,mTopLeftSpriteCoord.y,TILE_SCALE,TILE_SCALE));
}

bool Player::CheckBoxCollide(sf::RectangleShape boundingBoxIn)
{
	Vector2 collisionPoint(0.0f,0.0f);
	float vectorFraction = 0;
	if(positionVelocityRay.CollideWithAABB(boundingBoxIn,collisionPoint,vectorFraction))
	{
		int collisionSide = 0;//1 left,2 top,3 right,4 bottom// All of player
		
		bool horizontalCollision = false;
		bool verticalCollision = false;
		//abs(collisionPoint.

		if(collisionPoint.y > boundingBoxIn.getPosition().y - boundingBoxIn.getGlobalBounds().height/2
		&& collisionPoint.y < boundingBoxIn.getPosition().y + boundingBoxIn.getGlobalBounds().height/2)//Horizontal collision
		{
			horizontalCollision = true;

			if(collisionPoint.x > boundingBoxIn.getPosition().x)//midpoint
			{
				collisionSide = 1;//left of player
			}
			else
			{
				collisionSide = 3;//right of player
			}
		}

		
		if(collisionPoint.x > boundingBoxIn.getPosition().x - boundingBoxIn.getGlobalBounds().width/2
		&& collisionPoint.x < boundingBoxIn.getPosition().x + boundingBoxIn.getGlobalBounds().width/2)//Horizontal collision
		{
			verticalCollision = true;

			if(collisionPoint.y > boundingBoxIn.getPosition().y)//midpoint
			{
				collisionSide = 2;//top side of player
			}
			else
			{
				collisionSide = 4;//bottom side of player
			}
		}
		
		if(horizontalCollision&&verticalCollision)
		{
			setPosition(prevPosition);
			collisionSide = 5;
		}
		lastCollisionMarker.setFillColor(brightGreen);
		lastCollisionMarker.setSize(sf::Vector2f(2.0f,2.0f));
		lastCollisionMarker.setPosition(collisionPoint.x,collisionPoint.y);
		
		switch(collisionSide)
		{
		case 0://corner
			//setPosition(RESET_POSITION.x,RESET_POSITION.y);
			//velocity = 0;
			velocity = velocity*-1;
			break;
		case 1://left of player
			//setPosition(boundingBoxIn.getPosition().x + (boundingBoxIn.getGlobalBounds().width/2),collisionPoint.y);
			//velocity.x = 0.0f;
			velocity.x *= -1.0f;
			break;
		case 2://top of player
			//setPosition(collisionPoint.x,boundingBoxIn.getPosition().y + (boundingBoxIn.getGlobalBounds().height/2));
			//velocity.y = 0.0f;
			velocity.y *= -1.0f;
			break;
		case 3://right of player
			//setPosition(boundingBoxIn.getPosition().x - (boundingBoxIn.getGlobalBounds().width/2),collisionPoint.y);
			//velocity.x = 0.0f;
			velocity.x *= -1.0f;
			break;
		case 4://bottom of player
			//setPosition(collisionPoint.x,boundingBoxIn.getPosition().y - (boundingBoxIn.getGlobalBounds().height/2));
			//velocity.y = 0.0f;
			velocity.y *= -1.0f;
			break;
		case 5:
			break;
		default:
			velocity = 0;
			break;
		}
	
		*ptrDeltaTime *= vectorFraction;

		Movement();	

		return true;
	}
	else
	{
		return false;
	}
}

//Getters and Setters
float Player::getMovementBearing()
{
	return ptrInputState->movementBearing;
}

void Player::setMovementBearing(float degreesIn)//TODO//Should this functions exist?
{
	ptrInputState->movementBearing = degreesIn;
}

float Player::getMovementIntensity()
{
	return ptrInputState->movementIntensity;
}

void Player::setMovementIntensity(float intensityScaleIn)//TODO//OLD//Should this functions exist?
{
	//
}


float Player::getAimingBearing()
{
	return ptrInputState->aimingBearing;
}

void Player::setAimingBearing(float degreesIn)//TODO//OLD//Should this functions exist?
{
	ptrInputState->aimingBearing = degreesIn;
}

float Player::getAimingIntensity()
{
	return ptrInputState->aimingIntesity;
}

float Player::getDeadZone()
{
	return deadZone;
}

Vector2 Player::getVelocity()
{
	return velocity;
}

float Player::GetVelocityZ()
{
	return mZVelocity;
}

void Player::SetVelocity(float xVelIn, float yVelIn, float zVelIn)
{
	velocity.x = xVelIn;
	velocity.y = yVelIn;

	mZVelocity = zVelIn;

}

float Player::getVelocityMagnitude()
{
	return velocity.Get_magnitude();
}

/*void Player::setColor(sf::Color colorIn)
{
	//setFillColor(colorIn);
	Sprite.setColor(colorIn);
}*/

void Player::setDeltaTime(float *deltaTimeIn)
{
	ptrDeltaTime = deltaTimeIn;
}

void Player::SetInput(PlayerInput* playerInputIn)
{
	ptrInputState = playerInputIn;
}

PlayerInput Player::GetPlayerInput()
{
	return *ptrInputState;
}

void Player::SetBall(Ball *ballIn)
{
	mPtrBall = ballIn;
}

Ray Player::getPositionVelocityRay()
{
	return positionVelocityRay;
}

void Player::setTileX(int xIn)
{
	tileX = xIn;
}

int Player::getTileX()
{
	return tileX;
}

void Player::setTileY(int yIn)
{
	tileY = yIn;
}

int Player::getTileY()
{
	return tileY;
}