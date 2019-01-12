#include "controller/RessourcenManager.h"
#include "model/Player.h"
#include "cocos2d.h"

Player::Player()
	: mIsNewPlayer(false), mSettings(SETTINGS_NONE)
{

}

Player* Player::getInstance()
{
	static Player theOneAndOnly;
	return &theOneAndOnly;
}

bool Player::initNewPlayer()
{
	mIsNewPlayer = true;
	//mSavegame = new Savegame;
	auto firstStoryName = RessourcenManager::getInstance()->getStoryNameByIndex(0);
	mSavegame.setValuePair("story", firstStoryName);
	return true;
}

bool Player::load(Savegame* savegame)
{
	if (!savegame) return initNewPlayer();
	mSavegame = *savegame;

	return true;
}

