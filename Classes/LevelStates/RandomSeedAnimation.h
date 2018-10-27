#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_RANDOM_SEED_ANIMATION_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_RANDOM_SEED_ANIMATION_H

#include "iLevelState.h"


namespace level_state {
	class RandomSeedAnimation : public iLevelState
	{
	public:
		RandomSeedAnimation();
		virtual ~RandomSeedAnimation() {};
		virtual const char* getName() const { return "RandomSeedAnimation"; }
		virtual bool initState();
		virtual bool onEnterState();
		virtual bool onExitState();
		virtual void onCancelState();
		virtual void onTouchBegan(PlantNode* plantNode);

		void animationEnd();
		
	protected:
		void finish(PlantNode* plantNode);

		cocos2d::Point mTargetScale;
	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_RANDOM_SEED_ANIMATION_H