//#define NONIUS_RUNNER
#include <nonius/nonius_single.h++>

#include <list>
#include <forward_list>

#include "tests/common.h"


int main() {
	nonius::configuration cfg;
	cfg.output_file = "cfunction_from_lua";
	cfg.sub_title = "generated with <em>misused</em>  <a href = \"http://flamingdangerzone.com/nonius\"> nonius </a>";
	cfg.graph_type = nonius::configuration::line;

	cfg.samples = 10;
	cfg.verbose = true;
	cfg.summary = true;
	//double confidence_interval = 0.95;
	//int resamples = 100000;
	cfg.title = "cfunction_from_lua x 100000 x iteration";

	cfg.list_benchmarks = true;
	cfg.summary = true;

	std::string filename = "hello.html";


	auto& bench_cases = registry::get_cases();
	for (auto &p : bench_cases) {
		auto &name = p.first;
		auto &benchmarks = p.second.benchmarks();

		cfg.title = name + " x 100000 x iteration";
		cfg.output_file = name;

		//nonius::go(cfg, std::begin(benchmarks), std::end(benchmarks), nonius::html_reporter());
		nonius::go(cfg, benchmarks.begin(), benchmarks.end(), nonius::html_reporter());
	}
	
}