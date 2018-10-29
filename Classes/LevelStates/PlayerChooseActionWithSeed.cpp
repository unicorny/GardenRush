#include "PlayerChooseActionWithSeed.h"
#include "levelStates/iLevelState.h"
#include "nodes/Grid.h"
#include "nodes/PlantNode.h"
#include "cocos2d.h"
#include "MainGameScene.h"

#include "ErrorLog.h"
using namespace cocos2d;
namespace level_state {


	PlayerChooseActionWithSeed::PlayerChooseActionWithSeed()
		//: mMovedSum(Vec2::ZERO)
	{
	}

	bool PlayerChooseActionWithSeed::initState()
	{
		return true;
	}
	bool PlayerChooseActionWithSeed::onEnterState()
	{
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_END | ENABLED_TOUCH_CANCELLED | ENABLED_TOUCH_MOVE);

		return true;
	}

	bool PlayerChooseActionWithSeed::onExitState()
	{
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_NONE);
		return true;
	}

	void PlayerChooseActionWithSeed::onTouchEnded(GridType type, uint8_t x, uint8_t y)
	{		
		mMainGameScene->transitTo("DisplayInfo");
	}
	void PlayerChooseActionWithSeed::onTouchCancelled()
	{
		mMainGameScene->transitTo("PlayerChooseSeed");
	}

	void PlayerChooseActionWithSeed::onTouchMoved(float deltaX, float deltaY)
	{
		//auto plantNode = mMainGameScene->getTargetPlantNode();
		mMainGameScene->transitTo("DragSeed");
	}

	void PlayerChooseActionWithSeed::onCancelState()
	{

	}


};