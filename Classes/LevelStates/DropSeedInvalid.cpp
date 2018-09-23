#include "DropSeedInvalid.h"
#include "nodes/PlantNode.h"
#include "nodes/Grid.h"

using namespace cocos2d;

namespace level_state {
	bool DropSeedInvalid::initState()
	{
		return true;
	}
	bool DropSeedInvalid::onEnterState()
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		auto grid = plantNode->getParentGrid();
		auto targetPosition = grid->getOriginPosition(plantNode);
		auto visibleSize = Director::getInstance()->getVisibleSize();
		float distance = targetPosition.distanceSquared(plantNode->getPosition()) / Vec2(visibleSize).lengthSquared();
		auto plantNodeSequence = Sequence::create(
			EaseIn::create(MoveTo::create(0.25f, targetPosition), 1.2f),
			CallFunc::create(CC_CALLBACK_0(DropSeedInvalid::animationEnd, this)),
			nullptr);
		plantNode->runAction(plantNodeSequence);
		return true;
	}
	bool DropSeedInvalid::onExitState()
	{
		return true;
	}

	void DropSeedInvalid::onCancelState()
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		plantNode->stopAllActions();
		plantNode->setPosition(plantNode->getParentGrid()->getOriginPosition(plantNode));
		plantNode->setGlobalZOrder(0.0f);
	}

	void DropSeedInvalid::animationEnd()
	{
		mMainGameScene->getTargetPlantNode()->setGlobalZOrder(0.0f);
		mMainGameScene->transitTo("PlayerChooseSeed");
	}
}