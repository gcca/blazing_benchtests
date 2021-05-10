#pragma once

#include <arrow/buffer.h>

class FileSystem {
public:
	virtual std::shared_ptr<arrow::Buffer> ReadFile(
		const std::string & name, std::size_t datasize) = 0;

	virtual void CreateDirectory(
		const std::string & base, const std::string & name) = 0;

	virtual void DeleteDirectory(
		const std::string & base, const std::string & name) = 0;

	virtual void Move(const std::string &,
		const std::string & from,
		const std::string & to) = 0;
};
