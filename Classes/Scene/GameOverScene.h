#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#pragma once

#include "cocos2d.h"

class GameOverScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(GameOverScene);

private:

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;
};

#endif //__GAME_OVER_SCENE_H__