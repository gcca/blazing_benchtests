#include <string>

void S3CleanBucket(const std::string & bucketdir);
void S3CreateDirectory(const std::string & bucketdir, const std::string & name);
void S3CreateFile(const std::string & bucketdir,
	const std::string & name,
	const std::size_t datasize);
