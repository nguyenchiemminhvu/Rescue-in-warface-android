#ifndef __GAME_SETTINGS_H__
#define __GAME_SETTINGS_H__

#pragma once

#ifdef __MAX_VOLUMES__
	#undef __MAX_VOLUMES__
#endif // __MAX_VOLUMES__

#define __MAX_VOLUMES__ 1.0f

#ifdef __MIN_VOLUMES__
	#undef __MIN_VOLUMES__
#endif // __MIN_VOLUMES__

#define __MIN_VOLUMES__ 0.0f

#include "cocos2d.h"

class GameSettings : public cocos2d::Object {

public:
	
	static GameSettings* getInstance();

	///////////////////////////////////////////////
	// Setting variables
	float backgroundMusicVolume;
	float effectVolume;
	bool needToShowInstruction;

	///////////////////////////////////////////////
	// Setting methods
	void setBackgroundVolume(int percent);
	void setMaxBackgroundVolume();
	void setMinBackgroundVolume();

	void setEffectVolume(int percent);
	void setMaxEffectVolume();
	void setMinEffectVolume();

	void doNotShowInstructionAgain();
	void doNotDeclineInstruction();

private:

	GameSettings() {
		backgroundMusicVolume = 1.0f;
		effectVolume = 1.0f;
		needToShowInstruction = true;
	}

	GameSettings(const GameSettings &) = delete;
	GameSettings& operator= (const GameSettings &) = delete;

	static GameSettings *instance;
};

#endif //__GAME_SETTINGS_H__
