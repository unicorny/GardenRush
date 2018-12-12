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
//#include "nodes/Grid.h"
#include "model/ViewData.h"
#include "PlantTypesManager.h"
#include "levelStates/DisplayInfo.h"
#include "levelStates/DragSeed.h"
#include "levelStates/DropSeedInvalid.h"
#include "levelStates/DropSeedValid.h"
#include "levelStates/HarvestPlant.h"
#include "levelStates/iLevelState.h"
#include "levelStates/MoveSeed.h"
#include "levelStates/RandomSeed.h"
#include "levelStates/PlantSeed.h"
#include "levelStates/PlayerChooseCell.h"
#include "levelStates/PlayerChooseSeed.h"
#include "levelStates/PlayerChooseActionWithSeed.h"
#include "levelStates/PlayerChooseActionWithSeeded.h"
#include "model/LevelData.h"
#include "model/Points.h"
#include "ErrorLog.h"
#include "controller/RessourcenManager.h"
#include "nodes/GridNode.h"




USING_NS_CC;

Scene* MainGameScene::createScene(PlantTypesManager* plantTypesManager, Points* points, TemplateMemoryManager<SpriteAnimationState>* animationStateMemorymanager, RessourcenManager* ressourcenManager)
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
	result->mRessourcenManager = ressourcenManager;
	result->mPoints = points;
	result->mAnimationStateMemoryManager = animationStateMemorymanager;
	points->addPointChangeCallback(result, "main");
	// init plant types manager
	result->mPlantTypesManager->loadFromJson("graphics.json");
	result->mPlantTypesManager->makeFastAccessMap();
	ProfilingEndTimingBlock("init plant types");
	timer = profiler->_activeTimers.at("init plant types");
	ErrorLog::printf("init plant types duration: %.4f ms\n", (double)timer->totalTime / 1000.0);
	result->initAfterCreate();
	result->transitTo("RandomSeed");

	return result;
}

MainGameScene::MainGameScene()
	: mToogleStats(false), mPlantTypesManager(nullptr), mAnimationStateMemoryManager(nullptr), mPoints(nullptr),
 mLevelData(nullptr), mActiveLevelState(nullptr), mTargetPlantNode(nullptr), mEnabledTouchTypes(ENABLED_TOUCH_NONE)
{
	memset(mGameGrids, 0, GRID_SIZE * sizeof(Grid*));
	mLevelData = new LevelData(5);
	mLevelData->addPlantType("carrot");
	mLevelData->addPlantType("garlic");
	mLevelData->addPlantType("leek");
	mLevelData->addPlantType("pea");
	mLevelData->addPlantType("strawberry");
	mLevelData->enableAutoHarvesting();
	mLevelData->setMaxGrowthPhasis(PLANT_PHASIS_GROWTH_5);

	scheduleUpdate();

#ifdef _MSC_VER
	memset(mArrowPressed, 0, sizeof(bool) * 4);
#endif	
}

MainGameScene::~MainGameScene() 
{
	for (int i = 0; i < GRID_SIZE; i++) {
		mGameGrids[i] = nullptr;
	}

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

#define DISABLE_UI

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
	mResolution = visibleSize;

#ifndef DISABLE_UI
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

	auto resetItem = MenuItemImage::create(
		"ResetNormal.png", "ResetSelected.png", CC_CALLBACK_1(MainGameScene::menuResetCallback, this)
	);
	if (resetItem == nullptr ||
		resetItem->getContentSize().width <= 0 ||
		resetItem->getContentSize().height <= 0)
	{
		problemLoading("'ResetNormal.png' and 'ResetNormal.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - resetItem->getContentSize().width / 2 - toggleItem->getContentSize().width - closeItem->getContentSize().width;
		float y = origin.y + resetItem->getContentSize().height / 2;
		resetItem->setPosition(Vec2(x, y));
	}
	
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, toggleItem, resetItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 3);
#endif //DISABLE_UI

	// //////////////////////////
	// bg
	// 
	/*auto bg = Sprite::create("bg1.png");
	
	if (bg) {
		bg->setAnchorPoint(Vec2(0.0f, 0.0f));
		//bg->setScale(4.0f);	
		auto size = bg->getContentSize();
		bg->setScale(mResolution.width / size.width, mResolution.height / size.height);
		this->addChild(bg, 0, "bg");
	}
	//*/
	LayerColor *_bgColor = LayerColor::create(Color4B(130, 156, 184, 255));
	_bgColor->setGlobalZOrder(-10.0f);
	this->addChild(_bgColor, -10);
	
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
#ifndef DISABLE_UI
    auto label = Label::createWithTTF("Garden Rush Prototype", "fonts/Charmonman-Regular.ttf", 24);
	
    if (label == nullptr)
    {
        problemLoading("'fonts/Charmonman-Regular.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width,
                                origin.y + visibleSize.height - label->getContentSize().height));
		label->setAnchorPoint(Vec2(1.0f, 0.0f));
		label->setColor(Color3B(0, 0, 0));
        // add the label as a child to this layer
        this->addChild(label, 2);
    }
	auto fontCfg = label->getTTFConfig();

	mPointsLabel = Label::createWithTTF(fontCfg, "Punkte: 0");
	if (mPointsLabel == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the right side of the screen
		mPointsLabel->setPosition(Vec2(
			origin.x + visibleSize.width * 0.9f,
			origin.y + visibleSize.height - mPointsLabel->getContentSize().height - label->getContentSize().height));
		mPointsLabel->setColor(Color3B(0,0,0));
		mPointsLabel->setAnchorPoint(Vec2(1.0f, 0.0f));
		// add the label as a child to this layer
		this->addChild(mPointsLabel, 2);
	}
	mMovingPointsLabel = Label::createWithTTF(fontCfg, "");
	if (mMovingPointsLabel) {
		this->addChild(mMovingPointsLabel, 10);
		mMovingPointsLabel->setGlobalZOrder(10.0f);
	}
#else 
	Director::getInstance()->setDisplayStats(true);
#endif //DISABLE_UI

	
	//*/
	addLevelState(new level_state::DisplayInfo);
	addLevelState(new level_state::DragSeed);
	addLevelState(new level_state::DropSeedInvalid);
	addLevelState(new level_state::DropSeedValid);
	addLevelState(new level_state::HarvestPlant);
	addLevelState(new level_state::MoveSeed);
	addLevelState(new level_state::PlantSeed);
	addLevelState(new level_state::PlayerChooseCell);
	addLevelState(new level_state::PlayerChooseSeed);
	addLevelState(new level_state::PlayerChooseActionWithSeed);
	addLevelState(new level_state::PlayerChooseActionWithSeeded);
	addLevelState(new level_state::RandomSeed);
	
	
	
	// global touch listener
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(MainGameScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(MainGameScene::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(MainGameScene::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(MainGameScene::onTouchEnded, this);
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	// accelerometer event
	auto accelListener = EventListenerAcceleration::create(CC_CALLBACK_2(MainGameScene::onAcceleration, this));

	_eventDispatcher->addEventListenerWithSceneGraphPriority(accelListener, this);
 

	// mouse for debugging
#ifndef DISABLE_UI
#ifdef _MSC_VER
	mMousePosLabel = Label::createWithTTF(fontCfg, "Mouse:");
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

	auto _keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = CC_CALLBACK_2(MainGameScene::onKeyPressed, this);
	_keyboardListener->onKeyReleased = CC_CALLBACK_2(MainGameScene::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);
#endif //_MSC_VER


	// label for debugging
#ifdef _FAIRY_DEBUG_
	mCurrentGameStateLabel = Label::createWithTTF(fontCfg, "RandomSeed");
	if (mCurrentGameStateLabel == nullptr) {
		problemLoading("current game state 'fonts/Marker Felt.ttf'");
	}
	else {
		auto gameStateLabelPos = Vec2(origin.x,
			origin.y + visibleSize.height - mCurrentGameStateLabel->getContentSize().height
		);
#ifdef _MSC_VER
		gameStateLabelPos.y -= mMousePosLabel->getContentSize().height;
#endif 
		mCurrentGameStateLabel->setPosition(gameStateLabelPos);
		mCurrentGameStateLabel->setAnchorPoint(Vec2::ZERO);
		// add to layer
		this->addChild(mCurrentGameStateLabel, 2);
	}
#endif
#endif //DISABLE_UI

    return true;
}

bool MainGameScene::initAfterCreate()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	std::vector<IViewData*> mGroundCells;

	// add grid
	///*
/*	ViewDataSpriteAtlas view("plants1", "gridCell.png");
	// multiple bgs 
	mGroundCells.push_back(new ViewDataSpriteAtlas("plants1", "boden0.png"));
	mGroundCells.push_back(new ViewDataSpriteAtlas("plants1", "boden1.png"));
	mGroundCells.push_back(new ViewDataSpriteAtlas("plants1", "boden2.png"));
	*/
	ViewDataSimpleTexture view("gridCell.png");
	//ViewDataSimpleTexture view("gridCell_bl.png");

	// multiple bgs 
	mGroundCells.push_back(new ViewDataSimpleTexture("bg/boden0.png"));
	mGroundCells.push_back(new ViewDataSimpleTexture("bg/boden1.png"));
	mGroundCells.push_back(new ViewDataSimpleTexture("bg/boden2.png"));
	//*/
	//mGroundCells.push_back(new ViewDataSimpleTexture("bg/boden_bl.png"));
	//*/
	//bg_cells[0] = new ViewDataSimpleTexture("gridCell.png");
	// Layout
	float flatCellSize = (visibleSize.height * 0.9f) / 8.0f;
	float xBorder = visibleSize.width * 0.01f;
	float isoEdgeWidth = visibleSize.width - 2.0f * xBorder;// -2.0f * flatCellSize;
	float isoEdgeHeight = isoEdgeWidth / 2.0f;

	float yBorder = (visibleSize.height - isoEdgeHeight) / 2.0f;
	assert(yBorder >= 0.0f);

	//float gridDimension = visibleSize.height * 0.9f;
	//float cellSize = gridDimension / 8.0f;
	uint8_t levelMainGridCellCount = 8;
	Vec2 isoBoundingBoxSize = Vec2(
		(isoEdgeWidth / 8.0f) * static_cast<float>(levelMainGridCellCount),
		(isoEdgeHeight / 8.0f) * static_cast<float>(levelMainGridCellCount)
	);
	auto grid = Grid::create(levelMainGridCellCount, levelMainGridCellCount, GRID_MAIN);
	auto gridNode = new GridNode(mRessourcenManager);
	gridNode->setup(8, isoBoundingBoxSize, GRID_NODE_ISO, this);
	auto cam = getDefaultCamera();
	auto camPos = cam->getPosition3D();
	//cam->initPerspective(35.0f, visibleSize.width / visibleSize.height, 10.0f, 1500.0f);
	//	cam->
	//camPos.y = 450.0f;
	//camPos.z = -205.0f;
	//camPos.z = -150.0f; // 35° fov
	camPos.z = 0.0f;
	gridNode->setPosition3D(camPos);

	addChild(gridNode);

	auto mainGridPosition = Vec2(
		origin.x + xBorder,
		origin.y + yBorder
	);
	if (levelMainGridCellCount < 8) {
		float cellCountDiffHalf = static_cast<float>(8 - levelMainGridCellCount) / 2.0f;
		mainGridPosition.x += cellCountDiffHalf * (isoEdgeWidth / 8.0f);
		mainGridPosition.y += cellCountDiffHalf * (isoEdgeHeight / 8.0f);
	}
	if (grid == nullptr) {
		problemLoading("Grid");
	}
	else {
		ErrorLog::printf("position: %f/%f, boundingBoxSize: %f/%f\n",
			mainGridPosition.x, mainGridPosition.y, isoBoundingBoxSize.x, isoBoundingBoxSize.y);

		//grid->setup(isoBoundingBoxSize, mainGridPosition, mGroundCells, this);
		grid->setup(isoBoundingBoxSize, mainGridPosition, this, mRessourcenManager);
		//grid->setRotationSkewX(45);
		//grid->setRotationSkewY(45);
		//grid->setAnchorPoint(Vec2(0.5f, 0.5f));

		// SSR 30 Grad 
		// https://medium.com/gravitdesigner/designers-guide-to-isometric-projection-6bfd66934fc7
		// 86,6 % = cos(30 grad)
		//grid->setScaleY(0.866f * grid->getScaleY());
		//grid->setSkewX(30.0f);
		//grid->setRotation(30.0f);

		//grid->setRotationY(45.0f);
		//grid->setRotation3D(Vec3(45.0f, 45.0f, 0.0f));
		//grid->setScale(sqrt(5.0f) / 2.0f, sqrt(5.0f) / 2.0f);

		//grid->setScaleY(grid->getScaleY()*0.5f);
		mGameGrids[GRID_MAIN] = grid;
	}

	// vector from 0/0 to center of left bottom border line of iso grid
	auto v = Vec2(
		mainGridPosition.x + isoEdgeWidth * 0.25f,
		mainGridPosition.y + isoEdgeHeight * 0.25f
	);
	auto vLength = v.getLength();
	auto cellDiagonal = Vec2(flatCellSize, flatCellSize).getLength();
	// add second grid as inventory
	auto inventory_grid = Grid::create(2, 2, GRID_INVENTORY);
	if (inventory_grid == nullptr) {
		problemLoading("inventory Grid");
	}
	else {
		// vector from right edge to right bottom border line of iso grid
		auto v2 = Vec2(
			-v.x,
			v.y
		);
		auto percent = cellDiagonal * 2 / vLength;
		auto position = Vec2(
			//origin.x + visibleSize.width - flatCellSize * 2.0f - xBorder,
			//origin.y + visibleSize.height * 0.15f
			visibleSize.width + v2.x * 0.8f,
			v2.y * 0.8f - flatCellSize * 2.0f
		);
		inventory_grid->setup(flatCellSize * 2.0f, position, this, mRessourcenManager);
		mGameGrids[GRID_INVENTORY] = inventory_grid;

	}

	// add third grid as 
	auto bucket_grid = Grid::create(1, 1, GRID_BUCKET);
	if (bucket_grid == nullptr) {
		problemLoading("bucket grid");
	}
	else {

		auto percent = cellDiagonal / vLength;
		auto factor = (1.0f - percent) / 2.0f;// +percent;
		auto position = Vec2(
			origin.x + v.x * factor,
			origin.y + v.y * factor
		);
		ErrorLog::printf("[MainGameScene::initAfterCreate] inventorygrid position: %f/%f",
			position.x, position.y);
		bucket_grid->setup(flatCellSize, position, this, mRessourcenManager);
		mGameGrids[GRID_BUCKET] = bucket_grid;
	}

	return true;
}

// every frame
void MainGameScene::update(float delta)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	if (visibleSize.width != mResolution.width || visibleSize.height != mResolution.height) {
		// rescale
		auto bg = getChildByName("bg");
		auto size = bg->getContentSize();
		mResolution = visibleSize;
		bg->setScale(size.width / mResolution.width, size.height / mResolution.height);
	}
#ifdef _MSC_VER
	if (mArrowPressed[0]) {
		mGameGrids[GRID_MAIN]->setRotationSkewX(mGameGrids[GRID_MAIN]->getRotationSkewX() + delta * 10.0f);
	}
	else if (mArrowPressed[1]) {
		mGameGrids[GRID_MAIN]->setRotationSkewX(mGameGrids[GRID_MAIN]->getRotationSkewX() - delta * 10.0f);
	}
#endif //_MSC_VER

	if (mActiveLevelState) {
		mActiveLevelState->onUpdate(delta);
	}
}

void MainGameScene::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
}

void MainGameScene::onAcceleration(Acceleration* acc, Event* event)
{

}

bool MainGameScene::touchEndIfInsideGrid(cocos2d::Vec2 pos, GridType type)
{
	if (isInsideGrid(pos, type)) {
		auto grid = mGameGrids[type];
		GridIndex i = grid->getGridIndex(grid->fromWorldToLocal(pos));
		mActiveLevelState->onTouchEnded(type, i.x, i.y);
		return true;
	}
	else {
		mActiveLevelState->onTouchEnded(GRID_ERROR, 0, 0);
	}

	return false;
}

void MainGameScene::updatePoints(float pointDifference, float pointsSum, Vec2 worldPosition)
{
	std::stringstream sstream,sstream2;

	sstream << "Punkte: " << static_cast<int>(pointsSum);
#ifndef DISABLE_UI
	mPointsLabel->setString(sstream.str());
	//sstream.
	sstream2 << static_cast<int>(pointDifference);
	mMovingPointsLabel->stopAllActions();
	mMovingPointsLabel->setPosition(worldPosition);
	mMovingPointsLabel->setString(sstream2.str());
	cocos2d::Color3B color(255, 0, 0);
	if (pointDifference > 150) {
		color = Color3B(0, 0, 255);
	}
	else if(pointDifference >= 100) {
		color = Color3B(0, 255, 0);
	}
	else if (pointDifference < 0) {
		color = Color3B(255, 0, 0);
	}
	mMovingPointsLabel->setColor(color);
	mMovingPointsLabel->setScale(2.0f);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto sequence = cocos2d::Sequence::create(
		cocos2d::EaseOut::create(MoveTo::create(1.3f, Vec2(visibleSize.width / 2.0f, visibleSize.height + mMovingPointsLabel->getContentSize().height)), 2.0f),
		cocos2d::EaseSineInOut::create(ScaleTo::create(1.2f, 1.0f)),
		nullptr);
	mMovingPointsLabel->runAction(sequence);
#endif //DISABLE_UI
}

bool MainGameScene::isInsideGrid(cocos2d::Vec2 pos, GridType type)
{
	return mGameGrids[type]->isInsideGrid(pos);
}

bool MainGameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Vec2 pos = touch->getLocationInView();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	pos.y = visibleSize.height - pos.y;
	mCurrentTouchPosition = pos;

	if (!(mEnabledTouchTypes & ENABLED_TOUCH_BEGIN_PLANT) && !(mEnabledTouchTypes & ENABLED_TOUCH_BEGIN_GRID)) 
		return false;
	if (!mActiveLevelState) return false;
	// manuel bsp, check collision with grids
	// global pos
	
	
	// touch y seems reversed at windows and android, but also in iOs?
//#ifdef _MSC_VER
	
//#endif

	for (int i = 0; i < GRID_SIZE; i++) {
		GridType type = static_cast<GridType>(i);
		auto grid = mGameGrids[type];
		if (isInsideGrid(pos, type)) {
			ErrorLog::printf("[MainGameScene::onTouchBegan] inside grid\n");
			if (mEnabledTouchTypes & ENABLED_TOUCH_BEGIN_PLANT) {
				PlantNode* plantNode = grid->getPlantNodeAtWorldPosition(pos);
				mActiveLevelState->onTouchBegan(plantNode);
			}
			else if (mEnabledTouchTypes & ENABLED_TOUCH_BEGIN_GRID) {
				GridIndex i = grid->getGridIndex(grid->fromWorldToLocal(pos));
				mActiveLevelState->onTouchBegan(type, i.x, i.y);
			}
			break;
		}
	}
	return true;

}

void MainGameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 pos = touch->getLocationInView();
	pos.y = visibleSize.height - pos.y;
	mCurrentTouchPosition = pos;

	if (!(mEnabledTouchTypes & ENABLED_TOUCH_MOVE)) return;
	if (!mActiveLevelState) return;
	pos = touch->getLocationInView();
	Vec2 prevPos = touch->getPreviousLocationInView();
	Vec2 diff = pos - prevPos;
	
	mActiveLevelState->onTouchMoved(diff.x, diff.y);
}
void MainGameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 pos = touch->getLocationInView();
	pos.y = visibleSize.height - pos.y;
	mCurrentTouchPosition = pos;

	if (!(mEnabledTouchTypes & ENABLED_TOUCH_END)) return;
	if (!mActiveLevelState) return;
	
	
	// touch y seems reversed at windows and android, but also in iOs?
//#ifdef _MSC_VER
	
//#endif

	bool found = false;
	for (int i = 0; i < GRID_SIZE; i++) {
		GridType type = static_cast<GridType>(i);
		if (isInsideGrid(pos, type)) {
			auto grid = mGameGrids[type];
			GridIndex i = grid->getGridIndex(grid->fromWorldToLocal(pos));
			mActiveLevelState->onTouchEnded(type, i.x, i.y);
			found = true;
			break;
		}
	}
	if (!found) {
	//	mActiveLevelState->onTouchEnded(GRID_ERROR, 0, 0);
		mActiveLevelState->onTouchCancelled();
	}
	
}

void MainGameScene::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (!(mEnabledTouchTypes & ENABLED_TOUCH_CANCELLED)) return;
	if (!mActiveLevelState) return;

	mActiveLevelState->onTouchCancelled();
}

#ifdef _MSC_VER
void MainGameScene::onMouseMove(Event *event)
{
	// to illustrate the event....
	EventMouse* e = (EventMouse*)event;
	std::ostringstream os;
	os << "MousePosition X:" << e->getCursorX() << " Y:" << e->getCursorY();
	mMousePosLabel->setString(os.str());
}

void MainGameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW: mArrowPressed[0] = true; break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: mArrowPressed[1] = true; break;
	}
}
void MainGameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW: mArrowPressed[0] = false; break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: mArrowPressed[1] = false; break;
	}
}
#endif

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

void MainGameScene::menuResetCallback(cocos2d::Ref* pSender)
{
	setEnabledTouchType(ENABLED_TOUCH_NONE);
	mActiveLevelState->onCancelState();
	mActiveLevelState->onExitState();
	mActiveLevelState = NULL;

	// reset grids
	for (int i = 0; i < GRID_SIZE; i++) {
		mGameGrids[i]->removeAllGridCells();
	}

	// reset points
	mPoints->reset();
	mPointsLabel->setString("Punkte: 0");

	transitTo("RandomSeed");
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
	auto oldActiveLevelState = mActiveLevelState;
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
	if (!mActiveLevelState->onEnterState(oldActiveLevelState)) {
		ErrorLog::printf("error enter state: %s", mActiveLevelState->getName());
		LOG_ERROR("error enter state", false);
	}
#ifdef _FAIRY_DEBUG_
#ifndef DISABLE_UI
	mCurrentGameStateLabel->setString(mActiveLevelState->getName());
#endif //DISABLE_UI
#endif

	return true;
}

