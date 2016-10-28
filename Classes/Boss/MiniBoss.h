#ifndef __MINI_BOSS_H__
#define __MINI_BOSS_H__

#pragma once

#include "cocos2d.h"

class MiniBoss : public cocos2d::Node
{
public:

	MiniBoss(cocos2d::Layer *gameScene);
	virtual ~MiniBoss();

	void start();
	void setupLoopAction();
	bool isStarted();
	void setStarted(bool started);

	cocos2d::PhysicsBody* getBossBody1();
	cocos2d::PhysicsBody* getBossBody2();
	int getHealth1();
	int getHealth2();
	void decreaseHealth1();
	void decreaseHealth2();
	void onBossDead1();
	void onBossDead2();
	bool isBossDead1();
	bool isBossDead2();

private:

	cocos2d::Layer *gameScene;

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	bool started;
	int state;

	float verticalFlightDistance;
	float horizontalFlightDistance;

	/////////////////////////////////
	// boss1 properties
	cocos2d::Sprite *bossSprite1;
	int health1;
	bool flip1;
	bool dead1;

	/////////////////////////////////
	// boss2 properties
	cocos2d::Sprite *bossSprite2;
	int health2;
	bool flip2;
	bool dead2;

	/////////////////////////////////
	// boss action
	cocos2d::Sequence* preparation1();
	cocos2d::Sequence* preparation2();

	cocos2d::Sequence* action1_1();
	cocos2d::Sequence* action1_2();
	cocos2d::Sequence* action2_1();
	cocos2d::Sequence* action2_2();

	void flipBoss1();
	void flipBoss2();

	cocos2d::MoveBy* moveUp();
	cocos2d::MoveBy* moveDown();
	cocos2d::MoveBy* moveLeft();
	cocos2d::MoveBy* moveRight();

	void attack1();
	void attack2();

	void createBullet(cocos2d::Vec2 position, bool leftToRight = false);

	/////////////////////////////////
	// initialization
	void initMiniBoss1();
	void initMiniBoss2();
};

#endif //__MINI_BOSS_H__