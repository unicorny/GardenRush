#include "scenes/GameScene.h"
#include "lib/ProfilerManager.h"
#include "lib/TimeProfiler.h"

GameScene::GameScene()
{
	mAnimationStateMemoryManager.init(6);
}

GameScene::~GameScene()
{

}


bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}


	return true;
}


bool GameScene::reset()
{
	return true;
}

unsigned long GameScene::getMemoryConsumption()
{
	unsigned long memory = sizeof(GameScene);
	memory += mAnimationStateMemoryManager.getMemoryConsumption();
	return memory;
}