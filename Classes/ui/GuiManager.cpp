#include "GuiManager.h"

#include "ErrorLog.h"

using namespace cocos2d;

GuiManager* GuiManager::getInstance()
{
	static GuiManager theOneAndOnly;
	return &theOneAndOnly;
}

GuiManager::GuiManager()
{

}

GuiManager::~GuiManager()
{
	while (mMovingInfos.size() > 0) {
		mMovingInfos.front()->release();
		mMovingInfos.pop();
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