#include "model\PlantType.h"
#include "ErrorLog.h"

PlantType::PlantType(const char* name, u32 neighborCount/* = 0*/)
	: mName(name), mNameHash(DRMakeStringHash(name)), mIndex(0), mViewData(NULL),
	 mSeedView(NULL), mSeededView(NULL)
{
	if (neighborCount > 0) {
		mNeighbors.initMemory(neighborCount);
	}
	ErrorLog::printf("growth phasis: %d, final: %d", sizeof(mGrowthPhasisViews), sizeof(mFinalPhasisViews));
	memset(mGrowthPhasisViews, 0, sizeof(mGrowthPhasisViews));
	memset(mFinalPhasisViews, 0, sizeof(mFinalPhasisViews));
}

PlantType::~PlantType()
{
	if (mViewData) {
		delete mViewData;
		mViewData = NULL;
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
