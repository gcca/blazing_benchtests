#include "btest.hpp"

template <class FS>
static void CreateDirectory(benchmark::State & state) {
	std::filesystem::remove_all(tmpbenchdir);
	std::filesystem::create_directory(tmpbenchdir);

	std::size_t suffix = 0;
	for (auto _ : state) {
		const std::string dirname{"dir_" + std::to_string(suffix++)};
		FS fs;
		fs.CreateDirectory(tmpbenchdir, dirname);
	}

	state.counters["suffix"] = suffix;
	std::filesystem::remove_all(tmpbenchdir);
}

BENCHMARK_TEMPLATE(CreateDirectory, ArrowFileSystem);
BENCHMARK_TEMPLATE(CreateDirectory, BlazingSQLFileSystem);
