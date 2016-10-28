#include "Player.h"
#include "Enumeration.h"
#include "PhysicsBodyParser\PhysicsBodyParser.h"
#include <string>



Player::Player(cocos2d::Layer *gameScene)
{
	this->gameScene = gameScene;

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	this->initPlayerSprite();
	this->initPlayerAnimation();

	movement = 0;
	isUnderControl = true;

	this->scheduleUpdate();
}


Player::~Player()
{
}


void Player::update(float dt)
{
	updatePlayerPosition(dt);
}


void Player::resetPlayerMovement()
{
	movement = 0;
}


void Player::lostControl()
{
	isUnderControl = false;
}


cocos2d::Vec2 Player::getPlayerGunPosition()
{
	return cocos2d::Vec2(
		playerSprite->getPositionX() + playerSprite->getContentSize().width / 2 - 20, 
		playerSprite->getPositionY() + 10
	);
}


void Player::initPlayerSprite()
{
	//////////////////////////////////////////
	// load player sprite
	playerSprite = cocos2d::Sprite::create("images/player/player0.png");
	playerSprite->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2
		)
	);

	/////////////////////////////////////////
	// add player physics body
	PhysicsBodyParser::getInstance()->parseJsonFile("PhysicsBodies/PhysicsBodies.json");
	auto playerBody = PhysicsBodyParser::getInstance()->bodyFormJson(
		playerSprite,
		"player",
		cocos2d::PHYSICSBODY_MATERIAL_DEFAULT
	);
	playerBody->setGravityEnable(false);
	playerBody->setDynamic(false);
	playerBody->setContactTestBitmask(true);
	playerBody->setCollisionBitmask((int)CollisionBitmask::PLAYER_COLLISTION_BITMASK);
	playerSprite->setPhysicsBody(playerBody);

	gameScene->addChild(playerSprite);
}


void Player::initPlayerAnimation()
{
	cocos2d::Vector<cocos2d::SpriteFrame *> frames;
	for (int i = 0; i < __PLAYER_ANIMATION_TOTAL_FRAME__; i++) {

		auto file = cocos2d::String::createWithFormat("images/player/player%d.png", i);
		auto frame = cocos2d::SpriteFrame::create(file->getCString(), cocos2d::Rect(0, 0, 170, 50));
		frames.pushBack(frame);
	}

	cocos2d::Animation *animation = cocos2d::Animation::createWithSpriteFrames(frames, 2.0F / 60.0F);
	cocos2d::Animate *animate = cocos2d::Animate::create(animation);
	
	playerSprite->runAction(cocos2d::RepeatForever::create(animate));
}


void Player::updatePlayerPosition(float dt)
{

}

/*============================================================*/

PlayerBullet * PlayerBullet::createBullet(cocos2d::Layer * gameScene, cocos2d::Vec2 pos)
{
	return new PlayerBullet(gameScene, pos);
}


PlayerBullet::PlayerBullet(cocos2d::Layer * gameScene, cocos2d::Vec2 pos)
{
	this->gameScene = gameScene;
	this->autorelease();
	startPos = pos;

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	initPlayerBullet();
}


PlayerBullet::~PlayerBullet()
{

}


void PlayerBullet::initPlayerBullet()
{
	////////////////////////////////////////////
	// load player bullet sprite
	bulletSprite = cocos2d::Sprite::create("images/player/player_bullet.png");
	bulletSprite->setPosition(
		cocos2d::Vec2(
			
		)
	);

	////////////////////////////////////////////
	// add bullet physics body
	auto bulletBody = cocos2d::PhysicsBody::createBox(bulletSprite->getContentSize());
	bulletBody->setDynamic(true);
	bulletBody->setContactTestBitmask(true);
	bulletBody->setCollisionBitmask((int)CollisionBitmask::PLAYER_BULLET_BITMASK);
	bulletSprite->setPhysicsBody(bulletBody);

	////////////////////////////////////////////
	// setup bullet action
	auto sequence = cocos2d::Sequence::create(
		cocos2d::MoveBy::create(1.0F, cocos2d::Vec2(__PLAYER_BULLET_VELOCITY__, 0)),
		cocos2d::CallFunc::create(bulletSprite, callfunc_selector(cocos2d::Sprite::removeFromParent)),
		NULL
	);
	bulletSprite->runAction(sequence);

	gameScene->addChild(bulletSprite);
}

