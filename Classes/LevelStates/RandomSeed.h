#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_RANDOM_SEED_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_RANDOM_SEED_H

#include "iLevelState.h"



namespace level_state {
	class RandomSeed : public iLevelState
	{
	public:
		RandomSeed();
		virtual ~RandomSeed() {};
		virtual const char* getName() const { return "RandomSeed"; }
		virtual bool initState();
		virtual bool onEnterState();
		virtual bool onExitState();
		virtual void onCancelState();

		void animationEnd();
	protected:
		
	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_RANDOM_SEED_H