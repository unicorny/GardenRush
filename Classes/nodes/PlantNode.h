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
#include "CustomAnimationSprite.h"
#include "lib/DRHashList.hpp"
#include "nodes/Grid.h"
#include "Enums.h"

struct GridIndex;
class PlantType;



class PlantNode : public CustomAnimationSprite
{
public:
	PlantNode(const PlantType* plantType);
	virtual ~PlantNode();

	inline void setGridIndex(const GridIndex& index) { mGridIndex = index; }
	inline GridIndex getGridIndex() const { return mGridIndex; }

	inline void setParentGrid(Grid* grid) { mParentGrid = grid; }
	inline Grid* getParentGrid() const { return mParentGrid; }


	//inline DHASH getHash() const { return mPlantType->getNameHash(); }
	inline const PlantType* getPlantType() const { return mPlantType; }

	bool removeFromGrid(bool removeFromGame = false);

	inline float getPointsMultiplicator() const { return mPointsMultiplicator; }
	inline float getDiversityBonus() const { return mDiversityBonus; }
	inline void multPoints(float points) { mPointsMultiplicator *= points; }

	inline void setGrowPhasis(PlantTypePhasisView growPhasis) { mGrowPhasis = growPhasis; }

	// return growth phasis
	int countNewNeighbor(DHASH neigborHash, PlantTypeNeighborType neighborType, bool edge = true);

	

protected:
	const PlantType* mPlantType;
	GridIndex mGridIndex;
	Grid*	  mParentGrid;
	float	  mPointsMultiplicator;
	float	  mDiversityBonus;
	PlantTypePhasisView		  mGrowPhasis;
	bool	  mHalfGrow;
	DRHashList mCountedNeighborForDiversity;

	
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_VIEW_SPRITE_VIEW_H
