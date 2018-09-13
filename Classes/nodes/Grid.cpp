#include "nodes/Grid.h"
#include "model/ViewData.h"
#include "ErrorLog.h"

using namespace cocos2d;

Grid::Grid(uint8_t width, uint8_t height)
	: mWidth(width), mHeight(height), mPixelSize(Vec2::ZERO), mObstacleMap(nullptr), mPlantMap(nullptr)
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

Grid * Grid::create(uint8_t width, uint8_t height)
{
	Grid * ret = new (std::nothrow) Grid(width, height);
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
	return addCellSprite(viewData, x, y, 10);
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
		size.width / ((float)mPixelSize.x / (float)mWidth),
		size.height / ((float)mPixelSize.y / (float)mHeight)
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

void Grid::listenOnCellTouch(std::function<void(PlantNode*)> callMe)
{
	mCellTouchCallback = callMe;
	//auto touchListener = EventListenerTouchOneByOne::create();
//	touchListener->onTouchBegan = CC_CALLBACK_2(Grid::onTouchBegan, this);
//	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool Grid::onTouchBegan(Vec2 localPosition)
{	
	uint8_t x = static_cast<uint8_t>(floor(localPosition.x / mPixelSize.x * mWidth));
	uint8_t y = static_cast<uint8_t>(floor(localPosition.y / mPixelSize.y * mHeight));
	//assert(x < mWidth && y < mHeight);
	ErrorLog::printf("x: %d, y: %d\n", x, y);

	return true;
}