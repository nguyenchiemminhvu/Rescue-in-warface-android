#ifndef __FINISHED_SCENE_H__
#define __FINISHED_SCENE_H__

#pragma once

#include "cocos2d.h"

class FinishedScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(FinishedScene);

private:

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;
};

#endif //__FINISHED_SCENE_H__