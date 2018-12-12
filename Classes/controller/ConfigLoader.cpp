#include "controller/ConfigLoader.h"
#include "controller/RessourcenManager.h"
#include "ErrorLog.h"
#include "scenes/SpriteBatchNodesHolderScene.h"
#include "json/document.h"
#include "model/ViewData.h"
//
using namespace rapidjson;
using namespace cocos2d;

ConfigLoader::ConfigLoader()
{

}

ConfigLoader::~ConfigLoader()
{

}


bool ConfigLoader::loadFromJson(const char* path, RessourcenManager* ressourcenManager)
{
	// load json from file and parse
	std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
	Document document;
	document.Parse(content.data());

	// interpret json
	assert(document.IsObject());
	
	for (auto itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
	{
		// for every type
		if (!itr->name.IsString()) {
			LOG_ERROR("error, invalid index type(not a string)", false);
		}
		if (!strcmp(itr->name.GetString(), "materials")) {
			for (auto iMaterials = itr->value.MemberBegin(); iMaterials != itr->value.MemberEnd(); ++iMaterials) {
				if (iMaterials->value.IsString() && iMaterials->name.IsString()) {
					if (!ressourcenManager->loadMaterial(iMaterials->value.GetString(), iMaterials->name.GetString())) {
						LOG_ERROR("error loading material", false);
					}
				}
				else {
					LOG_ERROR("[ConfigLoader::loadFromJson] error, material type or name invalid (expect both be string)", false);
				}
			}

		}
		else if (!strcmp(itr->name.GetString(), "grid")) {


			for (rapidjson::Value::ConstMemberIterator iTypes = itr->value["types"].MemberBegin();
				iTypes != itr->value["types"].MemberEnd(); ++iTypes) {

				RessourcenManager::GridGraphicsConfig* config = new RessourcenManager::GridGraphicsConfig;
				config->type = RessourcenManager::getGridNodeTypeFromString(iTypes->name.GetString());

				// ground tile (array or single element)
				if (!iTypes->value.HasMember("GroundTile")) {
					DR_SAVE_DELETE(config);
					LOG_ERROR("error, GroundTile not exist", false);
				}
				if (iTypes->value["GroundTile"].IsArray()) {
					int count = 0;
					for (rapidjson::Value::ConstValueIterator itrArray = iTypes->value["GroundTile"].Begin(); itrArray != iTypes->value["GroundTile"].End(); ++itrArray) {
						count++;
					}
					config->groundTiles.reserve(count);
					for (rapidjson::Value::ConstValueIterator itrArray = iTypes->value["GroundTile"].Begin(); itrArray != iTypes->value["GroundTile"].End(); ++itrArray) {
						config->groundTiles.push_back(IViewData::createFromJson(&itrArray));
					}
				}
				else {
					config->groundTiles.push_back(IViewData::createFromJson(&iTypes->value.FindMember("GroundTile")));
				}

				// sides (only for iso and 3D)
				if (iTypes->value.HasMember("sides")) {
					auto sides = iTypes->value.FindMember("sides");
					if (sides->value.HasMember("left") && sides->value.HasMember("right")) {
						config->leftSide = IViewData::createFromJson(&sides->value.FindMember("left"));
						config->rightSide = IViewData::createFromJson(&sides->value.FindMember("right"));
					}
				}

				// overlay
				if (iTypes->value.HasMember("overlay")) {
					config->overlay = IViewData::createFromJson(&iTypes->value.FindMember("overlay"));
				}
				if (iTypes->value.HasMember("overlay_small")) {
					config->overlay_small = IViewData::createFromJson(&iTypes->value.FindMember("overlay_small"));
				}

				ressourcenManager->addGridConfig(config);
			}
		}
		else if (!strcmp(itr->name.GetString(), "spriteAtlas")) {
			auto spriteFrameCache = cocos2d::SpriteFrameCache::getInstance();
			for (auto iSpriteAtlas = itr->value.MemberBegin(); iSpriteAtlas != itr->value.MemberEnd(); ++iSpriteAtlas) {
				if (iSpriteAtlas->value.IsString() && iSpriteAtlas->name.IsString()) {
					auto spriteAtlasPath = iSpriteAtlas->value.GetString();
					auto spriteAtlasName = iSpriteAtlas->name.GetString();
					if (!ressourcenManager->addSpriteAtlas(spriteAtlasName, spriteAtlasPath)) {
						LOG_ERROR("error adding spriteAtlas", false);
					}
					spriteFrameCache->addSpriteFramesWithFile(spriteAtlasPath);
					//spriteBatchHolder->addSpriteBatchNode(spriteAtlasPath, spriteAtlasName, 16);
				}
			}
		}
	}
	return true;

}

bool ConfigLoader::loadJsonSpriteAtlas(const char* path, cocos2d::Rect** mResultRects)
{
	// load json from file and parse
	std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
	Document document;
	document.Parse(content.data());

	// interpret json
	assert(document.IsArray());
	auto rectCount = document.Capacity();
	*mResultRects = new cocos2d::Rect[rectCount];
	int cursor = 0;
	for (auto itr = document.Begin(); itr != document.End(); ++itr)
	{
		(*mResultRects)[cursor] = Rect(
			itr['x'].GetInt(),
			itr['y'].GetInt(),
			itr['w'].GetInt(),
			itr['h'].GetInt()
		);
	}
	return true;
}