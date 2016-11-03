#include "AntiAircraftGun.h"
#include "Enumeration.h"
#include "PhysicsBodyParser\PhysicsBodyParser.h"
#include "SimpleAudioEngine.h"
#include "CommonMath.h"


AntiAircraftGun * AntiAircraftGun::spawnGun(cocos2d::Layer *gameScene)
{
	return new AntiAircraftGun(gameScene);
}


AntiAircraftGun::AntiAircraftGun(cocos2d::Layer *gameScene)
{
	this->gameScene = gameScene;
	this->autorelease();

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	initAntiAircraftGun();
	setupGunAction();
}


AntiAircraftGun::~AntiAircraftGun()
{
	
}


cocos2d::Vec2 AntiAircraftGun::getGunCurrentPosition()
{
	if (!gunSprite)
		return cocos2d::Vec2();
	
	return gunSprite->getPosition();
}


void AntiAircraftGun::initAntiAircraftGun()
{
	gunSprite = cocos2d::Sprite::create("images/enemy/anti_aircraft.png");
	gunSprite->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width + gunSprite->getContentSize().width,
			origin.y + PLATFORM_THICKNESS + gunSprite->getContentSize().height / 2 - 25
		)
	);

	gameScene->addChild(gunSprite);
}


void AntiAircraftGun::setupGunAction()
{
	unsigned int shiftTimes = 0;
	float shiftedDistance = 0;

	countedAttackTime = 0;
	remainingShiftDistance = visibleSize.width + gunSprite->getContentSize().width;
	float remainingTime = (remainingShiftDistance / (BACKGROUND_SHIFT_DISTANCE * AVERAGE_DELTA_TIME)) * AVERAGE_DELTA_TIME;

	///////////////////////////////////////////////
	// Shift to the left and attack 1 time
	generateNextMoveDuration();
	shiftTimes = static_cast<unsigned int>(nextMoveDuration / AVERAGE_DELTA_TIME);
	shiftedDistance = BACKGROUND_SHIFT_DISTANCE * AVERAGE_DELTA_TIME * shiftTimes;
	remainingShiftDistance -= shiftedDistance;
	remainingTime -= nextMoveDuration;

	auto shiftLeft1 = cocos2d::MoveBy::create(nextMoveDuration, cocos2d::Vec2(-shiftedDistance, 0));
	auto attack1 = cocos2d::CallFunc::create(this, callfunc_selector(AntiAircraftGun::launchMissile));
	auto sequence1 = cocos2d::Sequence::create(
		shiftLeft1,
		attack1,
		NULL
	);

	//////////////////////////////////////////////
	// Shifting to the middle
	shiftedDistance = remainingShiftDistance - visibleSize.width / 2;
	shiftTimes = shiftedDistance / (BACKGROUND_SHIFT_DISTANCE * AVERAGE_DELTA_TIME);
	nextMoveDuration = shiftTimes * AVERAGE_DELTA_TIME;
	remainingShiftDistance -= shiftedDistance;
	remainingTime -= nextMoveDuration;
	
	auto shiftToMiddle = cocos2d::MoveBy::create(nextMoveDuration, cocos2d::Vec2(-shiftedDistance, 0));
	auto flip = cocos2d::CallFunc::create(this, callfunc_selector(AntiAircraftGun::flipGunSprite));
	auto sequence2 = cocos2d::Sequence::create(
		shiftToMiddle,
		flip,
		NULL
	);

	//////////////////////////////////////////////
	// Shift to the left and attack again
	generateNextMoveDuration();
	shiftTimes = static_cast<unsigned int>(nextMoveDuration / AVERAGE_DELTA_TIME);
	shiftedDistance = BACKGROUND_SHIFT_DISTANCE * AVERAGE_DELTA_TIME * shiftTimes;
	remainingShiftDistance -= shiftedDistance;
	remainingTime -= nextMoveDuration;
	
	auto shiftLeft2 = cocos2d::MoveBy::create(nextMoveDuration, cocos2d::Vec2(-shiftedDistance, 0));
	auto attack2 = cocos2d::CallFunc::create(this, callfunc_selector(AntiAircraftGun::launchMissile));
	auto sequence3 = cocos2d::Sequence::create(
		shiftLeft2,
		attack2,
		NULL
	);

	/////////////////////////////////////////////
	// Shift to the left with remaining distance
	auto shiftRemaining = cocos2d::MoveBy::create(remainingTime, cocos2d::Vec2(-remainingShiftDistance, 0));
	auto remove = cocos2d::CallFunc::create(gunSprite, callfunc_selector(cocos2d::Sprite::removeFromParent));
	
	auto sequence4 = cocos2d::Sequence::create(
		shiftRemaining,
		remove,
		NULL
	);

	auto combination = cocos2d::Sequence::create(
		sequence1,
		sequence2,
		sequence3,
		sequence4,
		NULL
	);

	gunSprite->runAction(combination);
}


void AntiAircraftGun::flipGunSprite()
{
	gunSprite->setFlippedX(!gunSprite->isFlipX());
}


void AntiAircraftGun::generateNextMoveDuration()
{
	nextMoveDuration = cocos2d::random(ANTI_AIRCRAFT_RELOAD_DURATION - 0.5F, ANTI_AIRCRAFT_RELOAD_DURATION + 0.5F);
}


void AntiAircraftGun::launchMissile()
{
	aimPosition = getTargetPosition();
	auto missile = new AntiAircraftMissile(
		gameScene, 
		getGunCurrentPosition(), 
		aimPosition, 
		(AntiAircraftMissile::MissileType)countedAttackTime
	);
	
	countedAttackTime++;
}


cocos2d::Vec2 AntiAircraftGun::getTargetPosition()
{
	switch (countedAttackTime)
	{
	case 0:
		return cocos2d::Vec2(
			cocos2d::random(origin.x + (visibleSize.width / 5), origin.x + (visibleSize.width / 5) * 2.5F),
			origin.y + visibleSize.height
		);
		break;

	case 1:
		return cocos2d::Vec2(
			cocos2d::random(origin.x + (visibleSize.width / 5) * 2.5F, origin.x + visibleSize.width),
			origin.y + visibleSize.height
		);
		break;

	default:
		break;
	}
}


/*======================================================================*/


AntiAircraftMissile::AntiAircraftMissile(cocos2d::Layer * gameScene, cocos2d::Vec2 startPos, cocos2d::Vec2 endPos, AntiAircraftMissile::MissileType type)
{
	this->gameScene = gameScene;
	this->startPosition = startPos;
	this->endPosition = endPos;
	missileType = type;

	initMissile();
}


AntiAircraftMissile::~AntiAircraftMissile()
{

}


void AntiAircraftMissile::initMissile()
{
	///////////////////////////////////////
	// load missile sprite
	missileSprite = cocos2d::Sprite::create("images/enemy/anti_aircraft_missile.png");
	missileSprite->setPosition(startPosition);

	///////////////////////////////////////
	// add missile physics body
	PhysicsBodyParser::getInstance()->parseJsonFile("PhysicsBodies/PhysicsBodies.json");
	cocos2d::PhysicsBody *missileBody;
	
	switch (missileType)
	{
	case AntiAircraftMissile::MissileType::FLY_TO_THE_LEFT:
		missileBody = PhysicsBodyParser::getInstance()->bodyFormJson(
			missileSprite,
			"air_defense_missile_left",
			cocos2d::PHYSICSBODY_MATERIAL_DEFAULT
		);
		break;

	case AntiAircraftMissile::MissileType::FLY_TO_THE_RIGHT:
		missileBody = PhysicsBodyParser::getInstance()->bodyFormJson(
			missileSprite,
			"air_defense_missile_right",
			cocos2d::PHYSICSBODY_MATERIAL_DEFAULT
		);
		break;

	default:
		break;
	}

	missileBody->setDynamic(false);
	missileBody->setContactTestBitmask(true);
	missileBody->setCollisionBitmask((int)CollisionBitmask::OBSTACLE_COLLISION_BITMASK);
	missileSprite->setPhysicsBody(missileBody);
	missileSprite->setFlippedX((bool)missileType);

	gameScene->addChild(missileSprite);

	///////////////////////////////////////
	// setup missile action
	auto sequence = cocos2d::Sequence::create(
		cocos2d::MoveTo::create(__ANTI_AIRCRAFT_MISSILE_FLYING_DURATION__, endPosition),
		cocos2d::CallFunc::create(missileSprite, callfunc_selector(cocos2d::Sprite::removeFromParent)),
		NULL
	);
	missileSprite->runAction(sequence);
}

