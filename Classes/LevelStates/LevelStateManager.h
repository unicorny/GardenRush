#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_STATE_MANAGER_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_STATE_MANAGER_H

/*!
 *
 * \author: Dario Rekowski
 *
 * \date: 10.09.18
 * 
 * \brief: Managed different level states
*/

#include "lib/DRHashList.hpp"

class LevelStateManager;

namespace level_state
{
	class iLevelState
	{
	public:
		iLevelState(LevelStateManager* manager) : mLevelStateManager(manager) {}
		virtual ~iLevelState() {};
		virtual const char* getName() const { return "iLevelState"; }
		virtual bool initState() = 0;
		virtual bool onEnterState() = 0;
		virtual bool onExitState() = 0;

	protected:
		LevelStateManager* mLevelStateManager;
	};
};

class LevelStateManager
{
public:
	LevelStateManager();
	~LevelStateManager();
	// free memory by exit
	bool addLevelState(level_state::iLevelState* levelState);

	bool transitTo(DHASH levelStateId);
	inline bool transitTo(const char* levelStateName) { transitTo(DRMakeStringHash(levelStateName)); }
protected:
	DRHashList mLevelStates;
	level_state::iLevelState* mActiveLevelState;
};



#endif //__FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_STATE_MANAGER_H