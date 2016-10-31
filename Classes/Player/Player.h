#ifndef __PLAYER_H__
#define __PLAYER_H__

#pragma once

#include "cocos2d.h"

#define __PLAYER_ANIMATION_TOTAL_FRAME__ 2
#define __PLAYER_RELOAD_DURATION__ 0.2F
#define __HELICOPTER_FLYING_SPEED__ 100.0F
#define __JOY_STICK_ORIGIN_HALF_LENGTH__ 10.0F

class PlayerBullet;

class Player : public cocos2d::Node
{
public:

	enum MovementDirection
	{
		LEFT = 1,			//0001
		RIGHT = (1 << 1),	//0010
		UP = (1 << 2),		//0100
		DOWN = (1 << 3)		//1000
	};

	Player(cocos2d::Layer *gameScene);
	virtual ~Player();

	void updatePlayerPosition(float dt);

	void resetPlayerMovementDirection();
	void setPlayerMovementDirection(cocos2d::Vec2 vec);
	void decreaseFuel();
	int getCurrentFuel();
	void increaseScore(int amount);
	unsigned long long getScore();
	void lostControl();
	bool stillUnderControl();
	cocos2d::Vec2 getPlayerGunPosition();

	void scheduleShooting();
	void unscheduleShooting();
	void createBullet(float t);

private:

	cocos2d::Layer *gameScene;

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	cocos2d::Sprite *playerSprite;

	////////////////////////////////////////////
	//Player properties
	int fuel;
	bool isUnderControl;
	int movementDirection;
	unsigned long long score;


	////////////////////////////////////////////
	// player initialization
	void initPlayerSprite();
	void initPlayerAnimation();
};

#endif //__PLAYER_H__

#ifndef __PLAYER_BULLET_H__
#define __PLAYER_BULLET_H__

#define __PLAYER_BULLET_VELOCITY__ 1000

class PlayerBullet : public cocos2d::Node {
public:

	static PlayerBullet* createBullet(cocos2d::Layer *gameScene, cocos2d::Vec2 pos);

	PlayerBullet(cocos2d::Layer *gameScene, cocos2d::Vec2 pos);
	virtual ~PlayerBullet();

private:

	cocos2d::Layer *gameScene;

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	cocos2d::Sprite *bulletSprite;
	cocos2d::Vec2 startPos;

	void initPlayerBullet();
};

#endif // !__PLAYER_BULLET_H__