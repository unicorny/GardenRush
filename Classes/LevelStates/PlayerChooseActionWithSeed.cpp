#include "PlayerChooseActionWithSeed.h"
#include "levelStates/iLevelState.h"
#include "nodes/Grid.h"
#include "nodes/PlantNode.h"
#include "model/PlantType.h"
#include "cocos2d.h"
#include "MainGameScene.h"

#include "ErrorLog.h"
using namespace cocos2d;
namespace level_state {

	const float C_MOVE_THRESHOLD = 50.0f;
	const float C_TIME_FAST_TOUCH = 0.10f;
	const float C_TIME_SLOW_TOUCH = 0.25f;

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
		mErrorTime = 0.0f;
		mTouchStartTime = 0.0f;
		mTouchMoveSum = Vec2::ZERO;

		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_BEGIN_GRID | ENABLED_TOUCH_END | ENABLED_TOUCH_CANCELLED | ENABLED_TOUCH_MOVE);
		auto plantNode = mMainGameScene->getTargetPlantNode();
		plantNode->getParentGrid()->glowSelectedCell(plantNode->getGridIndex(), true);

		return true;
	}

	bool PlayerChooseActionWithSeed::onExitState()
	{
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_NONE);
		/*mMainGameScene->getGrid(GRID_MAIN)->disableAllGlowCells();
		mMainGameScene->getGrid(GRID_INVENTORY)->disableAllGlowCells();
		mMainGameScene->getGrid(GRID_BUCKET)->disableAllGlowCells();
		*/
		return true;
	}

	void PlayerChooseActionWithSeed::onTouchEnded(GridType type, uint8_t x, uint8_t y)
	{	
		// timing
		clock_t diff = clock() - mTouchStartTime;
		float diffSeconds = (float)diff / CLOCKS_PER_SEC;

		auto plantNode = mMainGameScene->getTargetPlantNode();
		auto plantTypes = mMainGameScene->getPlantTypesManager();

		if (diffSeconds >= C_TIME_FAST_TOUCH) {
			mMainGameScene->getGrid(GRID_MAIN)->glowAutoCells(plantNode->getPlantType(), plantTypes);
			mMainGameScene->getGrid(GRID_INVENTORY)->glowEmptyCells(true);
			mMainGameScene->transitTo("PlayerChooseCell");
		}
		else if (diffSeconds >= C_TIME_SLOW_TOUCH) {
			mMainGameScene->transitTo("DisplayInfo");
		}

		mTouchMoveSum = Vec2::ZERO;

		
	}

	void PlayerChooseActionWithSeed::onTouchBegan(GridType type, uint8_t x, uint8_t y)
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		mTouchStartTime = clock();

		return;

		//if (mTouchMoveSum.x + mTouchMoveSum.y > C_MOVE_THRESHOLD)

/*		if (GRID_BUCKET == type) {
			mMainGameScene->transitTo("DisplayInfo");
			return;
		}*/
		//ErrorLog::printf("onTouchBegan");
		auto grid = mMainGameScene->getGrid(type);
		if ((GRID_MAIN == type || GRID_INVENTORY == type) && grid->isCellEmptyAndFree(x, y)) {
			mMainGameScene->getGrid(GRID_MAIN)->disableAllGlowCells();
			mMainGameScene->getGrid(GRID_INVENTORY)->disableAllGlowCells();
			mMainGameScene->getGrid(GRID_BUCKET)->disableAllGlowCells();

			auto pos = plantNode->getPosition();
			pos = plantNode->getParentGrid()->fromLocalToWorld(pos);
			plantNode->removeFromGrid(false);
			if (GRID_INVENTORY == type) {
				grid->addGridCell(plantNode, x, y);
				plantNode->setGlobalZOrder(2.0f);
				plantNode->setPosition(grid->fromWorldToLocal(pos));
				mMainGameScene->transitTo("DropSeedValid");
			} else if(GRID_MAIN == type) {
				mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_NONE);
				mMainGameScene->addChild(plantNode);
				plantNode->setPosition(pos);
				mTargetGridIndex = GridIndex(x, y);
				auto visibleSize = Director::getInstance()->getVisibleSize();
				auto targetPosition = grid->getWorldPositionForGridIndex(x, y);
				float distance = targetPosition.distanceSquared(plantNode->getPosition()) / Vec2(visibleSize).lengthSquared();
				float animDuration = distance * 100.0f;
				if (animDuration > 0.5f) animDuration = 0.5f;
				auto plantNodeSequence = Sequence::create(
					EaseIn::create(MoveTo::create(animDuration, targetPosition), 1.2f),
					CallFunc::create(CC_CALLBACK_0(PlayerChooseActionWithSeed::dropAnimationEnd, this)),
					nullptr);
				plantNode->runAction(plantNodeSequence);
			}
		}
		else 
		{
			if (!grid->isCellEmptyAndFree(x, y) && GRID_INVENTORY == type) {
				plantNode->getParentGrid()->glowCell(plantNode->getGridIndex(), "default");
				auto newTargetPlantNode = grid->getPlantNodeFromIndex(GridIndex(x, y));
				mMainGameScene->setTargetPlantNode(newTargetPlantNode);
				newTargetPlantNode->getParentGrid()->glowSelectedCell(newTargetPlantNode->getGridIndex());
				auto plantTypes = mMainGameScene->getPlantTypesManager();
				mMainGameScene->getGrid(GRID_MAIN)->glowAutoCells(newTargetPlantNode->getPlantType(), plantTypes);
			}
			else {
				// error sound
				//mErrorTime = 0.25f;
				//plantNode->getParentGrid()->glowErrorCell(plantNode->getGridIndex(), true);
				//mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_NONE);
			}
		}
	}


	void PlayerChooseActionWithSeed::dropAnimationEnd()
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		auto plantType = plantNode->getPlantType();// ->getViewData(PLANT_PHASIS_SEEDED);
		auto seededPlantNode = plantType->getViewData(PLANT_PHASIS_SEEDED)->createPlantNode(plantType);
		// original
		mMainGameScene->getGrid(GRID_MAIN)->addGridCell(seededPlantNode, mTargetGridIndex.x, mTargetGridIndex.y);

		seededPlantNode->setOpacity(0);

		const float fadeTime = 0.5f;

		auto seededPlantNodeSequence = Sequence::create(
			FadeIn::create(fadeTime),
			CallFunc::create(CC_CALLBACK_0(PlayerChooseActionWithSeed::fadeAnimationEnd, this)),
			nullptr);
		seededPlantNode->runAction(seededPlantNodeSequence);
		mMainGameScene->setTargetPlantNode(seededPlantNode);

		auto plantNodeSequence = Sequence::create(
			FadeOut::create(fadeTime),
			RemoveSelf::create(true),
			nullptr);
		plantNode->runAction(plantNodeSequence);
	}

	void PlayerChooseActionWithSeed::fadeAnimationEnd()
	{
		mMainGameScene->transitTo("PlantSeed");
	}

	void PlayerChooseActionWithSeed::onUpdate(float delta)
	{
		if (mErrorTime > 0.0f) {
			mErrorTime -= delta;
			if (mErrorTime <= 0.0f) {
				mErrorTime = 0.0f;
				auto plantNode = mMainGameScene->getTargetPlantNode();
				plantNode->getParentGrid()->glowSelectedCell(plantNode->getGridIndex(), true);
				mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_BEGIN_GRID | ENABLED_TOUCH_END | ENABLED_TOUCH_CANCELLED);
			}
		}
	}

	void PlayerChooseActionWithSeed::onTouchCancelled()
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		plantNode->getParentGrid()->glowSelectedCell(plantNode->getGridIndex(), false);

		mMainGameScene->transitTo("PlayerChooseCell");
	}

	void PlayerChooseActionWithSeed::onTouchMoved(float deltaX, float deltaY)
	{		
		mTouchMoveSum += Vec2(deltaX, deltaY);
		//ErrorLog::printf("[PlayerChooseActionWithSeed::onTouchMoved] touchMove: %f/%f\n", mTouchMoveSum.x, mTouchMoveSum.y);
		if (fabs(mTouchMoveSum.x) + fabs(mTouchMoveSum.y) > C_MOVE_THRESHOLD) {

			auto plantNode = mMainGameScene->getTargetPlantNode();
			//auto localPlantPos = plantNode->getPosition();
			//auto globalPlantPos = plantNode->convertToWorldSpace(localPlantPos);
			auto globalTouchPos = mMainGameScene->getCurrentTouchPosition();
			auto localTouchPos = globalTouchPos - plantNode->getParent()->getPosition();
			localTouchPos -= plantNode->getContentSize() * plantNode->getScale() * 0.5f;
			//ErrorLog::printf("[PlayerChooseActionWithSeed::onTouchMoved] localPlantPos: %f/%f, globalPlantPos: %f/%f, globalTouchPos: %f/%f, localTouchPos: %f/%f\n",
				//localPlantPos.x, localPlantPos.y, globalPlantPos.x, globalPlantPos.y, globalTouchPos.x, globalTouchPos.y, localTouchPos.x, localTouchPos.y);
			//plantNode->setAnchorPoint(Vec2(0.5f, 0.5f));

			plantNode->setPosition(localTouchPos);
			plantNode->getParentGrid()->glowSelectedCell(plantNode->getGridIndex(), false);

			mMainGameScene->transitTo("DragSeed");
		}
	}

	void PlayerChooseActionWithSeed::onCancelState()
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		if (plantNode) {
			plantNode->getParentGrid()->glowSelectedCell(plantNode->getGridIndex(), false);
		}
	}


};