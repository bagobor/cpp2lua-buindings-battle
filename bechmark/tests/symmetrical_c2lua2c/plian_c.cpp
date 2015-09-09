#include "benchmark_core.h"


namespace plain_c_benchmark {

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
		int result = benchmark_core::simple_math(value);
		lua_pushnumber(L, result);
		return 1;
	}

	struct benchmark : public  benchmark_core::base_bechmark {
		
		benchmark() {

			lua_register(lua_state, "simple_math", &plain_lua_test_wrapper);

			reg(nonius::benchmark("plain_c", [this] {
				int result = 0;
				auto L = this->lua_state;
				static size_t iteration_index = 1;
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					// the function name
					lua_getglobal(L, "simple_math");
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
			}));

		}

	} _benchmark;
}
