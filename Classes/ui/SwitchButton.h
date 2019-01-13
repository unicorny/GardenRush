#ifndef __FAIRY_GAMES_GARDEN_RUSH_UI_SWITCH_BUTTON_H
#define __FAIRY_GAMES_GARDEN_RUSH_UI_SWITCH_BUTTON_H

#include "cocos2d.h"
#include "controller/RessourcenManager.h"
//#include "GuiLayout.h"

/*
* \author: einhornimmond
*
* \date: 13.01.2019
*
* \brief: Button Handler for switch button
*
*/

enum SwitchButtonState {
	SWITCH_BUTTON_DEFAULT,
	SWITCH_BUTTON_PRESSED,
	SWITCH_BUTTON_DISABLED
};

class SwitchButton : public cocos2d::Sprite
{
public:
	SwitchButton();
	virtual ~SwitchButton();

	bool initDefault(const char* text, int pixelWidth);
	bool initDefault(const char* text, float pixelWidthPercent);

	bool switchState(SwitchButtonState state);

	inline bool isPressed() { return mCurrentState == SWITCH_BUTTON_PRESSED; }
	inline SwitchButtonState getButtonState() { return mCurrentState; }

protected:
	RessourcenManager::ButtonConfig* mButtonCfg;
	cocos2d::Label* mLabel;

	SwitchButtonState mCurrentState;
};


#endif // __FAIRY_GAMES_GARDEN_RUSH_UI_SWITCH_BUTTON_H