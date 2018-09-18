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
#include "Enums.h"

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
	bool setViewData(IViewData* viewData, PlantTypePhasisView phasis);
	inline const IViewData* getViewData(PlantTypePhasisView viewType) const { assert(viewType < PLANT_PHASIS_SIZE);  return mPlantViews[viewType]; }

	// called from PlantTypesManager
	inline void setIndex(u32 index) { mIndex = index; }

	inline u32 getIndex() const { return mIndex; }

	static PlantTypeNeighborType getNeighborTypeFromString(const char* name);
	static PlantTypePhasisView getPhasisViewFromString(const char* name);
	static PlantTypePhasisView getGrowPhasisViewFromInt(int growPhasis);

protected:
	std::string mName;
	DHASH mNameHash;
	u32 mIndex;
	// seed, after seeded, 3-5 growth phase, 4 different results (neutral, good, very good, bad)
	IViewData* mPlantViews[PLANT_PHASIS_SIZE];

	DRHashList mNeighbors;
	
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_MODEL_PLANT_TYPE_H