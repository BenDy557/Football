#include "ParticleEmitter.h"

//PARTICLES
Particle::Particle()
{
	isAlive = true;
}

Particle::Particle(Vector2 positionIn, Vector2 velocityIn, float rotationSpeedIn, float lifeTimeIn,float minSpeedIn, float sizeIn)
{
	isAlive = true;
	speedDeathToggle = true;
	timerDeathToggle = true;
	minSpeed = minSpeedIn;
	fillColor = sf::Color::White;
	fillColor.r = 128.0f;
	fillColor.g = 50.0f;
	fillColor.b = 50.0f;

	rotationSpeed = rotationSpeedIn;
	lifeTime = lifeTimeIn;
	velocity = velocityIn;
	setPosition(positionIn.x,positionIn.y);
	setFillColor(fillColor);
	setSize(sf::Vector2f(sizeIn,sizeIn));
	setOrigin(sizeIn/2,sizeIn/2);
}

void Particle::Update()
{
	lifeTime -= *ptrDeltaTime;
	move(ConvertVector2(velocity)* *ptrDeltaTime);
	velocity = velocity - (velocity*(2.5f * *ptrDeltaTime));//friction
	rotate(rotationSpeed);

	scale(1.02f,1.02f);
	fillColor.a *= 0.96f;//TODO//Fade
	setFillColor(fillColor);

	if(lifeTime<0)
	{
		isAlive = false;
	}
	if(velocity.Get_magnitude()<minSpeed)
	{
		isAlive = false;
	}
}

void Particle::SetDeltaTime(float *deltaTimeIn)
{
	ptrDeltaTime = deltaTimeIn;
}

void Particle::SpeedDeathToggle(bool activeIn)
{
	speedDeathToggle = activeIn;
}

void Particle::TimerDeathToggle(bool activeIn)
{
	timerDeathToggle = activeIn;
}

bool Particle::GetAlive()
{
	return isAlive;
}

//EMITTER
ParticleEmitter::ParticleEmitter()
{
	bearing = 0;
	spread = 360;
	duration = 2.0f;
	scale = 1.0f;
	rotationAmount = 1.0f;
	emitTimerActive = false;
}

void ParticleEmitter::Update()
{
	if(emitTimerActive)
	{
		if(emitTimer<0)
		{
			Emit(1);
			emitTimer = emitTimerMax;
		}
	}
	//particles
	for(std::list<Particle>::iterator it =particles.begin(); it != particles.end();)
	{
		if(it->GetAlive())
		{
			it->Update();
			it++;
		}
		else
		{
			it = particles.erase(it);
		}
	}

	emitTimer -= *ptrDeltaTime;
}

void ParticleEmitter::SetEmitPreferences(Vector2 positionIn,float bearingIn, float spreadIn,float durationIn,float minSpeedIn,float scaleIn,float rotationAmountIn)
{
	bearing = bearingIn;
	spread = spreadIn;
	duration = durationIn;
	scale = scaleIn;
	rotationAmount = rotationAmountIn;
	minSpeed = minSpeedIn;
	position = positionIn;
}

void ParticleEmitter::SetEmitPreferences(Vector2 positionIn,Vector2 velocityIn,float durationIn,float minSpeedIn,float scaleIn,float rotationAmountIn)
{
	bearing = 0;
	spread = 0;
	duration = durationIn;
	scale = scaleIn;
	rotationAmount = rotationAmountIn;
	minSpeed = minSpeedIn;
	position = positionIn;
	velocity = velocityIn;
}

void ParticleEmitter::setPosition(Vector2 positionIn)
{
	position = positionIn;
}

void ParticleEmitter::Emit(int amountIn)
{
	//CreateParticle();
	for(int i = 0;i<amountIn;i++)
	{
		particles.push_back(CreateParticle());
	}
}

Particle ParticleEmitter::CreateParticle()
{
	Vector2 tempVelocity((float)(rand() % 150-75),(float)(rand() % 150-75));
	
	Particle tempParticle(position,tempVelocity,(rand()% (int)(rotationAmount*10) -rotationAmount*5)/10,duration,minSpeed,scale);//TODO

	tempParticle.SetDeltaTime(ptrDeltaTime);
	

	return tempParticle;
}

void ParticleEmitter::setEmitFrequency(float maxEmitTimerIn)
{
	if(maxEmitTimerIn <= 0)
	{
		emitTimerActive = false;
	}
	else
	{
		emitTimerActive=true;
		emitTimerMax = maxEmitTimerIn;
	}
}

void ParticleEmitter::setDeltaTime(float *deltaTimeIn)
{
	ptrDeltaTime = deltaTimeIn;
}