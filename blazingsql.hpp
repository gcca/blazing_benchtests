#include "filesystem.hpp"

class BlazingSQLFileSystem : public FileSystem {
public:
	std::shared_ptr<arrow::Buffer> ReadFile(
		const std::string &, std::size_t) final;

	void CreateDirectory(const std::string &, const std::string &) final;

	void DeleteDirectory(const std::string &, const std::string &) final;

	void Move(
		const std::string &, const std::string &, const std::string &) final;
};


class BlazingSQLS3 : public FileSystem {
public:
  BlazingSQLS3();
	std::shared_ptr<arrow::Buffer> ReadFile(
		const std::string &, std::size_t) final;

	void CreateDirectory(const std::string &, const std::string &) final;

	void DeleteDirectory(const std::string &, const std::string &) final;

	void Move(
		const std::string &, const std::string &, const std::string &) final;
};
