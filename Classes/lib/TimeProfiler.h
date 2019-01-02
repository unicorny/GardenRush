#ifndef _FAIRY_GAMES_GARDEN_RUSH_LIB_PROFILER_
#define _FAIRY_GAMES_GARDEN_RUSH_LIB_PROFILER_

/*!
 * 
 * \author: einhornimmond
 * 
 * \date: 31.12.2018
 * 
 * \brief: For easy measure time elapsed for process
*/

#include <string>
#include <time.h>

class TimeProfiler
{
public:
	TimeProfiler();
	~TimeProfiler();

	void reset();
	double millis() const;
	double micros() const;
	double seconds() const;
	std::string string() const;

	static std::string makeStringFromSeconds(double seconds);
protected:
	clock_t		mStartTicks;
};

#endif //_FAIRY_GAMES_GARDEN_RUSH_LIB_PROFILER_