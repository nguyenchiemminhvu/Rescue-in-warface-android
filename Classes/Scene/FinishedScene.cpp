#include "FinishedScene.h"

cocos2d::Scene * FinishedScene::createScene()
{
	auto scene = cocos2d::Scene::create();
	auto layerFinished = FinishedScene::create();
	scene->addChild(layerFinished);
	return scene;
}


bool FinishedScene::init()
{
	if (!cocos2d::Layer::init())
		return false;

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	return true;
}
