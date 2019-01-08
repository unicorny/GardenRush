#include "scenes/MainMenuScene.h"
#include "lib/ProfilerManager.h"
#include "lib/TimeProfiler.h"

MainMenuScene::MainMenuScene()
{

}

MainMenuScene::~MainMenuScene()
{

}


bool MainMenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}


	return true;
}

bool MainMenuScene::reset()
{
	return true;
}

unsigned long MainMenuScene::getMemoryConsumption()
{
	return sizeof(MainMenuScene);
}