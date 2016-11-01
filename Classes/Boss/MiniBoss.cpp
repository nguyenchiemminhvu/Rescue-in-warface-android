#include "MiniBoss.h"
#include "Enumeration.h"
#include "PhysicsBodyParser\PhysicsBodyParser.h"

#include <cstdint>

#define MINI_BOSS_MOVE_UP_OR_DOWN_DURATION 2.0F
#define MINI_BOSS_MOVE_LEFT_OR_RIGHT_DURATION 3.0F
#define MINI_BOSS_HEIGHT 120

MiniBoss::MiniBoss(cocos2d::Layer *gameScene)
{
	started = false;
	this->gameScene = gameScene;

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	initMiniBoss1();
	initMiniBoss2();

	verticalFlightDistance = ((visibleSize.height - PLATFORM_THICKNESS) - bossSprite1->getContentSize().height) / 2;
	horizontalFlightDistance = visibleSize.width - bossSprite1->getContentSize().width;
}


MiniBoss::~MiniBoss()
{
	
}


void MiniBoss::start()
{
	this->started = true;
	bossSprite1->runAction(preparation1());
	bossSprite2->runAction(preparation2());
}


void MiniBoss::setupLoopAction()
{
	auto loopAction1 = cocos2d::RepeatForever::create(
		cocos2d::Sequence::create(
			action1_1(),
			action1_2(),
			NULL
		)
	);
	auto loopAction2 = cocos2d::RepeatForever::create(
		cocos2d::Sequence::create(
			action2_1(),
			action2_2(),
			NULL
		)
	);

	bossSprite1->runAction(loopAction1);
	bossSprite2->runAction(loopAction2);
}


bool MiniBoss::isStarted()
{
	return started;
}


void MiniBoss::setStarted(bool started)
{
	this->started = started;
}


cocos2d::PhysicsBody * MiniBoss::getBossBody1()
{
	return bossSprite1->getPhysicsBody();
}


cocos2d::PhysicsBody * MiniBoss::getBossBody2()
{
	return bossSprite2->getPhysicsBody();
}


int MiniBoss::getHealth1()
{
	return this->health1;
}


int MiniBoss::getHealth2()
{
	return this->health2;
}


void MiniBoss::decreaseHealth1()
{
	if (health1 > 0) {
		health1--;
	}
	else {
		onBossDead1();
	}
}


void MiniBoss::decreaseHealth2()
{
	if (health2 > 0) {
		health2--;
	}
	else {
		onBossDead2();
	}
}


void MiniBoss::onBossDead1()
{
	health1 = INT_MAX;
	dead1 = true;
	bossSprite1->getPhysicsBody()->removeFromWorld();
	bossSprite1->removeFromParentAndCleanup(true);
}


void MiniBoss::onBossDead2()
{
	health2 = INT_MAX;
	dead2 = true;
	bossSprite2->getPhysicsBody()->removeFromWorld();
	bossSprite2->removeFromParentAndCleanup(true);
}


bool MiniBoss::isBossDead1()
{
	return dead1;
}


bool MiniBoss::isBossDead2()
{
	return dead2;
}


cocos2d::Sequence * MiniBoss::preparation1()
{
	auto preparation = cocos2d::MoveBy::create(
		MINI_BOSS_PREPARE_DURATION, 
		cocos2d::Vec2(
			0,
			(origin.y + (visibleSize.height + PLATFORM_THICKNESS) / 2) - (origin.y + visibleSize.height + MINI_BOSS_HEIGHT)
		)
	);
	auto sequence = cocos2d::Sequence::create(preparation, NULL);
	return sequence;
}


cocos2d::Sequence * MiniBoss::preparation2()
{
	auto preparation = cocos2d::MoveBy::create(
		MINI_BOSS_PREPARE_DURATION,
		cocos2d::Vec2(
			0,
			(origin.y + (visibleSize.height + PLATFORM_THICKNESS) / 2) - (origin.y + visibleSize.height + MINI_BOSS_HEIGHT)
		)
	);
	auto sequence = cocos2d::Sequence::create(preparation, NULL);
	return sequence;
}


cocos2d::Sequence * MiniBoss::action1_1()
{
	auto flip = cocos2d::CallFunc::create(this, callfunc_selector(MiniBoss::flipBoss1));
	auto attack = cocos2d::CallFunc::create(this, callfunc_selector(MiniBoss::attack1));

	auto sequence = cocos2d::Sequence::create(
		moveUp(), attack, moveUp(), attack, moveUp(),
		moveRight(), flip,
		moveDown(), attack, moveDown(), attack, moveDown(),
		NULL
	);
	return sequence;
}


cocos2d::Sequence * MiniBoss::action1_2()
{
	auto flip = cocos2d::CallFunc::create(this, callfunc_selector(MiniBoss::flipBoss1));
	auto attack = cocos2d::CallFunc::create(this, callfunc_selector(MiniBoss::attack1));

	auto sequence = cocos2d::Sequence::create(
		moveDown(), attack, moveDown(), attack, moveDown(),
		moveLeft(), flip,
		moveUp(), attack, moveUp(), attack, moveUp(),
		NULL
	);
	return sequence;
}


cocos2d::Sequence * MiniBoss::action2_1()
{
	auto flip = cocos2d::CallFunc::create(this, callfunc_selector(MiniBoss::flipBoss2));
	auto attack = cocos2d::CallFunc::create(this, callfunc_selector(MiniBoss::attack2));

	auto sequence = cocos2d::Sequence::create(
		moveDown(), attack, moveDown(), attack, moveDown(),
		moveLeft(), flip,
		moveUp(), attack, moveUp(), attack, moveUp(),
		NULL
	);
	return sequence;
}


cocos2d::Sequence * MiniBoss::action2_2()
{
	auto flip = cocos2d::CallFunc::create(this, callfunc_selector(MiniBoss::flipBoss2));
	auto attack = cocos2d::CallFunc::create(this, callfunc_selector(MiniBoss::attack2));

	auto sequence = cocos2d::Sequence::create(
		moveUp(), attack, moveUp(), attack, moveUp(),
		moveRight(), flip,
		moveDown(), attack, moveDown(), attack, moveDown(),
		NULL
	);
	return sequence;
}


void MiniBoss::flipBoss1()
{
	flip1 = !flip1;
	bossSprite1->setFlippedX(flip1);
}


void MiniBoss::flipBoss2()
{
	flip2 = !flip2;
	bossSprite2->setFlippedX(flip2);
}


cocos2d::MoveBy * MiniBoss::moveUp()
{
	auto action = cocos2d::MoveBy::create(
		MINI_BOSS_MOVE_UP_OR_DOWN_DURATION / 3, 
		cocos2d::Vec2(
			0,
			verticalFlightDistance / 3
		)
	);
	return action;
}


cocos2d::MoveBy * MiniBoss::moveDown()
{
	auto action = cocos2d::MoveBy::create(
		MINI_BOSS_MOVE_UP_OR_DOWN_DURATION / 3,
		cocos2d::Vec2(
			0,
			-verticalFlightDistance / 3
		)
	);
	return action;
}


cocos2d::MoveBy * MiniBoss::moveLeft()
{
	auto action = cocos2d::MoveBy::create(
		MINI_BOSS_MOVE_LEFT_OR_RIGHT_DURATION,
		cocos2d::Vec2(
			-horizontalFlightDistance,
			0
		)
	);
	return action;
}


cocos2d::MoveBy * MiniBoss::moveRight()
{
	auto action = cocos2d::MoveBy::create(
		MINI_BOSS_MOVE_LEFT_OR_RIGHT_DURATION,
		cocos2d::Vec2(
			horizontalFlightDistance,
			0
		)
	);
	return action;
}


void MiniBoss::attack1()
{
	createBullet(bossSprite1->getPosition(), flip1);
}


void MiniBoss::attack2()
{
	createBullet(bossSprite2->getPosition(), flip2);
}


void MiniBoss::createBullet(cocos2d::Vec2 position, bool leftToRight)
{
	//////////////////////////////////////////////
	// load enemy bullet sprite
	auto bullet = cocos2d::Sprite::create("images/enemy/enemy_bullet.png");
	bullet->setPosition(position);

	/////////////////////////////////////////////
	// add bullet physics body
	auto bulletBody = cocos2d::PhysicsBody::createCircle(bullet->getContentSize().width / 2);
	bulletBody->setDynamic(false);
	bulletBody->setContactTestBitmask(true);
	bulletBody->setCollisionBitmask((int)CollisionBitmask::ENEMY_BULLET_BISMASK);
	bullet->setPhysicsBody(bulletBody);

	////////////////////////////////////////////
	// setup bullet action
	cocos2d::MoveBy *bulletMovement;
	if (leftToRight)
		bulletMovement = cocos2d::MoveBy::create(1, cocos2d::Vec2(horizontalFlightDistance, 0));
	else
		bulletMovement = cocos2d::MoveBy::create(1, cocos2d::Vec2(-horizontalFlightDistance, 0));
	auto remove = cocos2d::CallFunc::create(bullet, callfunc_selector(cocos2d::Sprite::removeFromParent));
	
	gameScene->addChild(bullet);
	bullet->runAction(
		cocos2d::Sequence::create(
			bulletMovement, 
			remove, 
			NULL
		)
	);
}


void MiniBoss::initMiniBoss1()
{
	//////////////////////////////////////////////
	// load mini boss sprite 1
	bossSprite1 = cocos2d::Sprite::create("images/bosses/mini_boss.png");
	bossSprite1->setPosition(
		cocos2d::Vec2(
			origin.x + bossSprite1->getContentSize().width / 2,
			origin.y + visibleSize.height + bossSprite1->getContentSize().height
		)
	);
	health1 = MINI_BOSS_MAX_HEALTH;
	dead1 = false;
	flip1 = true;
	bossSprite1->setFlippedX(flip1);

	/////////////////////////////////////////////
	// add mini boss physics body 1
	PhysicsBodyParser::getInstance()->parseJsonFile("PhysicsBodies/PhysicsBodies.json");
	auto miniBossBody = PhysicsBodyParser::getInstance()->bodyFormJson(
		bossSprite1,
		"mini_boss",
		cocos2d::PHYSICSBODY_MATERIAL_DEFAULT
	);
	miniBossBody->setDynamic(false);
	miniBossBody->setContactTestBitmask(true);
	miniBossBody->setCollisionBitmask((int)CollisionBitmask::BOSS_COLLISION_BITMASK);
	bossSprite1->setPhysicsBody(miniBossBody);

	gameScene->addChild(bossSprite1);
}


void MiniBoss::initMiniBoss2()
{
	//////////////////////////////////////////////
	// load mini boss sprite 1
	bossSprite2 = cocos2d::Sprite::create("images/bosses/mini_boss.png");
	bossSprite2->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width - bossSprite2->getContentSize().width / 2,
			origin.y + visibleSize.height + bossSprite2->getContentSize().height
		)
	);
	health2 = MINI_BOSS_MAX_HEALTH;
	dead2 = false;
	flip2 = false;
	bossSprite2->setFlippedX(flip2);

	/////////////////////////////////////////////
	// add mini boss physics body 1
	PhysicsBodyParser::getInstance()->parseJsonFile("PhysicsBodies/PhysicsBodies.json");
	auto miniBossBody = PhysicsBodyParser::getInstance()->bodyFormJson(
		bossSprite2,
		"mini_boss",
		cocos2d::PHYSICSBODY_MATERIAL_DEFAULT
	);
	miniBossBody->setDynamic(false);
	miniBossBody->setContactTestBitmask(true);
	miniBossBody->setCollisionBitmask((int)CollisionBitmask::BOSS_COLLISION_BITMASK);
	bossSprite2->setPhysicsBody(miniBossBody);

	gameScene->addChild(bossSprite2);
}
