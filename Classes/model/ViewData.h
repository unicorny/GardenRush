#ifndef __FAIRY_GAMES_GARDEN_RUSH_MODEL_VIEW_DATA_H
#define __FAIRY_GAMES_GARDEN_RUSH_MODEL_VIEW_DATA_H

/*!
 * \author: Dario Rekowski
 * 
 * \date: 30.08.18
 * 
 * \brief: contain infos for sprite view, texture name, sprite atlas infos and/or animation infos
*/

#include <string>
#include "cocos2d.h"
#include "nodes/PlantNode.h"
#include "lib/DRHash.hpp"

enum ViewType
{
	VIEW_TYPE_SIMPLE_TEXTUE = 1,
	VIEW_TYPE_SPRITE_ATLAS = 2,
	VIEW_TYPE_ANIMATION_SIMPLE_TEXTURE = 3,
	VIEW_TYPE_ANIMATION_SPRITE_ATLAS = 4
};


class IViewData
{
public:
	IViewData(ViewType type) : mType(type) {}
	virtual ~IViewData() {};
	inline ViewType getType() const { return mType; }
	virtual cocos2d::Sprite* createSprite() const = 0;
	virtual cocos2d::Animation* createAnimation() = 0;
	virtual PlantNode* createPlantNode() = 0;
	virtual DHASH getHash() = 0;

protected:
	ViewType mType;
};


class ViewDataSimpleTexture : public IViewData
{
public:
	ViewDataSimpleTexture(const char* textureName);
	virtual ~ViewDataSimpleTexture();
	virtual cocos2d::Sprite* createSprite() const;
	virtual cocos2d::Animation* createAnimation() { return NULL; }
	virtual PlantNode* createPlantNode();
	virtual DHASH getHash();
protected:
	std::string mTextureName;
};




#endif //__FAIRY_GAMES_GARDEN_RUSH_MODEL_VIEW_DATA_H