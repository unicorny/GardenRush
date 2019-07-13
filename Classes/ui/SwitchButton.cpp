#include "SwitchButton.h"
#include "controller/RessourcenManager.h"
#include "ErrorLog.h"

using namespace cocos2d;

SwitchButton::SwitchButton()
	: mLabel(nullptr), mButtonCfg(nullptr), mCurrentState(SWITCH_BUTTON_DEFAULT)
{

}

SwitchButton::~SwitchButton()
{
	DR_SAVE_DELETE(mButtonCfg);
}

bool SwitchButton::initDefault(const char* text, int pixelWidth)
{
	auto ressourcenManager = RessourcenManager::getInstance();
	mButtonCfg = new RessourcenManager::ButtonConfig(ressourcenManager->getButtonConfig());
	if (!mButtonCfg) {
		LOG_ERROR("btnConfig is empty", false);
	}
	initWithFile(mButtonCfg->defaultImg.data());
	//setCenterRect(mButtonCfg->sprite9Rect);
	auto contentSize = getContentSize();
	//setContentSize(Size(pixelWidth, contentSize.height));
	contentSize = getContentSize();
	autorelease();
	retain();
	//setStretchEnabled(true);
	auto fontPath = ressourcenManager->getFontPath(mButtonCfg->font.data());
	mLabel = cocos2d::Label::createWithBMFont(fontPath, text);
	mLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	mLabel->setPosition(contentSize.width / 2.0f, contentSize.height / 2.0f);
	//mLabel->setColor()
	addChild(mLabel);


	return true;
}

bool SwitchButton::switchState(SwitchButtonState state)
{
	if (state == mCurrentState) {
		return true;
	}
	if (!mButtonCfg) {
		LOG_ERROR("no button config set", false);
	}
	const char* newTexture;
	Color3B color = Color3B::WHITE;

	switch (state) {
	case SWITCH_BUTTON_DEFAULT: 
		newTexture = mButtonCfg->defaultImg.data(); 
		// color white
		break;
	case SWITCH_BUTTON_DISABLED: 
		newTexture = mButtonCfg->disabledImg.data(); 
		color = Color3B(62, 174, 92); 
		break;
	case SWITCH_BUTTON_PRESSED: 
		newTexture = mButtonCfg->pressedImg.data(); 
		color = Color3B(62, 174, 92); 
		break;
	default: LOG_ERROR("invalid state given", false);
	}

	if (!newTexture) {
		LOG_ERROR("texture not set", false);
	}
	mLabel->setColor(color);
	setTexture(newTexture);
	mCurrentState = state;
	return true;
}

bool SwitchButton::initDefault(const char* text, float pixelWidthPercent)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	return initDefault(text, static_cast<int>(pixelWidthPercent * visibleSize.width));
}