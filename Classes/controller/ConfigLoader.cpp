#include "controller/ConfigLoader.h"
#include "controller/RessourcenManager.h"
#include "ErrorLog.h"
#include "json/document.h"
#include "model/ViewData.h"
#include "model/PlantType.h"

#include "lib/TimeProfiler.h"
#include "lib/ProfilerManager.h"

#include "ui/GuiManager.h"
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
		else if (!strcmp(itr->name.GetString(), "plantTypes")) {
			assert(itr->value.IsString());
			if (!loadPlantTypesFromJson(itr->value.GetString())) {
				LOG_ERROR("error by loading plant types", false);
			}
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
		else if (!strcmp(itr->name.GetString(), "movingLabelFont")) {
			assert(itr->value.IsString());
			auto fontPath = ressourcenManager->getFontPath(itr->value.GetString());
			GuiManager::getInstance()->setMovingLabelFont(fontPath);
		}
		else if (!strcmp(itr->name.GetString(), "buttonConfig")) {
			if (itr->value.IsObject()) {
				auto btnCfg = new RessourcenManager::ButtonConfig;
				if (itr->value.HasMember("name")) {
					btnCfg->name = itr->value["name"].GetString();
				}
				if (itr->value.HasMember("font")) {
					btnCfg->font = itr->value["font"].GetString();
				}
				if (itr->value.HasMember("pressedImg")) {
					btnCfg->pressedImg = itr->value["pressedImg"].GetString();
				}
				if (itr->value.HasMember("disabledImg")) {
					btnCfg->disabledImg = itr->value["disabledImg"].GetString();
				}
				if (itr->value.HasMember("defaultImg")) {
					btnCfg->defaultImg = itr->value["defaultImg"].GetString();
				}
				if (itr->value.HasMember("sprite9Rect")) {
					auto rect = itr->value.FindMember("sprite9Rect");
					if (rect->value.HasMember("x") && rect->value.HasMember("y") &&
						rect->value.HasMember("width") && rect->value.HasMember("height")) {
						btnCfg->sprite9Rect.setRect(
							rect->value["x"].GetInt(),
							rect->value["y"].GetInt(),
							rect->value["width"].GetInt(),
							rect->value["height"].GetInt()
						);
					}
					else {
						ErrorLog::printf("sprite9Rect declaration from ButtonConfig %s is invalid", btnCfg->name.data());
					}
				}
				ressourcenManager->setButtonConfig(btnCfg);
			}
		}
	}
	return true;

}

bool ConfigLoader::loadPlantTypesFromJson(const char* path)
{
	TimeProfiler time;
	auto profiler = ProfilerManager::getInstance();

	// load json from file and parse
	std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
	Document document;
	document.Parse(content.data());

	profiler->addTimeProfilerEntry("parse plantTypes json", time.seconds());
	time.reset();
	auto ressourcenManager = RessourcenManager::getInstance();

	for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
	{
		// for every plant
		assert(itr->name.IsString());
		u32 neighborCount = 0;
		// count neighbors
		if (itr->value.HasMember("neighbors")) {
			for (rapidjson::Value::ConstMemberIterator iNeighbors = itr->value["neighbors"].MemberBegin();
				iNeighbors != itr->value["neighbors"].MemberEnd(); ++iNeighbors)
			{
				neighborCount += iNeighbors->value.Capacity();
			}
		}

		PlantType* plant = new PlantType(itr->name.GetString(), neighborCount);

		// add neighbors
		if (itr->value.HasMember("neighbors")) {
			for (rapidjson::Value::ConstMemberIterator iNeighbors = itr->value["neighbors"].MemberBegin();
				iNeighbors != itr->value["neighbors"].MemberEnd(); ++iNeighbors)
			{
				assert(iNeighbors->name.IsString());
				PlantTypeNeighborType type = PlantType::getNeighborTypeFromString(iNeighbors->name.GetString());
				for (rapidjson::Value::ConstValueIterator iNeighbor = iNeighbors->value.Begin(); iNeighbor != iNeighbors->value.End(); ++iNeighbor) {
					assert(iNeighbor->IsString());
					plant->addNeighbor(iNeighbor->GetString(), type);
				}
			}
		}

		// add views
		if (itr->value.HasMember("graphics")) {
			for (rapidjson::Value::ConstMemberIterator iGraphics = itr->value["graphics"].MemberBegin();
				iGraphics != itr->value["graphics"].MemberEnd(); ++iGraphics) {
				assert(iGraphics->name.IsString());
				IViewData* view = NULL;
				PlantTypePhasisView type = PlantType::getPhasisViewFromString(iGraphics->name.GetString());
				if (iGraphics->value.IsString()) {
					view = IViewData::createFromJson(&iGraphics);
					plant->setViewData(view, type);
				}
				else if (iGraphics->value.IsArray()) {
					int count = 1;
					for (rapidjson::Value::ConstValueIterator itrArray = iGraphics->value.Begin(); itrArray != iGraphics->value.End(); ++itrArray) {
						std::string typeName = iGraphics->name.GetString();
						typeName += "_";
						typeName += std::to_string(count);
						type = PlantType::getPhasisViewFromString(typeName.data());
						assert(type < PLANT_PHASIS_SIZE);

						view = IViewData::createFromJson(&itrArray);
						plant->setViewData(view, type);
						count++;
					}
				}
				else if (iGraphics->value.IsObject()) {
					// Sprite Atlas
					std::string typeName = iGraphics->name.GetString();
					type = PlantType::getPhasisViewFromString(typeName.data());
					assert(type < PLANT_PHASIS_SIZE);

					if (iGraphics->value.HasMember("atlas") && iGraphics->value.HasMember("part")) {
						const char* atlasName = iGraphics->value["atlas"].GetString();
						const char* partName = iGraphics->value["part"].GetString();
						//return new ViewDataSpriteAtlas(atlasName, partName);
						plant->setViewData(new ViewDataSpriteAtlas(atlasName, partName), type);
					}
					else {
						LOG_ERROR("unknown graphics type", false);
					}
				}

			}
		}

		ressourcenManager->addPlantType(plant);
	}
	profiler->addTimeProfilerEntry("loading plantTypes into ressourcenManager", time.seconds());

	return true;
}

bool ConfigLoader::loadJsonStory(const char* path)
{
	TimeProfiler time;
	auto profiler = ProfilerManager::getInstance();

	// load json from file and parse
	std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
	Document document;
	document.Parse(content.data());

	profiler->addTimeProfilerEntry("parse story json", time.seconds());
	time.reset();
	auto ressourcenManager = RessourcenManager::getInstance();

	auto size = document.Capacity();
	ressourcenManager->initStoryPart(size);
	size_t cursor = 0;
	for (rapidjson::Value::ConstValueIterator itr = document.Begin(); itr != document.End(); ++itr) {
		
		if (itr->HasMember("name") && itr->HasMember("path")) {
			ressourcenManager->setStoryPart(
				itr->FindMember("name")->value.GetString(),
				itr->FindMember("path")->value.GetString(),
				cursor++
			);
		}
		else {
			LOG_ERROR("story.json has invalid entrys", false);
		}
	}
	ressourcenManager->checkStoryPartNameDoublette();
	profiler->addTimeProfilerEntry("loading story.json into ressourcenManager", time.seconds());

	return true;
}
