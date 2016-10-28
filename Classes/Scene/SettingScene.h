#ifndef __SETTING_SCENE_H__
#define __SETTING_SCENE_H__

#pragma once

#include "cocos2d.h"
#include "ui\CocosGUI.h"


class SettingScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(SettingScene);

private:

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	cocos2d::ui::Slider *backgroundVolume;
	cocos2d::ui::Slider *effectVolume;

	void initFrame();
	void initSceneTitle();
	void initButtonDone();
	void initBackgroundMusicVolumeSetting();
	void initEffectVolumeSetting();
	void initKeyboardEventListener();

	//////////////////////////////////////////
	// UI event callbacks
	void onEffectVolumeChanged(cocos2d::Ref *ref, cocos2d::ui::Slider::EventType type);
	void onBackgroundMusicVolumeChanged(cocos2d::Ref *ref, cocos2d::ui::Slider::EventType type);

	void onTouchButtonBackgroundVolumeIncrement(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type);
	void onTouchButtonBackgroundVolumeDecrement(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type);
	void onTouchButtonEffectVolumeIncrement(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type);
	void onTouchButtonEffectVolumeDecrement(cocos2d::Ref *ref, cocos2d::ui::Widget::TouchEventType type);

	void onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event);
};

#endif //__SETTING_SCENE_H__