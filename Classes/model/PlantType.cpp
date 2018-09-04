#include "model\PlantType.h"

PlantType::PlantType(const char* name, u32 neighborCount/* = 0*/)
	: mName(name), mNameHash(DRMakeStringHash(name)), mIndex(0), mViewData(NULL)
{
	if (neighborCount > 0) {
		mNeighbors.initMemory(neighborCount);
	}
}

PlantType::~PlantType()
{
	if (mViewData) {
		delete mViewData;
		mViewData = NULL;
	}

}
