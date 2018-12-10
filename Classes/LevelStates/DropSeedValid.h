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
		virtual bool onEnterState() { return true; };
		virtual bool onEnterState(iLevelState* lastState);
		virtual bool onExitState();
		virtual void onCancelState();

		void animationEnd();

	protected:
	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_DROP_SEED_VALID_H