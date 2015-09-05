// #include <nonius/nonius.h++>

// #include <iterator>
// #include <string>

//int main() {
//     nonius::configuration cfg;
//     nonius::benchmark_registry benchmarks = {
//         nonius::benchmark("to_string(42)", []{ return std::to_string(42); }),
//         nonius::benchmark("to_string(4.2)", []{ return std::to_string(4.2); }),
//     };

//     nonius::go(cfg, benchmarks);
//}
//#define NONIUS_RUNNER
#include <nonius/nonius_single.h++>

#include <list>
#include <forward_list>

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

#include <selene.h>

//#include <LuaIntf.h>
//#include <impl/CppBindModule.h>

//#include <luapp/lua.hpp>
// LuaContext - conflicts with latest boost.
// luapp = won't build with VS2015
// lua-intf

//#include <boost/any.hpp>
//#include <boost/type_traits/add_rvalue_reference.hpp>

namespace bench_cfunction_from_lua {
	int show(int value) {
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

		sel::State sel_state;
		sel_state["show"] = &show;


		//auto LuaIntf = luaL_newstate();
		////LuaBinding(L).beginModule("utils")
		//LuaBinding(LuaIntf).beginModule("utils")
		//	//.addFunction("xpairs", &xpairs)
		//.endModule();

		nonius::benchmark benchmarks[] = {
			nonius::benchmark("show(42)", [&sel_state] {
				int result = sel_state["show"](42);
				return result;
			})
			//,nonius::benchmark("to_string(4.2)", [] { return std::to_string(4.2); })
		};

		nonius::go(cfg, std::begin(benchmarks), std::end(benchmarks), nonius::html_reporter());
	}

}


int main() {
	bench_cfunction_from_lua::run();
	//nonius::configuration cfg;
	//cfg.output_file = "test_results.html";
	//nonius::benchmark benchmarks[] = {
	//	nonius::benchmark("to_string(42)", [] { return std::to_string(42); }),
	//	nonius::benchmark("to_string(4.2)", [] { return std::to_string(4.2); })
	//};

	//nonius::go(cfg, std::begin(benchmarks), std::end(benchmarks), nonius::html_reporter());
}
//
//NONIUS_BENCHMARK("list<long long>", [](nonius::chronometer meter) {
//    std::list<long long> l;
//    meter.measure([&](int i) { l.push_front(i); });
//})
//
//NONIUS_BENCHMARK("list<char>", [](nonius::chronometer meter) {
//    std::list<char> l;
//    meter.measure([&](int i) { l.push_front(i); });
//})
//
//NONIUS_BENCHMARK("forward_list<long long>", [](nonius::chronometer meter) {
//    std::forward_list<long long> l;
//    meter.measure([&](int i) { l.push_front(i); });
//})
//
//NONIUS_BENCHMARK("forward_list<char>", [](nonius::chronometer meter) {
//    std::forward_list<char> l;
//    meter.measure([&](int i) { l.push_front(static_cast<char>(i)); });
//})