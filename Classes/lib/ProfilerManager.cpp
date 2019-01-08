#include "lib/ProfilerManager.h"
#include "lib/TimeProfiler.h"
#include "cocos2d.h"

#include <sstream>

#include "ErrorLog.h"

ProfilerManager* ProfilerManager::getInstance()
{
	static ProfilerManager theOneAndOnly;
	return &theOneAndOnly;
}

ProfilerManager::ProfilerManager()
{

}

ProfilerManager::~ProfilerManager()
{
	printProfilingToFile("profiling.txt");
}

std::string ProfilerManager::getTimeProfilerEntrysString() const
{
	std::stringstream ss;
	ss.precision(3);
	double sum = 0.0;
	for (auto it = mTimeProfilerEntrys.begin(); it != mTimeProfilerEntrys.end(); it++) {
		ss << it->mName << ": " << TimeProfiler::makeStringFromSeconds(it->mValue) << std::endl;
		sum += it->mValue;
	}
	ss << "Summe: " << TimeProfiler::makeStringFromSeconds(sum) << std::endl;

	return ss.str();
}

std::string ProfilerManager::getMemoryProfilerEntrysString() const
{
	std::stringstream ss;
	ss.precision(3);
	double sum = 0.0;
	for (auto it = mMemoryProfilerEntrys.begin(); it != mMemoryProfilerEntrys.end(); it++) {
		ss << it->mName << ": " << makeStringFromBytes(it->mValue) << std::endl;
		sum += it->mValue;
	}
	ss << "Summe: " << makeStringFromBytes(sum) << std::endl;

	return ss.str();

}

bool ProfilerManager::printProfilingToFile(const char* filename)
{
	auto fileUtils = cocos2d::FileUtils::getInstance();
	std::string path = fileUtils->getWritablePath() + "/" + std::string(filename);
	//FILE* f = fopen(path.c_str(), "wt");
	//FILE* f = fopen("/storage/emulated/0/Android/data/de.fairy_unicorn_games.garden_rush/files/profiler.txt", "wt");

	ErrorLog::printf("[ProfilerManager::printProfilingToFile] path: %s", path.data());

	/*if (f == nullptr) {
		//do stuff to create the empty data
		LOG_ERROR("cannot open file", false);
	}
	else
	{*/
		char buffer[2] = "\n";
		std::string timeProfilerString = getTimeProfilerEntrysString();
		std::string memoryProfilerString = getMemoryProfilerEntrysString();
		std::string wholeString;
		if (timeProfilerString.size() > 0) {
			//fwrite(timeProfilerString.data(), 1, timeProfilerString.size(), f);
			//fileUtils->writeStringToFile(timeProfilerString, path);
			wholeString = timeProfilerString;
		}
		if (memoryProfilerString.size() > 0 && timeProfilerString.size() > 0) {
			//fwrite(buffer, 1, 1, f);
			//fileUtils->writeStringToFile(buffer, path);
			wholeString += "\n";
		}
		if (memoryProfilerString.size() > 0) {
			//fwrite(memoryProfilerString.data(), 1, memoryProfilerString.size(), f);
			wholeString += memoryProfilerString;
		}
		auto result = fileUtils->writeStringToFile(wholeString, path);
		ErrorLog::printf("result write into file: %d\n", result);
		
		//fclose(f);
	//}

	return true;
}

std::string ProfilerManager::makeStringFromBytes(double bytes)
{
	std::stringstream ss;
	ss.precision(4);

	if (bytes < 1024) {
		ss << bytes << " Bytes";
	}
	else if (bytes < 1024 * 1024) {
		ss << (bytes / 1024.0) << " kBytes";
	}
	else {
		ss << (bytes / (1024.0 *1024.0)) << " mBytes";
	}
	return ss.str();
}