#pragma once

#include "../common.h"

namespace benchmark_core {

	int simple_math(int value);

	std::string lua_test_caller();


	// simple helper to reduce benchmarks code as possible
	class base_bechmark {
	protected:
		base_bechmark();
		~base_bechmark();
		
		bench_case& get_case();

		void reg(nonius::benchmark&& benchmark);

		enum {
			NUM_ITERATIONS = 100000
		};

		lua_State* lua_state = nullptr;
	};

}