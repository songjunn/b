#ifndef _CONVERT_H_
#define _CONVERT_H_

#include <stddef.h>
#include <string>

namespace google {
namespace protobuf {
	class Message;
}
}
typedef struct _bson_t bson_t;

// convert struct between bson and protobuf
void bson2pb(const bson_t &bson, google::protobuf::Message &msg);
void pb2bson(bson_t &bson, const google::protobuf::Message &msg);

// convert struct between json and protobuf
void json2pb(google::protobuf::Message &msg, const char *buf, size_t size);
std::string pb2json(const google::protobuf::Message &msg);

// convert struct between json and bson
void json2bson(bson_t &bson, const char *buf, size_t size);
std::string bson2json(const bson_t &bson);

#endif //_CONVERT_H_
