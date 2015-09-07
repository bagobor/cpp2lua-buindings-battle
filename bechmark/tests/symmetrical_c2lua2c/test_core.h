#pragma once

namespace benchmark_core {

	int simple_math(int value) {
		return value + 1;
	}

	std::string lua_script_caller() {
		return "simple_math(42);";
	}

}