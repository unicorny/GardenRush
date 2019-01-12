#include "GuiManager.h"
#include "IntroLayout.h"
#include "LevelLayout.h"
#include "MainMenuLayout.h"

#include "ErrorLog.h"

using namespace cocos2d;

GuiManager* GuiManager::getInstance()
{
	static GuiManager theOneAndOnly;
	return &theOneAndOnly;
}

GuiManager::GuiManager()
	: mEnabledLayout(nullptr)
{
	memset(mGuiLayouts, 0, sizeof(GuiLayout*) * GUI_LAYOUT_SIZE);
}

GuiManager::~GuiManager()
{
	while (mMovingInfos.size() > 0) {
		mMovingInfos.front()->release();
		mMovingInfos.pop();
	}
	if (mEnabledLayout) {
		removeChild(mEnabledLayout);
		mEnabledLayout = nullptr;
	}
	for (int i = 0; i < GUI_LAYOUT_SIZE; i++) {
		if (mGuiLayouts[i]) {
			delete mGuiLayouts[i];
			mGuiLayouts[i] = nullptr;
		}
	}
}

bool GuiManager::movingInfo(Vec2 startPos, Vec2 endPos, float duration, Color3B color, 
								const char* text, MovingInfoCallback* callback/* = nullptr*/)
{
	Label* label = nullptr;
	assert(mMovingLabelFont.size() > 0);
	// check if we can reuse label
	if (mMovingInfos.size() > 0 && mMovingInfos.front()->getNumberOfRunningActions() == 0) {
		label = mMovingInfos.front();
		mMovingInfos.pop();
		label->setString(text);
	}
	else {
		label = Label::createWithBMFont(mMovingLabelFont, text);
		addChild(label);
	}
	label->setPosition(startPos);
	label->setColor(color);
	label->setOpacity(255);
	
	auto moveAndScaleSequence = Sequence::create(
		EaseOut::create(MoveTo::create(duration, endPos), 1.2f),
		EaseSineInOut::create(ScaleTo::create(1.2f, 1.0f)),
		FadeOut::create(0.5f),
		nullptr);

	if (callback) {
		auto labelMoveSequence = Sequence::create(
			moveAndScaleSequence,
			//CallFunc::create(CC_CALLBACK_0(MovingInfoCallback::endMovingInfo, callback)),
			nullptr);
		label->runAction(labelMoveSequence);
	}
	else {
		label->runAction(moveAndScaleSequence);
	}

	return true;
	
}

bool GuiManager::addLabel(cocos2d::Label* label, const char* name)
{
	DHASH id = DRMakeStringHash(name);
	if (mLabels.findByHash(id)) {
		LOG_ERROR("label already exist", false);
	}

	mLabels.addByHash(id, label);
	addChild(label);
	return true;
}

void GuiManager::setMovingLabelFont(const char* movingLabelFont)
{
	if (mMovingLabelFont.size() > 0) {
		LOG_ERROR_VOID("warning, movingLabel has already a font");
	}
	mMovingLabelFont = movingLabelFont;
}

bool GuiManager::switchToGuiLayout(GuiLayoutType type)
{
	if (!mGuiLayouts[type]) {
		switch (type) {
		case GUI_LAYOUT_INTRO: mGuiLayouts[type] = new IntroLayout; break;
		case GUI_LAYOUT_LEVEL: mGuiLayouts[type] = new LevelLayout; break;
		case GUI_LAYOUT_MAIN_MENU: mGuiLayouts[type] = new MainMenuLayout; break;
		default: 
			ErrorLog::printf("Gui Layout Type int: %d", type);
			LOG_ERROR("error, gui with type didn't exist or not implemented yet", false);
		}
		if(!mGuiLayouts[type]->init()) {
			ErrorLog::printf("error with gui layout type: %s", guiLayoutTypeString(type));
			LOG_ERROR("error loading gui layout", false);
		}
	}
	removeChild(mEnabledLayout);
	if (!mGuiLayouts[type]->enable()) {
		ErrorLog::printf("error with gui layout type: %s", guiLayoutTypeString(type));
		LOG_ERROR("error enabling gui layout", false);
	}
	addChild(mGuiLayouts[type]);
	mEnabledLayout = mGuiLayouts[type];

	return true;
}

const char* GuiManager::guiLayoutTypeString(GuiLayoutType type)
{
	switch (type) {
	case GUI_LAYOUT_INTRO: return "GUI_LAYOUT_INTRO";
	case GUI_LAYOUT_LEVEL: return "GUI_LAYOUT_LEVEL";
	case GUI_LAYOUT_MAIN_MENU: return "GUI_LAYOUT_MAIN_MENU";
	default: return "unknown gui Layout";
	}

	return "- error -";
}

void GuiManager::unChild()
{
	if (getParent()) {
		auto parent = getParent();
		parent->removeChild(this);
	}
}