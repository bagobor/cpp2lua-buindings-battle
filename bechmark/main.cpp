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
#include <sol.hpp>

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
		cfg.sub_title = "generated with <em>misused</em>  <a href = \"http://flamingdangerzone.com/nonius\"> nonius </a>";
		cfg.graph_type = nonius::configuration::line;

		cfg.samples = 10;
		//double confidence_interval = 0.95;
		//int resamples = 100000;
		cfg.title = "cfunction_from_lua x 100000 x iteration";

		cfg.list_benchmarks = true;
		cfg.summary = true;



		sol::state sol_lua;
		sol_lua.set_function(std::string("test"), &test);

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
			nonius::benchmark("selene_cache", [&sel_state] {
				int result = 0;
				static size_t iteration_index = 1;
				auto func = sel_state["test"];
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					result += (int)func(42);
				}
				++iteration_index;
				return result;
			})
			,nonius::benchmark("selene", [&sel_state] {
				int result = 0;
				static size_t iteration_index = 1;
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					result += (int)sel_state["test"](42);
				}
				++iteration_index;
				return result;
			})
			,nonius::benchmark("luaintf_cache", [&luaintf] {
				int result = 0;
				static size_t iteration_index = 1;
				LuaIntf::LuaRef func(luaintf.state(), "luaintf.test");
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {					
					int rv = func.call<int, int>(42);
					result += rv;
				}
				++iteration_index;
				return result;
			})
			,nonius::benchmark("luaintf", [&luaintf] {
				int result = 0;				
				static size_t iteration_index = 1;
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					LuaIntf::LuaRef func(luaintf.state(), "luaintf.test");
					int rv = func.call<int, int>(42);
					result += rv;
				}
				++iteration_index;
				return result;
			})
			, nonius::benchmark("luabind_raw", [luabind_L] {
				int result = 0;
				static size_t iteration_index = 1;
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					int rv = luabind::call_function<int>(luabind_L, "test", 42);					
					result += rv;
				}
				++iteration_index;
				return result;
			})
			, nonius::benchmark("luabind_cache", [luabind_L] {
				int result = 0;
				static size_t iteration_index = 1;
				luabind::object func = luabind::globals(luabind_L)["test"];//(42);
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					int rv = luabind::call_function<int>(func, 42);
					result += rv;
				}
				++iteration_index;
				return result;
			})
			, nonius::benchmark("luabind", [luabind_L] {
				int result = 0;
				static size_t iteration_index = 1;
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					luabind::object func = luabind::globals(luabind_L)["test"];
					int rv = luabind::call_function<int>(func, 42);
					result += rv;
				}
				++iteration_index;
				return result;
			})
			, nonius::benchmark("plain_c", [plain_L] {
				int result = 0;
				auto L = plain_L;
				static size_t iteration_index = 1;
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
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
				++iteration_index;
				return result;
			})
			,nonius::benchmark("sol_1.1.0", [&sol_lua] {
				int result = 0;
				static size_t iteration_index = 1;
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					sol::function test = sol_lua["test"];
					result += test.call<int>(42);
				}
				++iteration_index;
				return result;
			})
			, nonius::benchmark("sol_1.1.0_cached", [&sol_lua] {
				int result = 0;
				sol::function test = sol_lua["test"];
				static size_t iteration_index = 1;
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					result += test.call<int>(42);
				}
				++iteration_index;
				return result;
			})

		};

		nonius::go(cfg, std::begin(benchmarks), std::end(benchmarks), nonius::html_reporter());

		lua_close(plain_L);
		lua_close(luabind_L);
	}

}


int main() {
	bench_cfunction_from_lua::run();
}