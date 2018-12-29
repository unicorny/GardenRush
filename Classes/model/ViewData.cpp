#include "model/ViewData.h"
#include "scenes/SpriteBatchNodesHolderScene.h"

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
	} else if ((*itr)->value.IsObject()) {
		//(*(*itr))["atlas"];
		if ((*itr)->value.HasMember("atlas") && (*itr)->value.HasMember("part")) {
			const char* atlasName = (*itr)->value["atlas"].GetString(); // iGraphics->value["atlas"].GetString();
			const char* partName = (*itr)->value["part"].GetString();
			//return new ViewDataSpriteAtlas(atlasName, partName);
			return new ViewDataSpriteAtlas(atlasName, partName);
		}
		else {
			LOG_ERROR("hasn't expected members", nullptr);
		}
	} else {
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
			LOG_ERROR("file doesn't exist", nullptr);
		}
	}
	else if ((*itr)->IsObject()) {
		//(*(*itr))["atlas"];
		if ((*itr)->HasMember("atlas") && (*itr)->HasMember("part")) {
			const char* atlasName = (*(*itr))["atlas"].GetString(); // iGraphics->value["atlas"].GetString();
			const char* partName  = (*(*itr))["part"].GetString();
			//return new ViewDataSpriteAtlas(atlasName, partName);
			return new ViewDataSpriteAtlas(atlasName, partName);
		}
		else {
			LOG_ERROR("hasn't expected members", nullptr);
		}
	}
	else {
		LOG_ERROR("not implemented yet", nullptr);
	}
}

cocos2d::SpriteFrame* viewGetSpriteFrame(IViewData* view)
{
	if (VIEW_TYPE_SPRITE_ATLAS == view->getType()) {
		auto viewDataSpriteAtlas = static_cast<ViewDataSpriteAtlas*>(view);
		return viewDataSpriteAtlas->getSpriteFrame();
	}
	LOG_ERROR("view isn't sprite Atlas", nullptr);
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

PlantNode* ViewDataSimpleTexture::createPlantNode(const PlantType* plantType) const
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

cocos2d::Texture2D* ViewDataSimpleTexture::getTexture() const
{
	return cocos2d::Director::getInstance()->getTextureCache()->addImage(mTextureName);
}


// **************************** Sprite Atlas ***********************************************

ViewDataSpriteAtlas::ViewDataSpriteAtlas(const char* spriteAtlasName, const char* objectName)
	: IViewData(VIEW_TYPE_SPRITE_ATLAS), mSpriteAtlasName(spriteAtlasName), mObjectName(objectName)
{

}

ViewDataSpriteAtlas::~ViewDataSpriteAtlas()
{

}

cocos2d::Sprite* ViewDataSpriteAtlas::createSprite() const
{
	//return cocos2d::Sprite::create(mTextureName);
	return cocos2d::Sprite::createWithSpriteFrameName(mObjectName);
	//LOG_ERROR("not implemented yet", nullptr);
}

PlantNode* ViewDataSpriteAtlas::createPlantNode(const PlantType* plantType) const
{
	PlantNode * ret = new (std::nothrow) PlantNode(plantType);
	//auto spriteBatchNode = spriteBatchNodeHolder->getSpriteBatchNode(mSpriteAtlasName.data());
	cocos2d::SpriteFrame *frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(mObjectName);

	if (ret && ret->initWithSpriteFrame(frame))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

cocos2d::Texture2D* ViewDataSpriteAtlas::getTexture() const
{
	auto textureName = RessourcenManager::getInstance()->getSpriteAtlasTexture(mSpriteAtlasName.data());
	return cocos2d::Director::getInstance()->getTextureCache()->addImage(textureName);
}



bool ViewDataSpriteAtlas::changePlantNodeSprite(PlantNode* plantNode) const
{
	cocos2d::SpriteFrame *frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(mObjectName);

	// seems updating sprite frame move also anchor back to default
	auto anchorPoint = plantNode->getAnchorPoint();
	plantNode->setSpriteFrame(frame);
	plantNode->setAnchorPoint(anchorPoint);
	
	return true;
}



DHASH ViewDataSpriteAtlas::getHash() const
{
	return DRMakeFilenameHash(mObjectName.data());
}
