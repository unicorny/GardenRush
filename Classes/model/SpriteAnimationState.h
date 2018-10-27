#ifndef __FAIRY_GAMES_GARDEN_RUSH_MODEL_SPRITE_ANIMATION_STATE_H
#define __FAIRY_GAMES_GARDEN_RUSH_MODEL_SPRITE_ANIMATION_STATE_H

/*!
* \author: Dario Rekowski
*
* \date: 25.10.18
*
* \brief: containing animation state for sprite
*
*/

#include "cocos2d.h"
#include "lib/TemplateMemoryManager.h"

class CustomAnimationSprite;

enum SpriteAnimationTags {
	SPRITE_ANIMATION_NONE = 0,
	SPRITE_ANIMATION_SCALE = 1,
	SPRITE_ANIMATION_MOVE = 2,
	SPRITE_ANIMATION_FADE = 4,
	SPRITE_ANIMATION_ALL = SPRITE_ANIMATION_SCALE | SPRITE_ANIMATION_MOVE | SPRITE_ANIMATION_FADE
};

class SpriteAnimationState
{
public:
	SpriteAnimationState();
	~SpriteAnimationState();

	inline void setMemoryManager(TemplateMemoryManager<SpriteAnimationState>* manager) { mManager = manager; }
	inline u8 getIndex() const { return mMemoryIndex; }

	bool reinit(u8 index);
	inline void setParentSprite(CustomAnimationSprite* parent) { mParentSprite = parent; }
	inline void free() { assert(mManager); mManager->free(this); }

	// animation end functions, called by animation handlers
	void animationEnd(SpriteAnimationTags animationTag, bool newAnimationFollowd = false);
	inline void animationEndScale() { animationEnd(SPRITE_ANIMATION_SCALE); }
	inline void animationEndMove() { animationEnd(SPRITE_ANIMATION_MOVE); }
	inline void animationEndFade() { animationEnd(SPRITE_ANIMATION_FADE); }

	void animationStop(SpriteAnimationTags animationTag, bool newAnimationFollowd = false);

	// access for parent
	inline bool isAnimationRunning(SpriteAnimationTags type) const {return  (mRunningAnimationTypes & type) == type;}
	inline void setTargetScale(cocos2d::Vec2 targetScale) { mTargetScale = targetScale; }
	inline void setTargetPosition(cocos2d::Vec2 targetPosition) { mTargetPosition = targetPosition; }
	inline void setTargetOpacity(float targetOpacity) { mTargetOpacity = targetOpacity; }

	// run action
	void runScaleAction(cocos2d::FiniteTimeAction* action, cocos2d::Vec2 targetScale);
	void runMoveAction(cocos2d::FiniteTimeAction* action, cocos2d::Vec2 targetPosition);
	void runFadeAction(cocos2d::FiniteTimeAction* action, float targetOpacity);

protected:
	// memory management
	u8 mMemoryIndex;
	TemplateMemoryManager<SpriteAnimationState>* mManager;

	// animation control
	SpriteAnimationTags mRunningAnimationTypes;
	cocos2d::Vec2		mTargetScale;
	cocos2d::Vec2		mTargetAnchorPoint;
	cocos2d::Vec2		mTargetPosition;
	float				mTargetOpacity;

	//
	CustomAnimationSprite*    mParentSprite;
};


#endif // __FAIRY_GAMES_GARDEN_RUSH_MODEL_SPRITE_ANIMATION_STATE_H