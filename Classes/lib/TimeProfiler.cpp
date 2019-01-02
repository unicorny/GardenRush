#include "lib/TimeProfiler.h"
#include <sstream>

TimeProfiler::TimeProfiler()
{

	reset();
}

TimeProfiler::~TimeProfiler()
{

}

void TimeProfiler::reset()
{
	mStartTicks = clock();
}

double TimeProfiler::seconds() const
{
	clock_t ticksDiff = clock() - mStartTicks;
	return (static_cast<double>(ticksDiff) / static_cast<double>(CLOCKS_PER_SEC));
}

double TimeProfiler::millis() const
{
	clock_t ticksDiff = clock() - mStartTicks;
	return (static_cast<double>(ticksDiff) / static_cast<double>(CLOCKS_PER_SEC)) * 1000.0;
}

double TimeProfiler::micros() const
{
	clock_t ticksDiff = clock() - mStartTicks;
	return (static_cast<double>(ticksDiff) / static_cast<double>(CLOCKS_PER_SEC)) * 1000000.0;
}

std::string TimeProfiler::string() const
{
	clock_t ticksDiff = clock() - mStartTicks;
	double seconds = static_cast<double>(ticksDiff) / static_cast<double>(CLOCKS_PER_SEC);
	return makeStringFromSeconds(seconds);
}

std::string TimeProfiler::makeStringFromSeconds(double seconds)
{
	std::stringstream ss;
	ss.precision(3);

	if (0.0 == seconds) {
		ss << "0";
	}

	else if (seconds > 0.5) {
		ss << seconds << " s";
	}
	else if (seconds > 0.0009) {
		ss << (seconds * 1000.0) << " ms";
	}
	else {
		ss << (seconds * 1000000.0) << " micro s";
	}
	return ss.str();
}