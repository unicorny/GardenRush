#include "PlantTypesManager.h"

PlantTypesManager::PlantTypesManager()
	: mNeigborFastAccessMap(NULL), mDirty(false)
{

}
PlantTypesManager::~PlantTypesManager()
{
	if (mNeigborFastAccessMap) {
		free(mNeigborFastAccessMap);
		mNeigborFastAccessMap = NULL;
	}
}

void PlantTypesManager::allocateMemoryForNPlantTypes(u32 N)
{
	mPlantTypes.initMemory(N);
}

bool PlantTypesManager::addPlantType(PlantType* plantType)
{
	if (!plantType) return false;
	
	bool result = mPlantTypes.addByHash(plantType->getNameHash(), plantType);
	if(result) mDirty = true;
	return result;
}

PlantType* PlantTypesManager::findPlantType(DHASH nameHash) const
{
	return (PlantType*)mPlantTypes.findByHash(nameHash);
}

bool PlantTypesManager::makeFastAccessMap()
{
	if (mNeigborFastAccessMap) {
		free(mNeigborFastAccessMap);
	}
	u32 itemCount = mPlantTypes.getNItems();
	assert(itemCount > 0);
	mNeigborFastAccessMap = (u8*)malloc(itemCount*itemCount);
	memset(mNeigborFastAccessMap, 0, itemCount*itemCount);
	for (u32 i = 0; i < itemCount; i++) {
		PlantType* plantType = (PlantType*)mPlantTypes.findByIndex(i);
		assert(plantType != NULL);
		plantType->setIndex(i);
		for (u32 j = 0; j < itemCount; j++) {
			if (i == j) continue;
			DHASH neighborPlantHash = mPlantTypes.findHashByIndex(j);
			mNeigborFastAccessMap[i*itemCount + j] = (u8)plantType->getNeighborType(neighborPlantHash);
		}
	}
	mDirty = false;
	return true;
}