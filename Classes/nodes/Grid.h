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
class IViewData;

enum GridObstacleType
{
	OBSTACLE_NONE = 0,
	OBSTACLE_DEFAULT = 1
};

class Grid : public cocos2d::Node
{
public:
	static Grid * create(uint8_t width, uint8_t height);

	bool addBgGridCell(const IViewData* viewData, bool obstacle, uint8_t x, uint8_t y);
	bool fillBgGridCells(const IViewData* viewData);
	void reset();

	void setPixelSize(cocos2d::Vec2 pixelSize);
	
CC_CONSTRUCTOR_ACCESS:
	// Nodes should be created using create();
	Grid(uint8_t width, uint8_t height);
	virtual ~Grid();

	virtual bool init();

protected:
	uint8_t mWidth;
	uint8_t mHeight;
	cocos2d::Vec2 mPixelSize;
	uint8_t* mObstacleMap;
	// geometrie

};

#endif // __FAIRY_GAMES_GARDEN_RUSH_GRID_H