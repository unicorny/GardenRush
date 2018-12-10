#include "nodes/GridNode.h"
#include "ErrorLog.h"
#include "controller/RessourcenManager.h"
#include "MainGameScene.h"

using namespace cocos2d;

GridNode::GridNode(RessourcenManager* ressourcenManager)
	: mType(GRID_NODE_NONE), mVertexBuffer(NULL), mIndexBuffer(0)
{
	mMaterial = ressourcenManager->getMaterial("GridNode");
	mMaterial->retain();
	//tex_coord_rects
	auto programState = mMaterial->getTechniqueByName("grid_sides_iso")->getPassByIndex(0)->getGLProgramState();
	mUniformLocationSidesTexCoords = glGetUniformLocation(programState->getGLProgram()->getProgram(), "tex_coord_rects");

	
	//mMaterial = Material::createWithFilename("materials/GridNode.material");
	//mMaterial->retain();
	autorelease();
}

GridNode::~GridNode()
{
	if (mMaterial) {
		mMaterial->release();
		mMaterial = NULL;
	}
	if (mVertexBuffer) {
		mVertexBuffer->release();
		mVertexBuffer = NULL;
	}
	if (mIndexBuffer) {
		glDeleteBuffers(1, &mIndexBuffer);
	}
}

bool GridNode::_setupVertices()
{
	mVertexBuffer = VertexBuffer::create(sizeof(Vec3), 7);
	mVertexBuffer->retain();

	glGenBuffers(1, &mIndexBuffer);

	// tex coords
	auto frameCache = SpriteFrameCache::getInstance();
	float sideTexCords[8];

	auto leftSideTex = frameCache->getSpriteFrameByName("bg_side_left.png");
	auto rightSideTex = frameCache->getSpriteFrameByName("bg_side_right.png");

	auto leftSideTexRect  = leftSideTex->getRectInPixels();
	auto rightSideTexRect = rightSideTex->getRectInPixels();

	auto leftSideTexture  = leftSideTex->getTexture();
	auto rightSideTexture = rightSideTex->getTexture();

	if (leftSideTexture == rightSideTexture) {
		mSidesTexture = leftSideTexture;
	}
	else {
		ErrorLog::printf("leftSide and rightSide Frame not same Texture");
		return false;
	}

	Size leftSideTextureSize = Size(leftSideTexture->getPixelsWide(), leftSideTexture->getPixelsHigh());
	Size rightSideTextureSize = Size(rightSideTexture->getPixelsWide(), rightSideTexture->getPixelsHigh());

	sideTexCords[0] = leftSideTexRect.origin.x / leftSideTextureSize.width;
	sideTexCords[1] = leftSideTexRect.origin.y / leftSideTextureSize.height;
	sideTexCords[2] = leftSideTexRect.size.width / leftSideTextureSize.width;
	sideTexCords[3] = leftSideTexRect.size.height / leftSideTextureSize.height;

	sideTexCords[4] = rightSideTexRect.origin.x / rightSideTextureSize.width;
	sideTexCords[5] = rightSideTexRect.origin.y / rightSideTextureSize.height;
	sideTexCords[6] = rightSideTexRect.size.width / rightSideTextureSize.width;
	sideTexCords[7] = rightSideTexRect.size.height / rightSideTextureSize.height;


	if (mUniformLocationSidesTexCoords != -1)
	{
		auto programState = mMaterial->getTechniqueByName("grid_sides_iso")->getPassByIndex(0)->getGLProgramState();
		programState->getGLProgram()->use();
		//GL::useProgram();
		glUniform2fv(mUniformLocationSidesTexCoords, 4, sideTexCords);
		CHECK_GL_ERROR_DEBUG();
	}

	Vec3 vertices[] = {
		{ -0.5f,  0.5f,  0.0f }, // 0
		{ -0.5f, -0.5f,  0.0f }, // 1
		{  0.5f, -0.5f,  0.0f }, // 2
		{  0.5f,  0.5f,  0.0f }, // 3

		{ -0.5f, -0.5f, -0.125f }, // 4 3D
		{  0.5f, -0.5f, -0.125f }, // 5 3D

		//{  0.5f,  0.5f, 0.125f }  // 6
		{ -0.5f,  0.5f, -0.125f }//,  // 6 3D,

	    //{ -0.5625f,  -0.5625f, 0.0f}, // 7 (4) ISO
		//{  0.4375f, -0.5625f, 0.0f}, // 8 (5) ISO,
		//{ -0.5625f,   0.4375f, 0.0f} // 9 (6) ISO
	};

	

	mVertexBuffer->updateVertices(vertices, 7, 0);

	uint16_t indices[] = { 
		0, 1, 3, 1, 2, 3,
		5, 4, 2, 2, 4, 1,
		4, 6, 1, 1, 6, 0//,
		//8, 7, 2, 2, 7, 1,
		//7, 9, 1, 1, 9, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 18 * sizeof(uint16_t), &indices[0], GL_STATIC_DRAW);

	mRenderCommand.func = std::bind(&GridNode::renderCommand, this);

	return true;
}

bool GridNode::setup(uint8_t cellCount, cocos2d::Vec2 gridSizePixels, GridNodeType type, MainGameScene* mainGameScene)
{
	// check if already initialized
	if (type == mType) { return true; } 
	if (mType == GRID_NODE_NONE) {
		if (!_setupVertices()) {
			LOG_ERROR("error setup vertices", false);
		}
		else {
			//mainGameScene->addSpriteBatchNode("spriteAtlas/bg_sides.png", "bg_sides", 2);
		}
	}

	setScaleX(gridSizePixels.x);
	setScaleY(gridSizePixels.y);
	setScaleZ(gridSizePixels.y);

	mType = type;

	return true;
}


void GridNode::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
	//renderer->addDrawnBatches(1);
	mRenderCommand.init(0.0f, transform, Node::FLAGS_RENDER_AS_3D);
	uint32_t flag = 0;
	switch (mType) {
	case GRID_NODE_3D:
		flag = Node::FLAGS_RENDER_AS_3D;
		break;
	default: break;
	}

	//mRenderCommand.init(10.0f, transform, flag);
	renderer->addCommand(&mRenderCommand);
}

void GridNode::renderCommand()
{
	//mMaterial->getTechniqueByName("default")->getPassByIndex(0)->getGLProgramState()->applyGLProgram(getNodeToWorldTransform());
	auto mat1 = getNodeToWorldTransform();
	//Mat4 mat2 = mTranslationObject.getMatrix();
	GLProgramState* programState = NULL;
	GLProgramState* programState2 = NULL;
	switch (mType) {
	case GRID_NODE_ISO:
		//programState = mMaterial->getTechniqueByName("grid_iso")->getPassByIndex(0)->getGLProgramState();
		programState2 = mMaterial->getTechniqueByName("grid_sides_iso")->getPassByIndex(0)->getGLProgramState();
		break;
	default: LOG_ERROR_VOID("no valid type for rendering");
	}

	//programState->setUniformFloat("grid_cell_count", 1.0f);
	//programState->applyGLProgram(mTranslationObject.getMatrix());
	


	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION);
	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer->getVBO());
	//glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT, GL_FILL);
	//glPolygonMode(GL_BACK, GL_LINE);
	//glDisable(GL_CULL_FACE);
	//programState->applyGLProgram(mat1);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
	programState2->applyGLProgram(mat1);
	GL::bindTexture2D(mSidesTexture);
	//mSidesTexture->drawAtPoint()

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, (void*)(sizeof(uint16_t) * 6));
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 12);
	//glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	CHECK_GL_ERROR_DEBUG();
}