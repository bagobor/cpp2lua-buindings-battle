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
//#include "LuaIntf/LuaIntf.h"
//#include <impl/CppBindModule.h>

//#include <luapp/lua.hpp>
// LuaContext - conflicts with latest boost.
// luapp = won't build with VS2015
// sol - not compatible with vs2015, uses exceptions
// lua-intff - uses exceptions, must be build

//#include <boost/any.hpp>
//#include <boost/type_traits/add_rvalue_reference.hpp>



namespace bench_cfunction_from_lua {
	int test(int value) {
		//std::cout << value << std::endl;
		return value + 1;
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

		sel::State sel_state;
		sel_state["test"] = &test;

		//sol::state sol_lua;
		//sol_lua.set_function(std::string("test"), &test);
	
		LuaIntf::LuaContext luaintf(true);
		LuaIntf::LuaBinding(luaintf.state())
			.beginModule("luaintf")
				.addFunction("test", &test)
			.endModule();

		//auto LuaIntf = luaL_newstate();
		////LuaBinding(L).beginModule("utils")
		//LuaBinding(LuaIntf).beginModule("utils")
		//	//.addFunction("xpairs", &xpairs)
		//.endModule();

		//std::string script = 

		nonius::benchmark benchmarks[] = {
			nonius::benchmark("selene", [&sel_state] {
				int result = 0;
				for (size_t i = 0; i < 100000; ++i)
					result += (int)sel_state["test"](42);
				return result;
			})
			,nonius::benchmark("luaintf", [&luaintf] {
				int result = 0;
				LuaIntf::LuaRef func(luaintf.state(), "luaintf.test");
				for (size_t i = 0; i < 100000; ++i) {
					int rv = func.call<int, int>(42);
					//func(42);
					result += rv;
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
	}

}


int main() {
	bench_cfunction_from_lua::run();
}