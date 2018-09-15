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

#include "MainGameScene.h"
#include "SimpleAudioEngine.h"
#include "nodes/Grid.h"
#include "model/ViewData.h"
#include "PlantTypesManager.h"
#include "levelStates/DisplayInfo.h"
#include "levelStates/DragSeed.h"
#include "levelStates/DropSeedInvalid.h"
#include "levelStates/DropSeedValid.h"
#include "levelStates/iLevelState.h"
#include "levelStates/RandomSeed.h"
#include "levelStates/PlayerChooseSeed.h"
#include "levelStates/PlayerChooseActionWithSeed.h"
#include "model/LevelData.h"
#include "ErrorLog.h"

USING_NS_CC;

Scene* MainGameScene::createScene(PlantTypesManager* plantTypesManager)
{
	cocos2d::Profiler* profiler = cocos2d::Profiler::getInstance();
	profiler->displayTimers();
	ProfilingBeginTimingBlock("initScene");
	MainGameScene* result = MainGameScene::create();
	ProfilingEndTimingBlock("initScene");
	ProfilingTimer* timer = profiler->_activeTimers.at("initScene");
	ErrorLog::printf("init Scene duration: %.4f ms\n", (double)timer->totalTime / 1000.0);
	ProfilingBeginTimingBlock("init plant types");
	result->mPlantTypesManager = plantTypesManager;
	// init plant types manager
	result->mPlantTypesManager->loadFromJson("graphics.json");
	result->mPlantTypesManager->makeFastAccessMap();
	ProfilingEndTimingBlock("init plant types");
	timer = profiler->_activeTimers.at("init plant types");
	ErrorLog::printf("init plant types duration: %.4f ms\n", (double)timer->totalTime / 1000.0);

	result->transitTo("RandomSeed");

	return result;
}

MainGameScene::MainGameScene()
	: mToogleStats(false), mPlantTypesManager(nullptr),
 mLevelData(nullptr), mLevelStateManager(nullptr), mActiveLevelState(nullptr), mTargetPlantNode(nullptr), mEnabledTouchTypes(ENABLED_TOUCH_NONE)
{
	memset(mGameGrids, 0, GRID_SIZE * sizeof(Grid*));
	mLevelData = new LevelData(5);
	mLevelData->addPlantType("carrot");
	mLevelData->addPlantType("garlic");
	mLevelData->addPlantType("leek");
	mLevelData->addPlantType("pea");
	mLevelData->addPlantType("strawberry");
}

MainGameScene::~MainGameScene() 
{
	for (int i = 0; i < GRID_SIZE; i++) {
		mGameGrids[i] = nullptr;
	}

	DR_SAVE_DELETE(mLevelStateManager);
	DR_SAVE_DELETE(mLevelData);	
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
bool MainGameScene::init()
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
                                           CC_CALLBACK_1(MainGameScene::menuCloseCallback, this));

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
		"ToggleNormal.png", "ToggleSelected.png", CC_CALLBACK_1(MainGameScene::menuToggleStatsCallback, this)
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
	auto grid = Grid::create(8, 8);
	if (grid == nullptr) {
		problemLoading("Grid");
	} else {
		auto position = Vec2(
			origin.x + (visibleSize.width) * 0.2f,
			origin.y + visibleSize.height * 0.05f
		);
		grid->setup(gridDimension, position, &view, this);
		mGameGrids[GRID_MAIN] = grid;
		mGridBoundingBoxes[GRID_MAIN * 3] = position.x;
		mGridBoundingBoxes[GRID_MAIN * 3 + 1] = position.y;
		mGridBoundingBoxes[GRID_MAIN * 3 + 2] = gridDimension;
	}

	// add second grid as inventory
	auto inventory_grid = Grid::create(2, 2);
	if (inventory_grid == nullptr) {
		problemLoading("inventory Grid");
	}
	else {
		auto position = Vec2(
			origin.x + gridDimension + visibleSize.width * 0.3f,
			origin.y + visibleSize.height * 0.05f
		);
		inventory_grid->setup(cellSize * 2.0f, position, &view, this);
		mGameGrids[GRID_INVENTORY] = inventory_grid;
		mGridBoundingBoxes[GRID_INVENTORY * 3] = position.x;
		mGridBoundingBoxes[GRID_INVENTORY * 3 + 1] = position.y;
		mGridBoundingBoxes[GRID_INVENTORY * 3 + 2] = cellSize * 2.0f;
	}

	// add third grid as 
	auto bucket_grid = Grid::create(1, 1);
	if (bucket_grid == nullptr) {
		problemLoading("bucket grid");
	}
	else {
		auto position = Vec2(
			origin.x + gridDimension + visibleSize.width * 0.3f + cellSize * 0.5f,
			origin.y + visibleSize.height * 0.15f + cellSize * 2.0f
		);
		bucket_grid->setup(cellSize, position, &view, this);
		mGameGrids[GRID_BUCKET] = bucket_grid;
		mGridBoundingBoxes[GRID_BUCKET * 3] = position.x;
		mGridBoundingBoxes[GRID_BUCKET * 3 + 1] = position.y;
		mGridBoundingBoxes[GRID_BUCKET * 3 + 2] = cellSize;
	}
	//*/
	
	addLevelState(new level_state::RandomSeed);
	addLevelState(new level_state::PlayerChooseSeed);
	addLevelState(new level_state::PlayerChooseActionWithSeed);
	addLevelState(new level_state::DisplayInfo);
	addLevelState(new level_state::DragSeed);
	addLevelState(new level_state::DropSeedInvalid);
	addLevelState(new level_state::DropSeedValid);

	// global touch listener
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(MainGameScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(MainGameScene::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(MainGameScene::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(MainGameScene::onTouchEnded, this);
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	// mouse for debugging
#ifdef _MSC_VER
	mMousePosLabel = Label::createWithTTF("Mouse:", "fonts/Marker Felt.ttf", 24);
	if (mMousePosLabel == nullptr)
	{
		problemLoading("mouse label 'fonts/Marker Felt.ttf'");
	}
	else
	{
		mMousePosLabel->setPosition(Vec2(origin.x,
			origin.y + visibleSize.height - mMousePosLabel->getContentSize().height));
		mMousePosLabel->setAnchorPoint(Vec2::ZERO);
		// add the label as a child to this layer
		this->addChild(mMousePosLabel, 2);
	}

	auto _mouseListener = EventListenerMouse::create();
	_mouseListener->onMouseMove = CC_CALLBACK_1(MainGameScene::onMouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
#endif
   
    return true;
}
bool MainGameScene::touchBeganIfInsideGrid(cocos2d::Vec2 pos, MainGridType type)
{
	if(isInsideGrid(pos, type)) {
		PlantNode* plantNode = mGameGrids[type]->isPlantNodeAtPosition(Vec2(pos.x - mGridBoundingBoxes[type * 3], pos.y - mGridBoundingBoxes[type * 3 + 1]));
		mActiveLevelState->onTouchBegan(plantNode);
	}
	return false;
}

bool MainGameScene::touchEndIfInsideGrid(cocos2d::Vec2 pos, MainGridType type)
{
	if (isInsideGrid(pos, type)) {
		GridIndex i = mGameGrids[type]->getGridIndex(pos);
		mActiveLevelState->onTouchEnded(type, i.x, i.y);
	}
	return false;
}

bool MainGameScene::isInsideGrid(cocos2d::Vec2 pos, MainGridType type)
{
	// if inside grid
	if (pos.x >= mGridBoundingBoxes[type * 3] &&
		pos.x <= mGridBoundingBoxes[type * 3] + mGridBoundingBoxes[type * 3 + 2] &&
		pos.y >= mGridBoundingBoxes[type * 3 + 1] &&
		pos.y <= mGridBoundingBoxes[type * 3 + 1] + mGridBoundingBoxes[type * 3 + 2]) {
		return true;
	}
	return false;
}

bool MainGameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (!(mEnabledTouchTypes & ENABLED_TOUCH_BEGIN)) return false;
	if (!mActiveLevelState) return false;
	// manuel bsp, check collision with grids
	// global pos
	Vec2 pos = touch->getLocationInView();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// touch y seems reversed at least under windows
#ifdef _MSC_VER
	pos.y = visibleSize.height - pos.y;
#endif
	// if lay on left screen size
	if (pos.x < mGridBoundingBoxes[GRID_INVENTORY * 3]) {
		touchBeganIfInsideGrid(pos, GRID_MAIN);
	}
	else {
		if (pos.y > mGridBoundingBoxes[GRID_BUCKET * 3 + 1]) {
			// if inside BUCKET
			touchBeganIfInsideGrid(pos, GRID_BUCKET);
		}
		else {
			// if inside INVENTORY
			touchBeganIfInsideGrid(pos, GRID_INVENTORY);
		}
	}

}

void MainGameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (!(mEnabledTouchTypes & ENABLED_TOUCH_MOVE)) return;
	if (!mActiveLevelState) return;
	Vec2 prevPos = touch->getPreviousLocationInView();
	Vec2 pos = touch->getLocationInView();
	Vec2 diff = pos - prevPos;
	mActiveLevelState->onTouchMoved(diff.x, diff.y);

}
void MainGameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (!(mEnabledTouchTypes & ENABLED_TOUCH_END)) return;
	if (!mActiveLevelState) return;

	Vec2 pos = touch->getLocationInView();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// touch y seems reversed at least under windows
#ifdef _MSC_VER
	pos.y = visibleSize.height - pos.y;
#endif
	// if lay on left screen size
	if (pos.x < mGridBoundingBoxes[GRID_INVENTORY * 3]) {
		touchEndIfInsideGrid(pos, GRID_MAIN);
	}
	else {
		if (pos.y > mGridBoundingBoxes[GRID_BUCKET * 3 + 1]) {
			// if inside BUCKET
			touchEndIfInsideGrid(pos, GRID_BUCKET);
		}
		else {
			// if inside INVENTORY
			touchEndIfInsideGrid(pos, GRID_INVENTORY);
		}
	}
}

void MainGameScene::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (!(mEnabledTouchTypes & ENABLED_TOUCH_CANCELLED)) return;
	if (!mActiveLevelState) return;

	mActiveLevelState->onTouchCancelled();
}

void MainGameScene::onMouseMove(Event *event)
{
	// to illustrate the event....
	EventMouse* e = (EventMouse*)event;
	std::ostringstream os;
	os << "MousePosition X:" << e->getCursorX() << " Y:" << e->getCursorY();
	mMousePosLabel->setString(os.str());
}

void MainGameScene::menuCloseCallback(Ref* pSender)
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

void MainGameScene::menuToggleStatsCallback(cocos2d::Ref* pSender)
{
	auto director = Director::getInstance();
	mToogleStats = !mToogleStats;
	director->setDisplayStats(mToogleStats);
}


bool MainGameScene::addLevelState(level_state::iLevelState* levelState)
{
	if (!levelState) LOG_ERROR("level state is zero", false);
	DHASH id = DRMakeStringHash(levelState->getName());
	if (!mLevelStates.addByHash(id, levelState)) {
		ErrorLog::printf("level state with name: %s exist", levelState->getName());
		LOG_ERROR("level state with this name already exist", false);
	}
	levelState->setMainGameScene(this);
	return true;
}

bool MainGameScene::transitTo(DHASH levelStateId)
{
	if (mActiveLevelState) {
		if (!mActiveLevelState->onExitState()) {
			ErrorLog::printf("error exit state: %s", mActiveLevelState->getName());
			LOG_ERROR("error exit state", false);
		}
	}
	auto newLevelState = (level_state::iLevelState*)mLevelStates.findByHash(levelStateId);
	if (!newLevelState) {
		ErrorLog::printf("level state with id: %d not exist", levelStateId);
		LOG_ERROR("level state not found", false);
	}
	mActiveLevelState = newLevelState;
	if (!mActiveLevelState->onEnterState()) {
		ErrorLog::printf("error enter state: %s", mActiveLevelState->getName());
		LOG_ERROR("error enter state", false);
	}

	return true;
}