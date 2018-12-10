#include "DisplayInfo.h"

namespace level_state {

	bool DisplayInfo::initState()
	{
		return true;
	}
	bool DisplayInfo::onEnterState()
	{
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_END | ENABLED_TOUCH_CANCELLED);
		mMainGameScene->transitTo("PlayerChooseCell");
		return true;
	}
	bool DisplayInfo::onExitState()
	{
		mMainGameScene->setEnabledTouchType(ENABLED_TOUCH_NONE);
		return true;
	}

	void DisplayInfo::onCancelState()
	{
		
	}

	void DisplayInfo::onTouchEnded(GridType type, uint8_t x, uint8_t y)
	{
		mMainGameScene->transitTo("PlayerChooseCell");
	}
	void DisplayInfo::onTouchCancelled()
	{
		mMainGameScene->transitTo("PlayerChooseCell");
	}
}