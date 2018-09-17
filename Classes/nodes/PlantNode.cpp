#include "nodes/PlantNode.h"


PlantNode::PlantNode(DHASH plantHash)
	: mPlantHash(plantHash), mParentGrid(nullptr)
{

}

PlantNode::~PlantNode()
{

}

bool PlantNode::removeFromGrid()
{
	if (mParentGrid) {
		if (getReferenceCount() == 1) {
			retain();
		}
		mParentGrid->removeGridCell(mGridIndex.x, mGridIndex.y);
		return true;
	}
	return false;
}
