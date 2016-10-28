#ifndef __HUD_H__
#define __HUD_H__

#pragma once

#include "cocos2d.h"

#include <cstdint>

class HUD : public cocos2d::LayerColor
{
public:

	static HUD* createHUD();

	HUD();
	virtual ~HUD();

	virtual void draw(
		cocos2d::Renderer * renderer, 
		const cocos2d::Mat4 & transform, 
		bool transformUpdated
	);

	virtual void update(float dt);

	void updateRemainingDistance(int rDistance);
	void updatePlayerFuel(int rFuel);
	void updatePlayerScore(unsigned long long score);
	void warning();

private:

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	cocos2d::LabelTTF *labelDistance;
	cocos2d::LabelTTF *labelScore;
	cocos2d::LabelTTF *labelFuel;
	cocos2d::LabelTTF *labelWarning;

	void initComponents();
	void turnOnWarning();
	void turnOffWarning();
};

#endif //__HUD_H__