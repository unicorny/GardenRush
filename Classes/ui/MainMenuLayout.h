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

enum MainMenuButtoType {
	MAIN_MENU_BUTTON_PLAY_ON,
	MAIN_MENU_BUTTON_NEW_GAME,
	MAIN_MENU_BUTTON_LOAD_GAME,
	MAIN_MENU_BUTTON_SETTINGS,
	MAIN_MENU_BUTTON_LIBRARY,
	MAIN_MENU_BUTTON_SIZE
};

class SwitchButton;

class MainMenuLayout : public GuiLayout
{
public:
	MainMenuLayout();
	virtual ~MainMenuLayout();

	virtual bool enable();
	virtual bool init();

protected:
	SwitchButton* mButtons[MAIN_MENU_BUTTON_SIZE];
};


#endif // __FAIRY_GAMES_GARDEN_RUSH_UI_MAIN_MENU_LAYOUT_H