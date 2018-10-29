#ifndef __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_CONFIG_LOADER_H
#define __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_CONFIG_LOADER_H

#include "cocos2d.h"
#include "lib/DRHashList.hpp"

class PlantTypesManager;

class ConfigLoader
{
public:
	ConfigLoader();
	~ConfigLoader();

	//bool initPlantTypes(PlantTypesManager* manager, )

	void loadMaterial(const char* path, const char* name);
	// \param fragmentShaderFilename filename with path for fragment/pixel shader
	// \param vertexShaderFilename filename with path for vertex shader
	void loadShader(const char* fragmentShaderFilename, const char* vertexShaderFilename, const char* name);

	inline cocos2d::Material* getMaterial(const char* name) const { 
		auto it = mMaterials.find(name);
		if (it != mMaterials.end())
			return it->second;
		return nullptr; 
	}
	inline cocos2d::GLProgram* getShader(const char* name) const {
		return (cocos2d::GLProgram*)mShaders.findByHash(DRMakeStringHash(name));
	}

protected:
	std::map<std::string, cocos2d::Material*> mMaterials;
	DRHashList		mShaders;
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_CONFIG_LOADER_H