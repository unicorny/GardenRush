#include "controller/ConfigLoader.h"
#include "controller/RessourcenManager.h"
#include "ErrorLog.h"

#include "json/document.h"
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
	}
	return true;

}