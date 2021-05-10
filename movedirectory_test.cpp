#include "btest.hpp"

static const std::string tmpbenchdir{"/tmp/bench_arrow"};

template <class FS>
static void Move(benchmark::State & state) {
	std::filesystem::remove_all(tmpbenchdir);
	std::filesystem::create_directory(tmpbenchdir);

	std::size_t suffix = 0;
	for (auto _ : state) {
		const std::string dirname{"dir_" + std::to_string(suffix++)};
		std::filesystem::create_directory(tmpbenchdir + "/" + dirname);
		FS fs;
		fs.Move(tmpbenchdir, dirname, dirname + "_t");
	}

	state.counters["suffix"] = suffix;
	std::filesystem::remove_all(tmpbenchdir);
}

BENCHMARK_TEMPLATE(Move, ArrowFileSystem);
BENCHMARK_TEMPLATE(Move, BlazingSQLFileSystem);
