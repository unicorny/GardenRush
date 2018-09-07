#include "model\PlantType.h"
#include "ErrorLog.h"

PlantType::PlantType(const char* name, u32 neighborCount/* = 0*/)
	: mName(name), mNameHash(DRMakeStringHash(name)), mIndex(0)
{
	if (neighborCount > 0) {
		mNeighbors.initMemory(neighborCount);
	}
	memset(mPlantViews, 0, sizeof(mPlantViews));
}

PlantType::~PlantType()
{
	for (int i = 0; i < PLANT_PHASIS_SIZE; i++) {
		delete mPlantViews[i];
		mPlantViews[i] = NULL;
	}
}

PlantTypeNeighborType PlantType::getNeighborTypeFromString(const char* name)
{
	if (strcmp(name, "good") == 0) {
		return NEIGHBOR_GOOD;
	}
	else if (strcmp(name, "bad") == 0) {
		return NEIGHBOR_BAD;
	}
	else if (strcmp(name, "very good") == 0) {
		return NEIGHBOR_REALLY_GOOD;
	}
	return NEIGHBOR_NEUTRAL;

}

PlantTypePhasisView PlantType::getPhasisViewFromString(const char* name)
{
	if (strcmp(name, "seed") == 0) return PLANT_PHASIS_SEED;
	else if (strcmp(name, "seeded") == 0) return PLANT_PHASIS_SEEDED;
	else if (strcmp(name, "growth_1") == 0) return PLANT_PHASIS_GROWTH_1;
	else if (strcmp(name, "growth_2") == 0) return PLANT_PHASIS_GROWTH_2;
	else if (strcmp(name, "growth_3") == 0) return PLANT_PHASIS_GROWTH_3;
	else if (strcmp(name, "growth_4") == 0) return PLANT_PHASIS_GROWTH_4;
	else if (strcmp(name, "growth_5") == 0) return PLANT_PHASIS_GROWTH_5;
	else if (strcmp(name, "final_neutral") == 0) return PLANT_PHASIS_FINAL_NEUTRAL;
	else if (strcmp(name, "final_good") == 0) return PLANT_PHASIS_FINAL_GOOD;
	else if (strcmp(name, "final_very_good") == 0) return PLANT_PHASIS_FINAL_VERY_GOOD;
	else if (strcmp(name, "final_bad") == 0) return PLANT_PHASIS_FINAL_BAD;
	
	return PLANT_PHASIS_ERROR;
}



bool PlantType::setViewData(IViewData* viewData, PlantTypePhasisView phasis)
{
	if (phasis >= PLANT_PHASIS_SIZE) {
		return false;
	}
	if (mPlantViews[phasis]) {
		delete mPlantViews[phasis];
	}
	mPlantViews[phasis] = viewData;
	return true;
}