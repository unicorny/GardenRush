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
	return true;
}

bool Player::load(Savegame* savegame)
{
	if (!savegame) return initNewPlayer();
}

