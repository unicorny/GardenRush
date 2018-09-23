#ifndef __FAIRY_GAMES_GARDEN_RUSH_MODEL_POINTS_H
#define __FAIRY_GAMES_GARDEN_RUSH_MODEL_POINTS_H

/*!
* \author: Dario Rekowski
*
* \date: 18.09.18
*
* \brief: containing points and logic for point calculation
*
*/

#include "Enums.h"
#include "lib/DRHashList.hpp"
#include "cocos2d.h"


class iPointsUpdate
{
public:
	virtual void updatePoints(float pointDifference, float pointsSum, cocos2d::Vec2 worldPosition) = 0;
};

class Points
{
public:
	
	Points();
	~Points();

	static float diversityBonus(bool edge = true) {
		if (edge) return 1.05f;
		return 1.045f;
	}
	static float neighborBonus(PlantTypeNeighborType type, bool edge = true) {
		switch (type) {
		case NEIGHBOR_GOOD: 
			if (edge) return 1.2f;
			return 1.1f;
		case NEIGHBOR_REALLY_GOOD:
			if (edge) return 1.5f;
			return 1.35f;
		case NEIGHBOR_BAD:
			if (edge) return 0.75f;
			return 0.85f;
		case NEIGHBOR_NEUTRAL: return 1.0f;
		default: return 0.0f;
		}
		return 0.0f;
	}

	inline float getPoints() { return mPoints; }
	void addPoints(float points, cocos2d::Vec2 position);
	inline void reset() { mPoints = 0.0f; }
	
	//! \return true if entry has exist and was removed
	bool removePointsChangeCallback(DHASH nameId);
	//! \return true if entry has exist and was removed
	inline bool removePointsChangeCallback(const char* name) {
		return removePointsChangeCallback(DRMakeStringHash(name));
	}
	//! \param callback function will be called with two arguments, float points changed and float new points sum
	//! \return true if callback could add and false if callback couldn't add (hash already exist)
	bool addPointChangeCallback(iPointsUpdate* callback, DHASH nameId);
	//! \param callback function will be called with two arguments, float points changed and float new points sum
	//! \return true if callback could add and false if callback couldn't add (hash already exist)
	inline bool addPointChangeCallback(iPointsUpdate* callback, const char* name) {
		return addPointChangeCallback(callback, DRMakeStringHash(name));
	}

protected:
	float		mPoints;
	DRHashList mPointChangeCallbacks;

};


#endif // __FAIRY_GAMES_GARDEN_RUSH_MODEL_POINTS_H