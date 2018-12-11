#ifndef __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_RESSOURCEN_MANAGER_H
#define __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_RESSOURCEN_MANAGER_H

#include "cocos2d.h"
#include "lib/DRHashList.hpp"

class PlantTypesManager;

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
	RessourcenManager();
	~RessourcenManager();

	//bool initPlantTypes(PlantTypesManager* manager, )

	bool loadMaterial(const char* path, const char* name);
	
	inline cocos2d::Material* getMaterial(const char* name) const {
		return (cocos2d::Material*)mMaterials.findByHash(DRMakeStringHash(name));
	}

	bool addSpriteAtlas(const char* name, const char* filename);
	const char* getSpriteAtlasPath(const char* name);


protected:
	DRHashList		mMaterials;
	DRHashList		mSpriteBatchNodes;

	std::map<DHASH, std::string> mSpriteAtlases;
	

};

#endif // __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_RESSOURCEN_MANAGER_H