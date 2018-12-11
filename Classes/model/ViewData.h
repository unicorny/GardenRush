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
#include "json/document.h"

class RessourcenManager;
class SpriteBatchNodesHolderScene;

enum ViewType
{
	VIEW_TYPE_SIMPLE_TEXTUE,
	VIEW_TYPE_SPRITE_ATLAS,
	VIEW_TYPE_ANIMATION_SIMPLE_TEXTURE,
	VIEW_TYPE_ANIMATION_SPRITE_ATLAS,
	VIEW_TYPE_SIZE,
	VIEW_TYPE_UNKNOWN
};

class IViewData
{
public:
	IViewData(ViewType type) : mType(type) {}
	virtual ~IViewData() {};
	inline ViewType getType() const { return mType; }
	virtual cocos2d::Sprite* createSprite() const = 0;
	virtual cocos2d::Animation* createAnimation() = 0;
	virtual PlantNode* createPlantNode(const PlantType* plantType) const = 0;
	virtual bool changePlantNodeSprite(PlantNode* plantNode) const = 0;
	virtual DHASH getHash() const = 0;
	

	static ViewType viewTypeFromString(const char* type);
	static IViewData* createFromJson(const rapidjson::Value::ConstMemberIterator* itr);
	static IViewData* createFromJson(const rapidjson::Value::ConstValueIterator* itr);

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
	virtual PlantNode* createPlantNode(const PlantType* plantType) const;
	virtual bool changePlantNodeSprite(PlantNode* plantNode) const;
	virtual DHASH getHash() const;
protected:
	std::string mTextureName;
};

class ViewDataSpriteAtlas : public IViewData
{
public:
	ViewDataSpriteAtlas(const char* spriteAtlasName, const char* objectName);
	virtual ~ViewDataSpriteAtlas();
	virtual cocos2d::Sprite* createSprite() const;
	virtual cocos2d::Animation* createAnimation() { return NULL; }
	virtual PlantNode* createPlantNode(const PlantType* plantType) const;
	virtual bool changePlantNodeSprite(PlantNode* plantNode) const;
	virtual DHASH getHash() const;

protected:
	std::string mSpriteAtlasName;
	std::string mObjectName;
};




#endif //__FAIRY_GAMES_GARDEN_RUSH_MODEL_VIEW_DATA_H