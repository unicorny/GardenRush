#include "nodes/Grid.h"
#include "model/ViewData.h"
#include "model/LevelData.h"
#include "model/Points.h"
#include "ErrorLog.h"
#include "PlantTypesManager.h"
#include "controller/RessourcenManager.h"
#include "MainGameScene.h"
#include "GridOverlay.h"
#include "Config.h"

#include "model/Player.h"


using namespace cocos2d;


Grid::Grid(uint8_t width, uint8_t height, GridType type)
	: mWidth(width), mHeight(height), mType(type), mBoundingBoxSize(Vec2::ZERO), mObstacleMap(nullptr), mObstaclesCount(0), mPlantMap(nullptr), mIsIsometric(false),
	mGridGraphicsConfig(nullptr), mBGCellQuads(nullptr), mBGCellQuadCommand(nullptr), 
	mGridOverlay(nullptr)
{
	mObstacleMap = (uint8_t*)(malloc(width * height * sizeof(uint8_t)));
	mPlantMap = (PlantNode**)(malloc(width * height * sizeof(PlantNode*)));
	//mBGCellsMap = (cocos2d::Sprite**)(malloc(width * height * sizeof(cocos2d::Sprite*)));
	//mBGCellQuads = new V3F_C4B_T2F_Quad[width * height];
	mBGCellQuadCommand = new QuadCommand;

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
	DR_SAVE_DELETE(mBGCellQuadCommand);
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


bool Grid::_setup(const cocos2d::Vec2& leftTopPosition, cocos2d::Node* parentNode)
{
	//setAnchorPoint(Vec2::ZERO);
	setAnchorPoint(Vec2(0.5f, 0.5f));
	setPosition(leftTopPosition + mBoundingBoxSize * 0.5f);
	fillGroundTilesIntoTextureAtlas();
	
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

cocos2d::Vec2 Grid::getLocalPositionForGridIndex(GridIndex index)
{
	auto cellSize = getCellSize();
	auto pos = Vec2(
		static_cast<float>(index.x) * cellSize.x,
		static_cast<float>(index.y) * cellSize.y
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
			((static_cast<float>(index.x - index.y) + static_cast<float>(mWidth)) / 2.0f - 0.5f) * mBoundingBoxSize.x / static_cast<float>(mWidth),
			((static_cast<float>(index.x + index.y)) / 2.0f) * mBoundingBoxSize.y / static_cast<float>(mHeight)
		);

		//ErrorLog::printf("flat pos: (%d)%f, (%d)%f\n", x, pos.x, y, pos.y);
		//sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		//sprite->setScale(0.25f);
	}
	// move by anchor
	auto anchor = getAnchorPoint();
	pos -= Vec2(anchor.x * mBoundingBoxSize.x, anchor.y * mBoundingBoxSize.y);

	return pos;
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

bool Grid::fillCellQuad(cocos2d::V3F_C4B_T2F_Quad* quad, const cocos2d::Vec2 corners[4], const cocos2d::Color4B& color, const cocos2d::Rect& textureRect, const cocos2d::Vec2& textureSize)
{
	quad->tl.vertices = Vec3(corners[3].x, corners[3].y, 0.0f);
	quad->tl.colors = Color4B(255, 255, 255, 255);
	quad->tl.texCoords = Tex2F(textureRect.getMinX() / textureSize.x, textureRect.getMaxY() / textureSize.y);

	quad->bl.vertices = Vec3(corners[0].x, corners[0].y, 0.0f);
	quad->bl.colors = Color4B(255, 255, 255, 255);
	quad->bl.texCoords = Tex2F(textureRect.getMinX() / textureSize.x, textureRect.getMinY() / textureSize.y);

	quad->tr.vertices = Vec3(corners[2].x, corners[2].y, 0.0f);
	quad->tr.colors = Color4B(255, 255, 255, 255);
	quad->tr.texCoords = Tex2F(textureRect.getMaxX() / textureSize.x, textureRect.getMaxY() / textureSize.y);

	quad->br.vertices = Vec3(corners[1].x, corners[1].y, 0.0f);
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

bool Grid::setGridOverlay(GridOverlay* gridOverlay)
{
	mGridOverlay = gridOverlay;
	GridNodeType nodeType = GRID_NODE_2D;
	if (isIsometric()) { nodeType = GRID_NODE_ISO; }
	Vec2 positionOffset = getPosition();
	auto texture = mGridGraphicsConfig->groundTiles[0]->getTexture();
	mGridOverlay->addSubgrid(mType, nodeType, GridIndex(mWidth, mHeight), positionOffset, texture);
	
	for (int y = 0; y < mHeight; y++) {
		for (int x = 0; x < mWidth; x++) {
			uint8_t index = x * mWidth + y;
			Color4B color = Color4B::WHITE;
			if (mObstacleMap[index] == OBSTACLE_DEFAULT) {
				color.a = 0;
			}
			else {
				color.a = 192;
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
	//assert(mBGCellQuads);

	if (!mBGCellQuads) {
		if (isIsometric()) {
			mBGCellQuads = new V3F_C4B_T2F_Quad[mWidth * mHeight + 8];
		}
		else {
			mBGCellQuads = new V3F_C4B_T2F_Quad[mWidth * mHeight];
		}
	}

	auto tiles = mGridGraphicsConfig->groundTiles;
	assert(tiles.size() > 0);

	SpriteFrame* firstSpriteFrame = nullptr;
	
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
			auto texture = spriteFrame->getTexture();
			auto textureSize = Vec2(static_cast<float>(texture->getPixelsWide()), static_cast<float>(texture->getPixelsHigh()));			

			auto position = getAbsGridTile(GridIndex(x, y));
			fillCellQuad(&mBGCellQuads[bgCellIndex], position, Color4B::WHITE, textureRect, textureSize);

			bgCellIndex++;
		}
	}

	// border
	if (isIsometric()) {

		auto leftTile = getAbsGridTile(GridIndex(0, mHeight - 1));
		auto centerTile = getAbsGridTile(GridIndex(0, 0));
		auto rightTile = getAbsGridTile(GridIndex(mWidth - 1, 0));
		auto cellSize = getCellSize();

		Vec2 leftPosIso = Vec2(leftTile.origin.x, leftTile.origin.y + cellSize.y * 0.5f);
		Vec2 centerPosIso = Vec2(centerTile.origin.x + cellSize.x * 0.5f, centerTile.origin.y);
		Vec2 rightPosIso = Vec2(rightTile.origin.x + cellSize.x, rightTile.origin.y + cellSize.y * 0.5f);

		// left 
		addQuadsForBorder(leftPosIso, centerPosIso, bgCellIndex, 4, viewGetSpriteFrame(mGridGraphicsConfig->leftSide));
		// right 
		addQuadsForBorder(centerPosIso, rightPosIso, bgCellIndex, 4, viewGetSpriteFrame(mGridGraphicsConfig->rightSide));
		
	}
	auto texture = firstSpriteFrame->getTexture();
	auto glProgram = texture->getGLProgram();
	auto programStateCache = GLProgramStateCache::getInstance();

	BlendFunc blend;

	// it is possible to have an untextured sprite
	if (!texture || !texture->hasPremultipliedAlpha()) { blend = BlendFunc::ALPHA_NON_PREMULTIPLIED; }
	else { blend = BlendFunc::ALPHA_PREMULTIPLIED; }

	Mat4 mat = Mat4::IDENTITY;
	mBGCellQuadCommand->init(-0.1f, texture->getName(), programStateCache->getGLProgramState(glProgram), blend, mBGCellQuads, bgCellIndex, mat, 0);

	return true;
}

bool Grid::addQuadsForBorder(cocos2d::Vec2 leftStartPos, cocos2d::Vec2 rightStartPos, int& cellQuadIndex, uint8_t quadCount, cocos2d::SpriteFrame* spriteFrame)
{
	auto textureRect = spriteFrame->getRectInPixels();
	auto texture = spriteFrame->getTexture();
	auto textureSize = Vec2(static_cast<float>(texture->getPixelsWide()), static_cast<float>(texture->getPixelsHigh()));

	// corners:  
	//		0 = bl
	//		1 = br
	//		2 = tr
	//		3 = tl
	Vec2 corners[4];

	auto cellSize = getCellSize();

	// side height percent cellSize
	const float sideHeight = 0.8f;

	corners[3] = Vec2(leftStartPos.x, leftStartPos.y - cellSize.y * sideHeight); // bottom origin A
	corners[2] = Vec2(rightStartPos.x, rightStartPos.y - cellSize.y * sideHeight); // bottom origin B

	corners[1] = rightStartPos;
	corners[0] = leftStartPos;

	Vec2 bottomVector = corners[2] - corners[3];
	Vec2 localCorners[4];

	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			localCorners[3] = corners[3] + (i * (bottomVector / 4.0f));
			localCorners[0] = Vec2(localCorners[3].x, localCorners[3].y + cellSize.y * sideHeight);
		}
		else {
			localCorners[3] = Vec2(localCorners[2].x - 1.0f, localCorners[2].y);
			localCorners[0] = Vec2(localCorners[1].x - 1.0f, localCorners[1].y);
		}

		localCorners[2] = corners[3] + ((i + 1.0f) * (bottomVector / 4.0f));
		localCorners[1] = Vec2(localCorners[2].x, localCorners[2].y + cellSize.y * sideHeight);

		//roundCorners(localCorners);

		fillCellQuad(&mBGCellQuads[cellQuadIndex], localCorners, Color4B::WHITE, textureRect, textureSize);
		cellQuadIndex++;
		//break;
	}
	return true;
}



void Grid::glowEmptyCells(bool enable/* = true*/)
{
	if (mGridOverlay) {
		mGridOverlay->setGlow(mType, enable);
		if (enable) {
			for (int y = 0; y < mWidth; y++) {
				for (int x = 0; x < mHeight; x++) {
					uint8_t index = x * mWidth + y;
					if (mObstacleMap[index] != OBSTACLE_DEFAULT) {
						Color4B color = config_getColorForCellGlow(CELL_GLOW_EMPTY);
						if (mPlantMap[index]) {
							color.a = 0;
						}
						mGridOverlay->updateCellQuadColor(mType, GridIndex(x, y), color);
					}
				}
			}
		}
	}
	
}
void Grid::disableAllGlowCells()
{
	if (mGridOverlay) {
		mGridOverlay->setGlow(mType, false);
		Color4B color = config_getColorForCellGlow(CELL_GLOW_NONE);

		for (int y = 0; y < mWidth; y++) {
			for (int x = 0; x < mHeight; x++) {
				uint8_t index = x * mWidth + y;
				mGridOverlay->updateCellQuadColor(mType, GridIndex(x, y), color);
			}
		}
	}

}

void Grid::glowAutoCells(const PlantType* type, const PlantTypesManager* plantTypesManager)
{
	//glowEmptyCells(true);
	assert(mGridOverlay);
	
	for (int y = 0; y < mWidth; y++) {
		for (int x = 0; x < mHeight; x++) {
			uint8_t index = x * mWidth + y;
			if (mObstacleMap[index] != OBSTACLE_DEFAULT) {
				
				GridCellGlowType glowType = CELL_GLOW_EMPTY;
				if (mPlantMap[index]) {
					auto neighborType = plantTypesManager->getNeighborType(type->getIndex(), mPlantMap[index]->getPlantType()->getIndex());
					//technique = getTechniqueForNeighborType(neighborType, mat);
					glowType = getGlowTypeForNeighborType(neighborType);
				}
				mGridOverlay->updateCellQuadColor(mType, GridIndex(x, y), config_getColorForCellGlow(glowType));
			}
		}
	}
	mGridOverlay->setGlow(mType, true);
	//*/
}


void Grid::glowNeighborCells(const PlantType* type, const PlantTypesManager* plantTypesManager, bool enable/* = true*/)
{
	assert(mGridOverlay);

	for (int y = 0; y < mWidth; y++) {
		for (int x = 0; x < mHeight; x++) {
			uint8_t index = x * mWidth + y;
			if (mObstacleMap[index] != OBSTACLE_DEFAULT) {

				if (mPlantMap[index]) {
					auto neighborType = plantTypesManager->getNeighborType(type->getIndex(), mPlantMap[index]->getPlantType()->getIndex());
					//technique = getTechniqueForNeighborType(neighborType, mat);
					auto glowType = getGlowTypeForNeighborType(neighborType);
					mGridOverlay->updateCellQuadColor(mType, GridIndex(x, y), config_getColorForCellGlow(glowType));
				}
				
			}
		}
	}
	mGridOverlay->setGlow(mType, true);
}

GridCellGlowType Grid::getGlowTypeForNeighborType(PlantTypeNeighborType type)
{
	switch (type) {
	case NEIGHBOR_GOOD: return CELL_GLOW_GOOD_NEIGHBOR;
	case NEIGHBOR_BAD: return CELL_GLOW_BAD_NEIGHBOR;
	case NEIGHBOR_REALLY_GOOD: return CELL_GLOW_GOOD_NEIGHBOR;
	case NEIGHBOR_NEUTRAL: return CELL_GLOW_NEUTRAL_NEIGHBOR;
	default: return CELL_GLOW_ERROR;
	}

}

void Grid::glowCell(const GridIndex& index, GridCellGlowType glowType)
{
	if (mGridOverlay) {
		Color4B color = config_getColorForCellGlow(glowType);
	
		for (uint8_t y = 0; y < mHeight; y++) {
			for (uint8_t x = 0; x < mWidth; x++) {
				uint8_t local_index = x * mWidth + y;
				if (mObstacleMap[local_index] != OBSTACLE_DEFAULT) {
					if (index.x == x && index.y == y) {
						mGridOverlay->updateCellQuadColor(mType, index, color);
					}
					else {
						//mGridOverlay->updateCellQuadColor(mType, index, Color4B(255,255,255,0));
					}
				}
			}
		}
		mGridOverlay->setGlow(mType, true);
	}
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


void Grid::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
//	if (GRID_MAIN == mType) return;
	renderer->addCommand(mBGCellQuadCommand);
}

