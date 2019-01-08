#ifndef __FAIRY_GAMES_GARDEN_RUSH_MODEL_SAVEGAME_H
#define __FAIRY_GAMES_GARDEN_RUSH_MODEL_SAVEGAME_H
/*!
*
* \author: einhornimmond
*
* \date: 07.01.2019
*
* \brief: class for savegame, describe structure of a savegame
*
*/

#include "cocos2d.h"

#define SAVEGAME_VERSION 1

class Savegame
{
public:
	Savegame();
	~Savegame();

	//! \param filePath complete path
	bool loadFromFile(const char* filePath);
protected:

	cocos2d::ValueMap mSavegameData;

};



#endif // __FAIRY_GAMES_GARDEN_RUSH_MODEL_SAVEGAME_H