#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLAYER_CHOOSE_SEED_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLAYER_CHOOSE_SEED_H

#include "LevelStateManager.h"

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

		bool touchPlant(PlantNode*);
	protected:

	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLAYER_CHOOSE_SEED_H