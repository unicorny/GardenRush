#include "scenes/StoryScene.h"
#include "lib/ProfilerManager.h"
#include "lib/TimeProfiler.h"

StoryScene::StoryScene()
{

}

StoryScene::~StoryScene()
{

}


bool StoryScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}


	return true;
}

bool StoryScene::reset()
{
	return true;
}


unsigned long StoryScene::getMemoryConsumption()
{
	return sizeof(StoryScene);
}