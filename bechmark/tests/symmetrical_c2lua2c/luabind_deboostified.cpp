#include "../common.h"
#include "benchmark_core.h"
#include <luabind/luabind.hpp>

namespace luabind_noboost_benchmark {
	

	struct benchmark : public  benchmark_core::base_bechmark {
		benchmark() {
			auto L = lua_state;

			luabind::open(L);

			using namespace luabind;
			module(L)
			[
				def("simple_math", &benchmark_core::simple_math)
			];		

			reg(nonius::benchmark("luabind_raw", [L] {
				int result = 0;
				static size_t iteration_index = 1;
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					int rv = luabind::call_function<int>(L, "simple_math", 42);
					result += rv;
				}
				++iteration_index;
				return result;
			}));


			reg(nonius::benchmark("luabind_cache", [L] {
				int result = 0;
				static size_t iteration_index = 1;
				luabind::object func = luabind::globals(L)["simple_math"];//(42);
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					int rv = luabind::call_function<int>(func, 42);
					result += rv;
				}
				++iteration_index;
				return result;
			}));

			reg(nonius::benchmark("luabind_misused", [L] {
				int result = 0;
				static size_t iteration_index = 1;
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					luabind::object func = luabind::globals(L)["simple_math"];
					int rv = luabind::call_function<int>(func, 42);
					result += rv;
				}
				++iteration_index;
				return result;
			}));
		}

	} _benchmark;
}
