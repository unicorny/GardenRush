#include "DropSeedValid.h"
#include "nodes/PlantNode.h"
#include "nodes/Grid.h"

using namespace cocos2d;

namespace level_state {

	bool DropSeedValid::initState()
	{
		return true;
	}

	bool DropSeedValid::onEnterState()
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		auto grid = plantNode->getParentGrid();
		auto targetPosition = grid->getOriginPosition(plantNode);
		auto visibleSize = Director::getInstance()->getVisibleSize();
		float distance = targetPosition.distanceSquared(plantNode->getPosition()) / Vec2(visibleSize).lengthSquared();
		auto plantNodeSequence = Sequence::create(
			EaseIn::create(MoveTo::create(distance*100.0f, targetPosition), 1.2f),
			CallFunc::create(CC_CALLBACK_0(DropSeedValid::animationEnd, this)),
			nullptr);
		plantNode->runAction(plantNodeSequence);
		return true;
	}
	bool DropSeedValid::onExitState()
	{
		return true;
	}

	void DropSeedValid::animationEnd()
	{
		auto gridType = mMainGameScene->getTargetPlantNode()->getParentGrid()->getType();
		if (GRID_MAIN == gridType) {
			mMainGameScene->transitTo("PlantSeed");
		}
		else if (GRID_INVENTORY == gridType) {
			if (mMainGameScene->getGrid(GRID_BUCKET)->isCellEmptyAndFree(0, 0)) {
				mMainGameScene->transitTo("RandomSeed");
			}
			else {
				mMainGameScene->transitTo("PlayerChooseSeed");
			}
		}
		
	}

}