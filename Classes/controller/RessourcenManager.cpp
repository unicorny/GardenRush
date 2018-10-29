#include "controller/RessourcenManager.h"
#include "ErrorLog.h"

using namespace cocos2d;

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
}

bool RessourcenManager::loadMaterial(const char* path, const char* name)
{
	Material* material = Material::createWithFilename(path);
	if (!material) {
		LOG_ERROR("coco2d report error by parsing/loading material", false);
	}
	material->retain();
	mMaterials.addByHash(DRMakeStringHash(name), material);

	return true;
}
