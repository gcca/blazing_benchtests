#include "aws_util.hpp"
#include "btest.hpp"

const std::string benchdir = "bdemo/base";

template <class FS>
static void S3_Delete(benchmark::State & state) {
	FS fs;
	std::size_t suffix = 0;
	for (auto _ : state) {
		const std::string dirname{"dir_" + std::to_string(suffix++) + "/"};
		S3CreateDirectory(benchdir, dirname);
		fs.DeleteDirectory(benchdir, dirname);
	}
	state.counters["suffix"] = suffix;
	S3CleanBucket(benchdir);
}

BENCHMARK_TEMPLATE(S3_Delete, ArrowS3);
BENCHMARK_TEMPLATE(S3_Delete, BlazingSQLS3);
