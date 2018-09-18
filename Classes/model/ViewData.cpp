#include "model/ViewData.h"
#include "cocos2d.h"
#include "ErrorLog.h"

ViewType IViewData::viewTypeFromString(const char* type)
{
	if (strcmp("simple_texture", type) == 0) return VIEW_TYPE_SIMPLE_TEXTUE;
	else if (strcmp("sprite_atlas", type) == 0) return VIEW_TYPE_SPRITE_ATLAS;
	else if (strcmp("animation_simple", type) == 0) return VIEW_TYPE_ANIMATION_SIMPLE_TEXTURE;
	else if (strcmp("animation_sprite_atlas", type) == 0) return VIEW_TYPE_ANIMATION_SPRITE_ATLAS;
	return VIEW_TYPE_UNKNOWN;
}

IViewData* IViewData::createFromJson(const rapidjson::Value::ConstMemberIterator* itr)
{
	if ((*itr)->value.IsString()) {
		const char* filename = (*itr)->value.GetString();
		if (cocos2d::FileUtils::getInstance()->isFileExist(filename)) {
			return new ViewDataSimpleTexture((*itr)->value.GetString());
		}
		else {
			ErrorLog::printf("file: %s doesn't exist", filename);
			LOG_ERROR("file doesn't exist", NULL);
		}
	}
	else {
		LOG_ERROR("not implemented yet", NULL);
	}
}
IViewData* IViewData::createFromJson(const rapidjson::Value::ConstValueIterator* itr)
{
	if ((*itr)->IsString()) {
		const char* filename = (*itr)->GetString();
		if (cocos2d::FileUtils::getInstance()->isFileExist(filename)) {
			return new ViewDataSimpleTexture((*itr)->GetString());
		}
		else {
			ErrorLog::printf("file: %s doesn't exist", filename);
			LOG_ERROR("file doesn't exist", NULL);
		}
	}
	else {
		LOG_ERROR("not implemented yet", NULL);
	}
}


// **************************** Simple Texture ***********************************************

ViewDataSimpleTexture::ViewDataSimpleTexture(const char* textureName)
	: IViewData(VIEW_TYPE_SIMPLE_TEXTUE), mTextureName(textureName)
{

}

ViewDataSimpleTexture::~ViewDataSimpleTexture()
{

}

cocos2d::Sprite* ViewDataSimpleTexture::createSprite() const
{
	return cocos2d::Sprite::create(mTextureName);
}

PlantNode* ViewDataSimpleTexture::createPlantNode(PlantType* plantType) const
{
	PlantNode * ret = new (std::nothrow) PlantNode(plantType);
	if (ret && ret->initWithFile(mTextureName))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool ViewDataSimpleTexture::changePlantNodeSprite(PlantNode* plantNode) const
{
	
	plantNode->setTexture(mTextureName);
	return true;
}

DHASH ViewDataSimpleTexture::getHash() const
{
	return DRMakeFilenameHash(mTextureName.data());
}

