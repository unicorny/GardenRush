#include "MainMenuLayout.h"

using namespace cocos2d;

MainMenuLayout::MainMenuLayout()
{

}

MainMenuLayout::~MainMenuLayout()
{

}

bool MainMenuLayout::enable()
{
	return true;
}

bool MainMenuLayout::init()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backContainer = Sprite::create("box1.png");
	backContainer->setCenterRect(Rect(18, 16, 36, 32));
	backContainer->setAnchorPoint(Vec2(0.0f, 0.0f));
	auto containerSize = backContainer->getContentSize();
	backContainer->setScaleX((visibleSize.width * 0.6f) / containerSize.width);
	backContainer->setScaleY((visibleSize.height * 0.9f) / containerSize.height);
	backContainer->setPosition(origin + Vec2(visibleSize.width * 0.2f, visibleSize.height * 0.05f));

	addChild(backContainer);

	// buttons


	return true;
}