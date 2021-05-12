#include "filesystem.hpp"

class ArrowFileSystem : public FileSystem {
public:
	std::shared_ptr<arrow::Buffer> ReadFile(
		const std::string &, std::size_t) final;

	void CreateDirectory(const std::string &, const std::string &) final;

	void DeleteDirectory(const std::string &, const std::string &) final;

	void Move(
		const std::string &, const std::string &, const std::string &) final;
};

class ArrowS3 : public FileSystem {
public:
  ArrowS3();
	std::shared_ptr<arrow::Buffer> ReadFile(
		const std::string &, std::size_t) final;

	void CreateDirectory(const std::string &, const std::string &) final;

	void DeleteDirectory(const std::string &, const std::string &) final;

	void Move(
		const std::string &, const std::string &, const std::string &) final;
};
