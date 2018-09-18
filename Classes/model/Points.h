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

class Points
{
public:
	
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

protected:
	

};

#endif // __FAIRY_GAMES_GARDEN_RUSH_MODEL_POINTS_H