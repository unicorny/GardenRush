#include "DragSeed.h"
#include "MainGameScene.h"
#include "nodes/Grid.h"
#include "nodes/PlantNode.h"
#include "model/PlantType.h"
#include "cocos2d.h"

using namespace cocos2d;

namespace level_state {
	bool DragSeed::initState()
	{
		return true;
	}
	bool DragSeed::onEnterState()
	{
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_END | ENABLED_TOUCH_CANCELLED | ENABLED_TOUCH_MOVE);

		auto plantNode = mMainGameScene->getTargetPlantNode();
		auto grid = plantNode->getParentGrid();
		mMainGameScene->getGrid(GRID_MAIN)->glowAutoCells(plantNode->getPlantType(), mMainGameScene->getPlantTypesManager());
		mMainGameScene->getGrid(GRID_INVENTORY)->glowEmptyCells(true);
		
		return true;
	}
	bool DragSeed::onExitState()
	{
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_NONE);
		return true;
	}

	void DragSeed::onTouchEnded(GridType type, uint8_t x, uint8_t y)
	{
		

		auto plantNode = mMainGameScene->getTargetPlantNode();
		auto localPlantPost = plantNode->getPosition();
		auto globalPlantPos = localPlantPost + plantNode->getParent()->getPosition();
		auto globalPlantPosCenter = globalPlantPos + plantNode->getContentSize() * plantNode->getScale() * 0.5f;
		auto currentGrid = mMainGameScene->getGrid(type);

		auto gridIndex = currentGrid->getGridIndex(currentGrid->fromWorldToLocal(globalPlantPosCenter));

		if (GRID_ERROR == type || GRID_BUCKET == type || !currentGrid->isCellEmptyAndFree(gridIndex.x, gridIndex.y)) {
			if (GRID_BUCKET == type) {
				plantNode->setGlobalZOrder(1.0f);
			}
			mMainGameScene->transitTo("DropSeedInvalid");
		}
		else {
			setGridCell(GridCell(gridIndex.x, gridIndex.y, type));
			mMainGameScene->transitTo("DropSeedValid");
			
		}
	}
	void DragSeed::onTouchCancelled()
	{
		mMainGameScene->getGrid(GRID_MAIN)->disableAllGlowCells();
		mMainGameScene->getGrid(GRID_INVENTORY)->glowEmptyCells(false);

		mMainGameScene->transitTo("DropSeedInvalid");
	}
	void DragSeed::onCancelState()
	{
		mMainGameScene->getGrid(GRID_MAIN)->glowEmptyCells(false);
		mMainGameScene->getGrid(GRID_INVENTORY)->glowEmptyCells(false);
		
		auto plantNode = mMainGameScene->getTargetPlantNode();
		auto plantNodeGrid = plantNode->getParentGrid();
		plantNode->setPosition(plantNodeGrid->getOriginPosition(plantNode));
	}
	void DragSeed::onTouchMoved(float deltaX, float deltaY)
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		auto globalTouchPos = mMainGameScene->getCurrentTouchPosition();
		//plantNode->setPosition(plantNode->convertToNodeSpace(globalTouchPos));

		plantNode->setPosition(plantNode->getPosition() + cocos2d::Vec2(deltaX, -deltaY));
	}

	void DragSeed::animationEnd()
	{
		mMainGameScene->transitTo("PlantSeed");
	}
}