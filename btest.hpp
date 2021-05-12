#include <experimental/filesystem>
#include <sstream>
#include <string>

#include <benchmark/benchmark.h>

#include <FileSystem/LocalFileSystem.h>

#include "arrow.hpp"
#include "blazingsql.hpp"

namespace std {
namespace filesystem = experimental::filesystem;
}

static const std::string tmpbenchdir{"/tmp/bench_arrow"};

template <class FS>
static void WithDirectory(benchmark::State & state,
	std::function<void(FS &, const std::string &)> && action) {
	std::filesystem::remove_all(tmpbenchdir);
	std::filesystem::create_directory(tmpbenchdir);

	std::size_t suffix = 0;
	for (auto _ : state) {
		const std::string dirname{"dir_" + std::to_string(suffix++)};
		std::filesystem::create_directory(tmpbenchdir + "/" + dirname);
		FS fs;
		action(fs, dirname);
	}

	state.counters["suffix"] = suffix;
	std::filesystem::remove_all(tmpbenchdir);
}
