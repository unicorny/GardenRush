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

#ifndef  __FAIRY_GAMES_GARDEN_RUSH_MAIN_GAME_SCENE_H
#define  __FAIRY_GAMES_GARDEN_RUSH_MAIN_GAME_SCENE_H

#include "cocos2d.h"
#include "lib/DRHashList.hpp"

class PlantTypesManager;
class LevelStateManager;
class LevelData;
class PlantNode;
class Grid;

namespace level_state {
	class iLevelState;
}

enum MainGridType {
	GRID_MAIN,
	GRID_INVENTORY,
	GRID_BUCKET,
	GRID_SIZE,
	GRID_ERROR
};

enum EnabledTouchType {
	ENABLED_TOUCH_NONE = 0,
	ENABLED_TOUCH_BEGIN = 1,
	ENABLED_TOUCH_END = 2,
	ENABLED_TOUCH_MOVE = 4,
	ENABLED_TOUCH_CANCELLED = 8
};

inline EnabledTouchType operator|(EnabledTouchType a, EnabledTouchType b)
{
	return static_cast<EnabledTouchType>(static_cast<int>(a) | static_cast<int>(b));
}


class MainGameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene(PlantTypesManager* plantTypesManager);
	MainGameScene();
	~MainGameScene();

    virtual bool init();
	bool initAfterCreate();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void menuToggleStatsCallback(cocos2d::Ref* pSender);

	// global events
	bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

#ifdef _MSC_VER
	void onMouseMove(cocos2d::Event *event);
#endif

	// enable/disable touch

	// getter for level state
	inline Grid* getGrid(const MainGridType type) const { assert(type < GRID_SIZE); return mGameGrids[type]; }
	inline const LevelData* getLevelData() const { return mLevelData; }
	inline const PlantTypesManager* getPlantTypesManager() const { return mPlantTypesManager; }

	inline void setEnabledTouchType(EnabledTouchType type) { mEnabledTouchTypes = type; }
	inline void setTargetPlantNode(PlantNode* target) { mTargetPlantNode = target; }
	
	bool transitTo(DHASH levelStateId);
	inline bool transitTo(const char* levelStateName) { return transitTo(DRMakeStringHash(levelStateName)); }

    // implement the "static create()" method manually
    CREATE_FUNC(MainGameScene);

protected:

	bool touchBeganIfInsideGrid(cocos2d::Vec2 pos, MainGridType type); 
	bool touchEndIfInsideGrid(cocos2d::Vec2 pos, MainGridType type);
	bool isInsideGrid(cocos2d::Vec2 pos, MainGridType type);
	bool addLevelState(level_state::iLevelState* levelState);

	bool mToogleStats;
	PlantTypesManager* mPlantTypesManager;
	LevelData* mLevelData;
	LevelStateManager* mLevelStateManager;
	Grid* mGameGrids[GRID_SIZE];
	// bounding box array for better cache use by touch point check
	// position x left, y bottom, edge size 
	uint16_t mGridBoundingBoxes[GRID_SIZE*3];

#ifdef _MSC_VER
	cocos2d::Label* mMousePosLabel;
#endif

	// level state
	DRHashList mLevelStates;
	level_state::iLevelState* mActiveLevelState;
	PlantNode*	mTargetPlantNode;
	EnabledTouchType mEnabledTouchTypes;
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_MAIN_GAME_SCENE_H
