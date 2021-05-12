#include "btest.hpp"

template <class FS>
static void DeleteDirectory(benchmark::State & state) {
	WithDirectory<FS>(state, [](FS & fs, const std::string & dirname) {
		fs.DeleteDirectory(tmpbenchdir, dirname);
	});
}

BENCHMARK_TEMPLATE(DeleteDirectory, ArrowFileSystem);
BENCHMARK_TEMPLATE(DeleteDirectory, BlazingSQLFileSystem);
