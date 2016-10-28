#include "HelloWorldScene.h"
#include "PhysicsBodyParser\PhysicsBodyParser.h"

cocos2d::Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = cocos2d::Scene::createWithPhysics();
	
	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();
	scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(cocos2d::Vec2(0, -100));
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}
	
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	playerPosition = cocos2d::Vec2(
		origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2
	);

	//add physics body to scene
	this->setPhysicsBody(cocos2d::PhysicsBody::createEdgeBox(this->getContentSize(), cocos2d::PHYSICSBODY_MATERIAL_DEFAULT, 5.0f));
	this->getPhysicsBody()->setDynamic(false);

	// my initialization
	spriteIndex = 0;

	player = cocos2d::Sprite::create("images/player/player0.png");
	player->setPosition(playerPosition);
	this->addChild(player);

	cocos2d::Vector<cocos2d::SpriteFrame*> aniFrames;
	aniFrames.reserve(2);

	aniFrames.pushBack(cocos2d::SpriteFrame::create("images/player/player0.png", cocos2d::Rect(0, 0, 170, 50)));
	aniFrames.pushBack(cocos2d::SpriteFrame::create("images/player/player1.png", cocos2d::Rect(0, 0, 170, 50)));
	
	cocos2d::Animation *animation = cocos2d::Animation::createWithSpriteFrames(aniFrames, (2.0f / 60.0f));
	cocos2d::Animate *animate = cocos2d::Animate::create(animation);

	player->runAction(cocos2d::RepeatForever::create(animate));

	this->scheduleUpdate();

	return true;
}


void HelloWorld::update(float dt)
{
	
}

