/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "GardenRushScene.h"
#include "SimpleAudioEngine.h"
#include "nodes/Grid.h"
#include "model/ViewData.h"
#include "PlantTypesManager.h"

USING_NS_CC;

Scene* GardenRush::createScene(PlantTypesManager* plantTypesManager)
{
	GardenRush* result = GardenRush::create();
	result->mPlantTypesManager = plantTypesManager;
	// init plant types manager
	result->mPlantTypesManager->loadFromJson("graphics.json");
	return result;
}


int printf2(const char *format, ...)
{

	char str[1024];

	va_list argptr;
	va_start(argptr, format);
	int ret = vsnprintf(str, sizeof(str), format, argptr);
	va_end(argptr);
#ifdef _MSC_VER 
	OutputDebugStringA(str);
#else 
	printf("%s", str);
#endif

	return ret;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf2("Error while loading: %s\n", filename);
    printf2("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}



// on "init" you need to initialize your instance
bool GardenRush::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	
	

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GardenRush::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

	auto toggleItem = MenuItemImage::create(
		"ToggleNormal.png", "ToggleSelected.png", CC_CALLBACK_1(GardenRush::menuToggleStatsCallback, this)
	);
	if (toggleItem == nullptr ||
		toggleItem->getContentSize().width <= 0 ||
		toggleItem->getContentSize().height <= 0)
	{
		problemLoading("'ToggleNormal.png' and 'ToggleSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - toggleItem->getContentSize().width / 2  - closeItem->getContentSize().width;
		float y = origin.y + toggleItem->getContentSize().height / 2;
		toggleItem->setPosition(Vec2(x, y));
	}

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, toggleItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Garden Rush Prototype", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 2);
    }

	// add grid
	///*
	ViewDataSimpleTexture view("gridCell.png");
	float gridDimension = visibleSize.height * 0.9f;
	float cellSize = gridDimension / 8.0f;
	Vec2 cellSizeVector(cellSize, cellSize);
	auto grid = Grid::create(8, 8);
	if (grid == nullptr) {
		problemLoading("Grid");
	} else {
		
		grid->setPixelSize(Vec2(gridDimension, gridDimension));
		grid->fillBgGridCells(&view);

		grid->setAnchorPoint(Vec2(0.0f, 0.0f));
		grid->setPosition(Vec2(
			//origin.x + (visibleSize.width - gridDimension) / 2.0f,
			origin.x + (visibleSize.width) * 0.2f,
			origin.y + visibleSize.height * 0.1f
		));
		this->addChild(grid, 1);
	}

	// add second grid as inventory
	auto inventory_grid = Grid::create(2, 2);
	if (inventory_grid == nullptr) {
		problemLoading("inventory Grid");
	}
	else {
		inventory_grid->setPixelSize(cellSizeVector * 2.0f);
		inventory_grid->fillBgGridCells(&view);
		inventory_grid->setAnchorPoint(Vec2::ZERO);
		inventory_grid->setPosition(Vec2(
			origin.x + gridDimension + visibleSize.width * 0.3f,
			origin.y + visibleSize.height * 0.1f
		));
		this->addChild(inventory_grid, 1);
	}

	// add third grid as 
	auto bucket_grid = Grid::create(1, 1);
	if (bucket_grid == nullptr) {
		problemLoading("bucket grid");
	}
	else {
		bucket_grid->setPixelSize(cellSizeVector);
		bucket_grid->fillBgGridCells(&view);
		bucket_grid->setAnchorPoint(Vec2::ZERO);
		bucket_grid->setPosition(Vec2(
			origin.x + gridDimension + visibleSize.width * 0.3f + cellSize * 0.5f,
			origin.y + visibleSize.height * 0.2f + cellSize * 2.0f
		));
		this->addChild(bucket_grid, 1);
	}
	//*/
   
    return true;
}


void GardenRush::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void GardenRush::menuToggleStatsCallback(cocos2d::Ref* pSender)
{
	auto director = Director::getInstance();
	mToogleStats = !mToogleStats;
	director->setDisplayStats(mToogleStats);
}
