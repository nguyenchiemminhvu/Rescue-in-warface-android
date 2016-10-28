#ifndef __MOTHER_FUCKER_H__
#define __MOTHER_FUCKER_H__

#pragma once

#include "cocos2d.h"

#define __MOTHER_FUCKER_TOTAL_BULLET__ 3
#define	__MOTHER_FUCKER_MAX_HEALTH__ 40

class MotherFucker : public cocos2d::Node
{
public:

	MotherFucker(cocos2d::Layer *gameScene);
	virtual ~MotherFucker();

	bool isStarted();
	void setStarted(bool started);

	int getHealth();
	void decreaseHealth();
	void onBossDead();
	bool isBossDead();

	void start();
	void preparing();
	void setupLoopAction();

private:

	cocos2d::Layer *gameScene;

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	float verticalFlightDistance;
	cocos2d::Vec2 bulletDirection[__MOTHER_FUCKER_TOTAL_BULLET__];

	bool started;

	///////////////////////////////////
	// Mother fucker properties
	cocos2d::Sprite *bossSprite;
	int health;
	bool flip;
	bool dead;

	////////////////////////////////////
	// action
	cocos2d::MoveBy* moveUp();
	cocos2d::MoveBy* moveDown();

	void attack();

	////////////////////////////////////
	// others
	void initMotherFucker();
	void flipMotherFucker();
	void initBulletDirection();
	void createBullets(cocos2d::Vec2 position);
};

#endif // !__MOTHER_FUCKER_H__
