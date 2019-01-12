#include "scenes/MainMenuScene.h"
#include "lib/ProfilerManager.h"
#include "lib/TimeProfiler.h"
#include "ui/GuiManager.h"
#include "ErrorLog.h"

using namespace cocos2d;

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

	LayerColor *_bgColor = LayerColor::create(Color4B(60, 122, 88, 255));
	_bgColor->setGlobalZOrder(-10.0f);
	this->addChild(_bgColor, -10);

	return true;
}

bool MainMenuScene::reset()
{
	auto guiManager = GuiManager::getInstance();
	if (!guiManager->switchToGuiLayout(GUI_LAYOUT_MAIN_MENU)) {
		LOG_ERROR("Error switching to main menu gui layout", false);
	}


	return true;
}

unsigned long MainMenuScene::getMemoryConsumption()
{
	return sizeof(MainMenuScene);
}