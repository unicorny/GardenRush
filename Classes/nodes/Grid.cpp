#include "nodes/Grid.h"
#include "model/ViewData.h"
#include "model/LevelData.h"
#include "model/Points.h"
#include "ErrorLog.h"
#include "PlantTypesManager.h"

using namespace cocos2d;

Grid::Grid(uint8_t width, uint8_t height, GridType type)
	: mWidth(width), mHeight(height), mType(type), mPixelSize(Vec2::ZERO), mObstacleMap(nullptr), mPlantMap(nullptr), mIsIsometric(false)
{
	mObstacleMap = (uint8_t*)(malloc(width * height * sizeof(uint8_t)));
	mPlantMap = (PlantNode**)(malloc(width * height * sizeof(PlantNode*)));
	memset(mObstacleMap, 0, width*height * sizeof(uint8_t));
	memset(mPlantMap, 0, width* height * sizeof(PlantNode*));
}

Grid::~Grid()
{
	free(mObstacleMap);
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
	setAnchorPoint(Vec2::ZERO);
	mPixelSize = Vec2(edge_size_pixels, edge_size_pixels);
	setPosition(pos); 
	bool ret = fillBgGridCells(bgTile);
	if (!ret) {
		LOG_ERROR("fill bg grid", false);
	}
	parentNode->addChild(this, 1);

	return true;
}
void Grid::setPixelSize(Vec2 pixelSize)
{
	mPixelSize = pixelSize;

}

void Grid::reset()
{
	memset(mObstacleMap, 0, mWidth*mHeight * sizeof(uint8_t));
	memset(mPlantMap, 0, mWidth* mHeight * sizeof(PlantNode*));
}

bool Grid::addBgGridCell(const IViewData* viewData, bool obstacle, uint8_t x, uint8_t y)
{
	assert(x < mWidth && y < mHeight);
	assert(mPixelSize.x > 0 && mPixelSize.y > 0);
	if (obstacle) {
		mObstacleMap[x * mWidth + y] = OBSTACLE_DEFAULT;
	}
	return addCellSprite(viewData->createSprite(), x, y, 0);
}

bool Grid::addGridCell(PlantNode* viewData, uint8_t x, uint8_t y)
{
	assert(x < mWidth && y < mHeight);
	assert(mPixelSize.x > 0 && mPixelSize.y > 0);
	if (OBSTACLE_DEFAULT == mObstacleMap[x * mWidth + y]) {
		return false;
	}
	mPlantMap[x * mWidth + y] = viewData;
	bool result = addCellSprite(viewData, x, y, 10);
	if (result) {
		viewData->setGridIndex(GridIndex(x, y));
		viewData->setParentGrid(this);
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
	sprite->setPosition(Vec2(x * (mPixelSize.x / mWidth), y * (mPixelSize.y / mHeight)));
	Size size = sprite->getContentSize();
	sprite->setAnchorPoint(Vec2::ZERO);
	sprite->setScale(
		(static_cast<float>(mPixelSize.x) / static_cast<float>(mWidth)) / size.width,
		(static_cast<float>(mPixelSize.y) / static_cast<float>(mHeight)) / size.height
	);
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
			this->removeChild(mPlantMap[i], true);
			mPlantMap[i] = NULL;
		}
	}
}

bool Grid::fillBgGridCells(const IViewData* viewData)
{
	assert(mPixelSize.x > 0 && mPixelSize.y > 0);
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

GridIndex Grid::getGridIndex(cocos2d::Vec2 localPosition) const
{
	GridIndex index(
		static_cast<uint8_t>(floor(localPosition.x / mPixelSize.x * mWidth)),
		static_cast<uint8_t>(floor(localPosition.y / mPixelSize.y * mHeight))
	);
	assert(index.x < mWidth && index.y < mHeight);
	return index;
}

cocos2d::Vec2 Grid::fromWorldToLocal(cocos2d::Vec2 worldCoords) const
{
	return convertToNodeSpace(worldCoords);
	//return worldCoords - _position;
}

cocos2d::Vec2 Grid::fromLocalToWorld(cocos2d::Vec2 localCoords) const
{
	return convertToWorldSpace(localCoords);
	//return _position + localCoords;
}

bool Grid::isInsideGrid(const cocos2d::Vec2& worldCoords) const
{
	auto localCoords = fromWorldToLocal(worldCoords);
	if (localCoords.x < 0.0f || localCoords.y < 0.0f) return false;
	if (localCoords.x > mPixelSize.x || localCoords.y > mPixelSize.y) return false;
	return true;
}

cocos2d::Vec2 Grid::getOriginPosition(PlantNode* viewNode)
{
	auto gridIndex = viewNode->getGridIndex();
	auto pos = cocos2d::Vec2(
		static_cast<float>(gridIndex.x) * (mPixelSize.x / static_cast<float>(mWidth)),
		static_cast<float>(gridIndex.y) * (mPixelSize.y / static_cast<float>(mHeight))
	);
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
