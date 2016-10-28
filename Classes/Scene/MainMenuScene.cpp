#include "MainMenuScene.h"
#include "SettingScene.h"
#include "IntroductionScene.h"
#include "Toast.h"


cocos2d::Scene * MainMenuScene::createScene()
{
	auto scene = cocos2d::Scene::create();
	auto layerMenu = MainMenuScene::create();
	scene->addChild(layerMenu);
	return scene;
}


bool MainMenuScene::init()
{
	if (!cocos2d::Layer::init())	{
		return false;
	}

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	keyEscapeCounter = 0;
	keyEscapteTimer = 0.0F;

	initBackground();
	initGameTitle();
	initMenu();
	initKeyboardEventListener();

	this->scheduleUpdate();

	return true;
}


void MainMenuScene::update(float dt)
{
	if (keyEscapteTimer <= 0) 
	{
		resetKeyEscapeCounter();
	}
	else
	{
		keyEscapteTimer -= dt;
	}
}


void MainMenuScene::initBackground()
{
	background = cocos2d::Sprite::create("images/backgrounds/main_menu_background.png");
	background->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2
		)
	);

	this->addChild(background);
}


void MainMenuScene::initGameTitle()
{
	cocos2d::Vector<cocos2d::SpriteFrame*> titleFrames;
	cocos2d::Animation *titleAnimation;
	cocos2d::Animate *titleAnimate;

	titlePosition = cocos2d::Vec2(
						origin.x + visibleSize.width / 2,
						origin.y + visibleSize.height - visibleSize.height / 4
					);

	titleSprite = cocos2d::Sprite::create("images/logo/logo_game0.png");
	titleSprite->setPosition(titlePosition);
	this->addChild(titleSprite);

	for (int i = 0; i <= 4; i++) {

		std::string fileName = cocos2d::StringUtils::format("images/logo/logo_game%d.png", i);
		titleFrames.pushBack(cocos2d::SpriteFrame::create(fileName, cocos2d::Rect(0, 0, 653, 94)));
	}

	titleAnimation = cocos2d::Animation::createWithSpriteFrames(titleFrames, 5.0F / 60.0F);
	titleAnimate = cocos2d::Animate::create(titleAnimation);

	titleSprite->runAction(cocos2d::RepeatForever::create(titleAnimate));
}


void MainMenuScene::initMenu()
{
	//////////////////////////////////////////////////////
	// Create menu buttons

	auto buttonStart = cocos2d::Sprite::create("images/buttons/button_play.png");
	buttonStart->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2
		)
	);

	auto buttonSetting = cocos2d::Sprite::create("images/buttons/button_setting.png");
	buttonSetting->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2 - buttonStart->getContentSize().height
		)
	);

	auto buttonQuit = cocos2d::Sprite::create("images/buttons/button_quit.png");
	buttonQuit->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2 - buttonStart->getContentSize().height * 2.0F
		)
	);

	this->addChild(buttonStart);
	this->addChild(buttonSetting);
	this->addChild(buttonQuit);

	/////////////////////////////////////////////////////
	// Add touch listener to buttons
	auto touchOneByOneListener = cocos2d::EventListenerTouchOneByOne::create();
	touchOneByOneListener->setSwallowTouches(false);
	touchOneByOneListener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event) -> bool {
		
		cocos2d::Point touchPosition = cocos2d::Director::getInstance()->convertToGL(touch->getLocationInView());
		
		cocos2d::Point convertButtonStartPos = buttonStart->convertToNodeSpace(touchPosition);
		cocos2d::Point convertButtonOptionPos = buttonSetting->convertToNodeSpace(touchPosition);
		cocos2d::Point convertButtonQuitPos = buttonQuit->convertToNodeSpace(touchPosition);

		cocos2d::Rect buttonStartRect(buttonStart->getOffsetPosition(), buttonStart->getContentSize());
		cocos2d::Rect buttonOptionRect(buttonSetting->getOffsetPosition(), buttonSetting->getContentSize());
		cocos2d::Rect buttonQuitRect(buttonQuit->getOffsetPosition(), buttonQuit->getContentSize());

		if (buttonStartRect.containsPoint(convertButtonStartPos)) {
			auto introductionScene = IntroductionScene::createScene();
			cocos2d::Director::getInstance()->replaceScene(introductionScene);
		}
		else if (buttonOptionRect.containsPoint(convertButtonOptionPos)) {
			auto settingScene = SettingScene::createScene();
			cocos2d::Director::getInstance()->pushScene(settingScene);
		}
		else if (buttonQuitRect.containsPoint(convertButtonQuitPos)) {
			cocos2d::Director::getInstance()->end();
		}

		return true;
	};


	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchOneByOneListener, this);
}


void MainMenuScene::initKeyboardEventListener()
{
	auto keyboardListener = cocos2d::EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(MainMenuScene::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}


void MainMenuScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event * event)
{
	if (key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
		keyEscapeCounter++;
		keyEscapteTimer = 2.0F;

		if (keyEscapeCounter >= 2) {
			cocos2d::Director::getInstance()->end();
		}
		Toast::displayToast(this, "Press BACK again to exit", 2.0F);
	}
}


void MainMenuScene::resetKeyEscapeCounter()
{
	keyEscapeCounter = 0;
	keyEscapteTimer = 0.0F;
}
