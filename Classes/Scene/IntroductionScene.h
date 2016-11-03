#ifndef __INTRODUCTION_SCENE_H__
#define __INTRODUCTION_SCENE_H__

#pragma once

#include "cocos2d.h"
#include "ui\CocosGUI.h"

class IntroductionScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(IntroductionScene);

private:

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	cocos2d::Vector<cocos2d::Sprite *> soldiers;
	cocos2d::Sprite *helicopter;

	void initBackground();
	void initSoldiers();
	void initHelicopter();
	void initButtonSkip();
	void initActions();

	void stopAllMusic();

	////////////////////////////////////////////////
	// actions
	cocos2d::Sequence* helicopterComing();
	cocos2d::Sequence* soldiersMoving();
	cocos2d::Sequence* helicopterLeaving();

	void flipHelicopter();

	void onButtonSkipTouched(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type);
	void replaceInstructionScene(float dt);
};

#endif //__INTRODUCTION_SCENE_H__