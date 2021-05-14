#include "blazingsql.hpp"

#include <arrow/result.h>
#include <aws/core/Aws.h>

#include <FileSystem/LocalFileSystem.h>
#include <FileSystem/S3FileSystem.h>

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


BlazingSQLS3::BlazingSQLS3() {
	Aws::SDKOptions awsOptions;
	Aws::InitAPI(awsOptions);
}

std::shared_ptr<arrow::Buffer> BlazingSQLS3::ReadFile(
	const std::string & base, const std::size_t datasize) {
	std::string::size_type index = base.find('/');
	const std::string bucketname = base.substr(0, index);
	const std::string name = base.substr(index + 1);

	FileSystemConnection fileSystemConnection(bucketname,
		S3FileSystemConnection::EncryptionType::NONE,
		"",
		"randomstring",
		"randomstring",
		"",
		"http://127.0.0.1:9000");

	Path localpath(base);
	std::unique_ptr<FileSystemInterface> fs =
		std::make_unique<S3FileSystem>(fileSystemConnection);

	Uri baseUri{"/" + name};
	std::shared_ptr<arrow::io::InputStream> inputStream =
		fs->openReadable(baseUri);

	std::shared_ptr<arrow::Buffer> buffer = *inputStream->Read(datasize);

	return buffer;
}

void BlazingSQLS3::CreateDirectory(
	const std::string & base, const std::string & name) {
	std::string::size_type index = base.find('/');

	const std::string bucketname = base.substr(0, index);
	const std::string subdirs = base.substr(index);

	FileSystemConnection fileSystemConnection(bucketname,
		S3FileSystemConnection::EncryptionType::NONE,
		"",
		"randomstring",
		"randomstring",
		"",
		"http://127.0.0.1:9000");

	Path localpath(base);
	std::unique_ptr<FileSystemInterface> fs =
		std::make_unique<S3FileSystem>(fileSystemConnection);

	Uri uriDir{subdirs + "/" + name + "/"};
	fs->makeDirectory(uriDir);
}

void BlazingSQLS3::DeleteDirectory(
	const std::string & base, const std::string & name) {
	std::string::size_type index = base.find('/');

	const std::string bucketname = base.substr(0, index);
	const std::string subdirs = base.substr(index);

	FileSystemConnection fileSystemConnection(bucketname,
		S3FileSystemConnection::EncryptionType::NONE,
		"",
		"randomstring",
		"randomstring",
		"",
		"http://127.0.0.1:9000");

	Path localpath(base);
	std::unique_ptr<FileSystemInterface> fs =
		std::make_unique<S3FileSystem>(fileSystemConnection);

	Uri uriDir{subdirs + "/" + name + "/"};
	fs->remove(uriDir);
}

void BlazingSQLS3::Move(const std::string & base,
	const std::string & src,
	const std::string & dest) {
	std::string::size_type index = base.find('/');

	const std::string bucketname = base.substr(0, index);
	const std::string subdir = base.substr(index);

	FileSystemConnection fileSystemConnection(bucketname,
		S3FileSystemConnection::EncryptionType::NONE,
		"",
		"randomstring",
		"randomstring",
		"",
		"http://127.0.0.1:9000");

	Path localpath(base);
	std::unique_ptr<FileSystemInterface> fs =
		std::make_unique<S3FileSystem>(fileSystemConnection);

	Uri srcUri{subdir + "/" + src};
	Uri dstUri{subdir + "/" + dest + "/"};
	fs->move(srcUri, dstUri);
}
