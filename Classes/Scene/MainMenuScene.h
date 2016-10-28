#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#pragma once

#include "cocos2d.h"

class MainMenuScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(MainMenuScene);

	virtual void update(float dt);

private:

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	int keyEscapeCounter;
	float keyEscapteTimer;

	cocos2d::Sprite *background;

	cocos2d::Vec2 titlePosition;
	cocos2d::Sprite *titleSprite;

	void initBackground();
	void initGameTitle();
	void initMenu();
	void initKeyboardEventListener();

	void onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event);

	void resetKeyEscapeCounter();
};

#endif //__MAIN_MENU_SCENE_H__