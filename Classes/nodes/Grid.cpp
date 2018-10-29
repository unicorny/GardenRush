#include "nodes/Grid.h"
#include "model/ViewData.h"
#include "model/LevelData.h"
#include "model/Points.h"
#include "ErrorLog.h"
#include "PlantTypesManager.h"

using namespace cocos2d;

cocos2d::GLProgram* Grid::mst_highlightCellShader = nullptr;
cocos2d::GLProgram* Grid::mst_highlightCellIsoShader = nullptr;
cocos2d::GLProgramState* Grid::mst_highlightCellEmptyShaderState = nullptr;
cocos2d::GLProgramState* Grid::mst_highlightCellEmptyIsoShaderState = nullptr;

Grid::Grid(uint8_t width, uint8_t height, GridType type)
	: mWidth(width), mHeight(height), mType(type), mBoundingBoxSize(Vec2::ZERO), mObstacleMap(nullptr), mPlantMap(nullptr), mIsIsometric(false)
{
	mObstacleMap = (uint8_t*)(malloc(width * height * sizeof(uint8_t)));
	mPlantMap = (PlantNode**)(malloc(width * height * sizeof(PlantNode*)));
	mBGCellsMap = (cocos2d::Sprite**)(malloc(width * height * sizeof(cocos2d::Sprite*)));
	memset(mObstacleMap, 0, width*height * sizeof(uint8_t));
	memset(mPlantMap, 0, width* height * sizeof(PlantNode*));
	memset(mBGCellsMap, 0, width* height * sizeof(cocos2d::Sprite*));
}

Grid::~Grid()
{
	free(mObstacleMap);
	free(mBGCellsMap);
	free(mPlantMap);
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

bool Grid::setup(float edge_size_pixels, cocos2d::Vec2 pos, IViewData* bgTile, cocos2d::Node* parentNode)
{
	mBoundingBoxSize = Vec2(edge_size_pixels, edge_size_pixels);
	std::vector<IViewData*> viewData;
	viewData.push_back(bgTile);

	return _setup(pos, viewData, parentNode);
}


bool Grid::setup(const Vec2& edgeSizes, const Vec2& leftTopPosition, const std::vector<IViewData*>& tiles, Node* parentNode)
{
	setIsometric();
	mBoundingBoxSize = edgeSizes;
	return _setup(leftTopPosition, tiles, parentNode);
}
bool Grid::setup(float edgeSizes, const Vec2& leftTopPosition, const std::vector<IViewData*>& tiles, Node* parentNode)
{
	mBoundingBoxSize = Vec2(edgeSizes, edgeSizes);
	return _setup(leftTopPosition, tiles, parentNode);
}

bool Grid::_setup(const cocos2d::Vec2& leftTopPosition, const std::vector<IViewData*>& tiles, cocos2d::Node* parentNode)
{
	//setAnchorPoint(Vec2::ZERO);
	setAnchorPoint(Vec2(0.5f, 0.5f));
	setPosition(leftTopPosition + mBoundingBoxSize * 0.5f);
	bool ret = false;
	if (tiles.size() > 1) {
		ret = fillBgGridCellsRandom(tiles);
	}
	else {
		ret = fillBgGridCells(tiles.front());
	}
	if (!ret) {
		LOG_ERROR("fill bg grid", false);
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
	memset(mBGCellsMap, 0, mWidth* mHeight * sizeof(Sprite*));
}

bool Grid::addBgGridCell(const IViewData* viewData, bool obstacle, uint8_t x, uint8_t y)
{
	assert(x < mWidth && y < mHeight);
	assert(mBoundingBoxSize.x > 0 && mBoundingBoxSize.y > 0);
	auto index = x * mWidth + y;
	if (obstacle) {
		mObstacleMap[index] = OBSTACLE_DEFAULT;
	}
	auto cellSprite = viewData->createSprite();
	mBGCellsMap[index] = cellSprite;
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

void Grid::enableGlowCell(uint8_t x, uint8_t y, cocos2d::Color3B color)
{

}

void Grid::disableGlowCell(uint8_t x, uint8_t y)
{

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
				if (growPhasis >= levelData->getMaxGrowthPhasis()) {
					if (levelData->isAutoHarvesting()) {
						auto cellSize = getCellSize();
						auto pos = fromLocalToWorld(Vec2((static_cast<float>(x)+0.5f) * cellSize.x, y * cellSize.y));
						points->addPoints(100 * neighborPlant->getPointsMultiplicator() * neighborPlant->getDiversityBonus(), pos);
						removeGridCell(x, y);
						neighborPlant = NULL;
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

PlantNode* Grid::removeGridCell(uint8_t x, uint8_t y)
{
	assert(x < mWidth && y < mHeight);
	PlantNode* plantNode = mPlantMap[x * mWidth + y];
	this->removeChild(plantNode, false);
	mPlantMap[x * mWidth + y] = NULL;
	return plantNode;
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

void Grid::glowEmpytCells(bool enable/* = true*/)
{
	if (isIsometric()) {
		if (!mst_highlightCellEmptyIsoShaderState) {
			mst_highlightCellEmptyIsoShaderState = GLProgramState::getOrCreateWithGLProgram(mst_highlightCellIsoShader);
			mst_highlightCellEmptyIsoShaderState->setUniformVec3("border_color", Vec3(0.0f, 0.0f, 1.0f));
			mst_highlightCellEmptyIsoShaderState->setUniformVec3("highlight_color", Vec3(0.0f, 1.0f, 1.0f));
		}
	}
	else {
		if (!mst_highlightCellEmptyShaderState) {
			mst_highlightCellEmptyShaderState = GLProgramState::getOrCreateWithGLProgram(mst_highlightCellShader);
			mst_highlightCellEmptyShaderState->setUniformVec3("border_color", Vec3(0.0f, 0.0f, 1.0f));
			mst_highlightCellEmptyShaderState->setUniformVec3("highlight_color", Vec3(1.0f, 1.0f, 0.0f));
		}
	}
	
	//glprogramState->setUniformInt("")
	for (int y = 0; y < mWidth; y++) {
		for (int x = 0; x < mHeight; x++) {
			uint8_t index = x * mWidth + y;
			if (mObstacleMap[index] != OBSTACLE_DEFAULT && mBGCellsMap[index]) {
				if (enable && !mPlantMap[index]) {
					//mBGCellsMap[index]->setGLProgram(mst_highlightCellShader);
					//mBGCellsMap[index]->setGLProgramState(glprogramState);
					if (isIsometric()) mBGCellsMap[index]->setGLProgramState(mst_highlightCellEmptyIsoShaderState);
					else mBGCellsMap[index]->setGLProgramState(mst_highlightCellEmptyShaderState);
				}
				else {
					mBGCellsMap[index]->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
					
				}
				//mBGCellsMap[index]->getGLProgram()->use();
			}
		}
	}
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
	// move by anchor
	auto anchor = getAnchorPoint();
	pos -= Vec2(anchor.x * mBoundingBoxSize.x, anchor.y * mBoundingBoxSize.y);

	// no iso 
	assert(!isIsometric());

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

bool Grid::isCellEmptyAndFree(uint8_t x, uint8_t y) const
{
	uint8_t index = x * mWidth + y;
	if (mObstacleMap[index] == OBSTACLE_DEFAULT) {
		return false;
	}
	if (mPlantMap[index]) {
		return false;
	}
	return true;

}
