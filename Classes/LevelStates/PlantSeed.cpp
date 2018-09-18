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
		auto plantType = plantNode->getPlantType();
		auto gridIndex = plantNode->getGridIndex();
		auto grid = plantNode->getParentGrid();

		// change sprite
		plantType->getViewData(PLANT_PHASIS_SEEDED)->changePlantNodeSprite(plantNode);
		plantNode->setGrowPhasis(PLANT_PHASIS_SEEDED);
		grid->updateParentsOfPlantOnIndex(gridIndex, plantTypesManager);

		if (mMainGameScene->getGrid(GRID_BUCKET)->isCellEmptyAndFree(0, 0)) {
			mMainGameScene->transitTo("RandomSeed");
		}
		else {
			mMainGameScene->transitTo("PlayerChooseSeed");
		}
		return true;
	}
	bool PlantSeed::onExitState()
	{
		return true;
	}
}