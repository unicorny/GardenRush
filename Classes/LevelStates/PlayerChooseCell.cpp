#include "PlayerChooseCell.h"
#include "nodes/PlantNode.h"

namespace level_state {

	bool PlayerChooseCell::initState()
	{
		return true;
	}

	bool PlayerChooseCell::onEnterState()
	{
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_BEGIN_GRID);
		return true;
	}
	bool PlayerChooseCell::onExitState()
	{
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_NONE);
		return true;
	}
	void PlayerChooseCell::onCancelState()
	{
		
	}
	/* 
	 * If choosen a empty cell,											transit to MoveSeed
	 * If choosen a seeded plant on Main Grid,							transit to PlayerChooseActionWithSeeded
	 * If choosen a seed on inventory or bucket grid,					transit to PlayerChooseActionWithSeed
	 */
	void PlayerChooseCell::onTouchBegan(GridType type, uint8_t x, uint8_t y)
	{
		auto grid = mMainGameScene->getGrid(type);
		auto plantNode = grid->getPlantNodeFromIndex(GridIndex(x, y));
		auto targetPlantNode = mMainGameScene->getTargetPlantNode();

		// if empty cell
		if (!plantNode) 
		{
			//mMainGameScene->setTargetPlantNode(nullptr);
			if (targetPlantNode) {
				//auto targetGridCell = GridCell(x, y, type);

				setGridCell(GridCell(x, y, type));
				mMainGameScene->transitTo("DropSeedValid");
			}
		}
		else
		{
			mMainGameScene->setTargetPlantNode(plantNode);

			if (GRID_MAIN == type) mMainGameScene->transitTo("PlayerChooseActionWithSeeded"); 
			else				   mMainGameScene->transitTo("PlayerChooseActionWithSeed");
		}

	}

}