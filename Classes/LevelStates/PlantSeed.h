#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLANT_SEED_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLANT_SEED_H

#include "iLevelState.h"


namespace level_state {
	class PlantSeed : public iLevelState
	{
	public:
		PlantSeed() {};
		virtual ~PlantSeed() {};
		virtual const char* getName() const { return "PlantSeed"; }
		virtual bool initState();
		virtual bool onEnterState();
		virtual bool onExitState();
		virtual void onCancelState();

	protected:

	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLANT_SEED_H