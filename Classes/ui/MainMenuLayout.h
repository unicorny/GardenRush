#ifndef __FAIRY_GAMES_GARDEN_RUSH_UI_MAIN_MENU_LAYOUT_H
#define __FAIRY_GAMES_GARDEN_RUSH_UI_MAIN_MENU_LAYOUT_H

#include "cocos2d.h"
#include "GuiLayout.h"

/*
* \author: einhornimmond
*
* \date: 12.01.2019
*
* \brief: 
*
*/

class MainMenuLayout : public GuiLayout
{
public:
	MainMenuLayout();
	virtual ~MainMenuLayout();

	virtual bool enable();
	virtual bool init();

protected:
};


#endif // __FAIRY_GAMES_GARDEN_RUSH_UI_MAIN_MENU_LAYOUT_H