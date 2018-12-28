#ifndef __FAIRY_GAMES_GARDEN_RUSH_CONFIG_H
#define __FAIRY_GAMES_GARDEN_RUSH_CONFIG_H

#include "Enums.h"
#include "cocos2d.h"

cocos2d::Color4B config_getColorForCellGlow(GridCellGlowType cellGlowType) {
	switch (cellGlowType) {
	case CELL_GLOW_SELECTED: return cocos2d::Color4B::YELLOW;
	case CELL_GLOW_EMPTY: return cocos2d::Color4B(255, 255, 255, 192);
	case CELL_GLOW_NEUTRAL_NEIGHBOR: return cocos2d::Color4B(192, 192, 192, 255);
	case CELL_GLOW_GOOD_NEIGHBOR: return cocos2d::Color4B(0, 255, 0, 255);
	case CELL_GLOW_BAD_NEIGHBOR: return cocos2d::Color4B(255, 0, 0, 255);
	case CELL_GLOW_ERROR: return cocos2d::Color4B(255, 0, 0, 192);
	default: return cocos2d::Color4B(255, 255, 255, 0);
		
	}
}


#endif // __FAIRY_GAMES_GARDEN_RUSH_CONFIG_H