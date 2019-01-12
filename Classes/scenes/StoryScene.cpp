#include "controller/LuaScripting.h"
#include "controller/RessourcenManager.h"
#include "lib/ProfilerManager.h"
#include "lib/TimeProfiler.h"
#include "scenes/StoryScene.h"

#include "model/Player.h"
#include "model/Savegame.h"


StoryScene::StoryScene()
{
	scheduleUpdate();
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
	// load correct story script
	auto storyScriptName = Player::getInstance()->getSavegame()->getValueForName("story");
	auto storyScript = RessourcenManager::getInstance()->getStoryLuaByName(storyScriptName.data());
	LuaScripting::getInstance()->startScript(storyScript);
	return true;
}


unsigned long StoryScene::getMemoryConsumption()
{
	return sizeof(StoryScene);
}

void StoryScene::update(float delta)
{
	LuaScripting::getInstance()->update();
}