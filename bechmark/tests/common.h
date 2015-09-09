#pragma once

#include <nonius/nonius_single.h++>

#include <list>
#include <forward_list>

#define LUA_COMPAT_APIINTCASTS

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <map>


struct bench_case {
	typedef std::list<nonius::benchmark> benchmarks_t;
	benchmarks_t m_benchmarks;

	inline benchmarks_t& benchmarks() { return m_benchmarks; }

	inline bench_case& add_benchmark(nonius::benchmark&& benchmark) {
		m_benchmarks.push_back(std::move(benchmark));
		return *this;
	}
};

class registry {
	typedef std::map<std::string, bench_case> cases_t;
	cases_t m_cases;
	static registry& Get() { // hate this but let's be fast and dirty for clarity
		static registry instance;
		return instance;
	}
public:
	static bench_case& get_case(const std::string& name) {
		return Get().m_cases[name];
	};

	static cases_t& get_cases() {
		return Get().m_cases;
	};
};

