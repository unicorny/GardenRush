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
#include "Enums.h"
//#include "PlantNode.h"
class IViewData;
class PlantNode;
class PlantType;
class LevelData;
class PlantTypesManager;
class RessourcenManager;
class Points;

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

struct GridCell {
	GridCell(uint8_t _x, uint8_t _y, GridType _type) :
		index(_x, _y), type(_type) {}
	GridCell() : type(GRID_ERROR) {}

	GridIndex index;
	GridType type;
};

class Grid : public cocos2d::Node
{
public:
	// init
	static Grid * create(uint8_t width, uint8_t height, GridType type);
	static void setRessourcenManager(RessourcenManager* ressources);

	bool setup(float edge_size_pixels, cocos2d::Vec2 pos, IViewData* bgTile, cocos2d::Node* parentNode);
	bool setup(const cocos2d::Vec2& edgeSizes, const cocos2d::Vec2& leftTopPosition, const std::vector<IViewData*>& tiles, cocos2d::Node* parentNode);
	bool setup(float edgeSizes, const cocos2d::Vec2& leftTopPosition, const std::vector<IViewData*>& tiles, cocos2d::Node* parentNode);
	void setIsometric() { mIsIsometric = true; }
	void setPixelSize(cocos2d::Vec2 pixelSize);
	bool fillBgGridCells(const IViewData* viewData);
	bool fillBgGridCellsRandom(const IViewData** viewData, int countViewData);
	bool fillBgGridCellsRandom(const std::vector<IViewData*>& tiles);

	// actions
	bool addBgGridCell(const IViewData* viewData, bool obstacle, uint8_t x, uint8_t y);

	void glowEmptyCells(bool enable = true);
	inline void glowSelectedCell(GridIndex index, bool enable = true) {
		if (enable) glowCell(index, "selectedCell");
		else glowCell(index, "default");
	}
	inline void glowErrorCell(GridIndex index, bool enable = true) {
		if (enable) glowCell(index, "errorCell");
		else glowCell(index, "default");
	}
	void glowCell(GridIndex index, const char* technique);
	void disableAllGlowCells();
	void glowNeighborCells(const PlantType* type, const PlantTypesManager* plantTypesManager, bool enable = true);
	void glowAutoCells(const PlantType* type, const PlantTypesManager* plantTypesManager);

	//! \return false if an obstacle is at this position
	bool addGridCell(PlantNode* viewNode, uint8_t x, uint8_t y);
	
	bool updateParentsOfPlantOnIndex(GridIndex index, const PlantTypesManager* plantTypesManager, const LevelData* levelData, Points* points);

	PlantNode* removeGridCell(uint8_t x, uint8_t y);
	bool removeGridCell(PlantNode* plantNode);
	void removeAllGridCells();
	cocos2d::Vec2 getOriginPosition(PlantNode* viewNode);

	void rescale();
	void reset();

	// infos
	PlantNode* isPlantNodeAtPosition(cocos2d::Vec2 localPosition) const;
	PlantNode* getPlantNodeAtWorldPosition(cocos2d::Vec2 worldPosition) const;
	PlantNode* getPlantNodeFromIndex(GridIndex index) const;
	bool isCellEmptyAndFree(uint8_t x, uint8_t y) const;
	GridIndex getGridIndex(cocos2d::Vec2 localPosition) const;
	cocos2d::Vec2 getWorldPositionForGridIndex(uint8_t x, uint8_t y);
	inline cocos2d::Vec2 getCellSize() const { return cocos2d::Vec2(mBoundingBoxSize.x / static_cast<float>(mWidth), mBoundingBoxSize.y / static_cast<float>(mHeight)); }
	cocos2d::Vec2 fromWorldToLocal(cocos2d::Vec2 worldCoords) const;
	cocos2d::Vec2 fromLocalToWorld(cocos2d::Vec2 localCoords) const;
	bool isInsideGrid(const cocos2d::Vec2& worldCoords) const;
	inline GridType getType() const { return mType; }
	inline float getEdgeSize() const {return mBoundingBoxSize.x;}
	
	
	inline bool isIsometric() const { return mIsIsometric; }


	// rendering, called from cocos2d-x
	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags);

	void renderCommand();

	
CC_CONSTRUCTOR_ACCESS:
	// Nodes should be created using create();
	Grid(uint8_t width, uint8_t height, GridType type);
	virtual ~Grid();

	virtual bool init();

protected:

	// coordinate transformation into and from isometric space
	// Quelle: https://gamedevelopment.tutsplus.com/tutorials/creating-isometric-worlds-a-primer-for-game-developers--gamedev-6511
	cocos2d::Vec2 isoLocalToFlatGrid(const cocos2d::Vec2& point) const;
	cocos2d::Vec2 flatToIso(const cocos2d::Vec2& point) const;

	bool addCellSprite(cocos2d::Sprite* sprite, uint8_t x, uint8_t y, uint32_t zIndex);
	bool _setup(const cocos2d::Vec2& leftTopPosition, const std::vector<IViewData*>& tiles, cocos2d::Node* parentNode);

	bool _setupRendering();
	bool _setupRenderingIso();
	bool _setupRendering3D();

	inline void setDefaultShader(cocos2d::Node* n) { n->setGLProgram(cocos2d::GLProgramCache::getInstance()->getGLProgram(cocos2d::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP)); }
	cocos2d::Technique* getTechniqueForNeighborType(PlantTypeNeighborType type, cocos2d::Material* material);

	uint8_t mWidth;
	uint8_t mHeight;
	GridType mType;
	// whole grid size in Pixel
	cocos2d::Vec2 mBoundingBoxSize;
	uint8_t* mObstacleMap;
	PlantNode** mPlantMap;
	cocos2d::Sprite** mBGCellsMap;
	// geometrie

	bool mIsIsometric;

	static RessourcenManager* mRessourcenManager;
	// ----------------------------------------------------------
	// render data
	cocos2d::VertexBuffer* mVertexBuffer;
	GLuint				   mIndexBuffer;
	cocos2d::CustomCommand mRenderCommand;
	

	// shader
	cocos2d::Material*     mMaterial;
	GLint				   mUniformLocationGridCell;

	// grid decoration details

};

#endif // __FAIRY_GAMES_GARDEN_RUSH_GRID_H