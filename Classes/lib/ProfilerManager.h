#ifndef _FAIRY_GAMES_GARDEN_RUSH_LIB_PROFILER_MANAGER_
#define _FAIRY_GAMES_GARDEN_RUSH_LIB_PROFILER_MANAGER_

#include <deque>
#include <string>

class ProfilerManager 
{
public:
	~ProfilerManager();
	static ProfilerManager* getInstance();

	inline void addTimeProfilerEntry(const char* name, double seconds) { mTimeProfilerEntrys.push_back(ProfilerEntry(name, seconds));}
	inline void addMemoryProfilerEntry(const char* name, double bytes) { mMemoryProfilerEntrys.push_back(ProfilerEntry(name, bytes)); }

	std::string getTimeProfilerEntrysString() const;
	std::string getMemoryProfilerEntrysString() const;

	static std::string makeStringFromBytes(double bytes);

	bool printProfilingToFile(const char* filename);

protected:
	ProfilerManager();
	
	struct ProfilerEntry
	{
		ProfilerEntry(const char* name, double value) : mName(name), mValue(value) {}
		std::string mName;
		double mValue;
	};


	std::deque<ProfilerEntry> mTimeProfilerEntrys;
	std::deque<ProfilerEntry> mMemoryProfilerEntrys;
		
};

#endif //_FAIRY_GAMES_GARDEN_RUSH_LIB_PROFILER_MANAGER_