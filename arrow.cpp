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

arrow::fs::S3Options s3Options;

ArrowS3::ArrowS3() {
	arrow::fs::S3GlobalOptions s3GlobalOptions{arrow::fs::S3LogLevel::Off};
	arrow::Status status = arrow::fs::InitializeS3(s3GlobalOptions);
	s3Options.ConfigureAccessKey("randomstring", "randomstring");
	s3Options.scheme = "http";
	s3Options.endpoint_override = "127.0.0.1:9000";
}

std::shared_ptr<arrow::Buffer> ArrowS3::ReadFile(
	const std::string & base, const std::size_t datasize) {
	std::string::size_type index = base.find('/');
	const std::string bucketname = base.substr(0, index);
	const std::string name = base.substr(index + 1);

	std::shared_ptr<arrow::fs::S3FileSystem> s3fs =
		*arrow::fs::S3FileSystem::Make(s3Options);

	std::shared_ptr<arrow::fs::FileSystem> fs =
		std::make_shared<arrow::fs::SubTreeFileSystem>(bucketname, s3fs);

	std::shared_ptr<arrow::io::InputStream> inputStream =
		*fs->OpenInputStream(name);

	std::shared_ptr<arrow::Buffer> buffer = *inputStream->Read(datasize);

	return buffer;
}

void ArrowS3::CreateDirectory(
	const std::string & base, const std::string & name) {
	std::shared_ptr<arrow::fs::S3FileSystem> s3fs =
		*arrow::fs::S3FileSystem::Make(s3Options);

	std::shared_ptr<arrow::fs::FileSystem> fs =
		std::make_shared<arrow::fs::SubTreeFileSystem>(base, s3fs);

	arrow::Status status = fs->CreateDir(name);
}

void ArrowS3::DeleteDirectory(
	const std::string & base, const std::string & name) {
	std::shared_ptr<arrow::fs::S3FileSystem> s3fs =
		*arrow::fs::S3FileSystem::Make(s3Options);

	std::shared_ptr<arrow::fs::FileSystem> fs =
		std::make_shared<arrow::fs::SubTreeFileSystem>(base, s3fs);

	arrow::Status status =
		fs->DeleteDir(name);  // DeleteDir only delete content
}

void ArrowS3::Move(const std::string & base,
	const std::string & src,
	const std::string & dest) {
	std::shared_ptr<arrow::fs::S3FileSystem> s3fs =
		*arrow::fs::S3FileSystem::Make(s3Options);

	std::shared_ptr<arrow::fs::FileSystem> fs =
		std::make_shared<arrow::fs::SubTreeFileSystem>(base, s3fs);

	arrow::Status status = fs->Move(src, dest);
}
