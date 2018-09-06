#include "model/ViewData.h"
#include "cocos2d.h"


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

PlantNode* ViewDataSimpleTexture::createPlantNode()
{
	PlantNode * ret = new (std::nothrow) PlantNode();
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

DHASH ViewDataSimpleTexture::getHash()
{
	return DRMakeFilenameHash(mTextureName.data());
}