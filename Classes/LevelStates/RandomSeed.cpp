#include "RandomSeed.h"
#include "cocos2d.h"
#include "model/LevelData.h"
#include "model/PlantType.h"
#include "nodes/Grid.h"
#include "nodes/PlantNode.h"
#include "fast/PlantTypesAccess.h"
#include "MainGameScene.h"

using namespace cocos2d;
namespace level_state {


	RandomSeed::RandomSeed()
	{
	}

	bool RandomSeed::initState()
	{
		return true;
	}
	bool RandomSeed::onEnterState()
	{
		auto levelData = mMainGameScene->getLevelData();
		auto animationStateMemoryManager = mMainGameScene->getAnimationStateMemoryManager();
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_NONE);

		int seedPlantTypeIndex = RandomHelper::random_int(0, (int)levelData->getPlantTypeCount()-1);
		DHASH plantTypeHash = levelData->getPlantType(seedPlantTypeIndex);
		auto plantType = mMainGameScene->getPlantTypesManager()->findPlantType(plantTypeHash);
		auto plantView = plantType->getViewData(PLANT_PHASIS_SEED);
		auto plantNode = plantView->createPlantNode(plantType);
		mMainGameScene->getGrid(GRID_BUCKET)->addGridCell(plantNode, 0, 0);
		

		auto planetNodeAnimationState = plantNode->getAnimationState(animationStateMemoryManager);	
		assert(planetNodeAnimationState);
		auto targetScale = Vec2(plantNode->getScaleX(), plantNode->getScaleY());
		auto scaleSequence = Sequence::create(
			ScaleTo::create(0.0f, 0.0f, 0.0f),
			EaseBounceInOut::create(ScaleTo::create(1.5f, targetScale.x, targetScale.y)),
			nullptr);

		planetNodeAnimationState->runScaleAction(scaleSequence, targetScale);

		mMainGameScene->setTargetPlantNode(plantNode);
		mMainGameScene->transitTo("PlayerChooseCell");

		return true;
	}


	bool RandomSeed::onExitState()
	{
		mMainGameScene->setTargetPlantNode(nullptr);
		return true;
	}
	void RandomSeed::onCancelState()
	{
		auto animationStateMemoryManager = mMainGameScene->getAnimationStateMemoryManager();
		auto plantNode = mMainGameScene->getTargetPlantNode();
		auto planetNodeAnimationState = plantNode->getAnimationState(animationStateMemoryManager);
		planetNodeAnimationState->animationStop(SPRITE_ANIMATION_ALL);
	}


};