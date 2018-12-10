#include "HarvestPlant.h"

namespace level_state {

	bool HarvestPlant::initState()
	{
		return true;
	}

	bool HarvestPlant::onEnterState()
	{
		// adding harvesting and points animation
		animationEnd();
		return true;
	}
	bool HarvestPlant::onExitState()
	{
		return true;
	}
	void HarvestPlant::onCancelState()
	{
	}

	void HarvestPlant::animationEnd()
	{
		mMainGameScene->transitTo("PlayerChooseCell");
	}
}