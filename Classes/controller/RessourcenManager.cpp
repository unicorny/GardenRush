#include "controller/RessourcenManager.h"
#include "ErrorLog.h"
#include "model/ViewData.h"
#include "model/PlantType.h"

using namespace cocos2d;

RessourcenManager* RessourcenManager::getInstance()
{
	static RessourcenManager theOneAndOnly;
	return &theOneAndOnly;
}

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


RessourcenManager::ButtonConfig::ButtonConfig()
{

}

RessourcenManager::ButtonConfig::ButtonConfig(ButtonConfig* ins) 
{
	name = ins->name;
	font = ins->font;
	pressedImg = ins->pressedImg;
	disabledImg = ins->disabledImg;
	defaultImg = ins->defaultImg;
	sprite9Rect = ins->sprite9Rect;
};

// --------------------------------- Ressourcen Manager ----------------------------------------

RessourcenManager::RessourcenManager()
	: mButtonConfig(nullptr), mStoryParts(nullptr), mStoryPartCount(0)
{

}

RessourcenManager::~RessourcenManager()
{
	for (u32 i = 0; i < mMaterials.getNItems(); i++) {
		Material* material = (Material*)mMaterials.findByIndex(i);
		material->release();
	}
	mMaterials.clear(true);

	for (auto it = mSpriteAtlases.begin(); it != mSpriteAtlases.end(); it++) {
		DR_SAVE_DELETE(it->second);
	}
	mSpriteAtlases.clear();

	for (auto it = mGridConfigs.begin(); it != mGridConfigs.end(); it++) {
		DR_SAVE_DELETE(it->second);
	}

	mGridConfigs.clear();

	if (mButtonConfig) {
		DR_SAVE_DELETE(mButtonConfig);
	}

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

bool RessourcenManager::addFont(const char* name, const char* path)
{
	DHASH id = DRMakeStringHash(name);
	auto it = mFonts.find(id);
	if (it != mFonts.end()) {
		if (it->second == std::string(path)) {
			return true;
		}
		else {
			LOG_ERROR("hash collision by font", false);
		}
	}
	mFonts.insert(std::pair<DHASH, std::string>(id, path));
	return true;
}

bool RessourcenManager::addSpriteAtlas(const char* name, const char* plistFilename, const char* textureFilename)
{
	DHASH id = DRMakeStringHash(name);
	auto it = mSpriteAtlases.find(id);
	if (it != mSpriteAtlases.end()) {
		LOG_ERROR("spriteAtlas already exist", false);
	}
	SpriteAtlasConfig* cfg = new SpriteAtlasConfig;
	cfg->plistName = plistFilename;
	if (textureFilename) {
		cfg->textureName = textureFilename;
	}
	mSpriteAtlases.insert(std::pair<DHASH, SpriteAtlasConfig*>(id, cfg));
	return true;
}

const char* RessourcenManager::getSpriteAtlasPath(const char* name)
{
	DHASH id = DRMakeStringHash(name);
	auto it = mSpriteAtlases.find(id);
	if (it == mSpriteAtlases.end()) {
		return nullptr;
	}

	return mSpriteAtlases[id]->plistName.data();
}

const char* RessourcenManager::getSpriteAtlasTexture(const char* name)
{
	DHASH id = DRMakeStringHash(name);
	auto it = mSpriteAtlases.find(id);
	if (it == mSpriteAtlases.end()) {
		return nullptr;
	}

	return mSpriteAtlases[id]->textureName.data();
}

bool RessourcenManager::addPlantType(PlantType* plantType)
{
	assert(plantType);
	HASH id = plantType->getNameHash();
	if (mPlantTypes.findByHash(id)) {
		LOG_ERROR("plantType already exist", false);
	}
	mPlantTypes.addByHash(id, plantType);
	return true;
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


bool RessourcenManager::initStoryPart(size_t storyPartCount)
{
	assert(storyPartCount > 0);

	mStoryPartCount = storyPartCount;
	if (mStoryParts) {
		delete[] mStoryParts;
	}
	mStoryParts = new StoryConfig[storyPartCount];
	if (!mStoryParts) {
		LOG_ERROR("not enough memory", false);
	}
	return true;
}

const char* RessourcenManager::getStoryLuaByName(const char* name)
{
	assert(mStoryParts && mStoryPartCount > 0);
	for (size_t i = 0; i < mStoryPartCount; i++) {
		if (mStoryParts[i].name == std::string(name)) {
			return mStoryParts[i].luaPath.data();
		}
	}
	return nullptr;
}

const char* RessourcenManager::getNextStoryName(const char* name)
{
	assert(mStoryParts && mStoryPartCount > 0);
	for (size_t i = 0; i < mStoryPartCount; i++) {
		if (mStoryParts[i].name == std::string(name)) {
			if (i + 1 < mStoryPartCount) {
				return mStoryParts[i + 1].name.data();
			}
			break;
		}
	}
	return nullptr;
}

bool RessourcenManager::checkStoryPartNameDoublette()
{
	assert(mStoryParts && mStoryPartCount > 0);
	std::list<std::string> mCompareList;
	for (size_t i = 0; i < mStoryPartCount; i++) {
		mCompareList.push_back(mStoryParts[i].name);
	}
	mCompareList.sort();
	const char* lastName = nullptr;
	for (auto it = mCompareList.begin(); it != mCompareList.end(); it++) {
		if (lastName) {
			if (*it == std::string(lastName)) return false;
		}
		lastName = it->data();
	}
	return true;
}

double RessourcenManager::getMemoryConsumption()
{
	unsigned long memory = sizeof(RessourcenManager);
	memory += mMaterials.getNItems() * (sizeof(DRHashListItem) + sizeof(Material));
	memory += mSpriteAtlases.size() * (sizeof(DHASH) + sizeof(SpriteAtlasConfig) + sizeof(SpriteAtlasConfig*));
	memory += mGridConfigs.size() * (sizeof(GridNodeType) + sizeof(GridGraphicsConfig) + sizeof(GridGraphicsConfig*));
	memory += mFonts.size() * (sizeof(DHASH) + sizeof(std::string));
	memory += mPlantTypes.getNItems() * (sizeof(DRHashListItem) + sizeof(PlantType));

	if (mStoryParts && mStoryPartCount > 0) {
		for (int i = 0; i < mStoryPartCount; i++)  {
			memory += mStoryParts[i].luaPath.size() + mStoryParts[i].name.size();
		}
		memory += mStoryPartCount * sizeof(StoryConfig);
	}

	return memory;
}