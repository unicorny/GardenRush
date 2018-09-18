#include "nodes/PlantNode.h"
#include "model/Points.h"
#include "model/PlantType.h"

PlantNode::PlantNode(PlantType* plantType)
	: mPlantType(plantType), mParentGrid(nullptr), mPointsMultiplicator(1.0f), mGrowPhasis(PLANT_PHASIS_SEED)
{
	mCountedNeighborForDiversity.addByHash(plantType->getNameHash(), NULL);
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

bool PlantNode::countNewNeighbor(DHASH neigborHash, PlantTypeNeighborType neighborType, bool edge /*= true*/)
{
	multPoints(Points::neighborBonus(neighborType, edge));

	// diversity bonus
	// addByHash return false if hash already exist
	if (mCountedNeighborForDiversity.addByHash(neigborHash, NULL)) {
		multPoints(Points::diversityBonus(edge));
	}
	if (edge && neighborType == NEIGHBOR_GOOD || neighborType == NEIGHBOR_REALLY_GOOD) {
		if (mGrowPhasis < PLANT_PHASIS_GROWTH_5) {
			mGrowPhasis = static_cast<PlantTypePhasisView>(static_cast<int>(mGrowPhasis) + 1);
			mPlantType->getViewData(mGrowPhasis)->changePlantNodeSprite(this);
			return false;
		}
		//mGrowPhasis++;
		return true;
	}
	return false;
}

