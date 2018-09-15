#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_DISPLAY_INFO_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_DISPLAY_INFO_H

#include "iLevelState.h"



namespace level_state {
	class DisplayInfo : public iLevelState
	{
	public:
		DisplayInfo() {};
		virtual ~DisplayInfo() {};
		virtual const char* getName() const { return "DisplayInfo"; }
		virtual bool initState();
		virtual bool onEnterState();
		virtual bool onExitState();

	protected:

	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_DISPLAY_INFO_H