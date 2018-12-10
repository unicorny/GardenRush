#include "PlayerChooseActionWithSeeded.h"

#include "nodes/PlantNode.h"

namespace level_state {
	bool PlayerChooseActionWithSeeded::initState()
	{
		return true;
	}

	bool PlayerChooseActionWithSeeded::onEnterState()
	{
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_END | ENABLED_TOUCH_CANCELLED);

		auto plantNode = mMainGameScene->getTargetPlantNode();
		assert(plantNode != nullptr);

		auto plantNodeParentGrid = plantNode->getParentGrid();
		assert(plantNodeParentGrid != nullptr);

		plantNodeParentGrid->glowSelectedCell(plantNode->getGridIndex(), true);
		mGlowCellGrid = plantNodeParentGrid->getType();

		return true;
	}

	bool PlayerChooseActionWithSeeded::onExitState()
	{
		mMainGameScene->getGrid(mGlowCellGrid)->disableAllGlowCells();
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_NONE);

		return true;
	}

	void PlayerChooseActionWithSeeded::onCancelState()
	{
		
	}

	void PlayerChooseActionWithSeeded::onTouchEnded(GridType type, uint8_t x, uint8_t y)
	{
		// check if outgrown, else switch to show info
		auto plantNode = mMainGameScene->getTargetPlantNode();
		assert(plantNode != nullptr);
		if (plantNode->isOutGrown()) {
			mMainGameScene->transitTo("HarvestPlant");
		}
		else {
			mMainGameScene->transitTo("ShowInfo");
		}
	}

	void PlayerChooseActionWithSeeded::onTouchCancelled()
	{
		mMainGameScene->transitTo("PlayerChooseCell");
	}
}