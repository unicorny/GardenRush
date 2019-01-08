#include "model/Savegame.h"
#include "ErrorLog.h"

Savegame::Savegame()
{

}

Savegame::~Savegame()
{

}

bool Savegame::loadFromFile(const char* filePath)
{
	
	auto fileUtils = cocos2d::FileUtils::getInstance();
	assert(fileUtils->isFileExist(filePath));

	mSavegameData = fileUtils->getValueMapFromFile(filePath);
	if (mSavegameData.find("version")->second.asInt() <= SAVEGAME_VERSION) {
		return true;
	}
	LOG_ERROR("savegame verson is to new", false);
}