#ifndef __ANTI_AIRCRAFT_GUN_H__
#define __ANTI_AIRCRAFT_GUN_H__

#pragma once

#include "cocos2d.h"

class AntiAircraftMissile;
class Explosion;

class AntiAircraftGun : public cocos2d::Node
{
public:

	static AntiAircraftGun* spawnGun(cocos2d::Layer *gameScene);
	AntiAircraftGun(cocos2d::Layer *gameScene);
	virtual ~AntiAircraftGun();

	cocos2d::Vec2 getGunCurrentPosition();

private:

	cocos2d::Layer *gameScene;

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 aimPosition;

	float remainingShiftDistance;
	float nextMoveDuration;
	int countedAttackTime;

	cocos2d::Sprite *gunSprite;
	cocos2d::Sprite *missile;

	void initAntiAircraftGun();
	void setupGunAction();
	void flipGunSprite();

	void generateNextMoveDuration();
	void launchMissile();
	cocos2d::Vec2 getTargetPosition();
};

#endif //__ANTI_AIRCRAFT_GUN_H__

#ifndef __ANTI_AIRCRAFT_MISSILE_H__
#define __ANTI_AIRCRAFT_MISSILE_H__

#define __ANTI_AIRCRAFT_MISSILE_FLYING_DURATION__ 1.5F

class AntiAircraftMissile : public cocos2d::Node
{
public:

	enum class MissileType
	{
		FLY_TO_THE_LEFT,
		FLY_TO_THE_RIGHT
	};

	AntiAircraftMissile(
		cocos2d::Layer *gameScene, 
		cocos2d::Vec2 startPos, 
		cocos2d::Vec2 endPos,
		MissileType type = MissileType::FLY_TO_THE_LEFT
	);
	virtual ~AntiAircraftMissile();

private:

	cocos2d::Layer *gameScene;

	cocos2d::Vec2 startPosition;
	cocos2d::Vec2 endPosition;

	cocos2d::Sprite *missileSprite;
	MissileType missileType;

	void initMissile();
};

#endif // !__ANTI_AIRCRAFT_MISSILE_H__

#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

class Explosion : public cocos2d::Node
{
public:

	Explosion(cocos2d::Layer *gameScene, cocos2d::Vec2 pos);
	virtual ~Explosion();

private:

	cocos2d::Layer *gameScene;
	cocos2d::Vec2 explodePos;

	void initExplosion();
};

#endif // !__EXPLOSION_H__