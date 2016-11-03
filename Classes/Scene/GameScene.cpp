#include "GameScene.h"
#include "GameSettings.h"
#include "Enumeration.h"
#include "SimpleAudioEngine.h"
#include "PauseScene.h"
#include "FinishedScene.h"
#include "GameOverScene.h"
#include "Toast.h"
#include "CommonMath.h"

#include "Enemy\Missile.h"
#include "Enemy\AntiAircraftGun.h"
#include "Enemy\Tower.h"


cocos2d::Scene * GameScene::createScene()
{
	auto scene = cocos2d::Scene::createWithPhysics();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
#endif
	
	auto layerGame = GameScene::create();
	layerGame->setPhysicsWorld(scene->getPhysicsWorld());
	scene->addChild(layerGame);

	return scene;
}


bool GameScene::init()
{
	if(!cocos2d::Layer::init())
		return false;

	isGameRunning = true;

	origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	/////////////////////////////////////////
	// initialization

	initGameSceneAttributes();
	initBoundPhysicsBody();
	initPlatform();
	initBackground();
	initPlatformBody();
	initWeather();
	playGameMusic();

	initPlayer();
	initHUD();
	initJoyStick();

	initContactListener();
	initKeyboardEventListener();
	initMultiTouchEventListener();

	initMiniBoss();
	initMotherFucker();

	/////////////////////////////////////////
	// schedule

	this->schedule(schedule_selector(GameScene::spawnGascan), GASCAN_SPAWN_DURATION);
	spawningObstacle();
	this->scheduleUpdate();

	return true;
}


void GameScene::update(float dt)
{
	countingFrames();
	updateBackgroundAndPlatform(dt);
	updateRemainingDistance();
	updatePlayerFuel();
	updatePlayerScore();
	updatePlayerDirection();
	player->updatePlayerPosition(dt);

	if (remainingDistance <= 0) {
		if (!miniBoss->isStarted()) {
			stopSpawningObstacle();
			invokeMiniBoss();
		}
	}
}


void GameScene::setPhysicsWorld(cocos2d::PhysicsWorld * world)
{
	this->world = world;
}

//////////////////////////////////////////////
// Initialization

void GameScene::initGameSceneAttributes()
{
	remainingDistance = GAME_SCENE_TOTAL_DISTANCE;
	countedFrames = 0;
}


void GameScene::initBoundPhysicsBody()
{
	auto frameBoundary = cocos2d::PhysicsBody::createEdgeBox(
		cocos2d::Size(visibleSize.width + 20, visibleSize.height + 20), 
		cocos2d::PhysicsMaterial(999, 0, 999), 
		5.0f, 
		cocos2d::Vec2::ZERO
	);

	frameBoundary->setDynamic(false);
	frameBoundary->setContactTestBitmask(true);
	frameBoundary->setCollisionBitmask((int)CollisionBitmask::OBSTACLE_COLLISION_BITMASK);
	this->setPhysicsBody(frameBoundary);
}


void GameScene::initPlatform()
{
	platform1 = cocos2d::Sprite::create("images/backgrounds/game_platform.png");
	platform1->setAnchorPoint(cocos2d::Vec2(0.5F, 0.0F));

	platform2 = cocos2d::Sprite::create("images/backgrounds/game_platform.png");
	platform2->setAnchorPoint(cocos2d::Vec2(0.5F, 0.0F));

	resetPlatformPosition();

	this->addChild(platform1);
	this->addChild(platform2);
}


void GameScene::initPlatformBody()
{
	auto invisiblePlatform = cocos2d::Sprite::create();
	invisiblePlatform->setContentSize(
		cocos2d::Size(
			visibleSize.width, 
			visibleSize.height - background1->getContentSize().height
		)
	);
	invisiblePlatform->setAnchorPoint(cocos2d::Vec2(0, 0));
	invisiblePlatform->setPosition(
		cocos2d::Vec2(
			origin.x,
			origin.y
		)
	);

	auto platformBody = cocos2d::PhysicsBody::createBox(
		invisiblePlatform->getContentSize()
	);
	
	platformBody->setDynamic(false);
	platformBody->setContactTestBitmask(true);
	platformBody->setCollisionBitmask((int)CollisionBitmask::OBSTACLE_COLLISION_BITMASK);
	invisiblePlatform->setPhysicsBody(platformBody);
	
	this->addChild(invisiblePlatform);
}


void GameScene::initBackground()
{
	background1 = cocos2d::Sprite::create("images/backgrounds/game_background.png");
	background1->setAnchorPoint(cocos2d::Vec2(0.5F, 1.0F));

	background2 = cocos2d::Sprite::create("images/backgrounds/game_background.png");
	background2->setAnchorPoint(cocos2d::Vec2(0.5F, 1.0F));

	resetBackgroundPosition();

	this->addChild(background1);
	this->addChild(background2);
}


void GameScene::initWeather()
{
	cocos2d::ParticleRain *rainEffect;
	rainEffect = cocos2d::ParticleRain::create();
	rainEffect->setEmitterMode(cocos2d::ParticleSystem::Mode::GRAVITY);
	rainEffect->setGravity(cocos2d::Vec2(-100, -300));
	rainEffect->setTotalParticles(200);
	rainEffect->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2 - rainEffect->getGravity().x / 2,
			origin.y + visibleSize.height
		)
	);
	this->addChild(rainEffect);
}


void GameScene::playGameMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/game_music.mp3", true);
}


void GameScene::initHUD()
{
	hud = HUD::createHUD();
	this->addChild(hud);
}


void GameScene::initPlayer()
{
	player = new Player(this);
}


void GameScene::initJoyStick()
{
	///////////////////////////////////////////
	// load button fire
	buttonFire = cocos2d::Sprite::create("images/buttons/button_fire.png");
	buttonFire->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width - buttonFire->getContentSize().width,
			origin.y + buttonFire->getContentSize().height
		)
	);

	//////////////////////////////////////////
	// add button fire physics body
	auto buttonFireBody = cocos2d::PhysicsBody::createCircle(buttonFire->getContentSize().width / 2);
	buttonFireBody->setDynamic(false);
	buttonFireBody->setGravityEnable(false);
	buttonFireBody->setContactTestBitmask(false);
	buttonFireBody->setCollisionBitmask(0);
	buttonFire->setPhysicsBody(buttonFireBody);

	this->addChild(buttonFire, 1000);

	//////////////////////////////////////////
	// load joy stick sprite
	joyStick = cocos2d::Sprite::create("images/buttons/joy_stick.png");
	joyStick->setPosition(
		cocos2d::Vec2(
			origin.x + joyStick->getContentSize().width,
			origin.y + joyStick->getContentSize().height
		)
	);
	joyStickOrigin = joyStick->getPosition();
	joyStickFirstTouchPosition = cocos2d::Vec2();
	joyStickDragged = cocos2d::Vec2();

	//////////////////////////////////////////
	// add joy stick physics body
	auto joyStickBody = cocos2d::PhysicsBody::createCircle(joyStick->getContentSize().width / 2);
	joyStickBody->setDynamic(false);
	joyStickBody->setContactTestBitmask(false);
	joyStickBody->setCollisionBitmask(0);
	joyStick->setPhysicsBody(joyStickBody);

	this->addChild(joyStick, 1000);
	isUsingJoyStick = false;

	/////////////////////////////////////////
	// load joy stick origin sprite
	joyStickOriginSprite = cocos2d::Sprite::create("images/buttons/joy_stick_origin.png");
	joyStickOriginSprite->setPosition(joyStickOrigin);
	joyStickOriginSprite->setCascadeOpacityEnabled(true);
	joyStickOriginSprite->setOpacity(100);
	this->addChild(joyStickOriginSprite, 900);

	/////////////////////////////////////////
	// Debug joy stick dragging
	debugDraggedValue = cocos2d::LabelTTF::create("", "", 30);
	debugDraggedValue->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + debugDraggedValue->getContentSize().height + 50
		)
	);
	this->addChild(debugDraggedValue);
}


void GameScene::initMiniBoss()
{
	miniBoss = new MiniBoss(this);
}


void GameScene::initMotherFucker()
{
	motherFucker = new MotherFucker(this);
}

//////////////////////////////////////////////////////
// Event listeners

void GameScene::initContactListener()
{
	contactListener = cocos2d::EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
}


void GameScene::initKeyboardEventListener()
{
	keyboardListener = cocos2d::EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}


void GameScene::initMultiTouchEventListener()
{
	touchesListener = cocos2d::EventListenerTouchAllAtOnce::create();

	touchesListener->onTouchesBegan = 
	[&](const std::vector<cocos2d::Touch *> touches, cocos2d::Event *event) {
		
		for (auto touch : touches) {
			//Check if player touch on button fire
			Node *nFire = this->getNodeUnderTouch(touch, buttonFire);
			if (
				nFire && 
				player->stillUnderControl()
				) 
			{
				buttonFire->setScale(1.1F);
				player->createBullet(0);
				this->schedule(schedule_selector(GameScene::playerShooting), __PLAYER_RELOAD_DURATION__);
				continue;
			}

			//Check if user touch on joy stick
			Node *nJoyStick = this->getNodeUnderTouch(touch, joyStick);

			if (
				nJoyStick && 
				player->stillUnderControl() && 
				!isUsingJoyStick
				) 
			{
				isUsingJoyStick = true;
				joyStickFirstTouchPosition = touch->getLocation();
				continue;
			}
		}
	};

	touchesListener->onTouchesMoved =
	[&](const std::vector<cocos2d::Touch *> touches, cocos2d::Event *event) {
		
		for (auto touch : touches) {
			//Check if user touch on joy stick
			Node *nJoyStick = this->getNodeUnderTouch(touch, joyStick);
			
			if (
				nJoyStick &&
				player->stillUnderControl() &&
				isUsingJoyStick
				)
			{
				joyStickDragged = (touch->getLocation() - joyStickFirstTouchPosition);
				joyStickDragged.x = cocos2d::clampf(joyStickDragged.x, -100.0F, 100.0F);
				joyStickDragged.y = cocos2d::clampf(joyStickDragged.y, -100.0F, 100.0F);
				joyStick->setPosition(joyStickOrigin + joyStickDragged);

				auto debugString = cocos2d::String::createWithFormat("(%.2f, %.2f)", joyStickDragged.x, joyStickDragged.y);
				debugDraggedValue->setString(debugString->getCString());
			}
		}
	};
	
	touchesListener->onTouchesEnded = 
	[&](const std::vector<cocos2d::Touch *> touches, cocos2d::Event *event) {
		
		for (auto touch : touches) {
			//Check if player untouched button fire
			Node *node = this->getNodeUnderTouch(touch, buttonFire);
			if (node) {
				buttonFire->setScale(1.0F);
				this->unschedule(schedule_selector(GameScene::playerShooting));
				continue;
			}

			//Check if user is using joy stick but not touch on it
			if (
				player->stillUnderControl() &&
				isUsingJoyStick
				)
			{
				//reset joy stick position and dragged vector
				isUsingJoyStick = false;
				joyStick->setPosition(joyStickOrigin);
				joyStickFirstTouchPosition = cocos2d::Vec2();
				joyStickDragged = cocos2d::Vec2();
				continue;
			}
		}
	};

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchesListener, this);
}


/////////////////////////////////////////////
// updating

void GameScene::countingFrames()
{
	countedFrames++;
	if (countedFrames >= 1000)
		countedFrames = 0;
}


void GameScene::updateBackgroundAndPlatform(float dt)
{
	float shiftLeftDistance = BACKGROUND_SHIFT_DISTANCE * dt;

	platform1->setPositionX(platform1->getPositionX() - shiftLeftDistance);
	platform2->setPositionX(platform1->getPositionX() + platform1->getContentSize().width);

	background1->setPositionX(background1->getPositionX() - shiftLeftDistance / 4);
	background2->setPositionX(background1->getPositionX() + background1->getContentSize().width);

	float landMarkPosX = origin.x + visibleSize.width / 2;
	if (platform2->getPositionX() <= landMarkPosX)
		resetPlatformPosition();
	if (background2->getPositionX() <= landMarkPosX)
		resetBackgroundPosition();
}


void GameScene::updateRemainingDistance()
{
	if (countedFrames % 100 == 0)	{
		
		if (remainingDistance > 0) {
			
			remainingDistance--;
			hud->updateRemainingDistance(remainingDistance);
		}
	}
}


void GameScene::updatePlayerDirection()
{
	player->setPlayerMovementDirection(joyStickDragged);
}


void GameScene::updatePlayerFuel()
{
	if (countedFrames % 200 == 0)	{
		
		player->decreaseFuel();
		hud->updatePlayerFuel(player->getCurrentFuel());
	}
}


void GameScene::updatePlayerScore()
{
	hud->updatePlayerScore(player->getScore());
}

/////////////////////////////////////////////
// slots


void GameScene::playBossBattleMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/boss_battle_music.mp3", true);
}


void GameScene::stopGameMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}


void GameScene::resumeGameMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
}


void GameScene::resetPlatformPosition()
{
	platform1->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y
		)
	);

	platform2->setPosition(
		cocos2d::Vec2(
			platform1->getPositionX() + platform1->getContentSize().width,
			platform1->getPositionY()
		)
	);
}


void GameScene::resetBackgroundPosition()
{
	background1->setPosition(
		cocos2d::Vec2(
			origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height
		)
	);

	background2->setPosition(
		cocos2d::Vec2(
			background1->getPositionX() + background1->getContentSize().width,
			background1->getPositionY()
		)
	);
}


void GameScene::replaceFinishedScene()
{
	stopGameMusic();
	auto finishedScene = FinishedScene::createScene();
	cocos2d::Director::getInstance()->replaceScene(cocos2d::CCTransitionFade::create(TRANSITION_FADE_TIME * 2, finishedScene));
}


void GameScene::replaceGameOverScene()
{
	stopGameMusic();
	auto gameOverScene = GameOverScene::createScene();
	cocos2d::Director::getInstance()->replaceScene(gameOverScene);
}


///////////////////////////////////////////////////////////
// physics contact detection

bool GameScene::onContactBegin(cocos2d::PhysicsContact & contact)
{
	auto shapeA = contact.getShapeA();
	auto shapeB = contact.getShapeB();
	auto nodeA = shapeA->getBody()->getNode();
	auto nodeB = shapeB->getBody()->getNode();

	auto bitmaskA = shapeA->getCollisionBitmask();
	auto bitmaskB = shapeB->getCollisionBitmask();
	
	//Player collided with obstacle
	if (
		(bitmaskA == (int)CollisionBitmask::PLAYER_COLLISTION_BITMASK && bitmaskB == (int)CollisionBitmask::OBSTACLE_COLLISION_BITMASK) ||
		(bitmaskA == (int)CollisionBitmask::OBSTACLE_COLLISION_BITMASK && bitmaskB == (int)CollisionBitmask::PLAYER_COLLISTION_BITMASK)
		) 
	{
		_eventDispatcher->removeAllEventListeners();
		this->unschedule(schedule_selector(GameScene::playerShooting));
		player->explodingHelicopter();
		this->runAction(
			cocos2d::Sequence::create(
				cocos2d::DelayTime::create(SCHEDULE_TRANSITION_TIME),
				cocos2d::CallFunc::create(this, callfunc_selector(GameScene::replaceGameOverScene)),
				NULL
			)
		);
		return true;
	}

	//Player collided with enemy bullet
	if (
		(bitmaskA == (int)CollisionBitmask::PLAYER_COLLISTION_BITMASK && bitmaskB == (int)CollisionBitmask::ENEMY_BULLET_BISMASK) ||
		(bitmaskA == (int)CollisionBitmask::ENEMY_BULLET_BISMASK && bitmaskB == (int)CollisionBitmask::PLAYER_COLLISTION_BITMASK)
		)
	{
		_eventDispatcher->removeAllEventListeners();
		this->unschedule(schedule_selector(GameScene::playerShooting));
		player->explodingHelicopter();
		this->runAction(
			cocos2d::Sequence::create(
				cocos2d::DelayTime::create(SCHEDULE_TRANSITION_TIME),
				cocos2d::CallFunc::create(this, callfunc_selector(GameScene::replaceGameOverScene)),
				NULL
			)
		);
		return true;
	}

	//Player collided with enemy physics body
	if (
		(bitmaskA == (int)CollisionBitmask::PLAYER_COLLISTION_BITMASK && bitmaskB == (int)CollisionBitmask::ENEMY_COLLISION_BITMASK) ||
		(bitmaskA == (int)CollisionBitmask::ENEMY_COLLISION_BITMASK && bitmaskB == (int)CollisionBitmask::PLAYER_COLLISTION_BITMASK)
		)
	{
		_eventDispatcher->removeAllEventListeners();
		this->unschedule(schedule_selector(GameScene::playerShooting));
		player->explodingHelicopter();
		this->runAction(
			cocos2d::Sequence::create(
				cocos2d::DelayTime::create(SCHEDULE_TRANSITION_TIME),
				cocos2d::CallFunc::create(this, callfunc_selector(GameScene::replaceGameOverScene)),
				NULL
			)
		);
		return true;
	}

	//Player collided with boss physics body
	if (
		(bitmaskA == (int)CollisionBitmask::PLAYER_COLLISTION_BITMASK && bitmaskB == (int)CollisionBitmask::BOSS_COLLISION_BITMASK) ||
		(bitmaskA == (int)CollisionBitmask::BOSS_COLLISION_BITMASK && bitmaskB == (int)CollisionBitmask::PLAYER_COLLISTION_BITMASK)
		)
	{
		_eventDispatcher->removeAllEventListeners();
		this->unschedule(schedule_selector(GameScene::playerShooting));
		player->explodingHelicopter();
		this->runAction(
			cocos2d::Sequence::create(
				cocos2d::DelayTime::create(SCHEDULE_TRANSITION_TIME),
				cocos2d::CallFunc::create(this, callfunc_selector(GameScene::replaceGameOverScene)),
				NULL
			)
		);
		return true;
	}
	
	//Player collided with gascan
	if (
		bitmaskA == (int)CollisionBitmask::PLAYER_COLLISTION_BITMASK && 
		bitmaskB == (int)CollisionBitmask::GASCAN_COLLISION_BITMASK
		)
	{
		shapeB->getBody()->getNode()->removeFromParentAndCleanup(false);
		shapeB->getBody()->removeFromWorld();
		nodeB->release();
		return true;
	}

	//in opposite, gascan collided with player
	if (
		bitmaskA == (int)CollisionBitmask::GASCAN_COLLISION_BITMASK &&
		bitmaskB == (int)CollisionBitmask::PLAYER_COLLISTION_BITMASK
		)
	{
		shapeA->getBody()->getNode()->removeFromParentAndCleanup(false);
		shapeA->getBody()->removeFromWorld();
		nodeA->release();
		return true;
	}

	//Player's bullet collided with obstacle, then remove bullet from scene
	if (
		bitmaskA == (int)CollisionBitmask::PLAYER_BULLET_BITMASK &&
		bitmaskB == (int)CollisionBitmask::OBSTACLE_COLLISION_BITMASK
		)
	{
		shapeA->getBody()->getNode()->removeFromParentAndCleanup(false);
		shapeA->getBody()->removeFromWorld();
		nodeA->release();
		return true;
	}

	//Player's bullet collided with obstacle, then remove bullet from scene
	if (
		bitmaskA == (int)CollisionBitmask::OBSTACLE_COLLISION_BITMASK &&
		bitmaskB == (int)CollisionBitmask::PLAYER_BULLET_BITMASK
		)
	{
		shapeB->getBody()->getNode()->removeFromParentAndCleanup(false);
		shapeB->getBody()->removeFromWorld();
		nodeB->release();
		return true;
	}

	//Player's bullet collided with boss, remove it from scene too
	if (
		bitmaskA == (int)CollisionBitmask::PLAYER_BULLET_BITMASK &&
		bitmaskB == (int)CollisionBitmask::BOSS_COLLISION_BITMASK
		)
	{
		shapeA->getBody()->getNode()->removeFromParentAndCleanup(false);
		shapeA->getBody()->removeFromWorld();
		nodeA->release();
		return true;
	}

	//In opposite, boss collided with player's bullet, remove the bullet
	if (
		bitmaskA == (int)CollisionBitmask::BOSS_COLLISION_BITMASK &&
		bitmaskB == (int)CollisionBitmask::PLAYER_BULLET_BITMASK
		)
	{
		shapeB->getBody()->getNode()->removeFromParentAndCleanup(false);
		shapeB->getBody()->removeFromWorld();
		nodeB->release();
		return true;
	}

	return true;
}


bool GameScene::onPlayerContactWithGascan(cocos2d::PhysicsContact & contact)
{
	player->increaseFuel(__GASCAN_FUEL_APPLY__);
	player->increaseScore(__GASCAN_SCORE__);

	return true;
}


bool GameScene::onPlayerBulletContactWithMiniBoss1(cocos2d::PhysicsContact & contact)
{
	miniBoss->decreaseHealth1();
	if (miniBoss->isBossDead1()) {
		player->increaseScore(__MINI_BOSS_SCORE__);
	}

	if (miniBoss->isBossDead2() && miniBoss->isBossDead1()) {
		invokeMotherFucker();
	}

	return true;
}


bool GameScene::onPlayerBulletContactWithMiniBoss2(cocos2d::PhysicsContact & contact)
{
	miniBoss->decreaseHealth2();
	if (miniBoss->isBossDead2()) {
		player->increaseScore(__MINI_BOSS_SCORE__);
	}

	if (miniBoss->isBossDead1() && miniBoss->isBossDead2()) {
		invokeMotherFucker();
	}

	return true;
}


bool GameScene::onPlayerBulletContactWithMotherFucker(cocos2d::PhysicsContact & contact)
{
	motherFucker->decreaseHealth();
	if (motherFucker->isBossDead()) {
		player->increaseScore(__MOTHER_FUCKER_SCORE__);
		this->runAction(
			cocos2d::Sequence::create(
				cocos2d::DelayTime::create(SCHEDULE_TRANSITION_TIME),
				cocos2d::CallFunc::create(this, callfunc_selector(GameScene::replaceFinishedScene)),
				NULL
			)
		);
	}

	return true;
}

///////////////////////////////////////////////
// schedule

void GameScene::spawnTank(float t)
{
	auto gun = AntiAircraftGun::spawnGun(this);
}


void GameScene::spawnMissile(float t)
{
	auto missile = Missile::spawnMissile(this, player->getPlayerCurrentPosition().y);
}


void GameScene::spawnTower(float t)
{
	auto tower = Tower::spawnTower(this);
}


void GameScene::spawnGascan(float t)
{
	auto currentGascan = Gascan::spawnGascan(this, player->getPlayerCurrentPosition().x);

	auto gascanBody = currentGascan->getGascanPhysicsBody();
	auto listenerPlayerAndGascan = cocos2d::EventListenerPhysicsContactWithBodies::create(player->getPlayerPhysicsBody(), gascanBody);
	listenerPlayerAndGascan->onContactBegin = CC_CALLBACK_1(GameScene::onPlayerContactWithGascan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerPlayerAndGascan, this);
}


void GameScene::playerShooting(float t)
{
	PlayerBullet *bullet = player->createBullet(t);

	auto bulletBody = bullet->getBulletSprite()->getPhysicsBody();
	auto listenerBulletAndMiniBoss1 = cocos2d::EventListenerPhysicsContactWithBodies::create(bulletBody, miniBoss->getBossBody1());
	auto listenerBulletAndMiniBoss2 = cocos2d::EventListenerPhysicsContactWithBodies::create(bulletBody, miniBoss->getBossBody2());
	auto listenerBulletAndFinalBoss = cocos2d::EventListenerPhysicsContactWithBodies::create(bulletBody, motherFucker->getBossBody());

	listenerBulletAndMiniBoss1->onContactBegin = CC_CALLBACK_1(GameScene::onPlayerBulletContactWithMiniBoss1, this);
	listenerBulletAndMiniBoss2->onContactBegin = CC_CALLBACK_1(GameScene::onPlayerBulletContactWithMiniBoss2, this);
	listenerBulletAndFinalBoss->onContactBegin = CC_CALLBACK_1(GameScene::onPlayerBulletContactWithMotherFucker, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerBulletAndMiniBoss1, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerBulletAndMiniBoss2, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerBulletAndFinalBoss, this);
}


void GameScene::spawningObstacle()
{
	this->schedule(schedule_selector(GameScene::spawnTank), ANTI_AIRCRAFT_SPAWN_DURATION);
	this->schedule(schedule_selector(GameScene::spawnMissile), RANGE_MISSILE_SPAWN_DURATION);
	this->schedule(schedule_selector(GameScene::spawnTower), TOWER_SPAWN_DURATION);
}


void GameScene::stopSpawningObstacle()
{
	this->unschedule(schedule_selector(GameScene::spawnTank));
	this->unschedule(schedule_selector(GameScene::spawnMissile));
	this->unschedule(schedule_selector(GameScene::spawnTower));
}


void GameScene::activatedMiniBoss(float t)
{
	miniBoss->start();
}


void GameScene::startMiniBossLoop(float t)
{
	miniBoss->setupLoopAction();
}


void GameScene::activatedMotherFucker(float t)
{
	motherFucker->start();
}


void GameScene::startMotherFuckerLoop(float t)
{
	motherFucker->setupLoopAction();
}


void GameScene::invokeMiniBoss()
{
	playBossBattleMusic();
	miniBoss->setStarted(true);
	hud->warning();
	this->scheduleOnce(schedule_selector(GameScene::activatedMiniBoss), MINI_BOSS_PREPARE_DURATION);
	this->scheduleOnce(schedule_selector(GameScene::startMiniBossLoop), MINI_BOSS_PREPARE_DURATION * 2 + 0.5F);
}


void GameScene::invokeMotherFucker()
{
	motherFucker->setStarted(true);
	hud->warning();
	this->scheduleOnce(schedule_selector(GameScene::activatedMotherFucker), BIG_BOSS_PREPARE_DURATION);
	this->scheduleOnce(schedule_selector(GameScene::startMotherFuckerLoop), BIG_BOSS_PREPARE_DURATION * 2 + 0.5F);
}


void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event * e)
{
	if (key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
		
		if (isGameRunning) 
		{
			isGameRunning = false;
			hud->onGamePause();
		}
		else
		{
			isGameRunning = true;
			hud->onGameResume();
		}
	}
}


void GameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event * e)
{

}


cocos2d::Node * GameScene::getNodeUnderTouch(cocos2d::Touch * touch, cocos2d::Node *target)
{
	cocos2d::Node *node = nullptr;

	auto location = touch->getLocation();

	//retrive all physics shape under touch location by running scene
	auto runningScene = cocos2d::Director::getInstance()->getRunningScene();
	auto shapes = runningScene->getPhysicsWorld()->getShapes(location);

	for (auto shape : shapes) {
		if (shape->getBody()->getNode() == target) {
			node = shape->getBody()->getNode();
			break;
		}
	}

	return node;
}
