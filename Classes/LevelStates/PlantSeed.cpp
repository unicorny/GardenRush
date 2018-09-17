#include "PlantSeed.h"

#include "PlantTypesManager.h"
#include "nodes/PlantNode.h"

namespace level_state {

	bool PlantSeed::initState()
	{
		return true;
	}
	bool PlantSeed::onEnterState()
	{
		auto plantNode = mMainGameScene->getTargetPlantNode();
		auto plantTypesManager = mMainGameScene->getPlantTypesManager();
		auto plantType = plantTypesManager->findPlantType(plantNode->getHash());
		auto gridIndex = plantNode->getGridIndex();
		auto grid = plantNode->getParentGrid();

		// change sprite
		plantType->getViewData(PLANT_PHASIS_SEEDED)->changePlantNodeSprite(plantNode);
		return true;
	}
	bool PlantSeed::onExitState()
	{
		return true;
	}
}