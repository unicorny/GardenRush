#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLAYER_CHOOSE_ACTION_WITH_SEED_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLAYER_CHOOSE_ACTION_WITH_SEED_H

#include "iLevelState.h"
#include "cocos2d.h"
class PlantNode;

namespace level_state {
	class PlayerChooseActionWithSeed : public iLevelState
	{
	public:
		PlayerChooseActionWithSeed();
		virtual ~PlayerChooseActionWithSeed() {};
		virtual const char* getName() const { return "PlayerChooseActionWithSeed"; }
		virtual bool initState();
		virtual bool onEnterState();
		virtual bool onExitState();
		virtual void onCancelState();

		virtual void onTouchBegan(GridType type, uint8_t x, uint8_t y);
		virtual void onTouchEnded(GridType type, uint8_t x, uint8_t y);
		virtual void onTouchCancelled();
		virtual void onTouchMoved(float deltaX, float deltaY);

		virtual void onUpdate(float delta);

		virtual void dropAnimationEnd();
		virtual void fadeAnimationEnd();

	protected:
		//cocos2d::Vec2 mMovedSum;
		float mErrorTime;
		clock_t mTouchStartTime;
		GridIndex mTargetGridIndex;
		cocos2d::Vec2 mTouchMoveSum;
	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLAYER_CHOOSE_ACTION_WITH_SEED_H