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

		virtual void onTouchEnded(GridType type, uint8_t x, uint8_t y);
		virtual void onTouchCancelled();
	protected:

	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_DISPLAY_INFO_H