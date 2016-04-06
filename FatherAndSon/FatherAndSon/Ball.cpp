#include "Ball.h"


Ball::Ball(bool hasShadowIn): GameObjectIso(hasShadowIn)
{
	mZVelocity = 0;
	mGravity = 5.0f;
	frictionCoefficient = 0.6f;
	mRestitutionCoefficient = 50.0f;
	mGrounded = true;
	mPrevGrounded = mGrounded;
	setWorldPositionZ(0.0f);
	mMinZVelocity = 0.5f;
	
}

void Ball::Update()
{
	Movement();
	
	GameObjectIso::Update();
}

void Ball::Movement()
{

	if(mGrounded)
	{
		velocity = velocity - (velocity*(frictionCoefficient * *ptrDeltaTime));
		if(mPrevGrounded != mGrounded)
		{
			mZVelocity *= -1;
			mZVelocity = mZVelocity - (mZVelocity*(mRestitutionCoefficient * *ptrDeltaTime));

			if((mZVelocity>0 && mZVelocity<mMinZVelocity)||(mZVelocity<0 && mZVelocity>-mMinZVelocity))
			{
				mZVelocity = 0;
			}
		}
	}
	else if(!mGrounded)
	{
		mZVelocity -= mGravity* *ptrDeltaTime;
	}

	moveWorldPosition(velocity.x* *ptrDeltaTime,velocity.y* *ptrDeltaTime,mZVelocity* *ptrDeltaTime);


	//COLLISION
	mPrevGrounded = mGrounded;
	if(getWorldPositionZ() < 0.0f)
	{
		setWorldPositionZ(0.0f);
		mGrounded = true;	
	}
	else
	{
		mGrounded = false;
	}
}

void Ball::Animate()
{

}

void Ball::setDeltaTime(float *deltaTimeIn)
{
	ptrDeltaTime = deltaTimeIn;
}

void Ball::SetVelocity(Vector2 velocityIn)
{
	velocity = velocityIn;
}

void Ball::SetVelocity(Vector2 velocityIn,float zVelocityIn)
{
	velocity = velocityIn;
	mZVelocity = zVelocityIn;
}