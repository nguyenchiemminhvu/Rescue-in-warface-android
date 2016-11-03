#include "GameOverScene.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "Enumeration.h"
#include "SimpleAudioEngine.h"


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

	playGameOverMusic();

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
	gameOverSprite = cocos2d::Sprite::create("images/UI/mission_failed.png");
	gameOverSprite->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + (visibleSize.height / 4) * 3
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
	displayBestScore();
	displayCurrentScore();

	return true;
}


void GameOverScene::backToMainMenu()
{
	stopAllMusic();
	auto menu = MainMenuScene::createScene();
	cocos2d::Director::getInstance()->replaceScene(menu);
}


void GameOverScene::playAgain()
{
	stopAllMusic();
	auto gameScene = GameScene::createScene();
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_FADE_TIME, gameScene));
}


void GameOverScene::playGameOverMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/dead_music.mp3");
}


void GameOverScene::stopAllMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}


void GameOverScene::displayBestScore()
{
	cocos2d::String *text = cocos2d::String::createWithFormat("Best score: %d", cocos2d::UserDefault::getInstance()->getIntegerForKey("bestScore"));
	cocos2d::ui::Text *bestScoreLabel = cocos2d::ui::Text::create(text->getCString(), "fonts/Schoolbook.ttf", 40);
	bestScoreLabel->setTextColor(cocos2d::Color4B(190, 0, 0, 255));
	bestScoreLabel->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 4,
			origin.y + visibleSize.height / 2
		)
	);

	this->addChild(bestScoreLabel);
}


void GameOverScene::displayCurrentScore()
{
	cocos2d::String *text = cocos2d::String::createWithFormat("Your score: %d", cocos2d::UserDefault::getInstance()->getIntegerForKey("score"));
	cocos2d::ui::Text *scoreLabel = cocos2d::ui::Text::create(text->getCString(), "fonts/Schoolbook.ttf", 40);
	scoreLabel->setTextColor(cocos2d::Color4B(190, 190, 0, 255));
	scoreLabel->setPosition(
		cocos2d::Vec2(
			origin.x + (visibleSize.width / 4) * 3,
			origin.y + visibleSize.height / 2
		)
	);

	this->addChild(scoreLabel);
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
