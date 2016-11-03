#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#pragma once

#include "cocos2d.h"
#include "ui\CocosGUI.h"

class GameOverScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(GameOverScene);

	void backToMainMenu();
	void playAgain();

private:

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	cocos2d::Sprite *background;
	cocos2d::Sprite *gameOverSprite;
	cocos2d::ui::Button *buttonPlayAgain;
	cocos2d::ui::Button *buttonMainMenu;

	void playGameOverMusic();
	void stopAllMusic();

	//display player score
	void displayBestScore();
	void displayCurrentScore();

	//button touch event
	void onButtonPlayAgainTouched(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type);
	void onButtonMainMenuTouched(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type);
};

#endif //__GAME_OVER_SCENE_H__