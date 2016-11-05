#include "Tower.h"
#include "Enumeration.h"
#include "GameSettings.h"
#include "SimpleAudioEngine.h"
#include "PhysicsBodyParser\PhysicsBodyParser.h"
#include "CommonMath.h"


Tower * Tower::spawnTower(cocos2d::Layer * gameScene)
{
	return new Tower(gameScene);
}


Tower::Tower(cocos2d::Layer * gameScene)
{
	this->gameScene = gameScene;
	this->autorelease();

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	this->initTower();
	this->initTowerBulletDirection();
	this->setupTowerAction();
}


Tower::~Tower()
{
	
}


void Tower::initTower()
{
	////////////////////////////////////
	// load tower sprite

	towerSprite = cocos2d::Sprite::create("images/enemy/tower.png");
	towerSprite->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width,
			origin.y + PLATFORM_THICKNESS + towerSprite->getContentSize().height / 2 - 25
		)
	);

	//////////////////////////////////
	// add tower physics body

	PhysicsBodyParser::getInstance()->parseJsonFile("PhysicsBodies/PhysicsBodies.json");
	auto towerBody = PhysicsBodyParser::getInstance()->bodyFormJson(
		towerSprite,
		"tower",
		cocos2d::PhysicsMaterial(999, 0, 0)
	);
	towerBody->setDynamic(false);
	towerBody->setContactTestBitmask(true);
	towerBody->setCollisionBitmask((int)CollisionBitmask::OBSTACLE_COLLISION_BITMASK);
	towerSprite->setPhysicsBody(towerBody);

	gameScene->addChild(towerSprite);
}


void Tower::initTowerBulletDirection()
{
	towerBulletDirection[0].x = -visibleSize.width;
	towerBulletDirection[0].y = 0;

	towerBulletDirection[1].x = -visibleSize.width * cos(45);
	towerBulletDirection[1].y = visibleSize.width * sin(45);

	towerBulletDirection[2].x = visibleSize.width * cos(45);
	towerBulletDirection[2].y = visibleSize.width * sin(45);

	towerBulletDirection[3].x = visibleSize.width;
	towerBulletDirection[3].y = 0;
}


void Tower::setupTowerAction()
{
	unsigned int shiftTimes = 0;
	float shiftedDistance = 0;

	remainingShiftDistance = visibleSize.width + towerSprite->getContentSize().width;
	float remainingTime = (remainingShiftDistance / (BACKGROUND_SHIFT_DISTANCE * AVERAGE_DELTA_TIME)) * AVERAGE_DELTA_TIME;
	
	////////////////////////////////////////////////
	// Phase 1

	generateNextAttackDuration();
	shiftTimes = static_cast<unsigned int>(nextAttackDuration / AVERAGE_DELTA_TIME);
	shiftedDistance = BACKGROUND_SHIFT_DISTANCE * AVERAGE_DELTA_TIME * shiftTimes;
	remainingShiftDistance -= shiftedDistance;
	remainingTime -= nextAttackDuration;

	auto shiftLeft1 = cocos2d::MoveBy::create(nextAttackDuration, cocos2d::Vec2(-shiftedDistance, 0));
	auto attack1 = cocos2d::CallFunc::create(this, callfunc_selector(Tower::attack));
	auto sequence1 = cocos2d::Sequence::create(
		shiftLeft1,
		attack1,
		NULL
	);

	////////////////////////////////////////////////
	// Phase 2

	generateNextAttackDuration();
	shiftTimes = static_cast<unsigned int>(nextAttackDuration / AVERAGE_DELTA_TIME);
	shiftedDistance = BACKGROUND_SHIFT_DISTANCE * AVERAGE_DELTA_TIME * shiftTimes;
	remainingShiftDistance -= shiftedDistance;
	remainingTime -= nextAttackDuration;

	auto shiftLeft2 = cocos2d::MoveBy::create(nextAttackDuration, cocos2d::Vec2(-shiftedDistance, 0));
	auto attack2 = cocos2d::CallFunc::create(this, callfunc_selector(Tower::attack));
	auto sequence2 = cocos2d::Sequence::create(
		shiftLeft2,
		attack2,
		NULL
	);

	////////////////////////////////////////////////
	// Phase 3

	generateNextAttackDuration();
	shiftTimes = static_cast<unsigned int>(nextAttackDuration / AVERAGE_DELTA_TIME);
	shiftedDistance = BACKGROUND_SHIFT_DISTANCE * AVERAGE_DELTA_TIME * shiftTimes;
	remainingShiftDistance -= shiftedDistance;
	remainingTime -= nextAttackDuration;

	auto shiftLeft3 = cocos2d::MoveBy::create(nextAttackDuration, cocos2d::Vec2(-shiftedDistance, 0));
	auto attack3 = cocos2d::CallFunc::create(this, callfunc_selector(Tower::attack));
	auto sequence3 = cocos2d::Sequence::create(
		shiftLeft3,
		attack3,
		NULL
	);

	////////////////////////////////////////////////
	// Phase 4
	auto shiftLeft4 = cocos2d::MoveBy::create(remainingTime, cocos2d::Vec2(-remainingShiftDistance, 0));
	auto sequence4 = cocos2d::Sequence::create(
		shiftLeft4,
		cocos2d::CallFunc::create(towerSprite, callfunc_selector(cocos2d::Sprite::removeFromParent)),
		NULL
	);

	auto combination = cocos2d::Sequence::create(
		sequence1,
		sequence2,
		sequence3,
		sequence4,
		NULL
	);

	towerSprite->runAction(combination);
}


cocos2d::Sprite * Tower::createBullet()
{
	/////////////////////////////////////////////
	// load bullet sprite

	auto bullet = cocos2d::Sprite::create("images/enemy/enemy_bullet.png");
	bullet->setPosition(
		cocos2d::Vec2(
			towerSprite->getPositionX(), 
			towerSprite->getPositionY() + towerSprite->getContentSize().height / 3
		)
	);
	
	///////////////////////////////////////////
	// add bullet physics body

	auto bulletBody = cocos2d::PhysicsBody::createCircle(
		bullet->getContentSize().width / 2,
		cocos2d::PhysicsMaterial(1, 1, 0), 
		cocos2d::Vec2::ZERO
	);
	bulletBody->setDynamic(false);
	bulletBody->setContactTestBitmask(true);
	bulletBody->setCollisionBitmask((int)CollisionBitmask::ENEMY_BULLET_BISMASK);
	bullet->setPhysicsBody(bulletBody);

	return bullet;
}


cocos2d::Sequence * Tower::createBulletAction(int bulletIndex)
{
	auto moveBy = cocos2d::MoveBy::create(
		TOWER_BULLET_DURATION,
		towerBulletDirection[bulletIndex]
	);
	auto sequence = cocos2d::Sequence::create(
		moveBy, 
		NULL
	);
	return sequence;
}


void Tower::attack()
{
	for (int i = 0; i < TOWER_TOTAL_BULLET; i++) {

		auto bullet = this->createBullet();
		gameScene->addChild(bullet);
		auto bulletSequence = cocos2d::Sequence::create(
			createBulletAction(i),
			cocos2d::CallFunc::create(bullet, callfunc_selector(cocos2d::Sprite::removeFromParent)), 
			NULL
		);
		bullet->runAction(bulletSequence);
	}

	if (GameSettings::getInstance()->willPlayEffectSounds)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/tower_sound.mp3");
}


void Tower::generateNextAttackDuration()
{
	nextAttackDuration = cocos2d::random(TOWER_RELOAD_DURATION - 0.5F, TOWER_RELOAD_DURATION + 0.5F);
}
