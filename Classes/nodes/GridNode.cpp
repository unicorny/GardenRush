#include "nodes/GridNode.h"
#include "ErrorLog.h"

using namespace cocos2d;

GridNode::GridNode()
	: mType(GRID_NODE_NONE), mVertexBuffer(NULL), mIndexBuffer(0)
{
	mMaterial = Material::createWithFilename("materials/GridNode.material");
	mMaterial->retain();
	autorelease();
}

GridNode::~GridNode()
{
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

	Vec3 vertices[] = {
		{0.0f, 1.0f, 0.0f}, // 0 left top
		{0.0f, 0.0f, 0.0f}, // 1 left bottom
		{1.0f, 0.0f, 0.0f}, // 2 right bottom
		{1.0f, 1.0f, 0.0f}, // 3 right top

		{0.0f, 0.0f, -0.125f}, // 4 edge 
		{0.0f, 1.0f, -0.125f}, // 5 left edge
		{1.0f, 0.0f, -0.125f}  // 6 right edge
	};

	mVertexBuffer->updateVertices(vertices, 7, 0);

	uint16_t indices[] = { 
		0, 1, 2, 2, 3, 0, // main
		1, 4, 5, 5, 1, 0, // left edge
		6, 2, 1, 4, 6, 2  // right edge
	}; 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 18 * sizeof(uint16_t), &indices[0], GL_STATIC_DRAW);

	mRenderCommand.func = std::bind(&GridNode::renderCommand, this);

	return true;
}

bool GridNode::setup(uint8_t cellCount, cocos2d::Vec2 gridSizePixels, GridNodeType type)
{
	// check if already initialized
	if (type == mType) { return true; } 
	if (mType == GRID_NODE_NONE) {
		if (!_setupVertices()) {
			LOG_ERROR("error setup vertices", false);
		}
	}

	setScaleX(gridSizePixels.x);
	setScaleY(gridSizePixels.y);
	setScaleZ(1.0f);


	switch (type) {
	GRID_NODE_ISO:
	GRID_NODE_3D:
		break;
	}

	

	mType = type;

	return true;
}


void GridNode::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
	//renderer->addDrawnBatches(1);
	mRenderCommand.init(10.0f, transform, Node::FLAGS_RENDER_AS_3D);
	uint32_t flag = 0;
	switch (mType) {
	GRID_NODE_3D:
		flag = Node::FLAGS_RENDER_AS_3D;
		break;
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
	switch (mType) {
	case GRID_NODE_ISO:
		programState = mMaterial->getTechniqueByName("grid_iso")->getPassByIndex(0)->getGLProgramState();
		break;
	default: LOG_ERROR_VOID("no valid type for rendering");
	}

	//programState->setUniformFloat("grid_cell_count", 1.0f);
	//programState->applyGLProgram(mTranslationObject.getMatrix());
	programState->applyGLProgram(mat1);


	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION);
	glDisable(GL_CULL_FACE);
	//glCullFace(GL_FRONT_AND_BACK);
	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer->getVBO());
	//glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT, GL_FILL);
	//glPolygonMode(GL_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 18);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	CHECK_GL_ERROR_DEBUG();
}