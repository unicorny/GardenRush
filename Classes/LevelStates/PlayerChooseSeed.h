#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLAYER_CHOOSE_SEED_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLAYER_CHOOSE_SEED_H

#include "iLevelState.h"

class PlantNode;

namespace level_state {
	class PlayerChooseSeed : public iLevelState
	{
	public:
		PlayerChooseSeed();
		virtual ~PlayerChooseSeed() {};
		virtual const char* getName() const { return "PlayerChooseSeed"; }
		virtual bool initState();
		virtual bool onEnterState();
		virtual bool onExitState();
		virtual void onCancelState();

		virtual void onTouchBegan(PlantNode* plantNode);
	protected:

	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLAYER_CHOOSE_SEED_H