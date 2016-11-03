#ifndef __FINISHED_SCENE_H__
#define __FINISHED_SCENE_H__

#pragma once

#include "cocos2d.h"
#include "ui\CocosGUI.h"

class FinishedScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(FinishedScene);

private:

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	cocos2d::Sprite *background;
	cocos2d::Sprite *logo;
	cocos2d::ui::Button *buttonMenu;
	cocos2d::ui::Button *buttonShare;

	void playMissionCompletedMusic();
	void stopAllMusic();

	//display player score
	void displayBestScore();
	void displayCurrentScore();

	void backToMainMenu();
	void shareFacebook();
	void onButtonMenuTouched(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type);
	void onButtonShareTouched(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type);
};

#endif //__FINISHED_SCENE_H__