#include <fstream>

#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentials.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/client/RetryStrategy.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/DeleteObjectsRequest.h>
#include <aws/s3/model/ListObjectsRequest.h>
#include <aws/s3/model/PutObjectRequest.h>

#include "aws_util.hpp"

static inline Aws::S3::S3Client MakeS3Client() {
	const std::string std_accesskey{"randomstring"};
	const std::string std_secretkey{"randomstring"};
	const std::string std_endpointOverride{"127.0.0.1:9000"};

	Aws::String accesskey{std_accesskey.cbegin(), std_accesskey.cend()};
	Aws::String secretkey{std_secretkey.cbegin(), std_secretkey.cend()};
	Aws::String endpointOverride{
		std_endpointOverride.cbegin(), std_endpointOverride.cend()};

	Aws::Client::ClientConfiguration clientConfiguration;
	clientConfiguration.endpointOverride = endpointOverride;
	clientConfiguration.scheme = Aws::Http::Scheme::HTTP;
	Aws::Auth::AWSCredentials credentials = {accesskey, secretkey};

	Aws::S3::S3Client s3Client(credentials,
		clientConfiguration,
		Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Never,
		false);

	return s3Client;
}

void S3CleanBucket(const std::string & bucketdir) {
	std::string::size_type index = bucketdir.find('/');
	const std::string std_bucketName = bucketdir.substr(0, index);
	const std::string subdir = bucketdir.substr(index + 1) + "/";

	Aws::S3::S3Client s3Client = MakeS3Client();

	const Aws::String bucketName(
		std_bucketName.cbegin(), std_bucketName.cend());

	Aws::S3::Model::ListObjectsRequest request;
	request.WithBucket(bucketName);

	Aws::S3::Model::ListObjectsOutcome outcome = s3Client.ListObjects(request);

	Aws::Vector<Aws::S3::Model::Object> objects =
		outcome.GetResult().GetContents();

	Aws::S3::Model::Delete Delete;
	for (Aws::S3::Model::Object & object : objects) {
		if (!object.GetKey().rfind(subdir, 0)) {
			Aws::S3::Model::ObjectIdentifier objectIdentifier;
			objectIdentifier.WithKey(object.GetKey());
			Delete.AddObjects(objectIdentifier);
		}
	}

	Aws::S3::Model::DeleteObjectsRequest deleteObjectsRequest;
	deleteObjectsRequest.WithBucket(bucketName).WithDelete(Delete);

	s3Client.DeleteObjects(deleteObjectsRequest);
}

void S3CreateDirectory(
	const std::string & bucketdir, const std::string & name) {
	std::string::size_type index = bucketdir.find('/');
	const std::string std_bucketName = bucketdir.substr(0, index);
	const std::string subdir = bucketdir.substr(index + 1) + "/" + name;

	Aws::S3::S3Client s3Client = MakeS3Client();

	const Aws::String bucketName{
		std_bucketName.cbegin(), std_bucketName.cend()};

	Aws::String key{subdir.cbegin(), subdir.cend()};

	Aws::S3::Model::PutObjectRequest putObjectRequest;
	putObjectRequest.WithBucket(bucketName).WithKey(key);

	s3Client.PutObject(putObjectRequest);
}

void S3CreateFile(const std::string & bucketdir,
	const std::string & name,
	const std::size_t datasize) {
	std::string::size_type index = bucketdir.find('/');
	const std::string std_bucketName = bucketdir.substr(0, index);
	const std::string subdir = bucketdir.substr(index + 1) + "/" + name;

	Aws::S3::S3Client s3Client = MakeS3Client();

	const Aws::String bucketName{
		std_bucketName.cbegin(), std_bucketName.cend()};

	Aws::String key{subdir.cbegin(), subdir.cend()};

	Aws::S3::Model::PutObjectRequest putObjectRequest;
	putObjectRequest.WithBucket(bucketName).WithKey(key);

	Aws::String payload;
	payload.resize(datasize, '7');

	std::shared_ptr<Aws::IOStream> body =
		Aws::MakeShared<Aws::StringStream>("AllocationTag", payload);

	putObjectRequest.SetBody(body);

	s3Client.PutObject(putObjectRequest);
}
