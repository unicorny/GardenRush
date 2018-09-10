#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_RANDOM_SEED_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_RANDOM_SEED_H

#include "LevelStateManager.h"

namespace level_state {
	class RandomSeed : public iLevelState
	{
	public:
		RandomSeed(LevelStateManager* manager) : iLevelState(manager) {}
		virtual ~RandomSeed() {};
		virtual const char* getName() const { return "RandomSeed"; }
		virtual bool initState();
		virtual bool onEnterState();
		virtual bool onExitState();
	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_RANDOM_SEED_H