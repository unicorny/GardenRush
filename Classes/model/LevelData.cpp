#include "model/LevelData.h"


LevelData::LevelData(size_t plantTypeSize)
	: mPlantTypes(nullptr), mPlantTypesSize(plantTypeSize), mPlantTypeCursor(0)
{
	mPlantTypes = new DHASH[plantTypeSize];
}

LevelData::~LevelData()
{
	DR_SAVE_DELETE_ARRAY(mPlantTypes);
	mPlantTypesSize = 0;
}

bool LevelData::addPlantType(const char* name)
{
	assert(mPlantTypeCursor < mPlantTypesSize);
	DHASH id = DRMakeStringHash(name);
	mPlantTypes[mPlantTypeCursor++] = id;

	return true;
}