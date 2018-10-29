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
		auto ressources = mMainGameScene->getRessourcenManager();
		auto plantNode = mMainGameScene->getTargetPlantNode();
		auto plantTypesManager = mMainGameScene->getPlantTypesManager();
		auto grid = plantNode->getParentGrid();
		mMainGameScene->getGrid(GRID_MAIN)->glowEmptyCells(ressources, true);
		mMainGameScene->getGrid(GRID_INVENTORY)->glowEmptyCells(ressources, true);
		mMainGameScene->getGrid(GRID_MAIN)->glowNeighborCells(plantNode->getPlantType(), plantTypesManager, ressources, true);
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
		auto ressources = mMainGameScene->getRessourcenManager();
		mMainGameScene->getGrid(GRID_MAIN)->disableAllGlowCells();
		mMainGameScene->getGrid(GRID_INVENTORY)->glowEmptyCells(ressources, false);

		auto plantNode = mMainGameScene->getTargetPlantNode();
		auto pos = plantNode->getPosition();

		if (GRID_ERROR == type || GRID_BUCKET == type || !mMainGameScene->getGrid(type)->isCellEmptyAndFree(x, y)) {
			if (plantNode->getReferenceCount() == 1) {
				plantNode->retain();
			}

			mMainGameScene->removeChild(plantNode, false);
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
			// instead of adding, we fade it out and fade in the new state
			if (GRID_MAIN == type) {
				auto plantType = plantNode->getPlantType();// ->getViewData(PLANT_PHASIS_SEEDED);
				auto seededPlantNode = plantType->getViewData(PLANT_PHASIS_SEEDED)->createPlantNode(plantType);
				mMainGameScene->getGrid(GRID_MAIN)->addGridCell(seededPlantNode, x, y);
				seededPlantNode->setOpacity(0);		

				const float fadeTime = 0.5f;

				auto seededPlantNodeSequence = Sequence::create(
					FadeIn::create(fadeTime),
					CallFunc::create(CC_CALLBACK_0(DragSeed::animationEnd, this)),
					nullptr);
				seededPlantNode->runAction(seededPlantNodeSequence);
				mMainGameScene->setTargetPlantNode(seededPlantNode);

				auto plantNodeSequence = Sequence::create(
					FadeOut::create(fadeTime),
					RemoveSelf::create(true),
					nullptr);
				plantNode->runAction(plantNodeSequence);
			}
			else if (GRID_INVENTORY == type) {
				if (plantNode->getReferenceCount() == 1) {
					plantNode->retain();
				}
				mMainGameScene->removeChild(plantNode, false);
				grid->addGridCell(plantNode, x, y);
				plantNode->setPosition(grid->fromWorldToLocal(pos));
				plantNode->setParentGrid(mMainGameScene->getGrid(type));
				mMainGameScene->transitTo("DropSeedValid");
			}
			//mMainGameScene->transitTo("RandomSeed");
		}
	}
	void DragSeed::onTouchCancelled()
	{
		auto ressources = mMainGameScene->getRessourcenManager();
		mMainGameScene->getGrid(GRID_MAIN)->disableAllGlowCells();
		mMainGameScene->getGrid(GRID_INVENTORY)->glowEmptyCells(ressources, false);

		auto plantNode = mMainGameScene->getTargetPlantNode();
		if (plantNode->getReferenceCount() == 1) {
			plantNode->retain();
		}
		mMainGameScene->removeChild(plantNode, false);
		auto pos = plantNode->getPosition();
		putBackPlantNode();
		plantNode->setPosition(plantNode->getParentGrid()->fromWorldToLocal(pos));
		mMainGameScene->transitTo("DropSeedInvalid");
	}
	void DragSeed::onCancelState()
	{
		mMainGameScene->getGrid(GRID_MAIN)->glowEmptyCells(false);
		mMainGameScene->getGrid(GRID_INVENTORY)->glowEmptyCells(false);

		putBackPlantNode();
		//auto plantNode = mMainGameScene->getTargetPlantNode();
	}
	void DragSeed::onTouchMoved(float deltaX, float deltaY)
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		plantNode->setPosition(plantNode->getPosition() + cocos2d::Vec2(deltaX, -deltaY));
	}

	void DragSeed::animationEnd()
	{
		mMainGameScene->transitTo("PlantSeed");
	}
}