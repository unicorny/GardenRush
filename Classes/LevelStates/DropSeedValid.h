#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_DROP_SEED_VALID_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_DROP_SEED_VALID_H

#include "iLevelState.h"



namespace level_state {
	class DropSeedValid : public iLevelState
	{
	public:
		DropSeedValid() {};
		virtual ~DropSeedValid() {};
		virtual const char* getName() const { return "DropSeedValid"; }
		virtual bool initState();
		virtual bool onEnterState();
		virtual bool onExitState();


	protected:

	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_DROP_SEED_VALID_H