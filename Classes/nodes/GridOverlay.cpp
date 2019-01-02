#include "GridOverlay.h"
#include "ErrorLog.h"


using namespace cocos2d;

GridOverlay::GridOverlay()
: mMaterial(nullptr)	//: mVertexBuffer(nullptr), mIndexBuffer(0)
{

}

GridOverlay::~GridOverlay()
{
	if (mMaterial) {
		mMaterial->release();
		mMaterial = nullptr;
	}
	
}

bool GridOverlay::addSubgrid(GridType type, GridNodeType drawType, GridIndex cellCount, cocos2d::Vec2 positionOffset, cocos2d::Texture2D* texture)
{
	if (type >= GRID_SIZE) {
		LOG_ERROR("type is invalid", false);
	}
	mSubGrids[type].mGridRenderType = drawType;
	mSubGrids[type].mCellCount = cellCount;
	mSubGrids[type].mPositionOffset = positionOffset;

	DR_SAVE_DELETE(mSubGrids[type].mQuads);
	mSubGrids[type].mQuads = new V3F_C4B_T2F_Quad[cellCount.x * cellCount.y];
	mSubGrids[type].mRenderCommand = new cocos2d::QuadCommand;

	auto quadsCount = mSubGrids[type].mCellCount.x * mSubGrids[type].mCellCount.y;


	mSubGrids[type].mRenderCommand->init(
		0.0f, texture, mMaterial->getTechniqueByName("default")->getPassByIndex(0)->getGLProgramState(),
		BlendFunc::ALPHA_NON_PREMULTIPLIED,
		mSubGrids[type].mQuads, quadsCount, Mat4::IDENTITY, 0);

	return true;

}

bool GridOverlay::fillCellQuad(GridType type, GridIndex cellIndex, const cocos2d::Rect& vertices, const cocos2d::Color4B& color)
{
	V3F_C4B_T2F_Quad& quad = mSubGrids[type].mQuads[mSubGrids[type].getQuadIndex(cellIndex)];

	quad.tl.vertices = Vec3(vertices.getMinX(), vertices.getMaxY(), 0.0f);
	quad.tl.texCoords = Tex2F(0.0f, 1.0f);
	quad.tl.colors = color;

	quad.bl.vertices = Vec3(vertices.getMinX(), vertices.getMinY(), 0.0f);
	quad.bl.texCoords = Tex2F(0.0f, 0.0f);
	quad.bl.colors = color;

	quad.tr.vertices = Vec3(vertices.getMaxX(), vertices.getMaxY(), 0.0f);
	quad.tr.texCoords = Tex2F(1.0f, 1.0f);
	quad.tr.colors = color;
	
	quad.br.vertices = Vec3(vertices.getMaxX(), vertices.getMinY(), 0.0f);
	quad.br.texCoords = Tex2F(1.0f, 0.0f);
	quad.br.colors = color;
	return true;
}

bool GridOverlay::fillCellQuad(GridType type, GridIndex cellIndex, const cocos2d::Vec2 corners[4], const cocos2d::Color4B& color)
{
	V3F_C4B_T2F_Quad& quad = mSubGrids[type].mQuads[mSubGrids[type].getQuadIndex(cellIndex)];

	quad.tl.vertices = Vec3(corners[3].x, corners[3].y, 0.0f);
	quad.tl.texCoords = Tex2F(0.0f, 1.0f);
	quad.tl.colors = color;

	quad.bl.vertices = Vec3(corners[0].x, corners[0].y, 0.0f);
	quad.bl.texCoords = Tex2F(0.0f, 0.0f);
	quad.bl.colors = color;

	quad.tr.vertices = Vec3(corners[2].x, corners[2].y, 0.0f);
	quad.tr.texCoords = Tex2F(1.0f, 1.0f);
	quad.tr.colors = color;

	quad.br.vertices = Vec3(corners[1].x, corners[1].y, 0.0f);
	quad.br.texCoords = Tex2F(1.0f, 0.0f);
	quad.br.colors = color;
	return true;
}

bool GridOverlay::updateCellQuadColor(GridType type, GridIndex cellIndex, const cocos2d::Color4B& color)
{
	V3F_C4B_T2F_Quad& quad = mSubGrids[type].mQuads[mSubGrids[type].getQuadIndex(cellIndex)];
	quad.tl.colors = color;
	quad.bl.colors = color;
	quad.tr.colors = color;
	quad.br.colors = color;

	return true;
}

bool GridOverlay::initRender()
{
	uint16_t sumQuadCount = 0;
	mMaterial = RessourcenManager::getInstance()->getMaterial("gridOverlay");
	mMaterial->retain();

	for (uint8_t i = 0; i < GRID_SIZE; i++) {
		auto quadsCount = mSubGrids[i].mCellCount.x * mSubGrids[i].mCellCount.y;
		sumQuadCount += quadsCount;
			
	}
	
	
	/*mVertexBuffer = VertexBuffer::create(sizeof(V3F_C4B), sumQuadCount * 4);
	mVertexBuffer->retain();

	glGenBuffers(1, &mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sumQuadCount * 6 * sizeof(uint16_t), nullptr, GL_STATIC_DRAW);
	*/
	/*uint16_t sumVertexCount = 0;
	uint16_t sumIndexCount = 0;

	uint16_t indices[] = {0,1,2,3,2,1};

	for (uint8_t i = 0; i < GRID_SIZE; i++) {
		uint16_t quadCount = mSubGrids[i].mCellCount.x * mSubGrids[i].mCellCount.y;
		uint16_t vertexCount = quadCount * 4;
		uint16_t indexCount = quadCount * 6;
		mVertexBuffer->updateVertices(mSubGrids[i].mQuads, vertexCount, sumVertexCount);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		for (int iQuad = 0; iQuad < quadCount; iQuad++) {
			for (int iIndices = 0; iIndices < 6; iIndices++) {
				indices[iIndices] += 4;
			}
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sumIndexCount + quadCount * 6, 6 * sizeof(uint16_t), &indices[0]);
		}
		
		sumVertexCount += vertexCount;
		sumIndexCount += indexCount;
	}

	mRenderCommand.func = std::bind(&GridOverlay::renderCommand, this);
	*/
	return true;
}

void GridOverlay::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
	//renderer->addDrawnBatches(1);
	//mRenderCommand.init(0.0f, transform, flags);

	//mRenderCommand.init(10.0f, transform, flag);
	//renderer->addCommand(mRenderCommand);
	for (uint8_t i = 0; i < GRID_SIZE; i++) {
		if (mSubGrids[i].mRenderCommand && mSubGrids[i].mGlowEnabled) {
			renderer->addCommand(mSubGrids[i].mRenderCommand);
		}
	}
}

double GridOverlay::getMemoryConsumption()
{
	unsigned long memory = sizeof(GridOverlay);
	memory += sizeof(Material);
	for (uint8_t i = 0; i < GRID_SIZE; i++) {
		memory += sizeof(QuadCommand);
		memory += sizeof(V3F_C4B_T2F_Quad) * mSubGrids[i].mCellCount.x * mSubGrids[i].mCellCount.y;
	}

	return memory;
}