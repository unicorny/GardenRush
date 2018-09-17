#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_DROP_SEED_INVALID_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_DROP_SEED_INVALID_H

#include "iLevelState.h"



namespace level_state {
	class DropSeedInvalid : public iLevelState
	{
	public:
		DropSeedInvalid() {};
		virtual ~DropSeedInvalid() {};
		virtual const char* getName() const { return "DropSeedInvalid"; }
		virtual bool initState();
		virtual bool onEnterState();
		virtual bool onExitState();

		void animationEnd();

	protected:

	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_DROP_SEED_INVALID_H