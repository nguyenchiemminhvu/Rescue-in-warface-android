#include "SettingScene.h"
#include "GameSettings.h"
#include "CommonMath.h"
#include "SimpleAudioEngine.h"
#include "Toast.h"

cocos2d::Scene * SettingScene::createScene()
{
	auto scene = cocos2d::Scene::create();
	auto layerOption = SettingScene::create();
	scene->addChild(layerOption);
	return scene;
}


bool SettingScene::init()
{
	if (!cocos2d::Layer::init()) {
		return false;
	}

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	initFrame();
	initSceneTitle();
	initButtonDone();
	initBackgroundMusicVolumeSetting();
	initEffectVolumeSetting();
	initKeyboardEventListener();

	return true;
}


void SettingScene::initFrame()
{
	auto frame = cocos2d::Sprite::create("images/UI/setting_scene_frame.png");
	frame->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2
		)
	);
	this->addChild(frame);
}


void SettingScene::initSceneTitle()
{
	auto title = cocos2d::Sprite::create("images/UI/setting_scene_title.png");
	title->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - title->getContentSize().height
		)
	);
	this->addChild(title);
}


void SettingScene::initButtonDone()
{
	auto buttonDone = cocos2d::Sprite::create("images/buttons/button_done.png");
	buttonDone->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width - buttonDone->getContentSize().width / 2 - 50,
			origin.y + buttonDone->getContentSize().height / 2 + 50
		)
	);

	this->addChild(buttonDone);

	auto touchButtonBackListener = cocos2d::EventListenerTouchOneByOne::create();
	touchButtonBackListener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event) -> bool {

		cocos2d::Point touchPosition = cocos2d::Director::getInstance()->convertToGL(touch->getLocationInView());
		cocos2d::Point convertButtonBackPos = buttonDone->convertToNodeSpace(touchPosition);
		cocos2d::Rect buttonBackRect(buttonDone->getOffsetPosition(), buttonDone->getContentSize());

		if (buttonBackRect.containsPoint(convertButtonBackPos)) {
			cocos2d::Director::getInstance()->popScene();
		}

		return true;
	};

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchButtonBackListener, this);
}


void SettingScene::initBackgroundMusicVolumeSetting()
{
	/*=========================================================*/
	backgroundVolume = cocos2d::ui::Slider::create();
	backgroundVolume->loadBarTexture("images/UI/slider_bar.png");
	backgroundVolume->loadSlidBallTextures("images/UI/slider_bar_ball.png", "images/UI/slider_bar_ball.png");
	backgroundVolume->loadProgressBarTexture("images/UI/slider_progress_bar.png");
	backgroundVolume->setPercent((GameSettings::getInstance()->backgroundMusicVolume / __MAX_VOLUMES__) * 100);
	backgroundVolume->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2 + backgroundVolume->getContentSize().height * 2.0F
		)
	);
	backgroundVolume->addEventListener(CC_CALLBACK_2(SettingScene::onBackgroundMusicVolumeChanged, this));
	this->addChild(backgroundVolume);

	/*=========================================================*/
	auto backgroundVolumeLabel = cocos2d::LabelTTF::create("Backgrounds Volume", "fonts/Schoolbook.ttf", 30);
	backgroundVolumeLabel->setColor(cocos2d::Color3B::GRAY);
	backgroundVolumeLabel->setPosition(
		cocos2d::Vec2(
			backgroundVolume->getPositionX(),
			backgroundVolume->getPositionY() + backgroundVolumeLabel->getContentSize().height
		)
	);
	this->addChild(backgroundVolumeLabel);

	/*=========================================================*/
	auto buttonIncrement = cocos2d::ui::Button::create();
	buttonIncrement->loadTextureNormal("images/buttons/button_increment.png");
	buttonIncrement->loadTexturePressed("images/buttons/button_increment.png");
	buttonIncrement->setPosition(
		cocos2d::Vec2(
			backgroundVolume->getPositionX() + backgroundVolume->getContentSize().width / 2 + buttonIncrement->getContentSize().width,
			backgroundVolume->getPositionY()
		)
	);
	buttonIncrement->addTouchEventListener(CC_CALLBACK_2(SettingScene::onTouchButtonBackgroundVolumeIncrement, this));
	this->addChild(buttonIncrement);

	/*=========================================================*/
	auto buttonDecrement = cocos2d::ui::Button::create();
	buttonDecrement->loadTextureNormal("images/buttons/button_decrement.png");
	buttonDecrement->loadTexturePressed("images/buttons/button_decrement.png");
	buttonDecrement->setPosition(
		cocos2d::Vec2(
			backgroundVolume->getPositionX() - backgroundVolume->getContentSize().width / 2 - buttonDecrement->getContentSize().width,
			backgroundVolume->getPositionY()
		)
	);
	buttonDecrement->addTouchEventListener(CC_CALLBACK_2(SettingScene::onTouchButtonBackgroundVolumeDecrement, this));
	this->addChild(buttonDecrement);
}


void SettingScene::initEffectVolumeSetting()
{
	/*=========================================================*/
	effectVolume = cocos2d::ui::Slider::create();
	effectVolume->loadBarTexture("images/UI/slider_bar.png");
	effectVolume->loadSlidBallTextures("images/UI/slider_bar_ball.png", "images/UI/slider_bar_ball.png");
	effectVolume->loadProgressBarTexture("images/UI/slider_progress_bar.png");
	effectVolume->setPercent((GameSettings::getInstance()->effectVolume / __MAX_VOLUMES__) * 100);
	effectVolume->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2 - effectVolume->getContentSize().height * 3.0F
		)
	);
	effectVolume->addEventListener(CC_CALLBACK_2(SettingScene::onEffectVolumeChanged, this));
	this->addChild(effectVolume);

	/*=========================================================*/
	auto effectVolumeLabel = cocos2d::LabelTTF::create("Effects Volume", "fonts/Schoolbook.ttf", 30);
	effectVolumeLabel->setColor(cocos2d::Color3B::GRAY);
	effectVolumeLabel->setPosition(
		cocos2d::Vec2(
			effectVolume->getPositionX(),
			effectVolume->getPositionY() + effectVolumeLabel->getContentSize().height
		)
	);
	this->addChild(effectVolumeLabel);

	/*=========================================================*/
	auto buttonIncrement = cocos2d::ui::Button::create();
	buttonIncrement->loadTextureNormal("images/buttons/button_increment.png");
	buttonIncrement->loadTexturePressed("images/buttons/button_increment.png");
	buttonIncrement->setPosition(
		cocos2d::Vec2(
			effectVolume->getPositionX() + effectVolume->getContentSize().width / 2 + buttonIncrement->getContentSize().width,
			effectVolume->getPositionY()
		)
	);
	buttonIncrement->addTouchEventListener(CC_CALLBACK_2(SettingScene::onTouchButtonEffectVolumeIncrement, this));
	this->addChild(buttonIncrement);

	/*=========================================================*/
	auto buttonDecrement = cocos2d::ui::Button::create();
	buttonDecrement->loadTextureNormal("images/buttons/button_decrement.png");
	buttonDecrement->loadTexturePressed("images/buttons/button_decrement.png");
	buttonDecrement->setPosition(
		cocos2d::Vec2(
			effectVolume->getPositionX() - effectVolume->getContentSize().width / 2 - buttonDecrement->getContentSize().width,
			effectVolume->getPositionY()
		)
	);
	buttonDecrement->addTouchEventListener(CC_CALLBACK_2(SettingScene::onTouchButtonEffectVolumeDecrement, this));
	this->addChild(buttonDecrement);
}


void SettingScene::initKeyboardEventListener()
{
	auto keyboardListener = cocos2d::EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(SettingScene::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}


void SettingScene::onEffectVolumeChanged(cocos2d::Ref * ref, cocos2d::ui::Slider::EventType type)
{
	cocos2d::ui::Slider *effectVolume = dynamic_cast<cocos2d::ui::Slider *>(ref);
	
	switch (type)
	{
	case cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
		GameSettings::getInstance()->setEffectVolume(effectVolume->getPercent());
		break;
	case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_DOWN:
		break;
	case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_UP:
		break;
	case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_CANCEL:
		break;
	default:
		break;
	}
}

void SettingScene::onBackgroundMusicVolumeChanged(cocos2d::Ref * ref, cocos2d::ui::Slider::EventType type)
{
	cocos2d::ui::Slider *backgroundVolume = dynamic_cast<cocos2d::ui::Slider *>(ref);

	switch (type)
	{
	case cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
		GameSettings::getInstance()->setBackgroundVolume(backgroundVolume->getPercent());
		break;
	case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_DOWN:
		break;
	case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_UP:
		break;
	case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_CANCEL:
		break;
	default:
		break;
	}
}


void SettingScene::onTouchButtonBackgroundVolumeIncrement(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		backgroundVolume->setPercent(backgroundVolume->getPercent() + 5);
		GameSettings::getInstance()->setBackgroundVolume(backgroundVolume->getPercent());
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


void SettingScene::onTouchButtonBackgroundVolumeDecrement(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		backgroundVolume->setPercent(backgroundVolume->getPercent() - 5);
		GameSettings::getInstance()->setBackgroundVolume(backgroundVolume->getPercent());
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


void SettingScene::onTouchButtonEffectVolumeIncrement(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		effectVolume->setPercent(effectVolume->getPercent() + 5);
		GameSettings::getInstance()->setEffectVolume(effectVolume->getPercent());
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


void SettingScene::onTouchButtonEffectVolumeDecrement(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		effectVolume->setPercent(effectVolume->getPercent() - 5);
		GameSettings::getInstance()->setEffectVolume(effectVolume->getPercent());
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


void SettingScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event * event)
{
	if (key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
		cocos2d::Director::getInstance()->popScene();
	}
}
