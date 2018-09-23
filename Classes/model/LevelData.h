#ifndef __FAIRY_GAMES_GARDEN_RUSH_MODEL_LEVEL_DATA_H
#define __FAIRY_GAMES_GARDEN_RUSH_MODEL_LEVEL_DATA_H

#include "lib/DRHashList.hpp"
#include "cocos2d.h"

class LevelData
{
public:
	LevelData(size_t plantTypeSize);
	~LevelData();

	bool addPlantType(const char* name);
	inline size_t getPlantTypeCount() const { return mPlantTypesSize; }
	inline DHASH getPlantType(size_t index) const { assert(index < mPlantTypesSize && index < mPlantTypeCursor); return mPlantTypes[index]; }
	
	inline void enableAutoHarvesting() { mAutoHarvesting = true; }
	inline bool isAutoHarvesting() const { return mAutoHarvesting; }

	inline void setMaxGrowthPhasis(int maxGrowthPhasis) { mMaxGrowthPhasis = maxGrowthPhasis; }
	inline int getMaxGrowthPhasis() const { return mMaxGrowthPhasis; }

protected:
	// valid plant types for level
	DHASH* mPlantTypes;
	size_t mPlantTypesSize;
	size_t mPlantTypeCursor;

	bool   mAutoHarvesting;
	int    mMaxGrowthPhasis;

};


#endif //__FAIRY_GAMES_GARDEN_RUSH_MODEL_LEVEL_DATA_H