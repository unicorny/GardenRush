#include "controller/RessourcenManager.h"
#include "ErrorLog.h"
#include "model/ViewData.h"

using namespace cocos2d;

// ------------------------------------- structs -----------------------------------------------

RessourcenManager::GridGraphicsConfig::~GridGraphicsConfig()
{
	DR_SAVE_DELETE(leftSide);
	DR_SAVE_DELETE(rightSide);
	for (auto it = groundTiles.begin(); it != groundTiles.end(); it++) {
		DR_SAVE_DELETE(*it);
	}
	groundTiles.clear();

}

// --------------------------------- Ressourcen Manager ----------------------------------------

RessourcenManager::RessourcenManager()
{

}

RessourcenManager::~RessourcenManager()
{
	for (u32 i = 0; i < mMaterials.getNItems(); i++) {
		Material* material = (Material*)mMaterials.findByIndex(i);
		material->release();
	}
	mMaterials.clear(true);

	for (auto it = mGridConfigs.begin(); it != mGridConfigs.end(); it++) {
		DR_SAVE_DELETE(it->second);
	}
	mGridConfigs.clear();
}

bool RessourcenManager::loadMaterial(const char* path, const char* name)
{
	
	Material* material = nullptr;
	ErrorLog::printf("try loading material from: %s\n", path);
	try {
		material = Material::createWithFilename(path);
		if (!material) {
			LOG_ERROR("coco2d report error by parsing/loading material", false);
		}

		material->retain();
		mMaterials.addByHash(DRMakeStringHash(name), material);

		return true;
	}
	catch (const std::exception& e) {
		ErrorLog::printf("exception thrown: %s\n", e.what());
		return false;
	}
	catch (...) {
		ErrorLog::printf("exception thrown while create material from file: %s\n", path);
		return false;
	}
	
}

bool RessourcenManager::addSpriteAtlas(const char* name, const char* filename)
{
	DHASH id = DRMakeStringHash(name);
	auto it = mSpriteAtlases.find(id);
	if (it != mSpriteAtlases.end()) {
		LOG_ERROR("spriteAtlas already exist", false);
	}
	mSpriteAtlases.insert(std::pair<DHASH, std::string>(id, filename));
	return true;
}

const char* RessourcenManager::getSpriteAtlasPath(const char* name)
{
	DHASH id = DRMakeStringHash(name);
	auto it = mSpriteAtlases.find(id);
	if (it == mSpriteAtlases.end()) {
		return nullptr;
	}

	return mSpriteAtlases[id].data();
}


// grid
GridNodeType RessourcenManager::getGridNodeTypeFromString(const char* gridNodeTypeName)
{
	if (strcmp(gridNodeTypeName, "2d") == 0) {
		return GRID_NODE_2D;
	}
	else if (strcmp(gridNodeTypeName, "iso") == 0) {
		return GRID_NODE_ISO;
	}
	else if (strcmp(gridNodeTypeName, "3d") == 0) {
		return GRID_NODE_3D;
	}
	return GRID_NODE_NONE;
}

bool RessourcenManager::addGridConfig(GridGraphicsConfig* config)
{
	assert(config);
	auto it = mGridConfigs.find(config->type);
	if (it != mGridConfigs.end()) {
		LOG_ERROR("error, grid type already exist", false);
	}
	mGridConfigs.insert(std::pair < GridNodeType, GridGraphicsConfig*>(config->type, config));

	return true;
}
