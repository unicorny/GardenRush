#include "MainMenuLayout.h"
#include "SwitchButton.h"

using namespace cocos2d;

MainMenuLayout::MainMenuLayout()
{
	memset(mButtons, 0, sizeof(SwitchButton*) * MAIN_MENU_BUTTON_SIZE);
}

MainMenuLayout::~MainMenuLayout()
{
	for (int i = 0; i < MAIN_MENU_BUTTON_SIZE; i++) {
		mButtons[i]->release();
	}
}

bool MainMenuLayout::enable()
{
	return true;
}

bool MainMenuLayout::init()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backContainer = Sprite::create("box1.png");
	backContainer->setCenterRect(Rect(18, 16, 36, 32));
	backContainer->setAnchorPoint(Vec2(0.0f, 0.0f));
	auto containerSize = backContainer->getContentSize();
	containerSize.width = visibleSize.width * 0.6f;
	containerSize.height = visibleSize.height * 0.9f;
	//backContainer->setScaleX((visibleSize.width * 0.6f) / containerSize.width);
	//backContainer->setScaleY((visibleSize.height * 0.9f) / containerSize.height);
	backContainer->setContentSize(containerSize);
	backContainer->setPosition(origin + Vec2(visibleSize.width * 0.2f, visibleSize.height * 0.05f));

	containerSize.width *= backContainer->getScaleX();
	containerSize.height *= backContainer->getScaleY();

	// buttons
	for (int i = 0; i < MAIN_MENU_BUTTON_SIZE; i++) {
		mButtons[i] = new SwitchButton;
		std::string text;
		switch (i) {
		case MAIN_MENU_BUTTON_PLAY_ON: text = "Weiterspielen"; break;
		case MAIN_MENU_BUTTON_NEW_GAME: text = "Neues Spiel"; break;
		case MAIN_MENU_BUTTON_LOAD_GAME: text = "Spiel laden"; break;
		case MAIN_MENU_BUTTON_SETTINGS: text = "Einstellungen"; break;
		case MAIN_MENU_BUTTON_LIBRARY: text = "Bibliothek"; break;
		default: text = "unknown"; 
		}
		mButtons[i]->initDefault(text.data(), 0.4f);
		mButtons[i]->setAnchorPoint(Vec2(0.5f, 0.5f));
		auto buttonSize = mButtons[i]->getContentSize();
		backContainer->addChild(mButtons[i]);
		auto iReverse = MAIN_MENU_BUTTON_SIZE - i;
		mButtons[i]->setPosition(Vec2(
			containerSize.width / 2.0f, 
			containerSize.height * (iReverse * 0.05f - 0.05f)  + iReverse * buttonSize.height));

	}
	
	addChild(backContainer);

	return true;
}