#include "benchmark_core.h"

#include <LuaContext.hpp>

namespace luawrapper_benchmark {

	struct benchmark : public  benchmark_core::base_bechmark {
		
		LuaContext context;

		benchmark() {

			context.writeVariable("simple_math", &benchmark_core::simple_math);
			//sol_state.set_function("simple_math", &benchmark_core::simple_math);

			reg(nonius::benchmark("luawrapper", [this] {
				auto& context = this->context;
				int result = 0;
				static size_t iteration_index = 1;
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {
					const auto test = context.readVariable<std::function<int(int)>>("simple_math");
					result += test(42);
				}
				++iteration_index;
				return result;
			}));

			reg(nonius::benchmark("luawrapper_cached", [this] {
				auto& context = this->context;
				int result = 0;
				static size_t iteration_index = 1;
				const auto test = context.readVariable<std::function<int(int)>>("simple_math");
				for (size_t i = 0, end = NUM_ITERATIONS*iteration_index; i < end; ++i) {					
					result += test(42);
				}
				++iteration_index;
				return result;
			}));
		}

	} _benchmark;
}
