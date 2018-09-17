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
#include "lib/DRHash.hpp"
#include "nodes/Grid.h"

struct GridIndex;

class PlantNode : public cocos2d::Sprite
{
public:
	PlantNode(DHASH plantHash);
	virtual ~PlantNode();

	inline void setGridIndex(const GridIndex& index) { mGridIndex = index; }
	inline GridIndex getGridIndex() const { return mGridIndex; }

	inline void setParentGrid(Grid* grid) { mParentGrid = grid; }
	inline Grid* getParentGrid() const { return mParentGrid; }

	inline DHASH getHash() const { return mPlantHash; }

	bool removeFromGrid();
protected:
	DHASH mPlantHash;
	GridIndex mGridIndex;
	Grid*	  mParentGrid;
	
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_VIEW_SPRITE_VIEW_H
