#include "SpriteAnimationState.h"
#include "nodes/CustomAnimationSprite.h"

using namespace cocos2d;

SpriteAnimationState::SpriteAnimationState()
	: mManager(nullptr), mMemoryIndex(0), mRunningAnimationTypes(SPRITE_ANIMATION_NONE), mTargetOpacity(255), mParentSprite(nullptr)
{

}

SpriteAnimationState::~SpriteAnimationState()
{
	mParentSprite = nullptr;
}

bool SpriteAnimationState::reinit(u8 index)
{
	mMemoryIndex = index;
	mRunningAnimationTypes = SPRITE_ANIMATION_NONE;
	mTargetOpacity = 255;
	mTargetPosition = Vec2(0.0f, 0.0f);
	mTargetScale = Vec2(1.0f, 1.0f);
	mTargetAnchorPoint = Vec2(0.0f, 0.0f);
	
	//animationStop(SPRITE_ANIMATION_ALL);
	return true;
}
// called from extern to stop animations
void SpriteAnimationState::animationStop(SpriteAnimationTags animationTag, bool newAnimationFollowd)
{
	SpriteAnimationTags runningAnimationsToStop = static_cast<SpriteAnimationTags>(mRunningAnimationTypes & animationTag);

	if (SPRITE_ANIMATION_SCALE == (runningAnimationsToStop & SPRITE_ANIMATION_SCALE)) {
		mParentSprite->stopActionByTag(SPRITE_ANIMATION_SCALE);
	}
	if (SPRITE_ANIMATION_MOVE == (runningAnimationsToStop & SPRITE_ANIMATION_MOVE)) {
		mParentSprite->stopActionByTag(SPRITE_ANIMATION_MOVE);
	}
	if (SPRITE_ANIMATION_FADE == (runningAnimationsToStop & SPRITE_ANIMATION_FADE)) {
		mParentSprite->stopActionByTag(SPRITE_ANIMATION_FADE);
	}
	animationEnd(runningAnimationsToStop, newAnimationFollowd);
}


// called automaticly at the end of animation or if animation was stopped
void SpriteAnimationState::animationEnd(SpriteAnimationTags animationTag, bool newAnimationFollowd)
{
	assert(mParentSprite);
	if (SPRITE_ANIMATION_SCALE == (animationTag & SPRITE_ANIMATION_SCALE)) {
		if (SPRITE_ANIMATION_SCALE == (mRunningAnimationTypes & SPRITE_ANIMATION_SCALE)) {
			mParentSprite->setAnchorPoint(cocos2d::Vec2::ZERO);
			auto pos = mParentSprite->getPosition();
			auto contentSize = mParentSprite->getContentSize();
			auto translationVector = Vec2(mTargetAnchorPoint.x - 0.5f, mTargetAnchorPoint.y - 0.5f);
			mParentSprite->setPosition(pos + Vec2(
				translationVector.x * mTargetScale.x * contentSize.width,
				translationVector.y * mTargetScale.y * contentSize.height
			));
			mParentSprite->setScale(mTargetScale.x, mTargetScale.y);
		}
		mRunningAnimationTypes = static_cast<SpriteAnimationTags>(mRunningAnimationTypes ^ SPRITE_ANIMATION_SCALE);
	}
	if (SPRITE_ANIMATION_MOVE == (animationTag & SPRITE_ANIMATION_MOVE)) {
		if (SPRITE_ANIMATION_MOVE == (mRunningAnimationTypes & SPRITE_ANIMATION_MOVE)) {
			mParentSprite->setPosition(mTargetPosition);
		}
		mRunningAnimationTypes = static_cast<SpriteAnimationTags>(mRunningAnimationTypes ^ SPRITE_ANIMATION_MOVE);
		
	}
	if (SPRITE_ANIMATION_FADE == (animationTag & SPRITE_ANIMATION_FADE)) {
		if (SPRITE_ANIMATION_FADE == (mRunningAnimationTypes & SPRITE_ANIMATION_FADE)) {
			mParentSprite->setOpacity(mTargetOpacity);
		}
		mRunningAnimationTypes = static_cast<SpriteAnimationTags>(mRunningAnimationTypes ^ SPRITE_ANIMATION_FADE);
	}

	if (false == newAnimationFollowd && mRunningAnimationTypes == SPRITE_ANIMATION_NONE) {
		mParentSprite->allAnimationsEnd();
	}
}

void SpriteAnimationState::runScaleAction(FiniteTimeAction* action, Vec2 targetScale)
{
	assert(mParentSprite);

	// run scale animation with anchor point at center
	mTargetAnchorPoint = mParentSprite->getAnchorPoint();
	if (mTargetAnchorPoint.x != 0.5f || mTargetAnchorPoint.y != 0.5f) {
		auto pos = mParentSprite->getPosition();
		auto scale = Vec2(mParentSprite->getScaleX(), mParentSprite->getScaleY());
		auto contentSize = mParentSprite->getContentSize();

		mParentSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		auto translationVector = Vec2(0.5f - mTargetAnchorPoint.x, 0.5f - mTargetAnchorPoint.y);
		mParentSprite->setPosition(pos + Vec2(
			translationVector.x * scale.x * contentSize.width,
			translationVector.y * scale.y * contentSize.height
		));
	}
	
	auto scaleSequence = Sequence::create(
		action,
		CallFunc::create(CC_CALLBACK_0(SpriteAnimationState::animationEndScale, this)),
		nullptr);
	scaleSequence->setTag(SPRITE_ANIMATION_SCALE);
	animationStop(SPRITE_ANIMATION_SCALE, true);
	mRunningAnimationTypes = static_cast<SpriteAnimationTags>(mRunningAnimationTypes | SPRITE_ANIMATION_SCALE);
	setTargetScale(targetScale);
	mParentSprite->runAction(scaleSequence);
}
void SpriteAnimationState::runMoveAction(FiniteTimeAction* action, Vec2 targetPosition)
{
	assert(mParentSprite);
	auto moveSequence = Sequence::create(
		action,
		CallFunc::create(CC_CALLBACK_0(SpriteAnimationState::animationEndMove, this)),
		nullptr);
	moveSequence->setTag(SPRITE_ANIMATION_MOVE);
	animationStop(SPRITE_ANIMATION_MOVE);
	mRunningAnimationTypes = static_cast<SpriteAnimationTags>(mRunningAnimationTypes | SPRITE_ANIMATION_MOVE);
	setTargetPosition(targetPosition);
	mParentSprite->runAction(moveSequence);
}
void SpriteAnimationState::runFadeAction(FiniteTimeAction* action, GLubyte targetOpacity)
{
	assert(mParentSprite);
	auto fadeSequence = Sequence::create(
		action,
		CallFunc::create(CC_CALLBACK_0(SpriteAnimationState::animationEndFade, this)),
		nullptr);
	fadeSequence->setTag(SPRITE_ANIMATION_FADE);
	animationStop(SPRITE_ANIMATION_FADE, true);
	mRunningAnimationTypes = static_cast<SpriteAnimationTags>(mRunningAnimationTypes | SPRITE_ANIMATION_FADE);
	setTargetOpacity(targetOpacity);
	mParentSprite->runAction(fadeSequence);
}