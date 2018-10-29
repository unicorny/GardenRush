#ifndef __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_STATE_MANAGER_H
#define __FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_STATE_MANAGER_H

/*!
 *
 * \author: Dario Rekowski
 *
 * \date: 10.09.18
 * 
 * \brief: Managed different level states
*/

#include "lib/DRHashList.hpp"
#include "MainGameScene.h"

class MainGameScene;
class PlantNode;

namespace level_state
{
	class iLevelState
	{
	public:
		iLevelState(MainGameScene* manager) : mMainGameScene(manager) {}
		iLevelState() : mMainGameScene(nullptr) {}

		inline void setMainGameScene(MainGameScene* manager) { mMainGameScene = manager; }
		virtual ~iLevelState() {};
		virtual const char* getName() const { return "iLevelState"; }
		virtual bool initState() = 0;
		virtual bool onEnterState() = 0;
		virtual bool onExitState() = 0;
		virtual void onCancelState() = 0;

		virtual void onTouchBegan(PlantNode* plantNode) { };
		virtual void onTouchBegan(GridType type, uint8_t x, uint8_t y) {};
		virtual void onTouchMoved(float deltaX, float deltaY) {};
		virtual void onTouchEnded(GridType type, uint8_t x, uint8_t y) { };
		virtual void onTouchCancelled() { };

		virtual void onUpdate(float delta) {};

	protected:
		MainGameScene* mMainGameScene;
	};
};

/*
class LevelStateManager
{
public:
	LevelStateManager(LevelData* levelData, MainGameScene* mainGameScene, PlantTypesManager* plantTypesManager);
	~LevelStateManager();
	// free memory by exit
	bool addLevelState(level_state::iLevelState* levelState);

	bool transitTo(DHASH levelStateId);
	inline bool transitTo(const char* levelStateName) { return transitTo(DRMakeStringHash(levelStateName)); }

	inline LevelData* getLevelData() { return mLevelData; }
	inline MainGameScene* getMainGameScene() { return mGameScene; }
	
	inline PlantTypesManager* getPlantTypesManager() { return mPlantTypesManager; }
	inline void setPlantTypeManager(PlantTypesManager* manager) { mPlantTypesManager = manager; }

	inline void setPlantNode(PlantNode* n) { mTargetPlantNode = n; }
	inline PlantNode* getPlantNode() { return mTargetPlantNode; }

protected:
	DRHashList mLevelStates;
	level_state::iLevelState* mActiveLevelState;

	// object to work on
	LevelData* mLevelData;
	MainGameScene* mGameScene;
	PlantTypesManager* mPlantTypesManager;
	
	
};
*/


#endif //__FAIRY_GAMES_GARDEN_RUSH_LEVEL_STATES_LEVEL_STATE_MANAGER_H