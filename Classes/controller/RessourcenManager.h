#ifndef __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_RESSOURCEN_MANAGER_H
#define __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_RESSOURCEN_MANAGER_H

#include "cocos2d.h"
#include "lib/DRHashList.hpp"

#include "Enums.h"

class PlantTypesManager;
class IViewData;
/*
 * \author: einhornimmond
 *
 * \date: 29.10.2018
 * 
 * \brief: manage ressourcen which not handled from cocos 2d like shader and materials
 *
 */

class RessourcenManager
{
public:
	
	~RessourcenManager();
	static RessourcenManager* getInstance();
	//bool initPlantTypes(PlantTypesManager* manager, )

	bool loadMaterial(const char* path, const char* name);
	
	inline cocos2d::Material* getMaterial(const char* name) const {
		return (cocos2d::Material*)mMaterials.findByHash(DRMakeStringHash(name));
	}

	bool addSpriteAtlas(const char* name, const char* plistFilename, const char* textureFilename);
	const char* getSpriteAtlasPath(const char* name);
	const char* getSpriteAtlasTexture(const char* name);

	
	static GridNodeType getGridNodeTypeFromString(const char* gridNodeTypeName);

	struct GridGraphicsConfig {
		GridGraphicsConfig() : type(GRID_NODE_NONE), leftSide(nullptr), rightSide(nullptr), overlay(nullptr), overlay_small(nullptr) {}
		~GridGraphicsConfig();

		GridNodeType type;
		std::vector<IViewData*> groundTiles;
		IViewData* leftSide;
		IViewData* rightSide;
		IViewData* overlay;
		IViewData* overlay_small;
	};

	struct SpriteAtlasConfig {
		std::string plistName;
		std::string textureName;
	};

	bool addGridConfig(GridGraphicsConfig* config);

	inline GridGraphicsConfig* getGridConfig(GridNodeType type) { assert(mGridConfigs.find(type) != mGridConfigs.end()); return mGridConfigs[type]; }

protected:
	RessourcenManager();

	DRHashList		mMaterials;
	std::map<DHASH, SpriteAtlasConfig*> mSpriteAtlases;
	std::map<GridNodeType, GridGraphicsConfig*> mGridConfigs;


};

#endif // __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_RESSOURCEN_MANAGER_H