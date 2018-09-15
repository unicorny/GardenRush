#include "DragSeed.h"

namespace level_state {
	bool DragSeed::initState()
	{
		return true;
	}
	bool DragSeed::onEnterState()
	{
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_END | ENABLED_TOUCH_CANCELLED | ENABLED_TOUCH_MOVE);
		return true;
	}
	bool DragSeed::onExitState()
	{
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_NONE);
		return true;
	}

	void DragSeed::onTouchEnded(MainGridType type, uint8_t x, uint8_t y)
	{

	}
	void DragSeed::onTouchCancelled()
	{

	}
	void DragSeed::onTouchMoved(float deltaX, float deltaY)
	{

	}
}