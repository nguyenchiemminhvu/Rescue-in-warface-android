#ifndef __MISSILE_H__
#define __MISSILE_H__

#pragma once

#include "cocos2d.h"

class Missile : public cocos2d::Node
{
public:

	static Missile* spawnMissile(cocos2d::Layer *gameScene, float playerPosY);
	Missile(cocos2d::Layer *gameScene, float playerPosY);
	virtual ~Missile();

	cocos2d::Sprite* getMissileSprite();
	cocos2d::ParticleSun *getFire();

private:

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	cocos2d::Layer *gameScene;
	cocos2d::Sprite *missileSprite;
	cocos2d::ParticleSun *fire;

	void createMissile(float playerPosY);
	void createFireParticle();

	cocos2d::Sequence* missileActions();
	cocos2d::Sequence* particleActions();
};

#endif //__MISSILE_H__