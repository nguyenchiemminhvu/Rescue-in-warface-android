#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#pragma once

#include "cocos2d.h"

#include "HUD\HUD.h"
#include "Player\Player.h"
#include "Boss\MiniBoss.h"
#include "Boss\MotherFucker.h"

#include <cstdint>
#include <vector>

class GameScene : public cocos2d::Layer
{
public:
	
	static cocos2d::Scene* createScene();
	virtual bool init();

	virtual void update(float dt);

	cocos2d::Node* getNodeUnderTouch(cocos2d::Touch *touch, cocos2d::Node *target);

	CREATE_FUNC(GameScene);

private:

	cocos2d::PhysicsWorld *world;

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	cocos2d::Sprite *background1;
	cocos2d::Sprite *background2;
	cocos2d::Sprite *platform1;
	cocos2d::Sprite *platform2;

	Player *player;

	cocos2d::Sprite *joyStick;
	cocos2d::Sprite *buttonFire;

	uint64_t countedFrames;
	uint32_t remainingDistance;

	HUD *hud;

	MiniBoss *miniBoss;
	MotherFucker *motherFucker;

	cocos2d::EventListenerPhysicsContact *contactListener;
	cocos2d::EventListenerKeyboard *keyboardListener;
	cocos2d::EventListenerTouchAllAtOnce *touchesListener;

	void setPhysicsWorld(cocos2d::PhysicsWorld *world);

	///////////////////////////////////
	// initialization

	void initGameSceneAttributes();
	void initBoundPhysicsBody();
	void initPlatform();
	void initPlatformBody();
	void initBackground();
	void initWeather();
	void playGameMusic();

	void initHUD();
	void initPlayer();
	void initButtons();

	void initMiniBoss();
	void initMotherFucker();

	void initContactListener();
	void initKeyboardEventListener();
	void initMultiTouchEventListener();

	///////////////////////////////////
	// updating

	void countingFrames();
	void updateBackgroundAndPlatform(float dt);
	void updateRemainingDistance();
	void updatePlayerFuel();
	void updatePlayerScore();

	///////////////////////////////////
	// slots

	void stopGameMusic();
	void resumeGameMusic();

	void resetPlatformPosition();
	void resetBackgroundPosition();

	void pauseGame();
	void replaceFinishedScene();
	void replaceGameOverScene();

	bool onContactBegin(cocos2d::PhysicsContact &contact);
	bool onPlayerBulletContactWithMiniBoss1(cocos2d::PhysicsContact &contact);
	bool onPlayerBulletContactWithMiniBoss2(cocos2d::PhysicsContact &contact);
	bool onPlayerBulletContactWithMotherFucker(cocos2d::PhysicsContact &contact);

	///////////////////////////////////
	// schedule

	void spawnTank(float t);
	void spawnMissile(float t);
	void spawnTower(float t);
	void spawnGascan(float t);

	void playerShooting(float t);

	void spawningObstacle();
	void stopSpawningObstacle();

	void activatedMiniBoss(float t);
	void startMiniBossLoop(float t);
	void activatedMotherFucker(float t);
	void startMotherFuckerLoop(float t);

	//////////////////////////////////
	// others
	void invokeMiniBoss();
	void invokeMotherFucker();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *e);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *e);
};

#endif //__GAME_SCENE_H__