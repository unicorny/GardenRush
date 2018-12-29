#include "PlantSeed.h"

#include "PlantTypesManager.h"
#include "nodes/PlantNode.h"

using namespace cocos2d;

namespace level_state {

	bool PlantSeed::initState()
	{
		return true;
	}
	bool PlantSeed::onEnterState(iLevelState* lastState)
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		
		auto plantType = plantNode->getPlantType();
		/*
		auto gridIndex = plantNode->getGridIndex();
		auto grid = plantNode->getParentGrid();
		*/
		auto targetGridCell = lastState->getGridCell();
		if (!targetGridCell) {
			LOG_ERROR("prev state hasn't set gridCell", false);
		}
		auto grid = mMainGameScene->getGrid(targetGridCell->type);
		auto gridIndex = targetGridCell->index;

		auto seededPlantNode = plantType->getViewData(PLANT_PHASIS_SEEDED)->createPlantNode(plantType);
		// original
		mMainGameScene->getGrid(GRID_MAIN)->addGridCell(seededPlantNode, gridIndex.x, gridIndex.y);
		seededPlantNode->setOpacity(0);

		const float fadeTime = 0.5f;

		auto animationState = seededPlantNode->getAnimationState(mMainGameScene->getAnimationStateMemoryManager());
		auto seededPlantNodeSequence = Sequence::create(
			FadeIn::create(fadeTime),
			CallFunc::create(CC_CALLBACK_0(PlantSeed::fadeAnimationEnd, this)),
			nullptr);
		animationState->runFadeAction(seededPlantNodeSequence, 255);
		//seededPlantNode->runAction(seededPlantNodeSequence);
		mMainGameScene->setTargetPlantNode(seededPlantNode);

		mFadeOutPlantSeed = plantNode;
		//plantNode->setPosition(plantNode->getParentGrid()->getLocalPositionForGridIndex(plantNode->getGridIndex()));
		auto plantNodeSequence = Sequence::create(
			FadeOut::create(fadeTime),
			nullptr);
		plantNode->runAction(plantNodeSequence);

		
		return true;
	}

	void PlantSeed::fadeAnimationEnd()
	{
		auto plantTypesManager = mMainGameScene->getPlantTypesManager();
		auto plantNode = mMainGameScene->getTargetPlantNode();
		plantNode->stopAllAnimations();
		auto grid = plantNode->getParentGrid();
		auto gridIndex = plantNode->getGridIndex();
		plantNode->setGrowPhasis(PLANT_PHASIS_SEEDED);

		if (mFadeOutPlantSeed) { mFadeOutPlantSeed->removeFromGrid(true); }

		grid->updateParentsOfPlantOnIndex(gridIndex, plantTypesManager, mMainGameScene->getLevelData(), mMainGameScene->getPoints());

		if (mMainGameScene->getGrid(GRID_BUCKET)->isCellEmptyAndFree(0, 0)) {
			mMainGameScene->transitTo("RandomSeed");
		}
		else {
			mMainGameScene->transitTo("PlayerChooseSeed");
		}
	}

	bool PlantSeed::onExitState()
	{

		return true;
	}

	void PlantSeed::onCancelState()
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		plantNode->stopAllAnimations();
	}
}