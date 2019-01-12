#ifndef __FAIRY_GAMES_GARDEN_RUSH_UI_GUI_LAYOUT_H
#define __FAIRY_GAMES_GARDEN_RUSH_UI_GUI_LAYOUT_H

#include "cocos2d.h"
#include "lib/DRHashList.hpp"

/*
* \author: einhornimmond
*
* \date: 06.01.2019
*
* \brief: contain a gui layout
*
*/


class GuiLayout : public cocos2d::Node
{
public:
	GuiLayout();
	virtual ~GuiLayout();	

	virtual bool enable() = 0;
	virtual bool init() = 0;

protected:
	
	
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_UI_GUI_LAYOUT_H