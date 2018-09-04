#ifndef __FAIRY_GAMES_GARDEN_RUSH_ERROR_LOG_H
#define __FAIRY_GAMES_GARDEN_RUSH_ERROR_LOG_H

class ErrorLog
{
public:
	static void printf(const char* str, ...);
};

#define LOG_ERROR(text, ret) {ErrorLog::printf("FEHLER: %s (%s, Zeile %d, Funktion %s)", text, __FILE__, __LINE__, __FUNCTION__); return ret;}

#endif //__FAIRY_GAMES_GARDEN_RUSH_ERROR_LOG_H