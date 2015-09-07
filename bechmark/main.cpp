//#define NONIUS_RUNNER
#include <nonius/nonius_single.h++>

#include <list>
#include <forward_list>

#define LUA_COMPAT_APIINTCASTS

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

#include <selene.h>

#define LUAINTF_HEADERS_ONLY 1
#include <LuaIntf.h>

#include <luabind/luabind.hpp>
//#include "LuaIntf/LuaIntf.h"
//#include <impl/CppBindModule.h>

//#include <luapp/lua.hpp>
// LuaContext - conflicts with latest boost.
// luapp = won't build with VS2015
// sol - not compatible with vs2015, uses exceptions
// lua-intff - uses exceptions, must be build

//#include <boost/any.hpp>
//#include <boost/type_traits/add_rvalue_reference.hpp>

enum {
	NUM_ITERATIONS = 100000
};

namespace bench_cfunction_from_lua {
	int test(int value) {
		//std::cout << value << std::endl;
		return value + 1;
	}

	int plain_lua_test_wrapper(lua_State *L) {
		int n = lua_gettop(L);

		if (n != 1) {
			lua_pushstring(L, "Incorrect number of arguments'");
			lua_error(L);
			return 0;
		}

		if (!lua_isnumber(L, 1)) {
			lua_pushstring(L, "Incorrect argument to 'test'");
			lua_error(L);
			return 0;
		}

		int value = lua_tonumber(L, 1);
		int result = test(value);
		lua_pushnumber(L, result);
		return 1;
	}

	//namespace luawrapper {
		//#include <LuaContext.hpp>
	//	int exec() {
	//		LuaContext lua;
	//		//lua.writeFunction("show", &show);
	//		lua.executeCode("show(7)");     // prints 7
	//	}
	//}

	void run() {
		nonius::configuration cfg;
		cfg.output_file = "cfunction_from_lua.html";

		cfg.samples = 10;
		//double confidence_interval = 0.95;
		//int resamples = 100000;
		cfg.title = "cfunction_from_lua x 100000";

		cfg.list_benchmarks = true;
		cfg.summary = true;



		//sol::state sol_lua;
		//sol_lua.set_function(std::string("test"), &test);

		//auto LuaIntf = luaL_newstate();
		////LuaBinding(L).beginModule("utils")
		//LuaBinding(LuaIntf).beginModule("utils")
		//	//.addFunction("xpairs", &xpairs)
		//.endModule();

		// Selene
		sel::State sel_state(true);
		sel_state["test"] = &test;
	
		// LuaIntf
		LuaIntf::LuaContext luaintf(true);
		LuaIntf::LuaBinding(luaintf.state())
			.beginModule("luaintf")
				.addFunction("test", &test)
			.endModule();

		// plain C 
		auto plain_L = luaL_newstate();
		luaL_openlibs(plain_L);
		lua_register(plain_L, "test", &plain_lua_test_wrapper);

		// Luabind
		lua_State* luabind_L = luaL_newstate();
		luaL_openlibs(luabind_L);
		luabind::open(luabind_L);
		{
			using namespace luabind;
			module(luabind_L)
			[
				def("test", &test)
			];
		}

		nonius::benchmark benchmarks[] = {
			nonius::benchmark("selene", [&sel_state] {
				int result = 0;
				for (size_t i = 0; i < NUM_ITERATIONS; ++i)
					result += (int)sel_state["test"](42);
				return result;
			})
			,nonius::benchmark("luaintf", [&luaintf] {
				int result = 0;
				
				for (size_t i = 0; i < NUM_ITERATIONS; ++i) {
					LuaIntf::LuaRef func(luaintf.state(), "luaintf.test");
					int rv = func.call<int, int>(42);
					//func(42);
					result += rv;
				}

				return result;
			})
			,
			nonius::benchmark("luabind", [luabind_L] {
				int result = 0;
				for (size_t i = 0; i < NUM_ITERATIONS; ++i) {
					int rv = luabind::call_function<int>(luabind_L, "test", 42);
					result += rv;
				}
				return result;
			})
			, nonius::benchmark("plain_c", [plain_L] {
				int result = 0;
				auto L = plain_L;
				for (size_t i = 0; i < NUM_ITERATIONS; ++i) {
					// the function name
					lua_getglobal(L, "test");
					// the argument 
					lua_pushnumber(L, 42);
					// call the function with 2	arguments, return 1 result 
					lua_call(L, 1, 1);
					int lua_out = (int)lua_tonumber(L, -1);
					lua_pop(L, 1);
					result += lua_out;
				}
				return result;
			})
			//,nonius::benchmark("sol", [&sol_lua] {
			//	int result = 0;
			//	for (size_t i = 0; i < 1000; ++i) {
			//		//sol_lua.script("test()");
			//		//auto test_lua_func = sol_lua.get<sol::function>("test");
			//		//result += (int)(sol_lua<sol::function>("test")(42));
			//	}
			//	return result;
			//})

		};

		nonius::go(cfg, std::begin(benchmarks), std::end(benchmarks), nonius::html_reporter());

		lua_close(plain_L);
		lua_close(luabind_L);
	}

}


int main() {
	bench_cfunction_from_lua::run();
}