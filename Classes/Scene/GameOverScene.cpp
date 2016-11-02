#include "GameOverScene.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "Enumeration.h"


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

	//////////////////////////////////////////////
	// game over background
	background = cocos2d::Sprite::create("images/backgrounds/game_over_background.png");
	background->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2
		)
	);

	//////////////////////////////////////////////
	// load game over sprite
	gameOverSprite = cocos2d::Sprite::create("images/UI/game_over.png");
	gameOverSprite->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2
		)
	);

	//////////////////////////////////////////////
	// button play again
	buttonPlayAgain = cocos2d::ui::Button::create("images/buttons/button_play_again.png");
	buttonPlayAgain->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 4,
			origin.y + visibleSize.height / 4
		)
	);
	buttonPlayAgain->addTouchEventListener(CC_CALLBACK_2(GameOverScene::onButtonPlayAgainTouched, this));

	//////////////////////////////////////////////
	// button main menu
	buttonMainMenu = cocos2d::ui::Button::create("images/buttons/button_main_menu.png");
	buttonMainMenu->setPosition(
		cocos2d::Vec2(
			origin.x + (visibleSize.width / 4) * 3,
			origin.y + visibleSize.height / 4
		)
	);
	buttonMainMenu->addTouchEventListener(CC_CALLBACK_2(GameOverScene::onButtonMainMenuTouched, this));

	//////////////////////////////////////////////
	// add all components to game over layer
	this->addChild(background);
	this->addChild(gameOverSprite);
	this->addChild(buttonMainMenu);
	this->addChild(buttonPlayAgain);

	return true;
}


void GameOverScene::backToMainMenu()
{
	auto menu = MainMenuScene::createScene();
	cocos2d::Director::getInstance()->replaceScene(menu);
}


void GameOverScene::playAgain()
{
	auto gameScene = GameScene::createScene();
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_FADE_TIME, gameScene));
}


void GameOverScene::displayBestScore()
{

}


void GameOverScene::displayCurrentScore()
{

}


void GameOverScene::onButtonPlayAgainTouched(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;

	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;

	case cocos2d::ui::Widget::TouchEventType::ENDED:
		playAgain();
		break;

	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}


void GameOverScene::onButtonMainMenuTouched(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType type)
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
