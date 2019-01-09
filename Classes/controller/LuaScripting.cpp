#include "controller/LuaScripting.h"
#include "lib/ProfilerManager.h"
#include "lib/TimeProfiler.h"
#include "ErrorLog.h"



LuaScripting* LuaScripting::getInstance()
{
	static LuaScripting theOneAndOnly;
	return &theOneAndOnly;
}

LuaScripting::LuaScripting()
	: mLuaResumeTime(0.0)
#ifdef FG_USING_LUA
	,mLuaState(nullptr), mCoroutineLuaState(nullptr)
#endif
{

}

LuaScripting::~LuaScripting()
{
#ifdef FG_USING_LUA
	if (mCoroutineLuaState) {
		//lua_close(mCoroutineLuaState);
		mCoroutineLuaState = nullptr;
	}

	if (mLuaState) {
		lua_close(mLuaState);
		mLuaState = nullptr;
	}
#endif //FG_USING_LUA
}

#ifdef FG_USING_LUA
static const struct luaL_Reg printlib[] = {
	{ "print", LuaScripting::l_custom_print },
	{ NULL, NULL } /* end of array */
};
#endif // FG_USING_LUA

bool LuaScripting::init()
{
	auto profiler = ProfilerManager::getInstance();
	TimeProfiler time;
	// test lua
	//auto engine = cocos2d::LuaEngine::getInstance();
	//engine->executeString("print \"Hello World\"");
#ifdef FG_USING_LUA
	mLuaState = luaL_newstate();
	luaL_openlibs(mLuaState);

	lua_getglobal(mLuaState, "_G");
	luaL_register(mLuaState, NULL, printlib);
	lua_pop(mLuaState, 1);

	mCoroutineLuaState = lua_newthread(mLuaState);
	luaL_openlibs(mCoroutineLuaState);

	luaL_dostring(mLuaState, "print(\"Hello World\\n\")");
	
#endif //FG_USING_LUA
	profiler->addTimeProfilerEntry("LuaScripting::init", time.seconds());

	return true;
}

#ifdef FG_USING_LUA
// custom print
int LuaScripting::l_custom_print(lua_State* L) {
	int nargs = lua_gettop(L);
	//std::cout << "in my_print:";
	for (int i = 1; i <= nargs; ++i) {
		//std::cout << lua_tostring(L, i);
		ErrorLog::printf(lua_tostring(L, i));
	}
	//std::cout << std::endl;

	return 0;
}

#endif //FG_USING_LUA

bool LuaScripting::yieldLuaState()
{
	int status = lua_resume(mCoroutineLuaState, 0);
	if (status == LUA_YIELD) {
		double seconds = lua_tonumber(mCoroutineLuaState, 0);
		mLuaResumeTime = now() + seconds;
	}
	else {
		mLuaResumeTime = 0.0;
	}

	return true;
}

bool LuaScripting::update()
{
	if (mLuaResumeTime > 0.0 && now() >= mLuaResumeTime) {
		yieldLuaState();
	}
	return true;
}

bool LuaScripting::startScript(const char* pathname)
{
	luaL_dofile(mCoroutineLuaState, pathname);
	lua_getglobal(mCoroutineLuaState, "main");
	yieldLuaState();

	return true;
}