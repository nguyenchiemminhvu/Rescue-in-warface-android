#ifndef __INSTRUCTION_SCENE_H__
#define __INSTRUCTION_SCENE_H__

#pragma once

#include "cocos2d.h"
#include "ui\CocosGUI.h"

class InstructionScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(InstructionScene);

private:

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	void initBackgroundFrame();
	void initDeclineInstruction();
	void initButtonSkip();

	void onButtonSkipTouched(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type);
	void onCheckBoxTouched(cocos2d::Ref *ref, cocos2d::ui::CheckBox::EventType type);
	void replaceGameScene();
};

#endif //__INSTRUCTION_SCENE_H__