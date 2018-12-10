#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLAYER_CHOOSE_CELL_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLAYER_CHOOSE_CELL_H

#include "iLevelState.h"

class PlantNode;

/*!
 * \author: einhornimmond
 * 
 * \date: 07.12.18
 * 
 * \brief forwarding to different states, based on conditions
 * 
 * If choosen a empty cell on Main Grid and a plantNode is choosen, transit to MoveSeed
 * If choosen a seeded plant on Main Grid,							transit to PlayerChooseSeeded
 * If choosen a seed on inventory or bucket grid,					transit to PlayerChooseSeed
 */

namespace level_state {



	class PlayerChooseCell : public iLevelState
	{
	public:
		PlayerChooseCell() {};
		virtual ~PlayerChooseCell() {};
		virtual const char* getName() const { return "PlayerChooseCell"; }
		virtual bool initState();
		virtual bool onEnterState();
		virtual bool onExitState();
		virtual void onCancelState();

		inline GridCell getTargetGridCell() { return mTargetGridCell; }

		virtual void onTouchBegan(GridType type, uint8_t x, uint8_t y);
	protected:
		GridCell mTargetGridCell;

	};
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_PLAYER_CHOOSE_CELL_H