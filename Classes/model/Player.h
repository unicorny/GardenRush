/*!
 *
 * \author: einhornimmond
 *
 * \date: 18.11.2018
 * 
 * \brief: Singleton Class for player relevant stuff, like savegame, points and settings
 *
*/

enum PlayerSettings {
	SETTINGS_NONE = 0,
	SETTINGS_GRID_3D = 1,
	SETTINGS_GRID_ISO = 2,

};

class Player 
{
public:
	static Player* getInstance();

	inline bool isGrid3D() { return (mSettings & SETTINGS_GRID_3D) == SETTINGS_GRID_3D; }
	inline bool isGridIso() { return (mSettings & SETTINGS_GRID_ISO) == SETTINGS_GRID_ISO; }
protected:
	Player();

	PlayerSettings mSettings;
	
};