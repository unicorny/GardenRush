#include "DragSeed.h"
#include "MainGameScene.h"
#include "nodes/Grid.h"
#include "nodes/PlantNode.h"
#include "cocos2d.h"

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
		auto pos = grid->fromLocalToWorld(plantNode->getPosition());
		plantNode->removeFromGrid();
		mMainGameScene->addChild(plantNode, 20);
		plantNode->setPosition(pos);
		return true;
	}
	bool DragSeed::onExitState()
	{
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_NONE);
		return true;
	}
	void DragSeed::putBackPlantNode()
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		auto gridIndex = plantNode->getGridIndex();
		plantNode->getParentGrid()->addGridCell(plantNode, gridIndex.x, gridIndex.y);
	}

	void DragSeed::onTouchEnded(GridType type, uint8_t x, uint8_t y)
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		if (plantNode->getReferenceCount() == 1) {
			plantNode->retain();
		}
		mMainGameScene->removeChild(plantNode, false);
		auto pos = plantNode->getPosition();

		if (GRID_ERROR == type || GRID_BUCKET == type || !mMainGameScene->getGrid(type)->isCellEmptyAndFree(x, y)) {
			putBackPlantNode();
			plantNode->setPosition(plantNode->getParentGrid()->fromWorldToLocal(pos));
			if (GRID_BUCKET == type) {
				plantNode->setGlobalZOrder(1.0f);
			}
			mMainGameScene->transitTo("DropSeedInvalid");
		}
		else {
			auto grid = mMainGameScene->getGrid(type);
			
			//plantNode->removeFromGrid();
			grid->addGridCell(plantNode, x, y);
			plantNode->setPosition(grid->fromWorldToLocal(pos));
			plantNode->setParentGrid(mMainGameScene->getGrid(type));
			mMainGameScene->transitTo("DropSeedValid");
		}
	}
	void DragSeed::onTouchCancelled()
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		putBackPlantNode();
		plantNode->setPosition(plantNode->getParentGrid()->fromWorldToLocal(plantNode->getPosition()));
		mMainGameScene->transitTo("DropSeedInvalid");
	}
	void DragSeed::onCancelState()
	{
		putBackPlantNode();
		auto plantNode = mMainGameScene->getTargetPlantNode();
	}
	void DragSeed::onTouchMoved(float deltaX, float deltaY)
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		plantNode->setPosition(plantNode->getPosition() + cocos2d::Vec2(deltaX, -deltaY));

	}
}