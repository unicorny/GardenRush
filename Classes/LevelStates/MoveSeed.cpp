#include "MoveSeed.h"
#include "PlayerChooseCell.h"
#include "nodes/PlantNode.h"

namespace level_state {


	bool MoveSeed::initState()
	{
		return true;
	}
	bool MoveSeed::onEnterState(iLevelState* lastState)
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		if (!plantNode) {
			mMainGameScene->transitTo("PlayerChooseCell");
		}
		else {
			if (!lastState || strcmp(lastState->getName(), "PlayerChooseCell") != 0) {
				LOG_ERROR("error lastState isn't set or hasn't excpected type", false);
			}
			auto lastStateCastet = dynamic_cast<PlayerChooseCell*>(lastState);
			auto targetGridCell = lastStateCastet->getTargetGridCell();

			// disable grid glow
			mMainGameScene->getGrid(GRID_MAIN)->disableAllGlowCells();
			mMainGameScene->getGrid(GRID_INVENTORY)->disableAllGlowCells();
			mMainGameScene->getGrid(GRID_BUCKET)->disableAllGlowCells();

			// 
			plantNode->removeFromGrid();
			/*targetGridCell->addGridCell(plantNode, x, y);
			plantNode->setPosition(currentGrid->fromWorldToLocal(globalPlantPos));
			plantNode->setParentGrid(currentGrid);
			mMainGameScene->transitTo("DropSeedValid");
			*/
		}
		return true;
	}
	bool MoveSeed::onExitState()
	{
		return true;
	}
	void MoveSeed::onCancelState()
	{

	}

	void MoveSeed::animationEnd()
	{
		mMainGameScene->transitTo("PlantSeed");
	}
}