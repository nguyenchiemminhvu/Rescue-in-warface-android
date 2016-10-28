#ifndef __TOWER_H__
#define __TOWER_H__

#pragma once

#include "cocos2d.h"

#define TOWER_TOTAL_BULLET 4

class Tower : public cocos2d::Node
{
public:

	static Tower* spawnTower(cocos2d::Layer *gameScene);
	Tower(cocos2d::Layer *gameScene);
	virtual ~Tower();

private:

	cocos2d::Layer *gameScene;

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;
	
	float remainingShiftDistance;
	float nextAttackDuration;

	cocos2d::Sprite *towerSprite;
	cocos2d::Vec2 towerBulletDirection[TOWER_TOTAL_BULLET];

	void initTower();
	void initTowerBulletDirection();
	void setupTowerAction();
	
	cocos2d::Sprite* createBullet();
	cocos2d::Sequence* createBulletAction(int bulletIndex);

	void attack();
	void generateNextAttackDuration();
};

#endif //__TOWER_H__