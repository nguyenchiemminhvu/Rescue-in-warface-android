#ifndef __GASCAN_H__
#define __GASCAN_H__

#pragma once

#include "cocos2d.h"

class Gascan : public cocos2d::Node
{
public:
	
	static Gascan* spawnGascan(cocos2d::Layer *gameScene);

	Gascan(cocos2d::Layer *gameScene);
	virtual ~Gascan();

private:

	cocos2d::Layer *gameScene;

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	cocos2d::Sprite *gascanSprite;

	void initGascan();
	void setupGascanAction();
};

#endif //__GASCAN_H__