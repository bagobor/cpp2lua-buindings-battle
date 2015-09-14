#include "benchmark_core.h"
#include <selene.h>

namespace selene_benchmark {

	struct benchmark : public  benchmark_core::base_bechmark {
		sel::State sel_state;
		benchmark() : sel_state(lua_state) {

			sel_state["simple_math"] = &benchmark_core::simple_math;

			reg(nonius::benchmark("selene", [this] {
				auto &sel_state = this->sel_state;
				int result = 0;
				static size_t iteration_index = 1;
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					result += (int)sel_state["simple_math"](42);
				}
				++iteration_index;
				return result;
			}));

			reg(nonius::benchmark("selene_cached", [this] {
				auto &sel_state = this->sel_state;
				int result = 0;
				static size_t iteration_index = 1;
				auto func = sel_state["simple_math"];
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					result += (int)func(42);
				}
				++iteration_index;
				return result;
			}));

		}

	} _benchmark;
}
