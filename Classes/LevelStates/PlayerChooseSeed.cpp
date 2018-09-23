#include "PlayerChooseSeed.h"
#include "levelStates/iLevelState.h"
#include "nodes/Grid.h"
#include "cocos2d.h"
#include "MainGameScene.h"
#include "nodes/PlantNode.h"

#include "ErrorLog.h"
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
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_BEGIN);
		

		return true;
	}

	bool PlayerChooseSeed::onExitState()
	{
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_NONE);
		return true;
	}

	void PlayerChooseSeed::onTouchBegan(PlantNode* plantNode)
	{
		//ErrorLog::printf("touchPlant");
		if (plantNode) {
			mMainGameScene->setTargetPlantNode(plantNode);
			if (GRID_MAIN == plantNode->getParentGrid()->getType()) {
				mMainGameScene->transitTo("DisplayInfo");
			}
			else {
				mMainGameScene->transitTo("PlayerChooseActionWithSeed");
			}
		}
	}

	void PlayerChooseSeed::onCancelState()
	{

	}

};