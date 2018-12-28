#include "nodes/Grid.h"
#include "model/ViewData.h"
#include "model/LevelData.h"
#include "model/Points.h"
#include "ErrorLog.h"
#include "PlantTypesManager.h"
#include "controller/RessourcenManager.h"
#include "MainGameScene.h"
#include "GridOverlay.h"

#include "model/Player.h"


using namespace cocos2d;


Grid::Grid(uint8_t width, uint8_t height, GridType type)
	: mWidth(width), mHeight(height), mType(type), mBoundingBoxSize(Vec2::ZERO), mObstacleMap(nullptr), mObstaclesCount(0), mPlantMap(nullptr), mIsIsometric(false),
	mGridGraphicsConfig(nullptr), 
	mGlowEnabled(false), mBGCellQuads(nullptr), mBGGlowCellQuads(nullptr), mBGCellQuadCommand(nullptr), mBGGlowCellQuadCommand(nullptr),
	mGridOverlay(nullptr)
{
	mObstacleMap = (uint8_t*)(malloc(width * height * sizeof(uint8_t)));
	mPlantMap = (PlantNode**)(malloc(width * height * sizeof(PlantNode*)));
	//mBGCellsMap = (cocos2d::Sprite**)(malloc(width * height * sizeof(cocos2d::Sprite*)));
	mBGCellQuads = new V3F_C4B_T2F_Quad[width * height];
	mBGGlowCellQuads = new V3F_C4B_T2F_Quad[width * height];
	mBGCellQuadCommand = new QuadCommand;
	mBGGlowCellQuadCommand = new QuadCommand;

	memset(mObstacleMap, 0, width*height * sizeof(uint8_t));
	memset(mPlantMap, 0, width* height * sizeof(PlantNode*));
	//memset(mBGCellsMap, 0, width* height * sizeof(cocos2d::Sprite*));
}

Grid::~Grid()
{
	free(mObstacleMap);
	//free(mBGCellsMap);
	free(mPlantMap);

	DR_SAVE_DELETE_ARRAY(mBGCellQuads);
	DR_SAVE_DELETE_ARRAY(mBGGlowCellQuads);
	DR_SAVE_DELETE(mBGCellQuadCommand);
	DR_SAVE_DELETE(mBGGlowCellQuadCommand);
}


Grid * Grid::create(uint8_t width, uint8_t height, GridType type)
{
	Grid * ret = new (std::nothrow) Grid(width, height, type);
	if (ret && ret->init())
	{
		ret->autorelease();
		
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool Grid::init() 
{
	bool ret = Node::init();
	if (ret) {
		// own initalisation
		reset();
	}
	return ret;
}

bool Grid::setup(float edge_size_pixels, cocos2d::Vec2 pos, cocos2d::Node* parentNode)
{

	mBoundingBoxSize = Vec2(edge_size_pixels, edge_size_pixels);
	mGridGraphicsConfig = RessourcenManager::getInstance()->getGridConfig(GRID_NODE_2D);

	return _setup(pos, parentNode);
}


bool Grid::setup(const cocos2d::Vec2& edgeSizes, const cocos2d::Vec2& leftTopPosition, cocos2d::Node* parentNode)
{
	setIsometric();
	mBoundingBoxSize = edgeSizes;
	mGridGraphicsConfig = RessourcenManager::getInstance()->getGridConfig(GRID_NODE_ISO);

	return _setup(leftTopPosition, parentNode);
}
bool Grid::setup(float edgeSizes, const Vec2& leftTopPosition, const std::vector<IViewData*>& tiles, Node* parentNode)
{
	mBoundingBoxSize = Vec2(edgeSizes, edgeSizes);
	ErrorLog::printf("[Grid::setup] not used anymore");
	return false;
	//return _setup(leftTopPosition, tiles, parentNode);
}

bool Grid::_setup(const cocos2d::Vec2& leftTopPosition, cocos2d::Node* parentNode)
{
	//setAnchorPoint(Vec2::ZERO);
	setAnchorPoint(Vec2(0.5f, 0.5f));
	setPosition(leftTopPosition + mBoundingBoxSize * 0.5f);
	auto tiles = mGridGraphicsConfig->groundTiles;
	bool ret = false;
	if (true) {
		fillGroundTilesIntoTextureAtlas();
	}
	else {
		if (tiles.size() > 1) {
			ret = fillBgGridCellsRandom(tiles);
		}
		else {
			ret = fillBgGridCells(tiles.front());
		}
		if (!ret) {
			LOG_ERROR("fill bg grid", false);
		}
	}
	parentNode->addChild(this, 1);
	return true;
}

void Grid::setPixelSize(Vec2 pixelSize)
{
	mBoundingBoxSize = pixelSize;

}

void Grid::reset()
{
	memset(mObstacleMap, 0, mWidth*mHeight * sizeof(uint8_t));
	memset(mPlantMap, 0, mWidth* mHeight * sizeof(PlantNode*));
	//memset(mBGCellsMap, 0, mWidth* mHeight * sizeof(Sprite*));
}

bool Grid::addBgGridCell(const IViewData* viewData, bool obstacle, uint8_t x, uint8_t y)
{
	LOG_ERROR("not used anymore", false);
	assert(x < mWidth && y < mHeight);
	assert(mBoundingBoxSize.x > 0 && mBoundingBoxSize.y > 0);
	auto index = x * mWidth + y;
	if (obstacle) {
		mObstacleMap[index] = OBSTACLE_DEFAULT;
	}
	auto cellSprite = viewData->createSprite();
	//mBGCellsMap[index] = cellSprite;
	return addCellSprite(cellSprite, x, y, 0);
}

bool Grid::addGridCell(PlantNode* viewData, uint8_t x, uint8_t y)
{
	assert(x < mWidth && y < mHeight);
	assert(mBoundingBoxSize.x > 0 && mBoundingBoxSize.y > 0);
	if (OBSTACLE_DEFAULT == mObstacleMap[x * mWidth + y]) {
		return false;
	}
	mPlantMap[x * mWidth + y] = viewData;
	// calculate z index
	//ErrorLog::printf("[Grid::addGridCell] x: %d, y: %d\n", x, y);
	bool result = addCellSprite(viewData, x, y, 10 + (mWidth * mHeight - x - y));
	if (result) {
		viewData->setGridIndex(GridIndex(x, y));
		viewData->setParentGrid(this);
		if (isIsometric()) {
	//		viewData->setAnchorPoint(Vec2(0.5f, 0.5f));
			//sprite->setPosition(Vec2(x * (mPixelSize.x / mWidth), y * (mPixelSize.y / mHeight)));
			//viewData->setScaleX(viewData->getScaleX() * 1.8f);
			//viewData->setScaleY(viewData->getScaleY() * 2.4f);
			//viewData->setScaleY((1.0f/0.866f)* viewData->getScaleY());
			//viewData->setSkewX(-30.0f);
			//viewData->setRotation(-30.0f);
//			viewData->setPositionX(viewData->getPositionX() + mBoundingBoxSize.x / static_cast<float>(mWidth));
		}
		return result;
	}
	return false;
}


bool Grid::updateParentsOfPlantOnIndex(GridIndex index, const PlantTypesManager* plantTypesManager, const LevelData* levelData, Points* points)
{
	assert(mType == GRID_MAIN);
	assert(mPlantMap[index.x * mWidth + index.y]);
	auto mainPlantNode = mPlantMap[index.x * mWidth + index.y];
	auto mainPlantType = mainPlantNode->getPlantType();
	auto mainPlantTypeHash = mainPlantType->getNameHash();
	
	auto mainPlantTypeIndex = mainPlantType->getIndex();

	assert(mainPlantType && mainPlantTypeHash);

	// 8 possible neighbors, 3 above, 1 left, 1 right, 3 at bottom
	// 
	for (int x = index.x - 1; x <= index.x + 1; x++) {
		if (x < 0 || x >= mWidth) continue;
		for (int y = index.y - 1; y <= index.y + 1; y++) {
			if(y < 0 || y >= mHeight) continue;
			// we don't need ourselves
			if (x == index.x && y == index.y) continue;

			// left above, right above, bottom left or bottom right
			bool edge = false;

			if (index.x == x || index.y == y) {
				// above, bottom, left or right
				edge = true;
			}
			auto neighborPlant = mPlantMap[x * mWidth + y];
			if (neighborPlant) {
				auto neighborPlantType = neighborPlant->getPlantType();
				int growPhasis = neighborPlant->countNewNeighbor(
					mainPlantTypeHash,
					plantTypesManager->getNeighborType(mainPlantTypeIndex, neighborPlantType->getIndex()),
					edge
				);
				// plant has out grown
				if (growPhasis >= levelData->getMaxGrowthPhasis() || growPhasis == PLANT_PHASIS_FINAL_BAD) {
					if (levelData->isAutoHarvesting()) {
						auto cellSize = getCellSize();
						auto pos = fromLocalToWorld(Vec2((static_cast<float>(x)+0.5f) * cellSize.x, y * cellSize.y));
						float mult = 100.0f;
						if (growPhasis == PLANT_PHASIS_FINAL_BAD) mult = -10.0f;
						points->addPoints(mult * neighborPlant->getPointsMultiplicator() * neighborPlant->getDiversityBonus(), pos);
						removeGridCell(x, y);
						neighborPlant = NULL;
					}
					else {
						neighborPlant->setOutGrown();
					}
				}
				
			}
		}
	}
	return true;
}


bool Grid::addCellSprite(Sprite* sprite, uint8_t x, uint8_t y, uint32_t zIndex)
{
	// default anchor lower left
	if (!sprite) {
		LOG_ERROR("creating sprite", false);
	}
	
	auto cellSize = getCellSize();
	Size spriteSize = sprite->getContentSize();
	Vec2 spriteScale = Vec2(sprite->getScaleX(), sprite->getScaleY());
	auto pos = Vec2(
		static_cast<float>(x) * cellSize.x,
		static_cast<float>(y) * cellSize.y
	);
	sprite->setAnchorPoint(Vec2::ZERO);

	if (isIsometric()) {
		//ErrorLog::printf("pos: %f, %f\n", pos.x, pos.y);
		//pos.x += 0.5f * cellSize.x;
		//pos.y += 0.5f * cellSize.y;
		//pos = flatToIso(pos);
		// origin : x = grid bounding box / 2
		//          y = 0
		// pos.x = origin.x + (x-y)/2 * cellWidth
		// pos.y = (x+y)/2 * cellHeight
		pos = Vec2(
			((static_cast<float>(x - y) + static_cast<float>(mWidth)) / 2.0f - 0.5f) * mBoundingBoxSize.x / static_cast<float>(mWidth),
			((static_cast<float>(x + y)) / 2.0f) * mBoundingBoxSize.y / static_cast<float>(mHeight)
		);
		
		//ErrorLog::printf("flat pos: (%d)%f, (%d)%f\n", x, pos.x, y, pos.y);
		//sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		//sprite->setScale(0.25f);
	} else {
		
	}
	// move by anchor
	auto anchor = getAnchorPoint();
	pos -= Vec2(anchor.x * mBoundingBoxSize.x, anchor.y * mBoundingBoxSize.y);
	sprite->setPosition(pos);
	float scale = cellSize.x / spriteSize.width;
	sprite->setScale(scale);
	//sprite->setScale(scale, cellSize.y / spriteSize.height);
	//sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
	//
	/*sprite->setScale(
		(static_cast<float>(mPixelSize.x) / static_cast<float>(mWidth)) / size.width,
		(static_cast<float>(mPixelSize.y) / static_cast<float>(mHeight)) / size.height
	);*/
	sprite->setLocalZOrder(zIndex);
	this->addChild(sprite);

	return true;
}

cocos2d::Vec2 Grid::getWorldPositionForGridIndex(uint8_t x, uint8_t y)
{
	auto cellSize = getCellSize();
	auto pos = Vec2(
		static_cast<float>(x) * cellSize.x,
		static_cast<float>(y) * cellSize.y
	);
	if (isIsometric()) {
		//ErrorLog::printf("pos: %f, %f\n", pos.x, pos.y);
		//pos.x += 0.5f * cellSize.x;
		//pos.y += 0.5f * cellSize.y;
		//pos = flatToIso(pos);
		// origin : x = grid bounding box / 2
		//          y = 0
		// pos.x = origin.x + (x-y)/2 * cellWidth
		// pos.y = (x+y)/2 * cellHeight
		pos = Vec2(
			((static_cast<float>(x - y) + static_cast<float>(mWidth)) / 2.0f - 0.5f) * mBoundingBoxSize.x / static_cast<float>(mWidth),
			((static_cast<float>(x + y)) / 2.0f) * mBoundingBoxSize.y / static_cast<float>(mHeight)
		);

		//ErrorLog::printf("flat pos: (%d)%f, (%d)%f\n", x, pos.x, y, pos.y);
		//sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		//sprite->setScale(0.25f);
	}
	// move by anchor
	auto anchor = getAnchorPoint();
	pos -= Vec2(anchor.x * mBoundingBoxSize.x, anchor.y * mBoundingBoxSize.y);

	return fromLocalToWorld(pos);

}

Rect Grid::getAbsGridTile(GridIndex index)
{
	Rect rect;
	rect.origin = getWorldPositionForGridIndex(index.x, index.y);
	rect.size = getCellSize();


	return rect;
}

PlantNode* Grid::removeGridCell(uint8_t x, uint8_t y)
{
	assert(x < mWidth && y < mHeight);
	PlantNode* plantNode = mPlantMap[x * mWidth + y];
	this->removeChild(plantNode, false);
	mPlantMap[x * mWidth + y] = NULL;
	return plantNode;
}

bool Grid::removeGridCell(PlantNode* plantNode)
{
	auto gridIndex = plantNode->getGridIndex();
	if (plantNode->getParentGrid() != this) {
		LOG_ERROR("does not belong to this grid ", false);
	}
	auto plantNodeReturn = removeGridCell(gridIndex.x, gridIndex.y);
	if (plantNode != plantNodeReturn) {
		LOG_ERROR("wasn't the plant node on this position", false);
	}
	return true;
}

void Grid::removeAllGridCells()
{
	for (int i = 0; i < mWidth * mHeight; i++) {
		if (mPlantMap[i]) {
			mPlantMap[i]->stopAllAnimations();
			this->removeChild(mPlantMap[i], true);
			mPlantMap[i] = NULL;
		}
	}
}

bool Grid::fillBgGridCells(const IViewData* viewData)
{
	assert(mBoundingBoxSize.x > 0 && mBoundingBoxSize.y > 0);
	assert(mWidth > 0 && mHeight > 0);

	for (int i = 0; i < mWidth; i++) {
		for (int j = 0; j < mHeight; j++) {
			bool ret = addBgGridCell(viewData, false, i, j);
			if (!ret) {
				LOG_ERROR("add bg grid cell", false);
			}	
		}
	}
	return true;
}


bool Grid::fillBgGridCellsRandom(const IViewData** viewData, int countViewData)
{
	assert(mBoundingBoxSize.x > 0 && mBoundingBoxSize.y > 0);
	assert(mWidth > 0 && mHeight > 0);
	assert(viewData != NULL && countViewData > 0);

	for (int i = 0; i < mWidth; i++) {
		for (int j = 0; j < mHeight; j++) {
			auto t = viewData[RandomHelper::random_int(0, countViewData - 1)];
			bool ret = addBgGridCell(t, false, i, j);
			if (!ret) {
				LOG_ERROR("add bg grid cell", false);
			}
		}
	}
	return true;
}

bool Grid::fillBgGridCellsRandom(const std::vector<IViewData*>& tiles)
{
	assert(mBoundingBoxSize.x > 0 && mBoundingBoxSize.y > 0);
	assert(mWidth > 0 && mHeight > 0);
	assert(tiles.size() > 0);

	for (int i = 0; i < mWidth; i++) {
		for (int j = 0; j < mHeight; j++) {
			auto t = tiles[RandomHelper::random_int(0, static_cast<int>(tiles.size() - 1))];
			bool ret = addBgGridCell(t, false, i, j);
			if (!ret) {
				LOG_ERROR("add bg grid cell", false);
			}
			//if (j == 1) return true;
			//return true;
		}
	}
	return true;
}

bool Grid::fillCellQuad(cocos2d::V3F_C4B_T2F_Quad* quad, const cocos2d::Rect& vertices, const cocos2d::Color4B& color, const cocos2d::Rect& textureRect, const cocos2d::Vec2& textureSize)
{
	quad->tl.vertices = Vec3(vertices.getMinX(), vertices.getMaxY(), 0.0f);
	quad->tl.colors = Color4B(255, 255, 255, 255);
	quad->tl.texCoords = Tex2F(textureRect.getMinX() / textureSize.x, textureRect.getMaxY() / textureSize.y);

	quad->bl.vertices = Vec3(vertices.getMinX(), vertices.getMinY(), 0.0f);
	quad->bl.colors = Color4B(255, 255, 255, 255);
	quad->bl.texCoords = Tex2F(textureRect.getMinX() / textureSize.x, textureRect.getMinY() / textureSize.y);

	quad->tr.vertices = Vec3(vertices.getMaxX(), vertices.getMaxY(), 0.0f);
	quad->tr.colors = Color4B(255, 255, 255, 255);
	quad->tr.texCoords = Tex2F(textureRect.getMaxX() / textureSize.x, textureRect.getMaxY() / textureSize.y);

	quad->br.vertices = Vec3(vertices.getMaxX(), vertices.getMinY(), 0.0f);
	quad->br.colors = Color4B(255, 255, 255, 255);
	quad->br.texCoords = Tex2F(textureRect.getMaxX() / textureSize.x, textureRect.getMinY() / textureSize.y);

	return true;
}

bool Grid::updateCellQuadTextureCoords(cocos2d::V3F_C4B_T2F_Quad* quad, const cocos2d::Rect& textureRect, const cocos2d::Vec2& textureSize)
{
	quad->tl.texCoords = Tex2F(textureRect.getMinX() / textureSize.x, textureRect.getMaxY() / textureSize.y);
	quad->bl.texCoords = Tex2F(textureRect.getMinX() / textureSize.x, textureRect.getMinY() / textureSize.y);
	quad->tr.texCoords = Tex2F(textureRect.getMaxX() / textureSize.x, textureRect.getMaxY() / textureSize.y);
	quad->br.texCoords = Tex2F(textureRect.getMaxX() / textureSize.x, textureRect.getMinY() / textureSize.y);

	quad->tl.colors = Color4B::RED;
	quad->bl.colors = Color4B::WHITE;
	quad->tr.colors = Color4B::WHITE;
	quad->br.colors = Color4B::WHITE;

	return true;
}

bool Grid::updateCellQuadColor(cocos2d::V3F_C4B_T2F_Quad* quad, const cocos2d::Color4B& color)
{
	quad->tl.colors = color;
	quad->bl.colors = color;
	quad->tr.colors = color;
	quad->br.colors = color;

	return true;
}

bool Grid::setGridOverlay(GridOverlay* gridOverlay)
{
	mGridOverlay = gridOverlay;
	GridNodeType nodeType = GRID_NODE_2D;
	if (isIsometric()) { nodeType = GRID_NODE_ISO; }
	Vec2 positionOffset = getPosition();
	auto texture = mGridGraphicsConfig->overlay->getTexture();
	mGridOverlay->addSubgrid(mType, nodeType, GridIndex(mWidth, mHeight), positionOffset, texture);
	
	for (int y = 0; y < mHeight; y++) {
		for (int x = 0; x < mWidth; x++) {
			uint8_t index = x * mWidth + y;
			Color4B color = Color4B::WHITE;
			if (mObstacleMap[index] == OBSTACLE_DEFAULT) {
				color.a = 0;
			}
			auto tile = getAbsGridTile(GridIndex(x, y));

			// corners:  
			//		0 = bl
			//		1 = br
			//		2 = tr
			//		3 = tl
			Vec2 corners[4];
			
			
				
			if(isIsometric()) {
				corners[0] = Vec2(tile.getMidX(), tile.getMinY());
				corners[1] = Vec2(tile.getMaxX(), tile.getMidY());
				corners[2] = Vec2(tile.getMidX(), tile.getMaxY());
				corners[3] = Vec2(tile.getMinX(), tile.getMidY());
			
			}
			else {
				corners[0] = Vec2(tile.getMinX(), tile.getMinY());
				corners[1] = Vec2(tile.getMaxX(), tile.getMinY());
				corners[2] = Vec2(tile.getMaxX(), tile.getMaxY());
				corners[3] = Vec2(tile.getMinX(), tile.getMaxY());
			}
			mGridOverlay->fillCellQuad(mType, GridIndex(x, y), corners, color);
		}
	}
	

	return true;
}


bool Grid::fillGroundTilesIntoTextureAtlas()
{
	assert(mBoundingBoxSize.x > 0 && mBoundingBoxSize.y > 0);
	assert(mWidth > 0 && mHeight > 0);
	//assert(tiles.size() > 0);
	assert(mBGCellQuads);
	assert(mBGGlowCellQuads);

	auto tiles = mGridGraphicsConfig->groundTiles;
	assert(tiles.size() > 0);

	SpriteFrame* firstSpriteFrame = nullptr;
	auto spriteFrameOverlaySmall = viewGetSpriteFrame(mGridGraphicsConfig->overlay_small);
	auto overlayTexture = spriteFrameOverlaySmall->getTexture();

	//mGroundTextureAtlas->create()
	int bgCellIndex = 0;
	for (int y = 0; y < mWidth; y++) {
		for (int x = 0; x < mHeight; x++) {
			uint8_t index = x * mWidth + y;
			if (mObstacleMap[index] == OBSTACLE_DEFAULT)
				continue;
			auto t = tiles[RandomHelper::random_int(0, static_cast<int>(tiles.size() - 1))];
			auto spriteFrame = viewGetSpriteFrame(t);

			if (!firstSpriteFrame) {
				firstSpriteFrame = spriteFrame;
			} else {
				if (firstSpriteFrame->getTexture() != spriteFrame->getTexture()) {
					ErrorLog::printf("[Grid::fillGroundTilesIntoTextureAtlas] Error, bg tiles have different textures");
					continue;
				}
			}
			
			auto textureRect = spriteFrame->getRectInPixels();
			auto overlayTextureRect = spriteFrameOverlaySmall->getRectInPixels();

			auto texture = spriteFrame->getTexture();
			

			auto textureSize = Vec2(static_cast<float>(texture->getPixelsWide()), static_cast<float>(texture->getPixelsHigh()));
			auto overlayTextureSize = Vec2(static_cast<float>(overlayTexture->getPixelsWide()), static_cast<float>(overlayTexture->getPixelsHigh()));


			auto position = getAbsGridTile(GridIndex(x, y));
			fillCellQuad(&mBGCellQuads[bgCellIndex], position, Color4B::WHITE, textureRect, textureSize);
			fillCellQuad(&mBGGlowCellQuads[bgCellIndex], position, Color4B::WHITE, overlayTextureRect, overlayTextureSize);

			bgCellIndex++;
		}
	}
	auto texture = firstSpriteFrame->getTexture();
	auto glProgram = texture->getGLProgram();
	auto glProgramOverlay = overlayTexture->getGLProgram();
	auto programStateCache = GLProgramStateCache::getInstance();

	BlendFunc blend;

	// it is possible to have an untextured sprite
	if (!texture || !texture->hasPremultipliedAlpha()) { blend = BlendFunc::ALPHA_NON_PREMULTIPLIED; }
	else { blend = BlendFunc::ALPHA_PREMULTIPLIED; }

	Mat4 mat = Mat4::IDENTITY;
	mBGCellQuadCommand->init(0.0, texture->getName(), programStateCache->getGLProgramState(glProgram), blend, mBGCellQuads, bgCellIndex, mat, 0);

	// it is possible to have an untextured sprite
	if (!overlayTexture || !overlayTexture->hasPremultipliedAlpha()) { blend = BlendFunc::ALPHA_NON_PREMULTIPLIED; }
	else { blend = BlendFunc::ALPHA_PREMULTIPLIED; }


	mBGGlowCellQuadCommand->init(0.1f, overlayTexture->getName(), programStateCache->getGLProgramState(glProgramOverlay), blend, mBGGlowCellQuads, bgCellIndex, mat, 0);

	return true;
}

void Grid::glowEmptyCells(bool enable/* = true*/)
{
	auto spriteFrameOverlaySmall = viewGetSpriteFrame(mGridGraphicsConfig->overlay_small);
	auto overlayTexture = spriteFrameOverlaySmall->getTexture();
	auto overlayTextureSize = Vec2(static_cast<float>(overlayTexture->getPixelsWide()), static_cast<float>(overlayTexture->getPixelsHigh()));

	if (!enable) {
		mGlowEnabled = false;
	}
	else 
	{
		mGlowEnabled = true;

		//glprogramState->setUniformInt("")
		int cellQuadIndex = 0;
		for (int y = 0; y < mWidth; y++) {
			for (int x = 0; x < mHeight; x++) {
				uint8_t index = x * mWidth + y;
				if (mObstacleMap[index] != OBSTACLE_DEFAULT) {
					if (!mPlantMap[index]) {
						updateCellQuadTextureCoords(&mBGGlowCellQuads[cellQuadIndex], spriteFrameOverlaySmall->getRectInPixels(), overlayTextureSize);
					}
					else {
						updateCellQuadColor(&mBGGlowCellQuads[cellQuadIndex], Color4B(255, 255, 255, 0));
					}
					cellQuadIndex++;
				}
			}
		}
	}
}
void Grid::disableAllGlowCells()
{
	mGlowEnabled = false;
	/*
	for (int y = 0; y < mWidth; y++) {
		for (int x = 0; x < mHeight; x++) {
			uint8_t index = x * mWidth + y;
			if (mObstacleMap[index] != OBSTACLE_DEFAULT && mBGCellsMap[index]) {
				setDefaultShader(mBGCellsMap[index]);
			}
		}
	}
	*/
}

void Grid::glowAutoCells(const PlantType* type, const PlantTypesManager* plantTypesManager)
{
	glowEmptyCells(true);
	/*
	for (int y = 0; y < mWidth; y++) {
		for (int x = 0; x < mHeight; x++) {
			uint8_t index = x * mWidth + y;
			if (mObstacleMap[index] != OBSTACLE_DEFAULT && mBGCellsMap[index]) {
				Material* mat = nullptr;
				if (isIsometric()) { mat = mRessourcenManager->getMaterial("highlightGridCellIso"); }
				else { mat = mRessourcenManager->getMaterial("highlightGridCell"); }

				Technique* technique = nullptr;

				if (mPlantMap[index]) {
					auto neighborType = plantTypesManager->getNeighborType(type->getIndex(), mPlantMap[index]->getPlantType()->getIndex());
					technique = getTechniqueForNeighborType(neighborType, mat);
				}
				else {
					technique = mat->getTechniqueByName("emptyCell");
				}

				if (technique) {
					mBGCellsMap[index]->setGLProgramState(technique->getPassByIndex(0)->getGLProgramState());
				}
				else {
					setDefaultShader(mBGCellsMap[index]);
				}

				
			}
		}
	}
	*/
}


void Grid::glowNeighborCells(const PlantType* type, const PlantTypesManager* plantTypesManager, bool enable/* = true*/)
{
	/*
	for (int y = 0; y < mWidth; y++) {
		for (int x = 0; x < mHeight; x++) {
			uint8_t index = x * mWidth + y;
			if (mObstacleMap[index] != OBSTACLE_DEFAULT && mBGCellsMap[index] && mPlantMap[index]) {
				if (enable) {
					Material* mat = nullptr;
					if (isIsometric()) { mat = mRessourcenManager->getMaterial("highlightGridCellIso"); }
					else { mat = mRessourcenManager->getMaterial("highlightGridCell"); }
					
					auto neighborType = plantTypesManager->getNeighborType(type->getIndex(), mPlantMap[index]->getPlantType()->getIndex());
					Technique* technique = getTechniqueForNeighborType(neighborType, mat);

					if (technique) {
						mBGCellsMap[index]->setGLProgramState(technique->getPassByIndex(0)->getGLProgramState());
					}
					else {
						setDefaultShader(mBGCellsMap[index]);
					}

				}
				else {
					setDefaultShader(mBGCellsMap[index]);
				}
			}
		}
	}
	*/
}

cocos2d::Technique* Grid::getTechniqueForNeighborType(PlantTypeNeighborType type, cocos2d::Material* material)
{
	switch (type) {
	case NEIGHBOR_GOOD: return material->getTechniqueByName("positiveNeighbor"); break;
	case NEIGHBOR_BAD: return material->getTechniqueByName("negativeNeighbor"); break;
	case NEIGHBOR_REALLY_GOOD: return material->getTechniqueByName("reallyGoodNeighbor"); break;
	case NEIGHBOR_NEUTRAL: return material->getTechniqueByName("neutralNeighbor"); break;
	default: break;
	}
	return nullptr;
}


void Grid::glowCell(const GridIndex& index, const char* technique)
{
	//LOG_ERROR_VOID("[Grid::glowCell] not updatet");

	auto spriteFrameOverlay = viewGetSpriteFrame(mGridGraphicsConfig->overlay);
	auto overlayTexture = spriteFrameOverlay->getTexture();
	auto overlayTextureSize = Vec2(static_cast<float>(overlayTexture->getPixelsWide()), static_cast<float>(overlayTexture->getPixelsHigh()));

	int cellIndex = 0;

	for (int y = 0; y < mWidth; y++) {
		for (int x = 0; x < mHeight; x++) {
			uint8_t local_index = x * mWidth + y;
			if (mObstacleMap[local_index] != OBSTACLE_DEFAULT) {
				if (index.x == x && index.y == y) {
					updateCellQuadTextureCoords(&mBGGlowCellQuads[cellIndex], spriteFrameOverlay->getRectInPixels(), overlayTextureSize);
				}
				else {
					updateCellQuadColor(&mBGGlowCellQuads[cellIndex], Color4B(255, 0, 255, 0));
				}
				cellIndex++;
			}
		}
	}

	
	mGlowEnabled = true;
	
	/*


	if (!strcmp(technique, "default")) {
		setDefaultShader(mBGCellsMap[i]);
	}
	else {
		Material* mat = nullptr;
		if (isIsometric()) { mat = mRessourcenManager->getMaterial("highlightGridCellIso"); }
		else { mat = mRessourcenManager->getMaterial("highlightGridCell"); }
		mBGCellsMap[i]->setGLProgramState(mat->getTechniqueByName(technique)->getPassByIndex(0)->getGLProgramState());
	}
	*/
}


GridIndex Grid::getGridIndex(cocos2d::Vec2 localPosition) const
{
	// move by anchor
	auto anchor = getAnchorPoint();
	localPosition += Vec2(anchor.x * mBoundingBoxSize.x, anchor.y * mBoundingBoxSize.y);

	if(!isIsometric()) {
		localPosition.x = (localPosition.x / mBoundingBoxSize.x) * mWidth;
		localPosition.y = (localPosition.y / mBoundingBoxSize.y) * mHeight;
		
	} else {
		localPosition = isoLocalToFlatGrid(localPosition);
	}
	
	GridIndex index(
		static_cast<uint8_t>(floor(localPosition.x)),
		static_cast<uint8_t>(floor(localPosition.y))
	);
	assert(index.x < mWidth && index.y < mHeight);
	//ErrorLog::printf("GridIndex: %dx%d", index.x, index.y);
	return index;
}

cocos2d::Vec2 Grid::fromWorldToLocal(cocos2d::Vec2 worldCoords) const
{
	auto local = convertToNodeSpace(worldCoords);
	/*if (isIsometric()) {
		return isoToFlat(local);
	}*/
	return local;
	//return worldCoords - _position;
}

Vec2 Grid::isoLocalToFlatGrid(const Vec2& pointIso) const
{
	// origin : x = grid bounding box / 2
	//          y = 0

	auto transformed = Vec2(
		pointIso.x / (mBoundingBoxSize.x / static_cast<float>(mWidth) ),
		pointIso.y / (mBoundingBoxSize.y / static_cast<float>(mHeight) )
	);
	// - origin.x
	transformed.x -= static_cast<float>(mWidth) / 2.0f;

	return Vec2(
		(transformed.x + transformed.y),// * static_cast<float>(mWidth),
		(transformed.y - transformed.x)// * static_cast<float>(mHeight)
	);

}

Vec2 Grid::flatToIso(const Vec2& point) const
{
	
	//((static_cast<float>(x - y) + static_cast<float>(mWidth)) / 2.0f) * mBoundingBoxSize.x / static_cast<float>(mWidth),
	//((static_cast<float>(x + y)) / 2.0f) * mBoundingBoxSize.y / static_cast<float>(mHeight)


	return Vec2(
		((point.x - point.y + static_cast<float>(mWidth)) / 2.0f) * mBoundingBoxSize.x / static_cast<float>(mWidth),
		((point.x + point.y) / 2.0f) * mBoundingBoxSize.y / static_cast<float>(mHeight)
	);
}

cocos2d::Vec2 Grid::fromLocalToWorld(cocos2d::Vec2 localCoords) const
{
/*	if (isIsometric()) {
		localCoords = flatToIso(localCoords);
	}*/
	return convertToWorldSpace(localCoords);
	//return _position + localCoords;
}

bool Grid::isInsideGrid(const cocos2d::Vec2& worldCoords) const
{
	auto localCoords = fromWorldToLocal(worldCoords);
	auto anchor = getAnchorPoint();
	localCoords += Vec2(anchor.x * mBoundingBoxSize.x, anchor.y * mBoundingBoxSize.y);

	if (isIsometric()) {
		localCoords = isoLocalToFlatGrid(localCoords);
	}
	
	if (localCoords.x < 0.0f || localCoords.y < 0.0f) return false;
	
	if (isIsometric()) {
		// visible area
		if (localCoords.x > static_cast<float>(mWidth) || localCoords.y > static_cast<float>(mHeight))
			return false;
	} else {
		// outer bounding box
		if (localCoords.x > mBoundingBoxSize.x || localCoords.y > mBoundingBoxSize.y) 
			return false;
	}
	return true;
}

cocos2d::Vec2 Grid::getOriginPosition(PlantNode* viewNode)
{
	auto gridIndex = viewNode->getGridIndex();
	auto pos = cocos2d::Vec2(
		static_cast<float>(gridIndex.x) * (mBoundingBoxSize.x / static_cast<float>(mWidth)),
		static_cast<float>(gridIndex.y) * (mBoundingBoxSize.y / static_cast<float>(mHeight))
	);
	if (isIsometric()) {
		//ErrorLog::printf("pos: %f, %f\n", pos.x, pos.y);
		//pos.x += 0.5f * cellSize.x;
		//pos.y += 0.5f * cellSize.y;
		//pos = flatToIso(pos);
		// origin : x = grid bounding box / 2
		//          y = 0
		// pos.x = origin.x + (x-y)/2 * cellWidth
		// pos.y = (x+y)/2 * cellHeight
		pos = Vec2(
			((static_cast<float>(gridIndex.x - gridIndex.y) + static_cast<float>(mWidth)) / 2.0f - 0.5f) * mBoundingBoxSize.x / static_cast<float>(mWidth),
			((static_cast<float>(gridIndex.x + gridIndex.y)) / 2.0f) * mBoundingBoxSize.y / static_cast<float>(mHeight)
		);

	}
	// move by anchor
	auto anchor = getAnchorPoint();
	pos -= Vec2(anchor.x * mBoundingBoxSize.x, anchor.y * mBoundingBoxSize.y);


	return pos;
}

PlantNode* Grid::isPlantNodeAtPosition(cocos2d::Vec2 localPosition) const
{
	GridIndex i = getGridIndex(localPosition);
	size_t index = i.x * mWidth + i.y;
	//ErrorLog::printf("x: %d, y: %d\n", x, y);
	if (mPlantMap[index]) {
		return mPlantMap[index];
	}
	return nullptr;
}

PlantNode* Grid::getPlantNodeAtWorldPosition(cocos2d::Vec2 worldPosition) const
{
	auto localPosition = fromWorldToLocal(worldPosition);
	GridIndex i = getGridIndex(localPosition);
	size_t index = i.x * mWidth + i.y;
	//ErrorLog::printf("x: %d, y: %d\n", i.x, i.y);
	if (mPlantMap[index]) {
		return mPlantMap[index];
	}
	return nullptr;

}

PlantNode* Grid::getPlantNodeFromIndex(GridIndex index) const
{
	if (index.x >= mWidth || index.y >= mHeight) return nullptr;

	size_t i = index.x * mWidth + index.y;
	if (mPlantMap[i]) {
		return mPlantMap[i];
	}
	return nullptr;
}

bool Grid::isCellEmptyAndFree(uint8_t x, uint8_t y) const
{
	if (x >= mWidth || y >= mHeight) return false;

	uint8_t index = x * mWidth + y;
	if (mObstacleMap[index] == OBSTACLE_DEFAULT) {
		return false;
	}
	if (mPlantMap[index]) {
		return false;
	}
	return true;

}

// -------------------------------------------------------------------
// rendering
// -------------------------------------------------------------------

bool Grid::_setupRendering()
{
	glGenBuffers(1, &mIndexBuffer);
	auto player = Player::getInstance();
	if (player->isGrid3D()) {
		_setupRendering3D();
	}
	else if(player->isGridIso()) {
		_setupRenderingIso();
	}
	return true;
}

bool Grid::_setupRenderingIso()
{
	
	mMaterial = Material::createWithFilename("materials/grid.material");
	mMaterial->retain();
	auto programState = mMaterial->getTechniqueByName("white_grid")->getPassByIndex(0)->getGLProgramState();
	mUniformLocationGridCell = glGetUniformLocation(programState->getGLProgram()->getProgram(), "grid_cell_count");

	if (mUniformLocationGridCell != -1)
	{
		glUniform1f(mUniformLocationGridCell, mWidth);
		CHECK_GL_ERROR_DEBUG();
	}

	Vec3 vertices[] = {
		{ -0.5f,  0.5f,  0.0f }, // 0
		{ -0.5f, -0.5f,  0.0f }, // 1
		{ 0.5f, -0.5f,  0.0f }, // 2
		{ 0.5f,  0.5f,  0.0f }, // 3

		{ -0.5f, -0.5f, -0.125f }, // 4
		{ 0.5f, -0.5f, -0.125f }, // 5

		{ 0.5f,  0.5f, -0.125f }  // 6
	};

	mVertexBuffer = VertexBuffer::create(sizeof(Vec3), 7);
	mVertexBuffer->retain();
	mVertexBuffer->updateVertices(vertices, 7, 0);

	uint16_t indices[] = { 
		0, 1, 3, 1, 2, 3,
		4, 5, 2, 4, 2, 1,
		6, 3, 2, 2, 5, 6 
	};
	// fill index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 18 * sizeof(uint16_t), &indices[0], GL_STATIC_DRAW);

//	mRenderCommand.func = std::bind(&Grid::renderCommand, this);

	return true;
}

bool Grid::_setupRendering3D()
{
	

	mMaterial = Material::createWithFilename("materials/grid.material");
	mMaterial->retain();
	auto programState = mMaterial->getTechniqueByName("white_grid")->getPassByIndex(0)->getGLProgramState();
	mUniformLocationGridCell = glGetUniformLocation(programState->getGLProgram()->getProgram(), "grid_cell_count");

	if (mUniformLocationGridCell != -1)
	{
		glUniform1f(mUniformLocationGridCell, mWidth);
		CHECK_GL_ERROR_DEBUG();
	}

	Vec3 vertices[] = {
		{ -0.5f,  0.5f,  0.0f }, // 0
		{ -0.5f, -0.5f,  0.0f }, // 1
		{ 0.5f, -0.5f,  0.0f }, // 2
		{ 0.5f,  0.5f,  0.0f }, // 3

		{ -0.5f, -0.5f, -0.125f }, // 4
		{ 0.5f, -0.5f, -0.125f }, // 5

		{ 0.5f,  0.5f, -0.125f }  // 6
	};

	mVertexBuffer = VertexBuffer::create(sizeof(Vec3), 7);
	mVertexBuffer->retain();
	mVertexBuffer->updateVertices(vertices, 7, 0);

	uint16_t indices[] = { 0, 1, 3, 1, 2, 3,
		4, 5, 2, 4, 2, 1,
		6, 3, 2, 2, 5, 6 };
	// fill index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 18 * sizeof(uint16_t), &indices[0], GL_STATIC_DRAW);

	//mRenderCommand.func = std::bind(&Grid::renderCommand, this);

	return true;
}

void Grid::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
//	if (GRID_MAIN == mType) return;
	renderer->addCommand(mBGCellQuadCommand);
	if (mGlowEnabled) {
		renderer->addCommand(mBGGlowCellQuadCommand);
	}
}

