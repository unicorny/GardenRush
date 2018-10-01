#include "RandomSeed.h"
#include "cocos2d.h"
#include "model/LevelData.h"
#include "model/PlantType.h"
#include "nodes/Grid.h"
#include "nodes/PlantNode.h"
#include "PlantTypesManager.h"
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
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_NONE);
		int seedPlantTypeIndex = RandomHelper::random_int(0, (int)levelData->getPlantTypeCount()-1);
		DHASH plantTypeHash = levelData->getPlantType(seedPlantTypeIndex);
		auto plantType = mMainGameScene->getPlantTypesManager()->findPlantType(plantTypeHash);
		auto plantView = plantType->getViewData(PLANT_PHASIS_SEED);
		auto plantNode = plantView->createPlantNode(plantType);
		mMainGameScene->getGrid(GRID_BUCKET)->addGridCell(plantNode, 0, 0);
		auto scale = Point(plantNode->getScaleX(), plantNode->getScaleY());
		plantNode->setScale(0.0f, 0.0f);
		auto pos = plantNode->getPosition();
		plantNode->setAnchorPoint(Vec2(0.5f, 0.5f));
		auto contentSize = plantNode->getContentSize();
		plantNode->setPosition(pos + Vec2(contentSize.width * scale.x, contentSize.height * scale.y)*0.5f);
		auto plantNodeSequence = Sequence::create(
			EaseBounceInOut::create(ScaleTo::create(1.5f, scale.x, scale.y)),
			CallFunc::create(CC_CALLBACK_0(RandomSeed::animationEnd, this)),
			nullptr);
		plantNode->runAction(plantNodeSequence);
		mMainGameScene->setTargetPlantNode(plantNode);

		return true;
	}

	void RandomSeed::animationEnd()
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		plantNode->setAnchorPoint(cocos2d::Vec2::ZERO);
		auto pos = plantNode->getPosition();
		auto contentSize = plantNode->getContentSize();
		plantNode->setPosition(pos - Vec2(contentSize.width * plantNode->getScaleX(), contentSize.height * plantNode->getScaleY())*0.5f);
		mMainGameScene->setTargetPlantNode(nullptr);
		mMainGameScene->transitTo("PlayerChooseSeed");
	}
	bool RandomSeed::onExitState()
	{
		return true;
	}
	void RandomSeed::onCancelState()
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		plantNode->stopAllActions();
		auto size = plantNode->getContentSize();
		auto cellSize = plantNode->getParentGrid()->getCellSize();
		plantNode->setScale(
			size.width / cellSize.x,
			size.height / cellSize.y
		);
	}


};