#include "benchmark_core.h"

#include <sol.hpp>

namespace sol_benchmark {

	struct benchmark : public  benchmark_core::base_bechmark {
		sol::state sol_state;
		benchmark() {

			sol_state.set_function("simple_math", &benchmark_core::simple_math);

			reg(nonius::benchmark("sol_1.1.0", [this] {
				auto& sol_lua = this->sol_state;
				int result = 0;
				static size_t iteration_index = 1;
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					sol::function test = sol_lua["simple_math"];
					result += test.call<int>(42);
				}
				++iteration_index;
				return result;
			}));

			reg(nonius::benchmark("sol_1.1.0_cached", [this] {
				auto& sol_lua = this->sol_state;
				int result = 0;
				sol::function test = sol_lua["simple_math"];
				static size_t iteration_index = 1;
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					result += test.call<int>(42);
				}
				++iteration_index;
				return result;
			}));
		}

	} _benchmark;
}
