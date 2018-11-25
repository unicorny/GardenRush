/*
 *
 * \author: Dario Rekowski
 * 
 * \date:   18.11.18
 * 
 * \brief: grid data container, contain
*/
#include "nodes/PlantNode.h"
#include "controller/RessourcenManager.h"

#include "cocos2d.h"
/*
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
*/

class GridModel
{
public:
	//GridModel()
protected:
	// static data
	// bounding box
	cocos2d::Vec2 mSizeInScreenSpace;
	uint8_t mCellsX;
	uint8_t mCellsY;
	GridType mType;

	uint8_t* mObstacleMap;
	PlantNode** mPlantMap;
	cocos2d::Sprite** mBGCellsMap;
	// geometrie

	bool mIsIsometric;

	static RessourcenManager* mRessourcenManager;
};