

#ifndef __FAIRY_GAMES_GARDEN_RUSH_SCENES_SPRITE_BATCH_NODES_HOLDERR_SCENE_H
#define __FAIRY_GAMES_GARDEN_RUSH_SCENES_SPRITE_BATCH_NODES_HOLDERR_SCENE_H

#include "cocos2d.h"
#include "lib/DRHashList.hpp"

class SpriteBatchNodesHolderScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// create sprite Batch Node
	// \return true by success, false if spriteBatchNode with Hash already exist
	bool addSpriteBatchNode(const char* textureFileName, const char* name, ssize_t capacity);
	cocos2d::SpriteBatchNode* getSpriteBatchNode(const char* name) { return (cocos2d::SpriteBatchNode*)mSpriteBatchNodes.findByHash(DRMakeStringHash(name)); }

	// implement the "static create()" method manually
	CREATE_FUNC(SpriteBatchNodesHolderScene);

protected:
	// rendering
	DRHashList		mSpriteBatchNodes;
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_SCENES_SPRITE_BATCH_NODES_HOLDERR_SCENE_H
