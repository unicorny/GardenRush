#include "PlantTypesAccess.h"


PlantTypesAccess::PlantTypesAccess()
	: mNeigborFastAccessMap(NULL), mDirty(false)
{

}
PlantTypesAccess::~PlantTypesAccess()
{
	if (mNeigborFastAccessMap) {
		free(mNeigborFastAccessMap);
		mNeigborFastAccessMap = NULL;
	}
}

void PlantTypesAccess::allocateMemoryForNPlantTypes(u32 N)
{
	mPlantTypes.initMemory(N);
}

bool PlantTypesAccess::addPlantType(PlantType* plantType)
{
	if (!plantType) return false;
	
	bool result = mPlantTypes.addByHash(plantType->getNameHash(), plantType);
	if(result) mDirty = true;
	return result;
}

PlantType* PlantTypesAccess::findPlantType(DHASH nameHash) const
{
	return (PlantType*)mPlantTypes.findByHash(nameHash);
}

bool PlantTypesAccess::makeFastAccessMap()
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


double PlantTypesAccess::getMemoryConsumption()
{
	unsigned long memory = sizeof(PlantTypesAccess);
	memory += mPlantTypes.getNItems() * (sizeof(DRHashListItem) + sizeof(PlantType));

	// neighbor fast access map
	memory += mPlantTypes.getNItems() * mPlantTypes.getNItems();

	return memory;
}