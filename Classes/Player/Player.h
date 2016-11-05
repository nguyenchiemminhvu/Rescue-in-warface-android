#ifndef __PLAYER_H__
#define __PLAYER_H__

#pragma once

#include "cocos2d.h"

#define __PLAYER_FLYING_ANIMATION_TOTAL_FRAME__ 2
#define __PLAYER_EXPLODING_ANIMATION_TOTAL_FRAME__ 5
#define __PLAYER_RELOAD_DURATION__ 0.2F
#define __HELICOPTER_FLYING_SPEED__ 100.0F
#define __JOY_STICK_ORIGIN_HALF_LENGTH__ 10.0F
#define __HELICOPTER_MAX_FUEL__ 20
#define __HELICOPTER_NOT_ROTATION_DEGREE__ 0.0F
#define __HELICOPTER_LEFT_ROTATION_CLAMP__ -10.0F
#define __HELICOPTER_RIGHT_ROTATION_CLAMP__ 10.0F

class PlayerBullet;
class Explosion;

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

	void saveScore();

	cocos2d::PhysicsBody* getPlayerPhysicsBody();

	void updatePlayerPosition(float dt);
	void explodingHelicopter();

	void balancingPlayerRotation();
	void rotatingToTheLeft();
	void rotatingToTheRight();

	void resetPlayerMovementDirection();
	void setPlayerMovementDirection(cocos2d::Vec2 vec);
	void increaseFuel(int amount);
	void decreaseFuel();
	int getCurrentFuel();
	void increaseScore(int amount);
	unsigned long long getScore();
	void lostControl();
	bool stillUnderControl();
	cocos2d::Vec2 getPlayerCurrentPosition();
	cocos2d::Vec2 getPlayerGunPosition();

	PlayerBullet* createBullet(float t);

private:

	cocos2d::Layer *gameScene;

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	cocos2d::Sprite *playerSprite;

	////////////////////////////////////////////
	//Player properties
	int fuel;
	bool isUnderControl;
	int rotationDegree;
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

	static PlayerBullet* createBullet(cocos2d::Layer *gameScene, cocos2d::Vec2 pos, float rotation);

	PlayerBullet(cocos2d::Layer *gameScene, cocos2d::Vec2 pos, float rotation);
	virtual ~PlayerBullet();

	cocos2d::Sprite *getBulletSprite();

private:

	cocos2d::Layer *gameScene;

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	cocos2d::Sprite *bulletSprite;
	cocos2d::Vec2 startPos;
	float radian;

	void initPlayerBullet();
};

#endif // !__PLAYER_BULLET_H__

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