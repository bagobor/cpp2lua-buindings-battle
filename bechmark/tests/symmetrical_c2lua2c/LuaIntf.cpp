#include "benchmark_core.h"

#define LUAINTF_HEADERS_ONLY 1
#include <LuaIntf.h>

namespace LuaIntf_benchmark {

	struct benchmark : public  benchmark_core::base_bechmark {
		LuaIntf::LuaContext luaintf_state;
		benchmark() : luaintf_state(lua_state) {

			LuaIntf::LuaBinding(luaintf_state.state())
				.beginModule("luaintf")
					.addFunction("simple_math", &benchmark_core::simple_math)
				.endModule();

			reg(nonius::benchmark("luaintf_cached", [this] {
				auto & luaintf = this->luaintf_state;
				int result = 0;
				static size_t iteration_index = 1;
				LuaIntf::LuaRef func(luaintf.state(), "luaintf.simple_math");
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					int rv = func.call<int, int>(42);
					result += rv;
				}
				++iteration_index;
				return result;
			}));
			reg(nonius::benchmark("luaintf", [this] {
				auto & luaintf = this->luaintf_state;
				int result = 0;
				static size_t iteration_index = 1;
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					LuaIntf::LuaRef func(luaintf.state(), "luaintf.simple_math");
					int rv = func.call<int, int>(42);
					result += rv;
				}
				++iteration_index;
				return result;
			}));

		}

	} _benchmark;
}
