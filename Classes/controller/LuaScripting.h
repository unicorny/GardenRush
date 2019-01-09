#ifndef __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_LUA_SCRIPTING_H
#define __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_LUA_SCRIPTING_H


/*
* \author: einhornimmond
*
* \date: 08.01.2019
*
* \brief: load lua states and provide functions for inside lua scripts
*
*/
#define FG_USING_LUA 

#ifdef FG_USING_LUA
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#endif // FG_USING_LUA
#include "time.h"

class LuaScripting
{
public:
	static LuaScripting* getInstance();
	~LuaScripting();

	bool init();
	
	bool startScript(const char* pathname);

	bool update();

#ifdef FG_USING_LUA
	static int l_custom_print(lua_State* L);
#endif

protected:
	LuaScripting();

	bool yieldLuaState();
	inline double now() { return static_cast<double>(clock()) / static_cast<double>(CLOCKS_PER_SEC); }

	double mLuaResumeTime;
#ifdef FG_USING_LUA
	lua_State* mLuaState;
	lua_State* mCoroutineLuaState;
#endif //FG_USING_LUA
};

#endif // __FAIRY_GAMES_GARDEN_RUSH_CONTROLLER_LUA_SCRIPTING_H