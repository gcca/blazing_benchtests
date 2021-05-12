#include "blazingsql.hpp"

#include <arrow/result.h>

#include <FileSystem/LocalFileSystem.h>

static const std::string localname = "/proc/self/fd";

std::shared_ptr<arrow::Buffer> BlazingSQLFileSystem::ReadFile(
	const std::string & name, const std::size_t datasize) {
	Path localpath(localname);

	std::unique_ptr<FileSystemInterface> fs =
		std::make_unique<LocalFileSystem>(localpath);

	Uri baseUri{"/" + name};
	std::shared_ptr<arrow::io::InputStream> inputStream =
		fs->openReadable(baseUri);

	std::shared_ptr<arrow::Buffer> buffer = *inputStream->Read(datasize);

	return buffer;
}

void BlazingSQLFileSystem::CreateDirectory(
	const std::string & base, const std::string & name) {
	Path localpath(base);
	std::unique_ptr<FileSystemInterface> fs =
		std::make_unique<LocalFileSystem>(localpath);

	Uri uriDir{"/" + name};
	fs->makeDirectory(uriDir);
}

void BlazingSQLFileSystem::DeleteDirectory(
	const std::string & base, const std::string & name) {
	Path localpath(base);
	std::unique_ptr<FileSystemInterface> fs =
		std::make_unique<LocalFileSystem>(localpath);

	Uri uriDir{"/" + name};
	fs->remove(uriDir);
}

void BlazingSQLFileSystem::Move(const std::string & base,
	const std::string & from,
	const std::string & to) {
	Path localpath(base);
	std::unique_ptr<FileSystemInterface> fs =
		std::make_unique<LocalFileSystem>(localpath);

	Uri fromUri{"/" + from};
	Uri toUri{"/" + to};
	fs->move(fromUri, toUri);
}

std::shared_ptr<arrow::Buffer> BlazingSQLS3::ReadFile(
	const std::string &, const std::size_t) {
	return nullptr;
}

void BlazingSQLS3::CreateDirectory(const std::string &, const std::string &) {}

void BlazingSQLS3::DeleteDirectory(const std::string &, const std::string &) {}

void BlazingSQLS3::Move(
	const std::string &, const std::string &, const std::string &) {}
