#include "GameSettings.h"
#include "CommonMath.h"
#include "SimpleAudioEngine.h"

GameSettings* GameSettings::instance = NULL;

GameSettings * GameSettings::getInstance()
{
	if (!instance)
		instance = new GameSettings();
	return instance;
}


void GameSettings::setBackgroundVolume(int percent)
{
	if (percent < 0 || percent > 100)
		return;

	backgroundMusicVolume = __MAX_VOLUMES__ * percent / 100.0F;
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(backgroundMusicVolume);
}


void GameSettings::setMaxBackgroundVolume()
{
	backgroundMusicVolume = __MAX_VOLUMES__;
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(backgroundMusicVolume);
}


void GameSettings::setMinBackgroundVolume()
{
	backgroundMusicVolume = __MIN_VOLUMES__;
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(backgroundMusicVolume);
}


void GameSettings::setEffectVolume(int percent)
{
	if (percent < 0 || percent > 100)
		return;

	effectVolume = __MAX_VOLUMES__ * percent / 100.0F;
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(effectVolume * 1.0F / 100);
}


void GameSettings::setMaxEffectVolume()
{
	effectVolume = __MAX_VOLUMES__;
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(effectVolume);
}


void GameSettings::setMinEffectVolume()
{
	effectVolume = __MIN_VOLUMES__;
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(effectVolume);
}

void GameSettings::doNotShowInstructionAgain()
{
	needToShowInstruction = false;
}


void GameSettings::doNotDeclineInstruction()
{
	needToShowInstruction = true;
}
