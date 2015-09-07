#pragma once

namespace benchmark_core {

	enum {
		NUM_ITERATIONS = 100000
	};

	int simple_math(int value) {
		return value + 1;
	}

	std::string lua_script_caller() {
		return "simple_math(42);";
	}

}