#include "SimpleAudioEngine.h"
#include "IntroductionScene.h"
#include "Enumeration.h"
#include "GameSettings.h"
#include "InstructionScene.h"
#include "GameScene.h"
#include "Toast.h"

#include <string>

cocos2d::Scene * IntroductionScene::createScene()
{
	auto scene = cocos2d::Scene::create();
	auto layerIntroduction = IntroductionScene::create();
	scene->addChild(layerIntroduction);
	return scene;
}


bool IntroductionScene::init()
{
	if (!cocos2d::Layer::init())
		return false;

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	initBackground();
	initSoldiers();
	initHelicopter();
	initButtonSkip();
	initActions();

	float totalTime = HELICOPTER_COMING_DURATION + HELICOPTER_LANDING_DURATION + SOLDIER_MOVING_DURATION + HELICOPTER_LEAVING_DURATION;
	this->scheduleOnce(schedule_selector(IntroductionScene::replaceInstructionScene), totalTime);

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/helicopter_sound.mp3");

	return true;
}


void IntroductionScene::initBackground()
{
	/*==================================================*/
	auto platform = cocos2d::Sprite::create("images/backgrounds/game_platform.png");
	platform->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + platform->getContentSize().height / 2
		)
	);
	this->addChild(platform);

	/*==================================================*/
	auto background = cocos2d::Sprite::create("images/backgrounds/introduction_background.png");
	background->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - background->getContentSize().height / 2
		)
	);
	this->addChild(background);
}


void IntroductionScene::initSoldiers()
{
	for (int i = 1; i <= 3; i++)	{

		auto soldier = cocos2d::Sprite::create("images/backgrounds/soldier.png");
		soldier->setPosition(
			cocos2d::Vec2(
				origin.x + soldier->getContentSize().width * i,
				origin.y + PLATFORM_THICKNESS + soldier->getContentSize().height / 2
			)
		);
		this->addChild(soldier);
		soldiers.pushBack(soldier);
	}
}


void IntroductionScene::initHelicopter()
{
	helicopter = cocos2d::Sprite::create("images/player/player0.png");
	helicopter->setFlipX(true);
	helicopter->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width + helicopter->getContentSize().width / 2,
			origin.y + visibleSize.height - helicopter->getContentSize().height
		)
	);

	cocos2d::Vector<cocos2d::SpriteFrame *> frames;
	for (int i = 0; i <= 1; i++)	{

		std::string fileName = cocos2d::String::createWithFormat("images/player/player%d.png", i)->getCString();
		cocos2d::SpriteFrame *frame = cocos2d::SpriteFrame::create(fileName, cocos2d::Rect(0, 0, 170, 50));
		frames.pushBack(frame);
	}

	cocos2d::Animation *animation = cocos2d::Animation::createWithSpriteFrames(frames, 2.0F / 60.0F);
	cocos2d::Animate *animate = cocos2d::Animate::create(animation);

	helicopter->runAction(cocos2d::RepeatForever::create(animate));
	this->addChild(helicopter);
}


void IntroductionScene::initButtonSkip()
{
	auto buttonSkip = cocos2d::ui::Button::create("images/buttons/button_skip.png", "images/button/button_skip.png");
	buttonSkip->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width - buttonSkip->getContentSize().width / 2,
			origin.y + buttonSkip->getContentSize().height / 2
		)
	);
	buttonSkip->addTouchEventListener(CC_CALLBACK_2(IntroductionScene::onButtonSkipTouched, this));
	this->addChild(buttonSkip);
}


void IntroductionScene::initActions()
{
	///////////////////////////////////////////////
	// Helicopter sequence action
	auto waitForSoldiers = cocos2d::DelayTime::create(SOLDIER_MOVING_DURATION);
	auto helicopterSequence = cocos2d::Sequence::create(
		helicopterComing(), 
		waitForSoldiers, 
		helicopterLeaving(), 
		NULL
	);
	helicopter->runAction(helicopterSequence);

	//////////////////////////////////////////////
	// Soldiers sequence action
	for (int i = 0; i < soldiers.size(); i++) {

		auto waitForHelicopter = cocos2d::DelayTime::create(HELICOPTER_COMING_DURATION + HELICOPTER_LANDING_DURATION);
		auto soldierSequence = cocos2d::Sequence::create(
			waitForHelicopter, 
			soldiersMoving(), 
			cocos2d::CallFunc::create(soldiers.at(i), callfunc_selector(cocos2d::Sprite::removeFromParent)), 
			NULL
		);
		soldiers.at(i)->runAction(soldierSequence);
	}
}


cocos2d::Sequence * IntroductionScene::helicopterComing()
{	
	auto moveLeft = cocos2d::MoveBy::create(HELICOPTER_COMING_DURATION, cocos2d::Vec2(-visibleSize.width + helicopter->getContentSize().width, 0));
	auto moveLeftEasing = cocos2d::EaseBackOut::create(moveLeft->clone());

	auto moveDown = cocos2d::MoveBy::create(HELICOPTER_LANDING_DURATION, cocos2d::Vec2(0, -visibleSize.height + PLATFORM_THICKNESS + helicopter->getContentSize().height * 4.0f / 3.0f));

	auto sequence = cocos2d::Sequence::create(
		moveLeftEasing, 
		cocos2d::CallFunc::create(this, callfunc_selector(IntroductionScene::flipHelicopter)), 
		moveDown, 
		NULL
	);
	return sequence;
}


cocos2d::Sequence * IntroductionScene::soldiersMoving()
{
	auto moveRight = cocos2d::MoveBy::create(SOLDIER_MOVING_DURATION, cocos2d::Vec2(helicopter->getContentSize().width, 0));
	auto sequence = cocos2d::Sequence::create(
		moveRight, 
		NULL
	);
	return sequence;
}


cocos2d::Sequence * IntroductionScene::helicopterLeaving()
{
	auto moveUp = cocos2d::MoveBy::create(HELICOPTER_LEAVING_DURATION - 4, cocos2d::Vec2(0, visibleSize.height / 2));

	auto rotate = cocos2d::RotateBy::create(1, 20);
	auto moveRight = cocos2d::MoveBy::create(HELICOPTER_LEAVING_DURATION - 2, cocos2d::Vec2(visibleSize.width, 0));
	auto spawning = cocos2d::Spawn::create(rotate, moveRight, NULL);

	auto sequence = cocos2d::Sequence::create(
		moveUp, 
		spawning, 
		NULL
	);
	return sequence;
}


void IntroductionScene::flipHelicopter()
{
	helicopter->setFlipX(false);
}


void IntroductionScene::onButtonSkipTouched(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		replaceInstructionScene(0);
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}


void IntroductionScene::replaceInstructionScene(float dt)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();

	if (GameSettings::getInstance()->needToShowInstruction) {
		auto instructionScene = InstructionScene::createScene();
		cocos2d::Director::getInstance()->replaceScene(instructionScene);
	}
	else {
		auto gameScene = GameScene::createScene();
		cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_FADE_TIME, gameScene));
	}
}
