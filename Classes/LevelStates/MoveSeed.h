#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_MOVE_SEED_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_MOVE_SEED_H

#include "iLevelState.h"



namespace level_state {
	class MoveSeed : public iLevelState
	{
	public:
		MoveSeed() {};
		virtual ~MoveSeed() {};
		virtual const char* getName() const { return "MoveSeed"; }
		virtual bool initState();
		virtual bool onEnterState() { return true; };
		virtual bool onEnterState(iLevelState* lastState);
		virtual bool onExitState();
		virtual void onCancelState();

		void animationEnd();

	protected:

	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_MOVE_SEED_H