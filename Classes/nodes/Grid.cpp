#include "nodes/Grid.h"
#include "model/ViewData.h"
#include "ErrorLog.h"

using namespace cocos2d;

Grid::Grid(uint8_t width, uint8_t height)
	: mWidth(width), mHeight(height), mPixelSize(Vec2::ZERO), mObstacleMap(NULL)
{
	mObstacleMap = (uint8_t*)(malloc(width*height));
	memset(mObstacleMap, 0, width*height);
}

Grid::~Grid()
{
	free(mObstacleMap);
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

void Grid::setPixelSize(Vec2 pixelSize)
{
	mPixelSize = pixelSize;

}

void Grid::reset()
{
	memset(mObstacleMap, 0, mWidth*mHeight);
}

bool Grid::addBgGridCell(const IViewData* viewData, bool obstacle, uint8_t x, uint8_t y)
{
	assert(x < mWidth && y < mHeight);
	assert(mPixelSize.x > 0 && mPixelSize.y > 0);
	if (obstacle) {
		mObstacleMap[x * mWidth + y] = OBSTACLE_DEFAULT;
	}
	// default anchor lower left
	Sprite* sprite = viewData->createSprite();
	if (!sprite) {
		LOG_ERROR("creating sprite", false);
	}
	sprite->setPosition(Vec2(x * (mPixelSize.x / mWidth), y * (mPixelSize.y / mHeight)));
	Size size = sprite->getContentSize();
	sprite->setScale(
		size.width / ((float)mPixelSize.x / (float)mWidth),
		size.height / ((float)mPixelSize.y / (float)mHeight)
	);
	this->addChild(sprite);

	return true;
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