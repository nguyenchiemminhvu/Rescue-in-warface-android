#ifndef __PAUSE_SCENE_H__
#define __PAUSE_SCENE_H__

#pragma once

#include "cocos2d.h"

class PauseScene : public cocos2d::Layer
{
public:
	
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(PauseScene);

private:

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

};

#endif //__PAUSE_SCENE_H__