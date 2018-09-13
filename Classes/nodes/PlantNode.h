#ifndef __FAIRY_GAMES_GARDEN_RUSH_VIEW_SPRITE_VIEW_H
#define __FAIRY_GAMES_GARDEN_RUSH_VIEW_SPRITE_VIEW_H

/*!
 *	\author: Dario Rekowski
 *
 *	\date: 28.08.18
 *
 *  \brief: Class for store sprite view, if it a single texture, animation or sprites in 
 *          a texture-atlas
*/
#include "cocos2d.h"
#include "lib/DRHash.hpp"

class PlantNode : public cocos2d::Sprite
{
public:
	PlantNode(DHASH plantHash);
	virtual ~PlantNode();
protected:
	DHASH mPlantHash;
	
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_VIEW_SPRITE_VIEW_H
