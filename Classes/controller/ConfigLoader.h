#ifndef __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_CONFIG_LOADER_H
#define __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_CONFIG_LOADER_H

#include "cocos2d.h"
#include "lib/DRHashList.hpp"

class PlantTypesAccess;
class RessourcenManager;
class SpriteBatchNodesHolderScene;

class ConfigLoader
{
public:
	ConfigLoader();
	~ConfigLoader();

	//bool initPlantTypes(PlantTypesManager* manager, )

	static bool loadFromJson(const char* path);

	static bool loadJsonStory(const char* path);

	// mResultRects pointer at memory allocated, caller must free memory after use
	//static bool loadJsonSpriteAtlas(const char* path, cocos2d::Rect** mResultRects);

	

protected:
	static bool loadPlantTypesFromJson(const char* path);

};

#endif // __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_CONFIG_LOADER_H