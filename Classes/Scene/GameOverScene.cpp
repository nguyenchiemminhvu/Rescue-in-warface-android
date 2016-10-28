#include "GameOverScene.h"

cocos2d::Scene * GameOverScene::createScene()
{
	auto scene = cocos2d::Scene::create();
	auto layerGameOver = GameOverScene::create();
	scene->addChild(layerGameOver);
	return scene;
}


bool GameOverScene::init()
{
	if (!cocos2d::Layer::init())
		return false;

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	return true;
}
