#include "DropSeedValid.h"
#include "nodes/PlantNode.h"
#include "nodes/Grid.h"

using namespace cocos2d;

namespace level_state {

	bool DropSeedValid::initState()
	{
		return true;
	}

	bool DropSeedValid::onEnterState(iLevelState* lastState)
	{
		mMainGameScene->getGrid(GRID_MAIN)->disableAllGlowCells();
		mMainGameScene->getGrid(GRID_INVENTORY)->glowEmptyCells(false);
		mMainGameScene->getGrid(GRID_BUCKET)->glowEmptyCells(false);

		auto gridCell = lastState->getGridCell();
		if (!gridCell) {
			mMainGameScene->transitTo("DropSeedInvalid");
		}
		else {
			setGridCell(*gridCell);
			auto plantNode = mMainGameScene->getTargetPlantNode();
			auto targetGrid = mMainGameScene->getGrid(gridCell->type);
			auto targetWorldPos = targetGrid->getWorldPositionForGridIndex(gridCell->index.x, gridCell->index.y);
			auto targetLocalPos = targetWorldPos - plantNode->getParent()->getPosition();
			targetLocalPos.x += targetGrid->getCellSize().x * 0.25f;

			auto visibleSize = Director::getInstance()->getVisibleSize();
			float distance = targetLocalPos.distanceSquared(plantNode->getPosition()) / Vec2(visibleSize).lengthSquared();
			float animDuration = distance * 100.0f;
			if (animDuration > 0.5f) animDuration = 0.5f;
			animDuration += 0.1f;
			//auto animationState = plantNode->getAnimationState(mMainGameScene->getAnimationStateMemoryManager());
			
			auto plantNodeSequence = Sequence::create(
				EaseIn::create(MoveTo::create(animDuration, targetLocalPos), 1.2f),
				CallFunc::create(CC_CALLBACK_0(DropSeedValid::animationEnd, this)),
				nullptr);
			plantNode->runAction(plantNodeSequence);
			//animationState->runMoveAction(plantNodeSequence, targetLocalPos);
		}

		return true;
	}
	bool DropSeedValid::onExitState()
	{
		return true;
	}

	
	void DropSeedValid::onCancelState()
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		plantNode->stopAllActions();
		plantNode->setPosition(plantNode->getParentGrid()->getOriginPosition(plantNode));
	}

	void DropSeedValid::animationEnd()
	{
		auto gridCell = getGridCell();
		if (!gridCell) {
			LOG_ERROR_VOID("gridCell is empty");
		}
		auto gridType = gridCell->type;

		if (GRID_MAIN == gridType) {
			mMainGameScene->transitTo("PlantSeed");
		}
		else if (GRID_INVENTORY == gridType) {
			auto plantNode = mMainGameScene->getTargetPlantNode();
			plantNode->removeFromGrid();
			
			auto targetGrid = mMainGameScene->getGrid(gridCell->type);
			targetGrid->addGridCell(plantNode, gridCell->index.x, gridCell->index.y);

			if (mMainGameScene->getGrid(GRID_BUCKET)->isCellEmptyAndFree(0, 0)) {
				mMainGameScene->transitTo("RandomSeed");
			}
			else {
				mMainGameScene->transitTo("PlayerChooseSeed");
			}
		}
		
	}

}