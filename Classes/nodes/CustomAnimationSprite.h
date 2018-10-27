#ifndef __FAIRY_GAMES_GARDEN_RUSH_NODES_CUSTOM_ANIMATION_SPRITE_H
#define __FAIRY_GAMES_GARDEN_RUSH_NODES_CUSTOM_ANIMATION_SPRITE_H

/*!
*	\author: Dario Rekowski
*
*	\date: 25.10.18
*
*  \brief: Class for custom animation sprite usable with SpriteAnimationState
*/
#include "cocos2d.h"
#include "model/SpriteAnimationState.h"



class CustomAnimationSprite : public cocos2d::Sprite
{
public:
	CustomAnimationSprite() : mAnimationState(nullptr) {}
	virtual ~CustomAnimationSprite();

	inline void allAnimationsEnd() {
		if (mAnimationState) {
			mAnimationState->free();
			mAnimationState = NULL;
		}
	}
	inline SpriteAnimationState* getAnimationState(TemplateMemoryManager<SpriteAnimationState>* animationStateMemoryManager) {
		if (!mAnimationState) {
			mAnimationState = animationStateMemoryManager->getFree();
			// maybe not enough space in memory manager if assert 
			assert(mAnimationState);
			mAnimationState->setParentSprite(this);
		}
		return mAnimationState; 
	}

protected:
	SpriteAnimationState* mAnimationState;

};

#endif // __FAIRY_GAMES_GARDEN_RUSH_NODES_CUSTOM_ANIMATION_SPRITE_H
