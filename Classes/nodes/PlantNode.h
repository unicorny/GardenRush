#ifndef __FAIRY_GAMES_GARDEN_RUSH_VIEW_SPRITE_VIEW_H
#define __FAIRY_GAMES_GARDEN_RUSH_VIEW_SPRITE_VIEW_H

/*!
 *	\author: Dario Rekowski
 *
 *	\date: 28.08.18
 *
 *  \brief: Class for store sprite view, if it a single texture, animation or sprites in 
 *          a texture-atlas
*/
#include "cocos2d.h"
#include "lib/DRHashlist.hpp"
#include "nodes/Grid.h"
#include "enums.h"

struct GridIndex;
class PlantType;

class PlantNode : public cocos2d::Sprite
{
public:
	PlantNode(PlantType* plantType);
	virtual ~PlantNode();

	inline void setGridIndex(const GridIndex& index) { mGridIndex = index; }
	inline GridIndex getGridIndex() const { return mGridIndex; }

	inline void setParentGrid(Grid* grid) { mParentGrid = grid; }
	inline Grid* getParentGrid() const { return mParentGrid; }

	//inline DHASH getHash() const { return mPlantType->getNameHash(); }
	inline const PlantType* getPlantType() const { return mPlantType; }

	bool removeFromGrid();

	inline float getPointsMultiplicator() const { return mPointsMultiplicator; }
	inline void multPoints(float points) { mPointsMultiplicator *= points; }

	inline void setGrowPhasis(PlantTypePhasisView growPhasis) { mGrowPhasis = growPhasis; }

	// return true if growth above max
	bool countNewNeighbor(DHASH neigborHash, PlantTypeNeighborType neighborType, bool edge = true);
protected:
	PlantType* mPlantType;
	GridIndex mGridIndex;
	Grid*	  mParentGrid;
	float	  mPointsMultiplicator;
	PlantTypePhasisView		  mGrowPhasis;
	DRHashList mCountedNeighborForDiversity;
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_VIEW_SPRITE_VIEW_H
