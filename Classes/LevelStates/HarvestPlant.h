#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_HARVEST_PLANT_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_HARVEST_PLANT_H

#include "iLevelState.h"



namespace level_state {
	class HarvestPlant : public iLevelState
	{
	public:
		HarvestPlant() {};
		virtual ~HarvestPlant() {};
		virtual const char* getName() const { return "HarvestPlant"; }
		virtual bool initState();
		virtual bool onEnterState();
		virtual bool onExitState();
		virtual void onCancelState();

		void animationEnd();

	protected:

	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_HARVEST_PLANT_H