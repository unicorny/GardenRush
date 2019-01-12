#ifndef __FAIRY_GAMES_GARDEN_RUSH_MODEL_PLAYER_H
#define __FAIRY_GAMES_GARDEN_RUSH_MODEL_PLAYER_H
/*!
 *
 * \author: einhornimmond
 *
 * \date: 18.11.2018
 * 
 * \brief: Singleton Class for player relevant stuff, like savegame, points and settings
 *
*/

#include "model/Savegame.h"

enum PlayerSettings {
	SETTINGS_NONE = 0,
	SETTINGS_GRID_3D = 1,
	SETTINGS_GRID_ISO = 2,

};

class Player 
{
public:
	static Player* getInstance();

	bool load(Savegame* savegame);

	inline bool isGrid3D() { return (mSettings & SETTINGS_GRID_3D) == SETTINGS_GRID_3D; }
	inline bool isGridIso() { return (mSettings & SETTINGS_GRID_ISO) == SETTINGS_GRID_ISO; }

	inline bool isNewPlayer() { return mIsNewPlayer; }

	inline Savegame* getSavegame() { return &mSavegame; }
protected:

	bool initNewPlayer();

	Player();

	// not to save
	bool mIsNewPlayer;
	Savegame mSavegame;
	// to save

	PlayerSettings mSettings;
	
};

#endif //__FAIRY_GAMES_GARDEN_RUSH_MODEL_PLAYER_H