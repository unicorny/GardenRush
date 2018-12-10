#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLAYER_CHOOSE_ACTION_WITH_SEEDED_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLAYER_CHOOSE_ACTION_WITH_SEEDED_H

#include "iLevelState.h"
#include "cocos2d.h"
class PlantNode;

namespace level_state {
	class PlayerChooseActionWithSeeded : public iLevelState
	{
	public:
		PlayerChooseActionWithSeeded() {};
		virtual ~PlayerChooseActionWithSeeded() {};
		virtual const char* getName() const { return "PlayerChooseActionWithSeeded"; }

		virtual bool initState();
		virtual bool onEnterState();
		virtual bool onExitState();
		virtual void onCancelState();

		virtual void onTouchEnded(GridType type, uint8_t x, uint8_t y);
		virtual void onTouchCancelled();

	protected:
		GridType	mGlowCellGrid;
	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLAYER_CHOOSE_ACTION_WITH_SEEDED_H