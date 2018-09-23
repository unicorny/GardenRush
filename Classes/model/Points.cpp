#include "Points.h"


Points::Points()
: mPoints(0.0f)
{

}

Points::~Points()
{

}

bool Points::addPointChangeCallback(iPointsUpdate* callback, DHASH nameId)
{
	return mPointChangeCallbacks.addByHash(nameId, callback);
}

bool Points::removePointsChangeCallback(DHASH nameId)
{
	return mPointChangeCallbacks.removeByHash(nameId);
}

void Points::addPoints(float points, cocos2d::Vec2 position)
{
	mPoints += points;
	for (int i = 0; i < mPointChangeCallbacks.getNItems(); i++) {
		iPointsUpdate* callback = (iPointsUpdate*)mPointChangeCallbacks.findByIndex(i);
		if (callback) {
			callback->updatePoints(points, mPoints, position);
		}
	}
}