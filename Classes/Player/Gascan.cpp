#include "Gascan.h"
#include "Enumeration.h"
#include "PhysicsBodyParser\PhysicsBodyParser.h"


Gascan * Gascan::spawnGascan(cocos2d::Layer * gameScene, float playerPosX)
{
	return new Gascan(gameScene, playerPosX);
}


Gascan::Gascan(cocos2d::Layer * gameScene, float playerPosX)
{
	this->gameScene = gameScene;
	this->playerPosX = playerPosX;
	this->autorelease();

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	initGascan();
	setupGascanAction();
}


Gascan::~Gascan()
{

}


cocos2d::PhysicsBody * Gascan::getGascanPhysicsBody()
{
	return gascanSprite->getPhysicsBody();
}


void Gascan::initGascan()
{
	///////////////////////////////////////////////
	// load gascan sprite

	gascanSprite = cocos2d::Sprite::create("images/player/gascan.png");
	gascanSprite->setPosition(
		cocos2d::Vec2(
			playerPosX,
			origin.y + visibleSize.height + gascanSprite->getContentSize().height / 2
		)
	);

	///////////////////////////////////////////////
	// add gascan physics body

	PhysicsBodyParser::getInstance()->parseJsonFile("PhysicsBodies/PhysicsBodies.json");
	auto gascanBody = PhysicsBodyParser::getInstance()->bodyFormJson(
		gascanSprite,
		"gascan",
		cocos2d::PhysicsMaterial(1, 0, 0)
	);
	gascanBody->setDynamic(false);
	gascanBody->setContactTestBitmask(true);
	gascanBody->setCollisionBitmask((int)CollisionBitmask::GASCAN_COLLISION_BITMASK);
	gascanSprite->setPhysicsBody(gascanBody);

	gameScene->addChild(gascanSprite);
}


void Gascan::setupGascanAction()
{
	auto fallDown = cocos2d::MoveBy::create(GASCAN_FALLING_DURATION, cocos2d::Vec2(0, -(visibleSize.height - PLATFORM_THICKNESS)));
	auto remove = cocos2d::CallFunc::create(gascanSprite, callfunc_selector(cocos2d::Sprite::removeFromParent));
	auto sequence = cocos2d::Sequence::create(
		fallDown,
		remove,
		NULL
	);
	gascanSprite->runAction(sequence);
}