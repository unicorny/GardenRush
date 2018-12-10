#include "iLevelState.h"

namespace level_state {

	void iLevelState::setGridCell(GridCell gridCell)
	{
		if (!mData) {
			mData = new LevelStateDataGridCell(gridCell);
		}
		else if(mData->isType(LEVEL_STATE_DATA_TARGET_GRID_CELL)) {
			auto data = static_cast<LevelStateDataGridCell*>(mData);
			data->mGridCell = gridCell;
		}
		else {
			DR_SAVE_DELETE(mData);
			mData = new LevelStateDataGridCell(gridCell);
		}
	}

	GridCell* iLevelState::getGridCell()
	{
		if (!mData) return nullptr;
		if (!mData->isType(LEVEL_STATE_DATA_TARGET_GRID_CELL)) return nullptr;
		auto data = static_cast<LevelStateDataGridCell*>(mData);
		return &data->mGridCell;
	}
}