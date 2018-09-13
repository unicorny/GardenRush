#include "PlayerChooseSeed.h"
#include "levelStates/LevelStateManager.h"
#include "nodes/Grid.h"
#include "cocos2d.h"
#include "MainGameScene.h"

using namespace cocos2d;
namespace level_state {


	PlayerChooseSeed::PlayerChooseSeed()
	{
	}

	bool PlayerChooseSeed::initState()
	{
		return true;
	}
	bool PlayerChooseSeed::onEnterState()
	{
		auto gameScene = mLevelStateManager->getMainGameScene();

		//gameScene->getGrid(GRID_BUCKET)->listenOnCellTouch(CC_CALLBACK_1(PlayerChooseSeed::touchPlant, this));
		gameScene->enableTouchListen();

		return true;
	}

	bool PlayerChooseSeed::onExitState()
	{
		return true;
	}

	bool PlayerChooseSeed::touchPlant(PlantNode* plantNode)
	{
		return true;
	}

};