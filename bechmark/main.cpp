// #include <nonius/nonius.h++>

// #include <iterator>
// #include <string>

// int main() {
//     nonius::configuration cfg;
//     nonius::benchmark_registry benchmarks = {
//         nonius::benchmark("to_string(42)", []{ return std::to_string(42); }),
//         nonius::benchmark("to_string(4.2)", []{ return std::to_string(4.2); }),
//     };

//     nonius::go(cfg, benchmarks);
// }
#define NONIUS_RUNNER
#include <nonius/nonius.h++>

#include <list>
#include <forward_list>

NONIUS_BENCHMARK("list<long long>", [](nonius::chronometer meter) {
    std::list<long long> l;
    meter.measure([&](int i) { l.push_front(i); });
})

NONIUS_BENCHMARK("list<char>", [](nonius::chronometer meter) {
    std::list<char> l;
    meter.measure([&](int i) { l.push_front(i); });
})

NONIUS_BENCHMARK("forward_list<long long>", [](nonius::chronometer meter) {
    std::forward_list<long long> l;
    meter.measure([&](int i) { l.push_front(i); });
})

NONIUS_BENCHMARK("forward_list<char>", [](nonius::chronometer meter) {
    std::forward_list<char> l;
    meter.measure([&](int i) { l.push_front(static_cast<char>(i)); });
})