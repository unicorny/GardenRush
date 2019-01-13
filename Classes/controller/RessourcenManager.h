#ifndef __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_RESSOURCEN_MANAGER_H
#define __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_RESSOURCEN_MANAGER_H

#include "cocos2d.h"
#include "lib/DRHashList.hpp"

#include "Enums.h"

class PlantTypesAccess;
class PlantType;
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

	// material
	bool loadMaterial(const char* path, const char* name);
	
	inline cocos2d::Material* getMaterial(const char* name) const {
		return (cocos2d::Material*)mMaterials.findByHash(DRMakeStringHash(name));
	}

	// Sprite Atlas
	bool addSpriteAtlas(const char* name, const char* plistFilename, const char* textureFilename);
	const char* getSpriteAtlasPath(const char* name);
	const char* getSpriteAtlasTexture(const char* name);

	// Font
	bool addFont(const char* name, const char* path);
	inline const char* getFontPath(const char* name) {
		DHASH id = DRMakeStringHash(name);
		assert(mFonts.find(id) != mFonts.end()); return mFonts[id].data();
	}
	
	// Plant Types
	bool addPlantType(PlantType* plantType);
	inline PlantType* findPlantType(const char* name) const { return findPlantType(DRMakeStringHash(name)); }
	inline PlantType* findPlantType(DHASH id) const { return static_cast<PlantType*>(mPlantTypes.findByHash(id)); }

	// grid node graphics infos
	static GridNodeType getGridNodeTypeFromString(const char* gridNodeTypeName);

	struct GridGraphicsConfig {
		GridGraphicsConfig() : type(GRID_NODE_NONE), leftSide(nullptr), rightSide(nullptr) {}
		~GridGraphicsConfig();

		GridNodeType type;
		std::vector<IViewData*> groundTiles;
		IViewData* leftSide;
		IViewData* rightSide;
	};

	struct ButtonConfig {
		ButtonConfig(ButtonConfig* ins);
		ButtonConfig();
		std::string name;
		std::string font;
		std::string pressedImg;
		std::string disabledImg;
		std::string defaultImg;
		cocos2d::Rect sprite9Rect;
	};
	

	bool addGridConfig(GridGraphicsConfig* config);
	inline GridGraphicsConfig* getGridConfig(GridNodeType type) { assert(mGridConfigs.find(type) != mGridConfigs.end()); return mGridConfigs[type]; }

	inline void setButtonConfig(ButtonConfig* btnCfg) { DR_SAVE_DELETE(mButtonConfig); mButtonConfig = btnCfg; }
	inline ButtonConfig* getButtonConfig() { return mButtonConfig; }

	bool initStoryPart(size_t storyPartCount);
	inline void setStoryPart(const char* name, const char* luaPath, size_t index) { 
		assert(index >= 0 && index < mStoryPartCount); 
		mStoryParts[index].name = name;
		mStoryParts[index].luaPath = luaPath;
	}
	// \return false if at least one name exist more than once
	bool checkStoryPartNameDoublette();

	inline size_t getStoryPartCount() { return mStoryPartCount; }
	const char* getStoryLuaByName(const char* name);
	const char* getNextStoryName(const char* name);
	inline const char* getStoryNameByIndex(size_t index) { assert(index >= 0 && index < mStoryPartCount); return mStoryParts[index].name.data(); }

	double getMemoryConsumption();

protected:
	struct SpriteAtlasConfig {
		std::string plistName;
		std::string textureName;
	};

	struct StoryConfig {
		std::string name;
		std::string luaPath;
	};

	

	RessourcenManager();

	DRHashList		mMaterials;
	std::map<DHASH, SpriteAtlasConfig*> mSpriteAtlases;
	std::map<GridNodeType, GridGraphicsConfig*> mGridConfigs;
	std::map<DHASH, std::string> mFonts;

	ButtonConfig*	mButtonConfig;

	DRHashList		mPlantTypes;

	// Story relevant stuff
	StoryConfig*	mStoryParts;
	size_t			mStoryPartCount;

};

#endif // __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_RESSOURCEN_MANAGER_H