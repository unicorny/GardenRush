#include "RandomSeed.h"
#include "cocos2d.h"
#include "model/LevelData.h"
#include "model/PlantType.h"
#include "nodes/Grid.h"
#include "nodes/PlantNode.h"
#include "PlantTypesManager.h"
#include "MainGameScene.h"

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
		int seedPlantTypeIndex = cocos2d::RandomHelper::random_int(0, (int)levelData->getPlantTypeCount()-1);
		DHASH plantTypeHash = levelData->getPlantType(seedPlantTypeIndex);
		auto plantType = mMainGameScene->getPlantTypesManager()->findPlantType(plantTypeHash);
		auto plantView = plantType->getViewData(PLANT_PHASIS_SEED);
		auto plantNode = plantView->createPlantNode(plantType);
		mMainGameScene->getGrid(GRID_BUCKET)->addGridCell(plantNode, 0, 0);
		auto scale = cocos2d::Point(plantNode->getScaleX(), plantNode->getScaleY());
		plantNode->setScale(0.0f, 0.0f);
		auto pos = plantNode->getPosition();
		plantNode->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
		plantNode->setPosition(pos + plantNode->getContentSize()*0.5f);
		auto plantNodeAction = cocos2d::ScaleTo::create(1.0f, scale.x, scale.y);
		auto plantNodeSequence = cocos2d::Sequence::create(
			cocos2d::EaseBounceInOut::create(cocos2d::ScaleTo::create(1.5f, scale.x, scale.y)),
			cocos2d::CallFunc::create(CC_CALLBACK_0(RandomSeed::animationEnd, this)),
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
		plantNode->setPosition(pos - plantNode->getContentSize()*0.5f);
		mMainGameScene->setTargetPlantNode(nullptr);
		mMainGameScene->transitTo("PlayerChooseSeed");
	}
	bool RandomSeed::onExitState()
	{
		return true;
	}

};