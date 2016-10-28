#include "PauseScene.h"

cocos2d::Scene * PauseScene::createScene()
{
	auto scene = cocos2d::Scene::create();
	auto layerPause = PauseScene::create();
	scene->addChild(layerPause);
	return scene;
}


bool PauseScene::init()
{
	if (!cocos2d::Layer::init())
		return false;

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	return true;
}
