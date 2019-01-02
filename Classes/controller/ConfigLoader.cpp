#include "controller/ConfigLoader.h"
#include "controller/RessourcenManager.h"
#include "ErrorLog.h"
#include "scenes/SpriteBatchNodesHolderScene.h"
#include "json/document.h"
#include "model/ViewData.h"

#include "lib/TimeProfiler.h"
#include "lib/ProfilerManager.h"
//
using namespace rapidjson;
using namespace cocos2d;

ConfigLoader::ConfigLoader()
{

}

ConfigLoader::~ConfigLoader()
{

}


bool ConfigLoader::loadFromJson(const char* path)
{
	TimeProfiler time;
	auto profiler = ProfilerManager::getInstance();

	// load json from file and parse
	std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
	Document document;
	document.Parse(content.data());

	profiler->addTimeProfilerEntry("parse json", time.seconds());
	auto ressourcenManager = RessourcenManager::getInstance();
	

	// interpret json
	assert(document.IsObject());

	for (auto itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
	{
		// for every type
		if (!itr->name.IsString()) {
			LOG_ERROR("error, invalid index type(not a string)", false);
		}
		if (!strcmp(itr->name.GetString(), "materials")) {
			time.reset();
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
			profiler->addTimeProfilerEntry("loading material", time.seconds());

		}
		else if (!strcmp(itr->name.GetString(), "grid")) {
			time.reset();

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
					auto groundTilesMember = iTypes->value.FindMember("GroundTile");
					config->groundTiles.push_back(IViewData::createFromJson(&groundTilesMember));
				}

				// sides (only for iso and 3D)
				if (iTypes->value.HasMember("sides")) {
					auto sides = iTypes->value.FindMember("sides");
					if (sides->value.HasMember("left") && sides->value.HasMember("right")) {
						auto leftMember = sides->value.FindMember("left");
						auto rightMember = sides->value.FindMember("right");
						config->leftSide = IViewData::createFromJson(&leftMember);
						config->rightSide = IViewData::createFromJson(&rightMember);
					}
				}

				ressourcenManager->addGridConfig(config);
			}
			profiler->addTimeProfilerEntry("parse grid graphic infos", time.seconds());
		}
		else if (!strcmp(itr->name.GetString(), "spriteAtlas")) {
			time.reset();
			auto spriteFrameCache = cocos2d::SpriteFrameCache::getInstance();
			for (auto iSpriteAtlas = itr->value.MemberBegin(); iSpriteAtlas != itr->value.MemberEnd(); ++iSpriteAtlas) {
				if (iSpriteAtlas->name.IsString()) {
					auto spriteAtlasName = iSpriteAtlas->name.GetString();

					if (iSpriteAtlas->value.IsString()) {
						auto spriteAtlasPList = iSpriteAtlas->value.GetString();

						if (!ressourcenManager->addSpriteAtlas(spriteAtlasName, spriteAtlasPList, nullptr)) {
							LOG_ERROR("error adding spriteAtlas", false);
						}
						spriteFrameCache->addSpriteFramesWithFile(spriteAtlasPList);
						//spriteBatchHolder->addSpriteBatchNode(spriteAtlasPath, spriteAtlasName, 16);
					}
					else if (iSpriteAtlas->value.IsObject()) {
						if (iSpriteAtlas->value.HasMember("plist") && iSpriteAtlas->value.HasMember("texture")) {
							auto spriteAtlasPList = iSpriteAtlas->value["plist"].GetString();
							auto spriteAtlasTexture = iSpriteAtlas->value["texture"].GetString();

							if (!ressourcenManager->addSpriteAtlas(spriteAtlasName, spriteAtlasPList, spriteAtlasTexture)) {
								LOG_ERROR("error adding spriteAtlas", false);
							}
							spriteFrameCache->addSpriteFramesWithFile(spriteAtlasPList);
						}
					}
				}
				
			}
			profiler->addTimeProfilerEntry("loading spriteAtlases", time.seconds());
		}
		else if (!strcmp(itr->name.GetString(), "fonts")) {
			time.reset();
			for (auto iFonts = itr->value.MemberBegin(); iFonts != itr->value.MemberEnd(); ++iFonts) {
				if (iFonts->value.IsString() && iFonts->name.IsString()) {
					if (!ressourcenManager->addFont(iFonts->name.GetString(), iFonts->value.GetString())) {
						LOG_ERROR("error loading font", false);
					}
				}
				else {
					LOG_ERROR("[ConfigLoader::loadFromJson] error, font name invalid (expect both be string)", false);
				}
			}
			profiler->addTimeProfilerEntry("parsing font infos", time.seconds());
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