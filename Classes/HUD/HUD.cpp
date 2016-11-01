#include "HUD.h"
#include "Enumeration.h"
#include "SimpleAudioEngine.h"
#include "Toast.h"


HUD * HUD::createHUD()
{
	HUD *hud = new HUD();
	hud->create();
	hud->init();
	hud->autorelease();
	hud->setColor(cocos2d::Color3B::BLACK);
	hud->setAnchorPoint(cocos2d::Vec2(0, 0));
	hud->initComponents();

	return hud;
}


HUD::HUD()
{
}


HUD::~HUD()
{
	CC_SAFE_RELEASE_NULL(this->labelDistance);
	CC_SAFE_RELEASE_NULL(this->labelScore);
	CC_SAFE_RELEASE_NULL(this->labelFuel);
	CC_SAFE_RELEASE_NULL(this->labelWarning);
}


void HUD::draw(cocos2d::Renderer * renderer, const cocos2d::Mat4 & transform, bool transformUpdated)
{
}


void HUD::update(float dt)
{
}


void HUD::updateRemainingDistance(int rDistance)
{
	auto distanceText = cocos2d::__String::createWithFormat("Remaining Distance: %2d", rDistance);
	labelDistance->setString(distanceText->getCString());
}


void HUD::updatePlayerFuel(int rFuel)
{
	auto fuelString = cocos2d::__String::create("");
	for (int i = 0; i < rFuel; i++)
		fuelString->append("I");
	auto fuelText = cocos2d::__String::createWithFormat("Fuel: %s", fuelString->getCString());
	labelFuel->setString(fuelText->getCString());
}


void HUD::updatePlayerScore(unsigned long long score)
{
	auto scoreText = cocos2d::__String::createWithFormat("Score: %llu", score);
	labelScore->setString(scoreText->getCString());
}


void HUD::warning()
{
	auto delay = cocos2d::DelayTime::create(HUD_WARNING_DELAY_TIME);
	auto turnOn = cocos2d::CallFunc::create(this, callfunc_selector(HUD::turnOnWarning));
	auto turnOff = cocos2d::CallFunc::create(this, callfunc_selector(HUD::turnOffWarning));

	this->runAction(
		cocos2d::Sequence::create(
			turnOn->clone(), delay->clone(), turnOff->clone(), delay->clone(), 
			turnOn->clone(), delay->clone(), turnOff->clone(), delay->clone(),
			turnOn->clone(), delay->clone(), turnOff->clone(), delay->clone(),
			NULL
		)
	);
}


void HUD::onGamePause()
{
	pauseSprite->setVisible(true);
	Toast::displayToast(this, "Press BACK button again to resume", 0.0F);
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	cocos2d::Director::getInstance()->pause();
}


void HUD::onGameResume()
{
	pauseSprite->setVisible(false);
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	cocos2d::Director::getInstance()->resume();
}


void HUD::initComponents()
{
	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	///////////////////////////////////////
	// Distance
	auto distanceText = cocos2d::__String::createWithFormat("Remaining distance: %2d", GAME_SCENE_TOTAL_DISTANCE);
	labelDistance = cocos2d::LabelTTF::create(distanceText->getCString(), "fonts/Backslash.ttf", 40);
	labelDistance->setAnchorPoint(cocos2d::Vec2(0, 0));
	labelDistance->setFontFillColor(cocos2d::Color3B::YELLOW);
	labelDistance->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width - labelDistance->getContentSize().width,
			origin.y + visibleSize.height - labelDistance->getContentSize().height
		)
	);

	///////////////////////////////////////
	// Score
	auto scoreText = cocos2d::__String::createWithFormat("Score: %d", 0);
	labelScore = cocos2d::LabelTTF::create(scoreText->getCString(), "fonts/Backslash.ttf", 40);
	labelScore->setAnchorPoint(cocos2d::Vec2(0, 1));
	labelScore->setFontFillColor(cocos2d::Color3B::YELLOW);
	labelScore->setPosition(
		cocos2d::Vec2(
			origin.x,
			origin.y + visibleSize.height
		)
	);

	///////////////////////////////////////
	// Fuel
	cocos2d::__String fuelString = "";
	for (int i = 0; i < PLAYER_MAX_FUEL; i++)
		fuelString.append("I");
	auto fuelText = cocos2d::__String::createWithFormat("Fuel: %s", fuelString.getCString());
	labelFuel = cocos2d::LabelTTF::create(fuelText->getCString(), "fonts/Backslash.ttf", 40);
	labelFuel->setAnchorPoint(cocos2d::Vec2(0, 0));
	labelFuel->setFontFillColor(cocos2d::Color3B::GREEN);
	labelFuel->setPosition(
		cocos2d::Vec2(
			labelDistance->getPositionX(),
			labelDistance->getPositionY() - labelFuel->getContentSize().height
		)
	);

	///////////////////////////////////////
	// Warning
	cocos2d::__String warningText = "W A R N I N G";
	labelWarning = cocos2d::LabelTTF::create(std::string(warningText.getCString()), "fonts/Backslash.ttf", 90);
	labelWarning->setAnchorPoint(cocos2d::Vec2(0.5F, 0.5F));
	labelWarning->setFontFillColor(cocos2d::Color3B::RED);
	labelWarning->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2
		)
	);
	turnOffWarning();

	///////////////////////////////////////
	// Pause game sprite
	pauseSprite = cocos2d::Sprite::create("images/UI/game_paused.png");
	pauseSprite->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2
		)
	);
	pauseSprite->setVisible(false);


	///////////////////////////////////////
	// Add all components to HUD
	this->addChild(labelDistance);
	this->addChild(labelScore);
	this->addChild(labelFuel);
	this->addChild(labelWarning);
	this->addChild(pauseSprite);

	this->scheduleUpdate();
}


void HUD::turnOnWarning()
{
	labelWarning->setVisible(true);
}


void HUD::turnOffWarning()
{
	labelWarning->setVisible(false);
}
