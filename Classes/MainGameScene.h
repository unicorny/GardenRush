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

// Infos for Android Build from command line:
// https://android.jlelse.eu/how-i-reduced-my-android-build-times-by-89-4242e51ce946
// gradlew assembleDebug
// gradlew installDebug


#ifndef  __FAIRY_GAMES_GARDEN_RUSH_MAIN_GAME_SCENE_H
#define  __FAIRY_GAMES_GARDEN_RUSH_MAIN_GAME_SCENE_H

#define _FAIRY_DEBUG_

//#include "cocos2d.h"
//#include "lib/DRHashList.hpp"
#include "scenes/SpriteBatchNodesHolderScene.h"
#include "nodes/Grid.h"

#include "model/Points.h"
#include "model/SpriteAnimationState.h"

class PlantTypesManager;
class RessourcenManager;
class LevelStateManager;
class LevelData;
class PlantNode;
class Points;
//class Grid;


namespace level_state {
	class iLevelState;
}


enum EnabledTouchType {
	ENABLED_TOUCH_NONE = 0,
	ENABLED_TOUCH_BEGIN_PLANT = 1,
	ENABLED_TOUCH_BEGIN_GRID = 2,
	ENABLED_TOUCH_END = 4,
	ENABLED_TOUCH_MOVE = 8,
	ENABLED_TOUCH_CANCELLED = 16
};

inline EnabledTouchType operator|(EnabledTouchType a, EnabledTouchType b)
{
	return static_cast<EnabledTouchType>(static_cast<int>(a) | static_cast<int>(b));
}


class MainGameScene : public SpriteBatchNodesHolderScene, public iPointsUpdate
{
public:
	static cocos2d::Scene* createScene(PlantTypesManager* plantTypesManager, Points* points, TemplateMemoryManager<SpriteAnimationState>* animationStateMemorymanager, RessourcenManager* ressourcenManager);
	MainGameScene();
	~MainGameScene();

	virtual bool init();
	bool initAfterCreate();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	void menuToggleStatsCallback(cocos2d::Ref* pSender);
	void menuResetCallback(cocos2d::Ref* pSender);

	// global events
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

	virtual void onAcceleration(cocos2d::Acceleration* acc, cocos2d::Event* event);

	// user defined events
	void updatePoints(float pointDifference, float pointsSum, cocos2d::Vec2 worldPosition);

#ifdef _MSC_VER
	virtual void onMouseMove(cocos2d::Event *event);
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
#endif

	virtual void update(float delta);

	// enable/disable touch

	// getter for level state
	inline Grid* getGrid(const GridType type) const { assert(type < GRID_SIZE); return mGameGrids[type]; }
	inline const LevelData* getLevelData() const { return mLevelData; }
	inline const PlantTypesManager* getPlantTypesManager() const { return mPlantTypesManager; }
	inline const RessourcenManager* getRessourcenManager() const { return mRessourcenManager; }
	inline TemplateMemoryManager<SpriteAnimationState>* getAnimationStateMemoryManager() { return mAnimationStateMemoryManager; }
	inline Points* getPoints() { return mPoints; }

	inline void setEnabledTouchType(EnabledTouchType type) { mEnabledTouchTypes = type; }
	inline void setTargetPlantNode(PlantNode* target) { mTargetPlantNode = target; }
	inline PlantNode* getTargetPlantNode() { return mTargetPlantNode; }
	inline cocos2d::Vec2 getCurrentTouchPosition() { return mCurrentTouchPosition; }
	
	bool transitTo(DHASH levelStateId);
	inline bool transitTo(const char* levelStateName) { return transitTo(DRMakeStringHash(levelStateName)); }

	

    // implement the "static create()" method manually
    CREATE_FUNC(MainGameScene);

protected:

	bool touchBeganIfInsideGrid(cocos2d::Vec2 pos, GridType type);
	bool touchEndIfInsideGrid(cocos2d::Vec2 pos, GridType type);
	
	bool isInsideGrid(cocos2d::Vec2 pos, GridType type);
	bool addLevelState(level_state::iLevelState* levelState);

	bool mToogleStats;
	PlantTypesManager* mPlantTypesManager;
	RessourcenManager*	   mRessourcenManager;
	TemplateMemoryManager<SpriteAnimationState>* mAnimationStateMemoryManager;

	Points*			   mPoints;
	LevelData* mLevelData;
	Grid* mGameGrids[GRID_SIZE];
	cocos2d::Vec2 mCurrentTouchPosition;

#ifdef _MSC_VER
	cocos2d::Label* mMousePosLabel;
	// left, right, up, down
	bool			mArrowPressed[4];
#endif
#ifdef _FAIRY_DEBUG_
	cocos2d::Label* mCurrentGameStateLabel;
#endif 
	cocos2d::Label* mPointsLabel;
	cocos2d::Label* mMovingPointsLabel;

	// level state
	DRHashList mLevelStates;
	level_state::iLevelState* mActiveLevelState;
	PlantNode*	mTargetPlantNode;
	EnabledTouchType mEnabledTouchTypes;

	// window size state
	cocos2d::Size mResolution;

	
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_MAIN_GAME_SCENE_H
