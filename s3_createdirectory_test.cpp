#include "btest.hpp"

const std::string benchdir = "bdemo/base";

template <class FS>
static void S3_CreateDirectory(benchmark::State & state) {
	FS fs;
	std::size_t suffix = 0;
	for (auto _ : state) {
		const std::string dirname{"dir_" + std::to_string(suffix++)};
		fs.CreateDirectory(benchdir, dirname);
	}
	state.counters["suffix"] = suffix;
}

BENCHMARK_TEMPLATE(S3_CreateDirectory, ArrowS3);
