#include "btest.hpp"

static inline std::string CreateBenchFile(const std::size_t size) {
	std::FILE * tmpf = std::tmpfile();
	std::string data;
	data.resize(size, 1);
	std::fputs(data.c_str(), tmpf);
	std::rewind(tmpf);
	std::fflush(tmpf);
	return std::to_string(fileno(tmpf));
}

template <class FS>
static void ReadLocalFile(benchmark::State & state) {
	const std::size_t datasize = state.range(0);
	const std::string filename = CreateBenchFile(datasize);
	for (auto _ : state) {
		FS fs;
		fs.ReadFile(filename, datasize);
	}
}

static const std::size_t start = 1 << 20;
static const std::size_t end = 1 << 28;
BENCHMARK_TEMPLATE(ReadLocalFile, ArrowFileSystem)->Range(start, end);
BENCHMARK_TEMPLATE(ReadLocalFile, BlazingSQLFileSystem)->Range(start, end);
