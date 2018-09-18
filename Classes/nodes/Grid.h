#ifndef __FAIRY_GAMES_GARDEN_RUSH_GRID_H
#define __FAIRY_GAMES_GARDEN_RUSH_GRID_H

/*!
 * \author: Dario Rekowski
 * 
 * \date: 29.08.18
 * 
 * \brief: A class holding the plants inside the grid
*/
#include "cocos2d.h"
//#include "PlantNode.h"
class IViewData;
class PlantNode;
class PlantTypesManager;

enum GridObstacleType
{
	OBSTACLE_NONE = 0,
	OBSTACLE_DEFAULT = 1
};

enum GridType {
	GRID_MAIN,
	GRID_INVENTORY,
	GRID_BUCKET,
	GRID_SIZE,
	GRID_ERROR
};

struct GridIndex {
	GridIndex(uint8_t _x, uint8_t _y) : x(_x), y(_y) {}
	GridIndex() : x(0), y(0) {}
	uint8_t x;
	uint8_t y;
};

class Grid : public cocos2d::Node
{
public:
	// init
	static Grid * create(uint8_t width, uint8_t height, GridType type);
	bool setup(float edge_size_pixels, cocos2d::Vec2 pos, IViewData* bgTile, cocos2d::Node* parentNode);
	void setPixelSize(cocos2d::Vec2 pixelSize);
	bool fillBgGridCells(const IViewData* viewData);

	// actions
	bool addBgGridCell(const IViewData* viewData, bool obstacle, uint8_t x, uint8_t y);
	//! \return false if an obstacle is at this position
	bool addGridCell(PlantNode* viewNode, uint8_t x, uint8_t y);
	bool updateParentsOfPlantOnIndex(GridIndex index, const PlantTypesManager* plantTypesManager);

	PlantNode* removeGridCell(uint8_t x, uint8_t y);
	cocos2d::Vec2 getOriginPosition(PlantNode* viewNode);

	
	void reset();

	// infos
	PlantNode* isPlantNodeAtPosition(cocos2d::Vec2 localPosition) const;
	bool isCellEmptyAndFree(uint8_t x, uint8_t y) const;
	GridIndex getGridIndex(cocos2d::Vec2 localPosition) const;
	cocos2d::Vec2 fromWorldToLocal(cocos2d::Vec2 worldCoords) const;
	cocos2d::Vec2 fromLocalToWorld(cocos2d::Vec2 localCoords) const;
	inline GridType getType() const { return mType; }
	inline float getEdgeSize() const {return mPixelSize.x;}
	
CC_CONSTRUCTOR_ACCESS:
	// Nodes should be created using create();
	Grid(uint8_t width, uint8_t height, GridType type);
	virtual ~Grid();

	virtual bool init();

protected:

	bool addCellSprite(cocos2d::Sprite* sprite, uint8_t x, uint8_t y, uint32_t zIndex);

	uint8_t mWidth;
	uint8_t mHeight;
	GridType mType;
	// cell size in Pixel
	cocos2d::Vec2 mPixelSize;
	uint8_t* mObstacleMap;
	PlantNode** mPlantMap;
	// geometrie

};

#endif // __FAIRY_GAMES_GARDEN_RUSH_GRID_H