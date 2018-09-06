#ifndef __FAIRY_GAMES_GARDEN_RUSH_MODEL_PLANT_TYPE_H
#define __FAIRY_GAMES_GARDEN_RUSH_MODEL_PLANT_TYPE_H

/*!
 * \author: Dario Rekowski
 *
 * \date: 29.08.18
 *
 * \brief: containing basic infos for planttype 
 *
 */

#include "cocos2d.h"
#include "../lib/DRHashList.hpp"
#include "ViewData.h"

enum PlantTypeNeighborType
{
	// neutral must be zero
	NEIGHBOR_NEUTRAL = 0,
	NEIGHBOR_GOOD = 1,
	NEIGHBOR_REALLY_GOOD = 2,
	NEIGHBOR_BAD = 4,
	NEIGHBOR_ERROR = 7
};

enum PlantTypeFinalPhasis
{
	PLANT_FINAL_NEUTRAL = 0,
	PLANT_FINAL_GOOD = 1,
	PLANT_FINAL_VERY_GOOD = 2,
	PLANT_FINAL_BAD = 3,
	PLANT_FINAL_SIZE = 4
};
class PlantNode;

class PlantType
{
public:
	PlantType(const char* name, u32 neighborCount = 0);
	~PlantType();

	// return false if nameHash already exist
	inline bool addNeighbor(const char* name, PlantTypeNeighborType type) {
		return addNeighbor(DRMakeStringHash(name), type);
	}
	// return false if nameHash already exist
	inline bool addNeighbor(DHASH nameHash, PlantTypeNeighborType type) {
		return mNeighbors.addByHash(nameHash, (void*)type);
	}

	inline DHASH getNameHash() const { return mNameHash; }
	inline PlantTypeNeighborType getNeighborType(DHASH neighborHash) const {
		return (PlantTypeNeighborType)((intptr_t)(mNeighbors.findByHash(neighborHash)));
	}
	// set view data, PlantType delete pointer after it won't use it anymore
	inline void setViewData(IViewData* viewData) { if (mViewData) delete mViewData; mViewData = viewData; }
	inline PlantNode* createPlantNode() { if (!mViewData) return NULL; return mViewData->createPlantNode(); }

	// called from PlantTypesManager
	inline void setIndex(u32 index) { mIndex = index; }

	inline u32 getIndex() const { return mIndex; }

	static PlantTypeNeighborType getNeighborTypeFromString(const char* name);

protected:
	std::string mName;
	DHASH mNameHash;
	u32 mIndex;
	IViewData* mViewData;
	// seed, after seeded, 3-5 growth phase, 4 different results (neutral, good, very good, bad)
	IViewData* mSeedView;
	IViewData* mSeededView;
	IViewData* mGrowthPhasisViews[5];
	IViewData* mFinalPhasisViews[PLANT_FINAL_SIZE];

	DRHashList mNeighbors;
	
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_MODEL_PLANT_TYPE_H