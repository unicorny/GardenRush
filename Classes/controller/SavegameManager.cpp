#include "controller/SavegameManager.h"
#include "ErrorLog.h"

SavegameManager::SavegameManager()
	: mSavegames(nullptr), mSavegameCount(0)
{

}

SavegameManager::~SavegameManager()
{
	if (mSavegames) {
		delete[] mSavegames;
		mSavegames = nullptr;
	}
}
/*
SavegameManager* SavegameManager::getInstance()
{
	static SavegameManager theOneAndOnly;
	return &theOneAndOnly;
}
*/
bool SavegameManager::init()
{
	auto fileUtils = cocos2d::FileUtils::getInstance();
	std::string path = fileUtils->getWritablePath() + std::string("savegame");
	if (!fileUtils->isDirectoryExist(path)) {
		fileUtils->createDirectory(path);
		return true;
	}
	else {
		auto savegameFiles = fileUtils->listFiles(path);
		// list files return also . and .. (at least with win32)
		if (savegameFiles.size() < 3) {
			return true;
		}
		else {
			mSavegameCount = savegameFiles.size();
			mSavegames = new Savegame[mSavegameCount];
			for (size_t i = 0; i < mSavegameCount; i++) {
				
				if (!mSavegames[i].loadFromFile(savegameFiles[i].data())) {
					LOG_ERROR("loading savegame file", false);
				}
				
			}
		}
	}
	return true;
}