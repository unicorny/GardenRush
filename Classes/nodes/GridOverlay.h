#ifndef __FAIRY_GAMES_GARDEN_RUSH_NODES_GRID_OVERLAY_H
#define __FAIRY_GAMES_GARDEN_RUSH_NODES_GRID_OVERLAY_H

/*!
*
* \author: Dario Rekowski
*
* \date: 15.12.2018
*
* \brief: Custom Render grid overlay, all together in one render call
*/

#include "cocos2d.h"
#include "Enums.h"
#include "nodes/Grid.h"

struct V3F_C4B
{
	/// vertices (3F)
	cocos2d::Vec3     vertices;            // 12 bytes

								  /// colors (4B)
	cocos2d::Color4B      colors;              // 4 bytes

};

struct V3F_C4B_Quad
{
	/// top left
	V3F_C4B    tl;
	/// bottom left
	V3F_C4B    bl;
	/// top right
	V3F_C4B    tr;
	/// bottom right
	V3F_C4B    br;
};

class GridOverlay : public cocos2d::Node
{
public:
	GridOverlay();
	~GridOverlay();

	bool addSubgrid(GridType type, GridNodeType drawType, GridIndex cellCount, cocos2d::Vec2 positionOffset, cocos2d::Texture2D* texture);

	bool fillCellQuad(GridType type, GridIndex cellIndex, const cocos2d::Rect& vertices, const cocos2d::Color4B& color);
	//! \param corners 0 = bl, 1 = br, 2 = tr, 3 = tl
	bool fillCellQuad(GridType type, GridIndex cellIndex, const cocos2d::Vec2 corners[4], const cocos2d::Color4B& color);
	bool updateCellQuadColor(GridType type, GridIndex cellIndex, const cocos2d::Color4B& color);

	inline void setGlow(GridType type, bool enable) { mSubGrids[type].mGlowEnabled = enable; }

	// init vertex and index buffer and draw command
	bool initRender();

	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags);	

	double getMemoryConsumption();


protected:
	struct GridOverlaySubGrid
	{
		GridOverlaySubGrid() : mQuads(nullptr), mRenderCommand(nullptr), mGlowEnabled(false) {}
		~GridOverlaySubGrid() {
			DR_SAVE_DELETE_ARRAY(mQuads);
			DR_SAVE_DELETE(mRenderCommand);
		}
		GridNodeType mGridRenderType;
		GridIndex mCellCount;
		cocos2d::Vec2 mPositionOffset;

		cocos2d::V3F_C4B_T2F_Quad* mQuads;
		cocos2d::QuadCommand*   mRenderCommand;

		bool mGlowEnabled;

		uint8_t getQuadIndex(GridIndex i) {
			return i.x * mCellCount.x + i.y;
		}
	};

	// vertex data
	GridOverlaySubGrid mSubGrids[GRID_SIZE];

	cocos2d::Material* mMaterial;
	// render data
	//cocos2d::VertexBuffer* mVertexBuffer;
	//GLuint				   mIndexBuffer;
	//cocos2d::CustomCommand mRenderCommand;
};

#endif //__FAIRY_GAMES_GARDEN_RUSH_NODES_GRID_OVERLAY_H