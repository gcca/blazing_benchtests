#include "aws_util.hpp"
#include "btest.hpp"

const std::string benchdir = "bdemo/base";

template <class FS>
static void S3_ReadFile(benchmark::State & state) {
	FS fs;
	const std::size_t datasize = state.range(0);
	const std::string filename = "demo";
	S3CreateFile(benchdir, filename, datasize);
	for (auto _ : state) {
		fs.ReadFile(benchdir + "/" + filename, datasize);
	}
	S3CleanBucket(benchdir);
}

static const std::size_t start = 1 << 20;
static const std::size_t end = 1 << 28;
BENCHMARK_TEMPLATE(S3_ReadFile, ArrowS3)->Range(start, end);
BENCHMARK_TEMPLATE(S3_ReadFile, BlazingSQLS3)->Range(start, end);
