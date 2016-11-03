#include "Enumeration.h"
#include "FinishedScene.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
#include "Toast.h"

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

	playMissionCompletedMusic();

	/////////////////////////////////////////////////
	// load background sprite
	background = cocos2d::Sprite::create("images/backgrounds/game_over_background.png");
	background->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2
		)
	);

	/////////////////////////////////////////////////
	// mission completed logo
	logo = cocos2d::Sprite::create("images/UI/mission_completed.png");
	logo->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + (visibleSize.height / 4) * 3
		)
	);

	/////////////////////////////////////////////////
	// button main menu
	buttonMenu = cocos2d::ui::Button::create("images/buttons/button_main_menu.png");
	buttonMenu->setPosition(
		cocos2d::Vec2(
			origin.x + (visibleSize.width / 4) * 3,
			origin.y + (visibleSize.height / 4)
		)
	);
	buttonMenu->addTouchEventListener(CC_CALLBACK_2(FinishedScene::onButtonMenuTouched, this));

	/////////////////////////////////////////////////
	// button share facebook
	buttonShare = cocos2d::ui::Button::create("images/buttons/button_share_facebook.png");
	buttonShare->setPosition(
		cocos2d::Vec2(
			origin.x + (visibleSize.width / 4),
			origin.y + (visibleSize.height / 4)
		)
	);
	buttonShare->addTouchEventListener(CC_CALLBACK_2(FinishedScene::onButtonShareTouched, this));

	/////////////////////////////////////////////////
	// add all components to finished scene
	this->addChild(background);
	this->addChild(logo);
	this->addChild(buttonMenu);
	this->addChild(buttonShare);
	displayBestScore();
	displayCurrentScore();

	return true;
}


void FinishedScene::playMissionCompletedMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/finished_music.mp3");
}


void FinishedScene::stopAllMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}


void FinishedScene::displayBestScore()
{

}


void FinishedScene::displayCurrentScore()
{

}


void FinishedScene::backToMainMenu()
{
	stopAllMusic();
	auto menu = MainMenuScene::createScene();
	cocos2d::Director::getInstance()->replaceScene(menu);
}


void FinishedScene::shareFacebook()
{
	Toast::displayToast(this, "This function is under construction", 2.0F);
}


void FinishedScene::onButtonMenuTouched(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;

	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;

	case cocos2d::ui::Widget::TouchEventType::ENDED:
		backToMainMenu();
		break;

	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}


void FinishedScene::onButtonShareTouched(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;

	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;

	case cocos2d::ui::Widget::TouchEventType::ENDED:
		shareFacebook();
		break;

	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
