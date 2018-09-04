#include "ErrorLog.h"
#include <cstdarg>
#include <cstdio>

#ifdef _MSC_VER
#include <windows.h>
#else 
#include "cocos2d.h"
#endif

void ErrorLog::printf(const char* pcText, ...)
{
	//Textbuffer zum schreiben
	char acBuffer[1024];

	va_list   Argumente;

	//Buffer fuellen
	va_start(Argumente, pcText);
	vsprintf(acBuffer, pcText, Argumente);
	va_end(Argumente);
#ifdef _MSC_VER
	OutputDebugStringA(acBuffer);
#else
	cocos2d::log("%s", acBuffer);
#endif
	
}