#include "GameScene.h"
#include "GameSettings.h"
#include "Enumeration.h"
#include "SimpleAudioEngine.h"
#include "PauseScene.h"
#include "FinishedScene.h"
#include "GameOverScene.h"
#include "Toast.h"

#include "Enemy\Missile.h"
#include "Enemy\AntiAircraftGun.h"
#include "Enemy\Tower.h"

#include "Player\Gascan.h"


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
	initButtons();

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


void GameScene::initButtons()
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

	this->addChild(buttonFire);

	//////////////////////////////////////////
	// loadjoy stick origin sprite
	joyStickOrigin = cocos2d::Sprite::create("images/buttons/joy_stick_origin.png");
	joyStickOrigin->setPosition(cocos2d::Vec2());
	joyStickOrigin->setVisible(false);

	this->addChild(joyStickOrigin);

	//////////////////////////////////////////
	// load joy stick sprite
	joyStick = cocos2d::Sprite::create("images/buttons/joy_stick.png");
	joyStick->setPosition(cocos2d::Vec2());
	joyStick->setVisible(false);

	this->addChild(joyStick);

	movementVector = cocos2d::Vec2();
}


void GameScene::initMiniBoss()
{
	miniBoss = new MiniBoss(this);
}


void GameScene::initMotherFucker()
{
	motherFucker = new MotherFucker(this);
}


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
	
	cocos2d::Sprite *&jStickOrigin = joyStickOrigin;
	cocos2d::Sprite *&jStick = joyStick;
	cocos2d::Sprite *&bFire = buttonFire;
	Player *&pPlayer = player;

	touchesListener->onTouchesBegan = 
	[&](const std::vector<cocos2d::Touch *> touches, cocos2d::Event *event) {
		
		for (auto touch : touches) {
			//Check if player touch on button fire
			Node *node = this->getNodeUnderTouch(touch, bFire);
			if (node && pPlayer->stillUnderControl()) {
				bFire->setScale(1.1F);
				player->createBullet(0);
				this->schedule(schedule_selector(GameScene::playerShooting), __PLAYER_RELOAD_DURATION__);
			}


		}
	};

	touchesListener->onTouchesMoved =
	[&](const std::vector<cocos2d::Touch *> touches, cocos2d::Event *event) {

	};
	
	touchesListener->onTouchesEnded = 
	[&](const std::vector<cocos2d::Touch *> touches, cocos2d::Event *event) {
		
		//Check if player untouched button fire
		for (auto touch : touches) {
			Node *node = this->getNodeUnderTouch(touch, bFire);
			if (node) {
				bFire->setScale(1.0F);
				this->unschedule(schedule_selector(GameScene::playerShooting));
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


void GameScene::updatePlayerFuel()
{
	if (countedFrames % 200 == 0)	{
		
		player->decreaseFuel();
		hud->updatePlayerFuel(player->getCurrentFuel());
	}
}


void GameScene::updatePlayerScore()
{

}

/////////////////////////////////////////////
// slots

void GameScene::stopGameMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}


void GameScene::resumeGameMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
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


void GameScene::pauseGame()
{
	auto pauseScene = PauseScene::createScene();
	cocos2d::Director::getInstance()->pushScene(pauseScene);
}


void GameScene::replaceFinishedScene()
{
	auto finishedScene = FinishedScene::createScene();
	cocos2d::Director::getInstance()->replaceScene(cocos2d::CCTransitionFade::create(TRANSITION_FADE_TIME, finishedScene));
}


void GameScene::replaceGameOverScene()
{
	auto gameOverScene = GameOverScene::createScene();
	cocos2d::Director::getInstance()->replaceScene(gameOverScene);
}


bool GameScene::onContactBegin(cocos2d::PhysicsContact & contact)
{
	auto shapeA = contact.getShapeA();
	auto shapeB = contact.getShapeB();

	auto bitmaskA = shapeA->getCollisionBitmask();
	auto bitmaskB = shapeB->getCollisionBitmask();



	return true;
}


bool GameScene::onPlayerBulletContactWithMiniBoss1(cocos2d::PhysicsContact & contact)
{
	miniBoss->decreaseHealth1();
	if (miniBoss->isBossDead1()) {
		
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
		
	}

	if (miniBoss->isBossDead1() && miniBoss->isBossDead2()) {
		invokeMotherFucker();
	}

	return true;
}


bool GameScene::onPlayerBulletContactWithMotherFucker(cocos2d::PhysicsContact & contact)
{
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
	auto missile = Missile::spawnMissile(this, origin.y + visibleSize.height / 2);
}


void GameScene::spawnTower(float t)
{
	auto tower = Tower::spawnTower(this);
}


void GameScene::spawnGascan(float t)
{
	auto gascan = Gascan::spawnGascan(this);
}


void GameScene::playerShooting(float t)
{
	player->createBullet(t);
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
