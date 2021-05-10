#include "arrow.hpp"

#include <arrow/filesystem/api.h>
#include <arrow/result.h>
#include <arrow/status.h>

static const std::string localname = "/proc/self/fd";

std::shared_ptr<arrow::Buffer> ArrowFileSystem::ReadFile(
	const std::string & name, const std::size_t datasize) {
	std::shared_ptr<arrow::fs::LocalFileSystem> localfs =
		std::make_shared<arrow::fs::LocalFileSystem>();

	std::shared_ptr<arrow::fs::FileSystem> fs =
		std::make_shared<arrow::fs::SubTreeFileSystem>(localname, localfs);

	std::shared_ptr<arrow::io::InputStream> inputStream =
		*fs->OpenInputStream(name);

	std::shared_ptr<arrow::Buffer> buffer = *inputStream->Read(datasize);

	return buffer;
}

void ArrowFileSystem::CreateDirectory(
	const std::string & base, const std::string & name) {
	std::shared_ptr<arrow::fs::LocalFileSystem> localfs =
		std::make_shared<arrow::fs::LocalFileSystem>();
	std::shared_ptr<arrow::fs::FileSystem> fs =
		std::make_shared<arrow::fs::SubTreeFileSystem>(base, localfs);

	arrow::Status status = fs->CreateDir(name);

	// if (!status.ok()) {
	// std::cout << "Error: " << status.message() << std::endl;
	//}
}

void ArrowFileSystem::DeleteDirectory(
	const std::string & base, const std::string & name) {
	std::shared_ptr<arrow::fs::LocalFileSystem> localfs =
		std::make_shared<arrow::fs::LocalFileSystem>();
	std::shared_ptr<arrow::fs::FileSystem> fs =
		std::make_shared<arrow::fs::SubTreeFileSystem>(base, localfs);

	arrow::Status status = fs->DeleteDir(name);
}

void ArrowFileSystem::Move(const std::string & base,
	const std::string & from,
	const std::string & to) {
	std::shared_ptr<arrow::fs::LocalFileSystem> localfs =
		std::make_shared<arrow::fs::LocalFileSystem>();
	std::shared_ptr<arrow::fs::FileSystem> fs =
		std::make_shared<arrow::fs::SubTreeFileSystem>(base, localfs);

	arrow::Status status = fs->Move(from, to);
}
