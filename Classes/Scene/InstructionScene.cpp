#include "InstructionScene.h"
#include "GameScene.h"
#include "GameSettings.h"
#include "Enumeration.h"
#include "Toast.h"

cocos2d::Scene * InstructionScene::createScene()
{
	auto scene = cocos2d::Scene::create();
	auto layerInstruction = InstructionScene::create();
	scene->addChild(layerInstruction);
	return scene;
}

bool InstructionScene::init()
{
	if (!cocos2d::Layer::init())
		return false;

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	initBackgroundFrame();
	initDeclineInstruction();
	initButtonSkip();

	return true;
}


void InstructionScene::initBackgroundFrame()
{
	auto text = cocos2d::ui::Text::create("Instruction", "fonts/Schoolbook.ttf", 50);
	text->setTextColor(cocos2d::Color4B(0, 0, 180, 255));
	text->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - text->getContentSize().height
		)
	);
	this->addChild(text);
}


void InstructionScene::initDeclineInstruction()
{
	auto checkBox = cocos2d::ui::CheckBox::create();
	checkBox->loadTextureBackGround("images/UI/check_box_unchecked.png");
	checkBox->loadTextureFrontCross("images/UI/check_box_cross.png");
	checkBox->loadTextureBackGroundDisabled("images/UI/check_box_selected.png");
	checkBox->setPosition(
		cocos2d::Vec2(
			origin.x + checkBox->getContentSize().width,
			origin.y + checkBox->getContentSize().height
		)
	);
	checkBox->addEventListener(CC_CALLBACK_2(InstructionScene::onCheckBoxTouched, this));
	this->addChild(checkBox);

	auto label = cocos2d::LabelTTF::create("Don't show again!", "fonts/Schoolbook.ttf", 40);
	label->setFontFillColor(cocos2d::Color3B::GRAY);
	label->setPosition(
		cocos2d::Vec2(
			checkBox->getPositionX() + checkBox->getContentSize().width + label->getContentSize().width / 2,
			checkBox->getPositionY()
		)
	);
	this->addChild(label);
}


void InstructionScene::initButtonSkip()
{
	auto buttonSkip = cocos2d::ui::Button::create("images/buttons/button_skip.png", "images/button/button_skip.png");
	buttonSkip->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width - buttonSkip->getContentSize().width / 2,
			origin.y + buttonSkip->getContentSize().height / 2
		)
	);
	buttonSkip->addTouchEventListener(CC_CALLBACK_2(InstructionScene::onButtonSkipTouched, this));
	this->addChild(buttonSkip);
}


void InstructionScene::onButtonSkipTouched(cocos2d::Ref * ref, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		replaceGameScene();
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


void InstructionScene::onCheckBoxTouched(cocos2d::Ref * ref, cocos2d::ui::CheckBox::EventType type)
{
	auto checkBox = dynamic_cast<cocos2d::ui::CheckBox *>(ref);
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		GameSettings::getInstance()->doNotShowInstructionAgain();
		break;

	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		GameSettings::getInstance()->doNotDeclineInstruction();
		break;

	default:
		break;
	}
}


void InstructionScene::replaceGameScene()
{
	auto gameScene = GameScene::createScene();
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_FADE_TIME, gameScene));
}
