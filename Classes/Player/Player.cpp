#include "Player.h"
#include "Enumeration.h"
#include "SimpleAudioEngine.h"
#include "GameSettings.h"
#include "PhysicsBodyParser\PhysicsBodyParser.h"
#include <string>
#include <algorithm>


Player::Player(cocos2d::Layer *gameScene)
{
	this->gameScene = gameScene;

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	rotationDegree = 0;
	movementDirection = 0;
	fuel = PLAYER_MAX_FUEL;
	score = 0;
	isUnderControl = true;

	this->initPlayerSprite();
	this->initPlayerAnimation();

	this->scheduleUpdate();
}


Player::~Player()
{
}


void Player::saveScore()
{
	auto user = cocos2d::UserDefault::getInstance();
	user->setIntegerForKey("score", getScore());
	if(user->getIntegerForKey("bestScore") < getScore())
		user->setIntegerForKey("bestScore", getScore());
}


cocos2d::PhysicsBody * Player::getPlayerPhysicsBody()
{
	return playerSprite->getPhysicsBody();
}


void Player::resetPlayerMovementDirection()
{
	movementDirection = 0;
}


void Player::balancingPlayerRotation()
{
	if (rotationDegree < 0)
		rotationDegree++;
	if (rotationDegree > 0)
		rotationDegree--;
}


void Player::rotatingToTheLeft()
{
	rotationDegree = cocos2d::clampf(
		rotationDegree - 1,
		__HELICOPTER_LEFT_ROTATION_CLAMP__,
		__HELICOPTER_NOT_ROTATION_DEGREE__
	);
}


void Player::rotatingToTheRight()
{
	rotationDegree = cocos2d::clampf(
		rotationDegree + 1,
		__HELICOPTER_NOT_ROTATION_DEGREE__,
		__HELICOPTER_RIGHT_ROTATION_CLAMP__
	);
}


void Player::setPlayerMovementDirection(cocos2d::Vec2 vec)
{
	resetPlayerMovementDirection();

	if (vec.x <= -(__JOY_STICK_ORIGIN_HALF_LENGTH__)) {
		movementDirection |= MovementDirection::LEFT;
		rotatingToTheLeft();
	}
	
	if (vec.x >= __JOY_STICK_ORIGIN_HALF_LENGTH__) {
		movementDirection |= MovementDirection::RIGHT;
		rotatingToTheRight();
	}

	if (vec.y >= __JOY_STICK_ORIGIN_HALF_LENGTH__) {
		movementDirection |= MovementDirection::UP;
		balancingPlayerRotation();
	}

	if (vec.y <= -(__JOY_STICK_ORIGIN_HALF_LENGTH__)) {
		movementDirection |= MovementDirection::DOWN;
		balancingPlayerRotation();
	}

	if (movementDirection == 0)
		balancingPlayerRotation();
}


void Player::increaseFuel(int amount)
{
	fuel += amount;
	if (fuel > __HELICOPTER_MAX_FUEL__)
		fuel = __HELICOPTER_MAX_FUEL__;
}


void Player::decreaseFuel()
{
	if (fuel > 0)
		fuel--;
	else
		lostControl();
}


int Player::getCurrentFuel()
{
	return this->fuel;
}


void Player::increaseScore(int amount)
{
	score += amount;
}


unsigned long long Player::getScore()
{
	return this->score;
}


void Player::lostControl()
{
	isUnderControl = false;
	playerSprite->getPhysicsBody()->setGravityEnable(true);
}


bool Player::stillUnderControl()
{
	return isUnderControl;
}


cocos2d::Vec2 Player::getPlayerCurrentPosition()
{
	return playerSprite->getPosition();
}


cocos2d::Vec2 Player::getPlayerGunPosition()
{
	return cocos2d::Vec2(
		playerSprite->getPositionX() + playerSprite->getContentSize().width / 2 - 20, 
		playerSprite->getPositionY() - 10
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
	playerSprite->setRotation(rotationDegree);

	/////////////////////////////////////////
	// add player physics body
	PhysicsBodyParser::getInstance()->parseJsonFile("PhysicsBodies/PhysicsBodies.json");
	auto playerBody = PhysicsBodyParser::getInstance()->bodyFormJson(
		playerSprite,
		"player",
		cocos2d::PHYSICSBODY_MATERIAL_DEFAULT
	);
	playerBody->setGravityEnable(false);
	playerBody->setDynamic(true);
	playerBody->setRotationEnable(false);
	playerBody->setContactTestBitmask(true);
	playerBody->setCollisionBitmask((int)CollisionBitmask::PLAYER_COLLISION_BITMASK);
	playerSprite->setPhysicsBody(playerBody);

	gameScene->addChild(playerSprite);
}


void Player::initPlayerAnimation()
{
	//flying animation
	cocos2d::Vector<cocos2d::SpriteFrame *> flyingFrames;
	for (int i = 0; i < __PLAYER_FLYING_ANIMATION_TOTAL_FRAME__; i++) {

		auto file = cocos2d::String::createWithFormat("images/player/player%d.png", i);
		auto frame = cocos2d::SpriteFrame::create(file->getCString(), cocos2d::Rect(0, 0, 170, 50));
		flyingFrames.pushBack(frame);
	}
	cocos2d::Animation *animation = cocos2d::Animation::createWithSpriteFrames(flyingFrames, (float)__PLAYER_FLYING_ANIMATION_TOTAL_FRAME__ / 60.0F);
	auto flyingAnimate = cocos2d::Animate::create(animation);

	//player start with flying animation
	playerSprite->runAction(cocos2d::RepeatForever::create(flyingAnimate));
}


void Player::updatePlayerPosition(float dt)
{
	playerSprite->setRotation(rotationDegree);

	if (movementDirection & MovementDirection::LEFT) {
		playerSprite->setPositionX(
			cocos2d::clampf(
				playerSprite->getPositionX() - __HELICOPTER_FLYING_SPEED__ * dt, 
				origin.x + playerSprite->getContentSize().width / 2,
				origin.x + visibleSize.width - playerSprite->getContentSize().width / 2
			)
		);
	}

	if (movementDirection & MovementDirection::RIGHT) {
		playerSprite->setPositionX(
			cocos2d::clampf(
				playerSprite->getPositionX() + __HELICOPTER_FLYING_SPEED__ * dt,
				origin.x + playerSprite->getContentSize().width / 2,
				origin.x + visibleSize.width - playerSprite->getContentSize().width / 2
			)
		);
	}

	if (movementDirection & MovementDirection::UP) {
		playerSprite->setPositionY(
			cocos2d::clampf(
				playerSprite->getPositionY() + __HELICOPTER_FLYING_SPEED__ * dt, 
				origin.y + playerSprite->getContentSize().height / 2,
				origin.y + visibleSize.height - playerSprite->getContentSize().height / 2
			)
		);
	}

	if (movementDirection & MovementDirection::DOWN) {
		playerSprite->setPositionY(
			cocos2d::clampf(
				playerSprite->getPositionY() - __HELICOPTER_FLYING_SPEED__ * dt,
				origin.y,
				origin.y + visibleSize.height - playerSprite->getContentSize().height / 2
			)
		);
	}
}


void Player::explodingHelicopter()
{
	playerSprite->stopAllActions();
	playerSprite->getPhysicsBody()->removeFromWorld();
	playerSprite->setVisible(false);
	Explosion *explosion = new Explosion(gameScene, getPlayerCurrentPosition());
}


PlayerBullet* Player::createBullet(float t)
{
	return PlayerBullet::createBullet(gameScene, getPlayerGunPosition(), rotationDegree);
}

/*============================================================*/

PlayerBullet * PlayerBullet::createBullet(cocos2d::Layer * gameScene, cocos2d::Vec2 pos, float rotation)
{
	return new PlayerBullet(gameScene, pos, rotation);
}


PlayerBullet::PlayerBullet(cocos2d::Layer * gameScene, cocos2d::Vec2 pos, float rotation)
{
	this->gameScene = gameScene;
	this->startPos = pos;
	this->radian = -(rotation * 3.1416F / 180.0F);

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	initPlayerBullet();
}


PlayerBullet::~PlayerBullet()
{

}


cocos2d::Sprite * PlayerBullet::getBulletSprite()
{
	return bulletSprite;
}


void PlayerBullet::initPlayerBullet()
{
	////////////////////////////////////////////
	// load player bullet sprite
	bulletSprite = cocos2d::Sprite::create("images/player/player_bullet.png");
	bulletSprite->setPosition(startPos);

	////////////////////////////////////////////
	// add bullet physics body
	auto bulletBody = cocos2d::PhysicsBody::createBox(bulletSprite->getContentSize());
	bulletBody->setDynamic(true);
	bulletBody->setGravityEnable(false);
	bulletBody->setContactTestBitmask(true);
	bulletBody->setCollisionBitmask((int)CollisionBitmask::PLAYER_BULLET_BITMASK);
	bulletSprite->setPhysicsBody(bulletBody);

	////////////////////////////////////////////
	// setup bullet action
	auto sequence = cocos2d::Sequence::create(
		cocos2d::MoveBy::create(1.0F, cocos2d::Vec2(__PLAYER_BULLET_VELOCITY__, __PLAYER_BULLET_VELOCITY__ * tan(radian))),
		cocos2d::CallFunc::create(bulletSprite, callfunc_selector(cocos2d::Sprite::removeFromParent)),
		NULL
	);
	
	gameScene->addChild(bulletSprite);
	bulletSprite->runAction(sequence);
}

/*======================================================================*/


Explosion::Explosion(cocos2d::Layer * gameScene, cocos2d::Vec2 pos)
{
	this->gameScene = gameScene;
	this->explodePos = pos;

	initExplosion();
}


Explosion::~Explosion()
{

}


void Explosion::initExplosion()
{
	auto explosion = cocos2d::ParticleExplosion::createWithTotalParticles(100);
	explosion->setEmitterMode(cocos2d::ParticleSystem::Mode::RADIUS);
	explosion->setPosition(explodePos);
	explosion->setStartColor(cocos2d::Color4F(0xFF, 0, 0, 0xFF));
	explosion->setEndColor(cocos2d::Color4F(0xFF, 0x55, 0, 0xFF));
	explosion->setEndRadius(300);

	explosion->runAction(
		cocos2d::Sequence::create(
			cocos2d::DelayTime::create(1.0F),
			cocos2d::CallFunc::create(explosion, callfunc_selector(cocos2d::ParticleExplosion::removeFromParent)),
			NULL
		)
	);

	gameScene->addChild(explosion);
	
	if (GameSettings::getInstance()->willPlayEffectSounds)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/explosion.mp3");
}