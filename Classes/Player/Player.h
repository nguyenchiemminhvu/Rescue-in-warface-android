#ifndef __PLAYER_H__
#define __PLAYER_H__

#pragma once

#include "cocos2d.h"

#define __PLAYER_ANIMATION_TOTAL_FRAME__ 2

class Player : public cocos2d::Node
{
public:

	enum PlayerProperties
	{
		FLYING_SPEED = 100
	};

	enum MovementDirection
	{
		LEFT = 1,			//0001
		RIGHT = (1 << 1),	//0010
		UP = (1 << 2),		//0100
		DOWN = (1 << 3)		//1000
	};

	Player(cocos2d::Layer *gameScene);
	virtual ~Player();

	virtual void update(float dt);

	void resetPlayerMovement();
	void lostControl();
	cocos2d::Vec2 getPlayerGunPosition();

private:

	cocos2d::Layer *gameScene;

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	cocos2d::Sprite *playerSprite;

	//Player properties
	int movement;
	bool isUnderControl;

	void initPlayerSprite();
	void initPlayerAnimation();

	void updatePlayerPosition(float dt);
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