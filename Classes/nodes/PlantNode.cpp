#include "nodes/PlantNode.h"
#include "model/Points.h"
#include "model/PlantType.h"

PlantNode::PlantNode(const PlantType* plantType)
	: mPlantType(plantType), mParentGrid(nullptr), mPointsMultiplicator(1.0f), mDiversityBonus(1.0f), 
	mGrowPhasis(PLANT_PHASIS_SEED), mHalfGrow(false)
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

int PlantNode::countNewNeighbor(DHASH neigborHash, PlantTypeNeighborType neighborType, bool edge /*= true*/)
{
	multPoints(Points::neighborBonus(neighborType, edge));

	// diversity bonus
	// addByHash return false if hash already exist
	if (mCountedNeighborForDiversity.addByHash(neigborHash, NULL)) {
		mDiversityBonus *= Points::diversityBonus(edge);
	}
	if (neighborType == NEIGHBOR_GOOD || neighborType == NEIGHBOR_REALLY_GOOD) {
		if (mGrowPhasis < PLANT_PHASIS_GROWTH_5) {

			if (edge || mHalfGrow) {
				mGrowPhasis = static_cast<PlantTypePhasisView>(static_cast<int>(mGrowPhasis) + 1);
				mPlantType->getViewData(mGrowPhasis)->changePlantNodeSprite(this);
				if (!edge) mHalfGrow = false;
				return mGrowPhasis;
			}
			else {
				mHalfGrow = true;
			}
		}
		return mGrowPhasis;
	}
	else if (NEIGHBOR_BAD == neighborType) {
		if (!edge && mHalfGrow) {
			mHalfGrow = false;
		}
		else if(mGrowPhasis > PLANT_PHASIS_GROWTH_1) {
			// maybe change to bad grow phasis
			mGrowPhasis = static_cast<PlantTypePhasisView>(static_cast<int>(mGrowPhasis) - 1);
			mPlantType->getViewData(mGrowPhasis)->changePlantNodeSprite(this);
			// edge true, half grow true
			// edge true, half grow false
			// grow don't change

			// edge false, half grow false
			if (!edge && !mHalfGrow) {
				mHalfGrow = true;
			}
		}
		
	}
	return mGrowPhasis;
}

