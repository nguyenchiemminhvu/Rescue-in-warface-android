#include "MotherFucker.h"
#include "Enumeration.h"
#include "PhysicsBodyParser\PhysicsBodyParser.h"
#include "SimpleAudioEngine.h"
#include "Toast.h"

#include <cstdint>


MotherFucker::MotherFucker(cocos2d::Layer *gameScene)
{
	started = false;
	this->gameScene = gameScene;

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	initMotherFucker();
	initBulletDirection();

	verticalFlightDistance = ((visibleSize.height - PLATFORM_THICKNESS) - bossSprite->getContentSize().height) / 2;
}


MotherFucker::~MotherFucker()
{

}


bool MotherFucker::isStarted()
{
	return started;
}


void MotherFucker::setStarted(bool started)
{
	this->started = started;
}


int MotherFucker::getHealth()
{
	return health;
}


void MotherFucker::decreaseHealth()
{
	if (health > 0)
		health--;
	else
		onBossDead();
}


void MotherFucker::onBossDead()
{
	health = INT_MAX;
	dead = true;
	bossSprite->getPhysicsBody()->removeFromWorld();
	bossSprite->removeFromParentAndCleanup(true);
}


bool MotherFucker::isBossDead()
{
	return dead;
}


void MotherFucker::start()
{
	setStarted(true);
	preparing();
}


void MotherFucker::preparing()
{
	auto moveDown = cocos2d::MoveBy::create(
		BIG_BOSS_PREPARE_DURATION, 
		cocos2d::Vec2(
			0,
			(origin.y + (visibleSize.height + PLATFORM_THICKNESS) / 2) - bossSprite->getPositionY()
		)
	);
	bossSprite->runAction(moveDown);
}


void MotherFucker::setupLoopAction()
{
	auto attack = CallFunc::create(this, callfunc_selector(MotherFucker::attack));
	auto loop = cocos2d::RepeatForever::create(
		cocos2d::Sequence::create(
			moveDown(), attack, moveDown(), attack,
			moveUp(), attack, moveUp(), attack,
			moveUp(), attack, moveUp(), attack,
			moveDown(), attack, moveDown(), attack,
			NULL
		)
	);
	bossSprite->runAction(loop);
}


cocos2d::MoveBy * MotherFucker::moveUp()
{
	auto moveUp = cocos2d::MoveBy::create(1.0F, cocos2d::Vec2(0, verticalFlightDistance / 2));
	return moveUp;
}


cocos2d::MoveBy * MotherFucker::moveDown()
{
	auto moveDown = cocos2d::MoveBy::create(1.0F, cocos2d::Vec2(0, -verticalFlightDistance / 2));
	return moveDown;
}


void MotherFucker::attack()
{
	createBullets(
		cocos2d::Vec2(
			bossSprite->getPositionX() - bossSprite->getContentSize().width / 2, 
			bossSprite->getPositionY()
		)
	);
}


void MotherFucker::initMotherFucker()
{
	////////////////////////////////////////////
	// load boss sprite
	bossSprite = cocos2d::Sprite::create("images/bosses/big_boss.png");
	bossSprite->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width - bossSprite->getContentSize().width / 2,
			origin.y + visibleSize.height + bossSprite->getContentSize().height * 2
		)
	);

	health = __MOTHER_FUCKER_MAX_HEALTH__;
	flip = false;
	dead = false;
	bossSprite->setFlippedX(flip);

	///////////////////////////////////////////
	// add boss physics body
	PhysicsBodyParser::getInstance()->parseJsonFile("PhysicsBodies/PhysicsBodies.json");
	auto bossBody = PhysicsBodyParser::getInstance()->bodyFormJson(
		bossSprite,
		"mother_fucker",
		cocos2d::PHYSICSBODY_MATERIAL_DEFAULT
	);
	bossBody->setDynamic(false);
	bossBody->setContactTestBitmask(true);
	bossBody->setCollisionBitmask((int)CollisionBitmask::BOSS_COLLISION_BITMASK);
	bossSprite->setPhysicsBody(bossBody);

	gameScene->addChild(bossSprite);
}


void MotherFucker::flipMotherFucker()
{
	bossSprite->setFlippedX(flip);
}


void MotherFucker::initBulletDirection()
{
	bulletDirection[0].x = -visibleSize.width;
	bulletDirection[0].y = 0;

	bulletDirection[1].x = -visibleSize.width;
	bulletDirection[1].y = visibleSize.width * sin(160.0f);

	bulletDirection[2].x = -visibleSize.width;
	bulletDirection[2].y = -visibleSize.width * sin(160.0f);
}


void MotherFucker::createBullets(cocos2d::Vec2 position)
{
	cocos2d::Sprite *bullets[__MOTHER_FUCKER_TOTAL_BULLET__];

	for (int i = 0; i < 3; i++) {
		bullets[i] = cocos2d::Sprite::create("images/enemy/enemy_bullet.png");
		bullets[i]->setPosition(position);

		auto bulletBody = cocos2d::PhysicsBody::createCircle(bullets[i]->getContentSize().width / 2);
		bulletBody->setContactTestBitmask(true);
		bulletBody->setCollisionBitmask((int)CollisionBitmask::ENEMY_BULLET_BISMASK);
		bulletBody->setDynamic(false);
		bullets[i]->setPhysicsBody(bulletBody);

		bullets[i]->runAction(
			cocos2d::Sequence::create(
				cocos2d::MoveBy::create(1.5F, bulletDirection[i]),
				cocos2d::CallFunc::create(bullets[i], callfunc_selector(cocos2d::Sprite::removeFromParent)),
				NULL
			)
		);

		gameScene->addChild(bullets[i]);
	}
}
