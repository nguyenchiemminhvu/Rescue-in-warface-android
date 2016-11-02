#include "Missile.h"
#include "Tower.h"
#include "Enumeration.h"
#include "SimpleAudioEngine.h"

#include "PhysicsBodyParser\PhysicsBodyParser.h"


Missile * Missile::spawnMissile(cocos2d::Layer * gameScene, float playerPosY)
{
	return new Missile(gameScene, playerPosY);
}


Missile::Missile(cocos2d::Layer * gameScene, float playerPosY)
{
	this->gameScene = gameScene;

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	this->createMissile(playerPosY);
	this->createFireParticle();

	missileSprite->runAction(missileActions());
	fire->runAction(particleActions());
	
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/missile_sound.mp3");
}


Missile::~Missile()
{
	this->missileSprite->release();
	this->fire->release();
}


void Missile::createMissile(float playerPosY)
{
	/////////////////////////////////////////
	// load missile sprite

	missileSprite = cocos2d::Sprite::create("images/enemy/missile.png");
	float randomPosY = cocos2d::random(playerPosY - 100, playerPosY + 100);
	if (randomPosY < origin.y + PLATFORM_THICKNESS + TOWER_HEIGHT) {
		randomPosY = origin.y + PLATFORM_THICKNESS + TOWER_HEIGHT;
	}
	missileSprite->setPosition(
		cocos2d::Vec2(
			origin.x - missileSprite->getContentSize().width / 2,
			randomPosY
		)
	);

	////////////////////////////////////////
	// add missile physics body

	PhysicsBodyParser::getInstance()->parseJsonFile("PhysicsBodies/PhysicsBodies.json");
	auto missileBody =	PhysicsBodyParser::getInstance()->bodyFormJson(
							missileSprite,
							"missile",
							cocos2d::PhysicsMaterial(1, 0, 1)
						);
	missileBody->setDynamic(false);
	missileBody->setContactTestBitmask(true);
	missileBody->setCollisionBitmask((int)CollisionBitmask::OBSTACLE_COLLISION_BITMASK);
	missileSprite->setPhysicsBody(missileBody);

	gameScene->addChild(missileSprite);
}


void Missile::createFireParticle()
{
	fire = cocos2d::ParticleSun::createWithTotalParticles(200);
	fire->setEmitterMode(cocos2d::ParticleSystem::Mode::GRAVITY);
	fire->setGravity(cocos2d::Vec2(200, 0));
	fire->setPosition(
		cocos2d::Vec2(
			missileSprite->getPositionX() - missileSprite->getContentSize().width / 2,
			missileSprite->getPositionY()
		)
	);

	gameScene->addChild(fire);
}


cocos2d::Sequence * Missile::missileActions()
{
	auto moveRight = cocos2d::MoveBy::create(
		MISSILE_FLYING_DURATION, 
		cocos2d::Vec2(
			visibleSize.width + missileSprite->getContentSize().width * 2, 
			0
		)
	);
	auto removeMissile = cocos2d::CallFunc::create(missileSprite, callfunc_selector(cocos2d::Sprite::removeFromParent));
	auto sequence = cocos2d::Sequence::create(
		moveRight,
		removeMissile,
		NULL
	);

	return sequence;
}


cocos2d::Sequence * Missile::particleActions()
{
	auto moveRight = cocos2d::MoveBy::create(
		MISSILE_FLYING_DURATION, 
		cocos2d::Vec2(
			visibleSize.width + missileSprite->getContentSize().width * 2,
			0
		)
	);
	auto delay = cocos2d::DelayTime::create(1.0F);
	auto removeParticle = cocos2d::CallFunc::create(fire, callfunc_selector(cocos2d::ParticleSun::removeFromParent));
	auto sequence = cocos2d::Sequence::create(
		moveRight, 
		delay,
		removeParticle,
		NULL
	);

	return sequence;
}

