#include "controller/GameStateManager.h"
#include "model/Player.h"
#include "controller/SavegameManager.h"
#include "controller/ConfigLoader.h"
#include "controller/LuaScripting.h"
#include "controller/RessourcenManager.h"

#include "lib/ProfilerManager.h"
#include "lib/TimeProfiler.h"

#include "scenes/GameScene.h"
#include "scenes/MainMenuScene.h"
#include "scenes/StoryScene.h"

#include "ui/GuiManager.h"

//#include "lua.hpp"


GameStateManager* GameStateManager::getInstance()
{
	static GameStateManager theOneAndOnly;
	return &theOneAndOnly;
}

GameStateManager::GameStateManager()
	: mFirstScene(true)
	
{
	memset(mScenes, 0, sizeof(cocos2d::Scene*) * GAME_SCENE_SIZE);
}

GameStateManager::~GameStateManager()
{
	for (int i = 0; i < GAME_SCENE_SIZE; i++) {
		if (mScenes[i]) {
			mScenes[i]->release();
			mScenes[i] = nullptr;
		}
	}
	

}

bool GameStateManager::init()
{

	if (!LuaScripting::getInstance()->init()) {
		LOG_ERROR("error loading lua", false);
	}

	ConfigLoader::loadJsonStory("story/story.json");

	auto player = Player::getInstance();
	SavegameManager savegames;
	if (!savegames.init()) {
		LOG_ERROR("error loading savegames", false);
	}
	if (false && savegames.getSavegameCount() == 0) {
		if (!player->load(nullptr)) {
			LOG_ERROR("error loading player", false);
		}
		// start intro story
		if (!ConfigLoader::loadFromJson("graphicsConfig/introRessources.json")) {
			LOG_ERROR("loading introRessources", false);
		} 
		switchToScene(GAME_SCENE_STORY);
	}
	else {
		// start with main menu for savegame choosing
		if (!ConfigLoader::loadFromJson("graphicsConfig/mainMenuRessources.json")) {
			LOG_ERROR("loading main menu ressources", false);
		}
		switchToScene(GAME_SCENE_MAIN_MENU);
	}

	return true;
}

const char* GameStateManager::gameSceneTypeToString(GameSceneTypes type)
{
	switch (type) {
	case GAME_SCENE_MAIN_GAME: return "GameScene";
	case GAME_SCENE_MAIN_MENU: return "MainMenuScene";
	case GAME_SCENE_STORY: return "StoryScene";
	default: return "unknown";
	}
	return "error";
}

bool GameStateManager::switchToScene(GameSceneTypes type)
{
	auto profiler = ProfilerManager::getInstance();

	// create scene if not already exist
	if (!mScenes[type]) {

		TimeProfiler time;
		switch (type) {
		case GAME_SCENE_MAIN_GAME: mScenes[type] = GameScene::create(); break;
		case GAME_SCENE_MAIN_MENU: mScenes[type] = MainMenuScene::create(); break;
		case GAME_SCENE_STORY: mScenes[type] = StoryScene::create(); break;
		default: LOG_ERROR("unknow game scene type", false);
		}
		mScenes[type]->retain();
		std::string entryName = gameSceneTypeToString(type);
		entryName += "::create";
		profiler->addTimeProfilerEntry(entryName.data(), time.seconds());
		if (!mScenes[type]) {
			std::string errorString = "error creating ";
			errorString += gameSceneTypeToString(type);
			LOG_ERROR(errorString.data(), false);
		}
	}
	TimeProfiler time;
	if (!mScenes[type]->reset()) {
		std::string errorString = "error resetting ";
		errorString += gameSceneTypeToString(type);
		LOG_ERROR(errorString.data(), false);
	}
	std::string entryName = gameSceneTypeToString(type);
	entryName += "::reset";
	profiler->addTimeProfilerEntry(entryName.data(), time.seconds());

	auto director = cocos2d::Director::getInstance();

	unsigned long bytes = 0;
	unsigned int textureCount = 0;
	director->getTextureCache()->getCachedTextureInfo(bytes, textureCount);
	
	profiler->addMemoryProfilerEntry("Textures", bytes);
	profiler->addMemoryProfilerEntry("RessourcenManager", RessourcenManager::getInstance()->getMemoryConsumption());
	profiler->addMemoryProfilerEntry(gameSceneTypeToString(type), mScenes[type]->getMemoryConsumption());

	// run
	auto guiManager = GuiManager::getInstance();
	if (director->getRunningScene() != mScenes[type]) {
		if (mFirstScene) {
			director->runWithScene(mScenes[type]);
			mFirstScene = false;
		}
		else {
			director->replaceScene(mScenes[type]);
		}
		guiManager->unChild();
		mScenes[type]->addChild(guiManager);
	}
	else {
		ErrorLog::printf("[GameStateManager::switchToScene] Warning, scene already running");
	}
	

	return true;
}