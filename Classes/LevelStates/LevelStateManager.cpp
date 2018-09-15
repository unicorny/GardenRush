#include "LevelStateManager.h"
#include "ErrorLog.h"

LevelStateManager::LevelStateManager(LevelData* levelData, MainGameScene* mainGameScene, PlantTypesManager* plantTypesManager)
	: mActiveLevelState(nullptr), mLevelData(levelData), mGameScene(mainGameScene), mPlantTypesManager(plantTypesManager), mTargetPlantNode(nullptr)
{

}

LevelStateManager::~LevelStateManager()
{
	for (u32 i = 0; i < mLevelStates.getNItems(); i++) {
		auto* l = (level_state::iLevelState*)mLevelStates.findByIndex(i);
		DR_SAVE_DELETE(l);
	}
	mLevelStates.clear(true);
}

bool LevelStateManager::addLevelState(level_state::iLevelState* levelState)
{
	if (!levelState) LOG_ERROR("level state is zero", false);
	DHASH id = DRMakeStringHash(levelState->getName());
	if (!mLevelStates.addByHash(id, levelState)) {
		ErrorLog::printf("level state with name: %s exist", levelState->getName());
		LOG_ERROR("level state with this name already exist", false);
	}
	levelState->setLevelStateManager(this);
	return true;
}

bool LevelStateManager::transitTo(DHASH levelStateId)
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