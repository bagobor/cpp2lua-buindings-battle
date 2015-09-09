#pragma once

#include "benchmark_core.h"

namespace benchmark_core {

	int simple_math(int value) {
		return value + 1;
	}

	std::string lua_test_caller() {
		return "simple_math(42);";
	}

	base_bechmark::base_bechmark() {
		lua_state = luaL_newstate();
		luaL_openlibs(lua_state);
	}

	base_bechmark::~base_bechmark() {
		lua_close(lua_state);
	}

		
	bench_case& base_bechmark::get_case() {
		return registry::get_case("cfunction_from_lua");
	}

	void base_bechmark::reg(nonius::benchmark&& benchmark) {
		get_case().add_benchmark(std::move(benchmark));
	}
}