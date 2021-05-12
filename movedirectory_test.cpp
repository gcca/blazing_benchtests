#include "btest.hpp"

template <class FS>
static void Move(benchmark::State & state) {
	WithDirectory<FS>(state, [](FS & fs, const std::string & dirname) {
		fs.Move(tmpbenchdir, dirname, dirname + "_t");
	});
}

BENCHMARK_TEMPLATE(Move, ArrowFileSystem);
BENCHMARK_TEMPLATE(Move, BlazingSQLFileSystem);
