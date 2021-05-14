#include "btest.hpp"
#include "aws_util.hpp"

const std::string benchdir = "bdemo/base";

template <class FS>
static void S3_Move(benchmark::State & state) {
	FS fs;
	std::size_t suffix = 0;
	for (auto _ : state) {
		const std::string dirname{"dir_" + std::to_string(suffix++)};
    S3CreateDirectory(benchdir, dirname);
		fs.Move(benchdir, dirname, dirname + "_t");
	}
	state.counters["suffix"] = suffix;
	S3CleanBucket(benchdir);
}

BENCHMARK_TEMPLATE(S3_Move, ArrowS3);
BENCHMARK_TEMPLATE(S3_Move, BlazingSQLS3);
