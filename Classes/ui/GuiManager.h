#ifndef __FAIRY_GAMES_GARDEN_RUSH_UI_GUI_MANAGER_H
#define __FAIRY_GAMES_GARDEN_RUSH_UI_GUI_MANAGER_H

#include "cocos2d.h"
#include "lib/DRHashList.hpp"

/*
* \author: einhornimmond
*
* \date: 05.01.2019
*
* \brief: manage GUI elements, cache ressources between scene switch
*
*/

class MovingInfoCallback
{
public:
	virtual void endMovingInfo(cocos2d::Label* caller) = 0;
};

enum GuiLayoutType {
	GUI_LAYOUT_INTRO,
	GUI_LAYOUT_LEVEL,
	GUI_LAYOUT_MAIN_MENU,
	GUI_LAYOUT_SIZE
};

class GuiLayout;

class GuiManager : public cocos2d::Node
{
public:
	//GuiManager();
	~GuiManager();
	static GuiManager* getInstance();

	void setMovingLabelFont(const char* movingLabelFont);
	bool movingInfo(cocos2d::Vec2 startPos, cocos2d::Vec2 endPos, float duration, cocos2d::Color3B color, const char* text, MovingInfoCallback* callback = nullptr);

	bool addLabel(cocos2d::Label* label, const char* name);
	inline cocos2d::Label* findLabel(const char* name) { return findLabel(DRMakeStringHash(name)); }
	inline cocos2d::Label* findLabel(DHASH id) { return static_cast<cocos2d::Label*>(mLabels.findByHash(id)); }

	bool switchToGuiLayout(GuiLayoutType type);

	static const char* guiLayoutTypeString(GuiLayoutType type);

	void unChild();

protected:
	GuiManager();
	std::queue<cocos2d::Label*> mMovingInfos;
	DRHashList mLabels;

	std::string mMovingLabelFont;

	GuiLayout* mEnabledLayout;
	GuiLayout* mGuiLayouts[GUI_LAYOUT_SIZE];


};

#endif // __FAIRY_GAMES_GARDEN_RUSH_UI_GUI_MANAGER_H