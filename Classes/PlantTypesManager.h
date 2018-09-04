#ifndef __FAIRY_GAMES_GARDEN_RUSH_PLANT_TYPES_MANAGER_H
#define __FAIRY_GAMES_GARDEN_RUSH_PLANT_TYPES_MANAGER_H

#include "lib/DRHashList.hpp"
#include "model/PlantType.h"

class PlantTypesManager
{
public:
	PlantTypesManager();
	~PlantTypesManager();

	// adding and getting plant types
	void allocateMemoryForNPlantTypes(u32 N);
	bool addPlantType(PlantType* plantType);
	PlantType* findPlantType(DHASH nameHash) const;

	// make fast access plant type neigbor array
	bool makeFastAccessMap();
	inline PlantTypeNeighborType getNeighborType(u32 mainPlantIndex, u32 neighborPlantIndex) const {
		if (mDirty) return NEIGHBOR_ERROR;
		assert(mNeigborFastAccessMap != NULL);
		return (PlantTypeNeighborType)mNeigborFastAccessMap[mPlantTypes.getNItems()*mainPlantIndex + neighborPlantIndex];
	};

protected:
	DRHashList mPlantTypes;
	u8* mNeigborFastAccessMap;
	bool mDirty;

};

#endif // __FAIRY_GAMES_GARDEN_RUSH_PLANT_TYPES_MANAGER_H