/*!
 * 
 * \author: Dario Rekowski 
 *
 * \date: 18.11.2018
 * 
 * \brief: Custom Render for grid
*/

#include "cocos2d.h"

class RessourcenManager;
class MainGameScene;

enum GridNodeType {
	GRID_NODE_NONE,
	GRID_NODE_2D,
	GRID_NODE_ISO,
	GRID_NODE_3D
};

class GridNode : public cocos2d::Node
{
public:
	GridNode(RessourcenManager* ressourcenManager);
	~GridNode();

	bool setup(uint8_t cellCount, cocos2d::Vec2 gridSizePixels, GridNodeType type, MainGameScene* mainGameScene);

	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags);

	void renderCommand();

protected:

	bool _setupVertices();

	GridNodeType			mType;

	// render data
	cocos2d::VertexBuffer* mVertexBuffer;
	GLuint				   mIndexBuffer;
	cocos2d::CustomCommand mRenderCommand;
	

	// shader	
	cocos2d::Material*     mMaterial;
	GLint				   mUniformLocationGridCell;

	// shader uniforms
	GLint				   mUniformLocationSidesTexCoords;
	cocos2d::Texture2D*    mSidesTexture;
};
