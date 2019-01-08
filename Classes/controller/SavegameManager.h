#ifndef __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_SAVEGAME_MANAGER_H
#define __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_SAVEGAME_MANAGER_H


/*
* \author: einhornimmond
*
* \date: 06.01.2019
*
* \brief: knows the savegames 
*
*/

#include "model/Savegame.h"


class SavegameManager
{
public:
	//static SavegameManager* getInstance();
	SavegameManager();
	~SavegameManager();

	bool init();

	inline size_t getSavegameCount() { return mSavegameCount; }
	inline Savegame* operator[] (size_t index) { assert(index < mSavegameCount); return &mSavegames[index]; }
protected:

	Savegame* mSavegames;
	size_t mSavegameCount;

	


};

#endif // __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_SAVEGAME_MANAGER_H