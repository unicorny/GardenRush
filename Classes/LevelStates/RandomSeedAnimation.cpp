#include "RandomSeedAnimation.h"
#include "nodes/PlantNode.h"

using namespace cocos2d;

namespace level_state {

	RandomSeedAnimation::RandomSeedAnimation() 
	: mTargetScale(0.0f, 0.0f)
	{

	}

	bool RandomSeedAnimation::initState()
	{
		return true;
	}

	bool RandomSeedAnimation::onEnterState()
	{
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_BEGIN);

		auto plantNode = mMainGameScene->getTargetPlantNode();
		mTargetScale = Point(plantNode->getScaleX(), plantNode->getScaleY());
		plantNode->setScale(0.0f, 0.0f);
		auto pos = plantNode->getPosition();
		plantNode->setAnchorPoint(Vec2(0.5f, 0.5f));
		auto contentSize = plantNode->getContentSize();
		plantNode->setPosition(pos + Vec2(contentSize.width * mTargetScale.x, contentSize.height * mTargetScale.y)*0.5f);
		auto plantNodeSequence = Sequence::create(
			EaseBounceInOut::create(ScaleTo::create(1.5f, mTargetScale.x, mTargetScale.y)),
			CallFunc::create(CC_CALLBACK_0(RandomSeedAnimation::animationEnd, this)),
			nullptr);
		plantNode->runAction(plantNodeSequence);

		return true;
	}

	void RandomSeedAnimation::animationEnd()
	{
		auto localPlantNode = mMainGameScene->getTargetPlantNode();
		mMainGameScene->setTargetPlantNode(nullptr);
		finish(localPlantNode);
	}
	bool RandomSeedAnimation::onExitState()
	{
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_NONE);
		return true;
	}
	void RandomSeedAnimation::onCancelState()
	{
		auto localPlantNode = mMainGameScene->getTargetPlantNode();
		mMainGameScene->setTargetPlantNode(nullptr);
		finish(localPlantNode);
	}

	void RandomSeedAnimation::onTouchBegan(PlantNode* plantNode)
	{
		auto scaleAction = plantNode->getActionByTag(1);
		plantNode->stopAllActions();

		auto localPlantNode = mMainGameScene->getTargetPlantNode();
		mMainGameScene->setTargetPlantNode(plantNode);
		finish(localPlantNode);
	}

	void RandomSeedAnimation::finish(PlantNode* plantNode)
	{
		plantNode->setAnchorPoint(cocos2d::Vec2::ZERO);
		auto pos = plantNode->getPosition();
		auto contentSize = plantNode->getContentSize();
		plantNode->setPosition(pos - cocos2d::Vec2(contentSize.width * plantNode->getScaleX(), contentSize.height * plantNode->getScaleY())*0.5f);
		plantNode->setScale(mTargetScale.x, mTargetScale.y);
		
		mMainGameScene->transitTo("PlayerChooseSeed");
	}
};