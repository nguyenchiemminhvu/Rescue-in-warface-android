#include "AppDelegate.h"
#include "Scene\SplashScene.h"
#include "SimpleAudioEngine.h"
#include "GameSettings.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(720, 480);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {
	
}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

	GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//glview = GLViewImpl::createWithRect("AndroidGame", Rect(0, 0, mediumResolutionSize.width, mediumResolutionSize.height));
		glview = GLViewImpl::createWithRect("AndroidGame", cocos2d::Rect(cocos2d::Vec2(0, 0), smallResolutionSize));
		//glview = GLViewImpl::createWithFullScreen("AndroidGame");
#else
		glview = GLViewImpl::create("AndroidGame");
#endif
		director->setOpenGLView(glview);
	}
	
	// turn on display FPS
	director->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);

	// Set the design resolution
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	glview->setDesignResolutionSize(mediumResolutionSize.width, mediumResolutionSize.height, ResolutionPolicy::EXACT_FIT);
#else
	glview->setDesignResolutionSize(mediumResolutionSize.width, mediumResolutionSize.height, ResolutionPolicy::EXACT_FIT);
#endif
	
	register_all_packages();

	// create a scene. it's an autorelease object
	auto scene = SplashScene::createScene();

	// run
	director->runWithScene(scene);

	// preload sound effects
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(GameSettings::getInstance()->effectVolume);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sounds/helicopter_sound.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/missile_sound.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/tank_sound.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sounds/tower_sound.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sounds/game_music.mp3");

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
