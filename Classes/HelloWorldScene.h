#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>

class HelloWorld : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	
	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);

	void update(float dt) override;

private:

	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 playerPosition;
	cocos2d::Sprite *player;
	__int32 spriteIndex;
};

#endif // __HELLOWORLD_SCENE_H__
