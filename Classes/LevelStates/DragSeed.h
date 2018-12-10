#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_DRAG_SEED_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_DRAG_SEED_H

#include "iLevelState.h"



namespace level_state {
	class DragSeed : public iLevelState
	{
	public:
		DragSeed() {};
		virtual ~DragSeed() {};
		virtual const char* getName() const { return "DragSeed"; }
		virtual bool initState();
		virtual bool onEnterState();
		virtual bool onExitState();
		virtual void onCancelState();

		virtual void onTouchEnded(GridType type, uint8_t x, uint8_t y);
		virtual void onTouchCancelled();
		virtual void onTouchMoved(float deltaX, float deltaY);

		void animationEnd();

	protected:

	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_DRAG_SEED_H