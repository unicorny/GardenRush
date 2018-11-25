#include "model/Player.h"

Player::Player()
	: mSettings(SETTINGS_NONE)
{

}

Player* Player::getInstance()
{
	static Player theOneAndOnly;
	return &theOneAndOnly;
}