#ifndef _PARTICLE_EMITTER_H_
#define _PARTICLE_EMITTER_H_

#include <list>
#include "globals.h"
#include "SFML\Graphics.hpp"
#include "Vector2.h"

//PARTICLE
class Particle : public sf::RectangleShape
{
	//METHODS
public:
	Particle();
	Particle(Vector2 positionIn,Vector2 velocityIn,float rotationSpeedIn, float lifeTimeIn,float minSpeedIn,float scaleIn);
	
	void Update();
	
	void SetDeltaTime(float *deltaTimeIn);

	void SpeedDeathToggle(bool activeIn);
	void TimerDeathToggle(bool activeIn);

	bool GetAlive();
private:


	//VARIABLES
public:

private:
	Vector2 velocity;
	float rotationSpeed;
	float lifeTime;
	float minSpeed;

	bool speedDeathToggle;
	bool timerDeathToggle;
	bool isAlive;

	sf::Color fillColor;

	float *ptrDeltaTime;
};

//EMITTER
class ParticleEmitter
{
	//METHODS
public:
	ParticleEmitter();
	
	void Update();
	void Emit(int amountIn);
	void setPosition(Vector2 positionIn);
	void SetEmitPreferences(Vector2 positionIn, float bearingIn, float spreadIn,float durationIn,float minSpeedIn, float scaleIn,float rotationAmountIn);
	void SetEmitPreferences(Vector2 positionIn,Vector2 velocityIn,float durationIn,float minSpeedIn,float scaleIn,float rotationAmountIn);
	void SetEmitBearing();
	void SetEmitSpread();
	void SetEmitDuration();
	void SetEmitSize(int startSizeIn);
	void SetEmitSize(int startSizeIn,float endSizeIn);

	void setEmitFrequency(float maxEmitTimerIn);

	void setDeltaTime(float *deltaTimeIn);
private:
	Particle CreateParticle();

	//VARIABLES
public:
	std::list<Particle> particles;
private:
	
	Vector2 position;
	Vector2 velocity;
	float bearing;
	float spread;
	float duration;
	float scale;//TODO
	float rotationAmount;
	float emitSpeed;//TODO
	float minSpeed;//TODO//minspeed does nothing

	bool emitTimerActive;
	float emitTimer;
	float emitTimerMax;

	float *ptrDeltaTime;
};

#endif