
#include "SpriteBatchNodesHolderScene.h"

USING_NS_CC;

Scene* SpriteBatchNodesHolderScene::createScene()
{
	return SpriteBatchNodesHolderScene::create();
}


// on "init" you need to initialize your instance
bool SpriteBatchNodesHolderScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	return true;
}

// for rendering
bool SpriteBatchNodesHolderScene::addSpriteBatchNode(const char* textureFileName, const char* name, ssize_t capacity)
{
	auto spriteBatchNode = SpriteBatchNode::create(textureFileName, capacity);
	//addChild(spriteBatchNode);
	bool ret = mSpriteBatchNodes.addByHash(DRMakeStringHash(name), spriteBatchNode);
	if (ret) {
		addChild(spriteBatchNode);
	}
	return ret;
}