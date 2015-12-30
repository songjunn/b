#include "convert.h"
#include "types.h"
#include "bson.h"
#include "google/protobuf/message.h"
#include "google/protobuf/descriptor.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace std;
using namespace rapidjson;
using google::protobuf::Message;
using google::protobuf::MessageFactory;
using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;
using google::protobuf::EnumDescriptor;
using google::protobuf::EnumValueDescriptor;
using google::protobuf::Reflection;

void _pb2json(const Message &msg, Value &json, Document &doc)
{
	const Reflection *reflection = msg.GetReflection();
	const Descriptor *descriptor = msg.GetDescriptor();

	for(int i = 0; i < descriptor->field_count(); i++){

		FieldDescriptor *field = (FieldDescriptor *)descriptor->field(i);

		if (field->is_repeated()) {
			Value jsonValue;
			jsonValue.SetArray();
			switch(field->cpp_type())
			{
			case FieldDescriptor::CPPTYPE_INT32:
				for(int fieldNum = 0; fieldNum < reflection->FieldSize(msg, field); fieldNum++){
					jsonValue.PushBack((int)reflection->GetRepeatedInt32(msg, field, fieldNum), doc.GetAllocator());
				}
				break;
			case FieldDescriptor::CPPTYPE_INT64:
				for(int fieldNum = 0; fieldNum < reflection->FieldSize(msg, field); fieldNum++){
					jsonValue.PushBack((int64)reflection->GetRepeatedInt64(msg, field, fieldNum), doc.GetAllocator());
				}
				break;
			case FieldDescriptor::CPPTYPE_STRING:
				for(int fieldNum = 0; fieldNum < reflection->FieldSize(msg, field); fieldNum++){
					jsonValue.PushBack(reflection->GetRepeatedString(msg, field, fieldNum).c_str(), doc.GetAllocator());
				}
				break;
			case FieldDescriptor::CPPTYPE_BOOL:
				for(int fieldNum = 0; fieldNum < reflection->FieldSize(msg, field); fieldNum++){
					jsonValue.PushBack(reflection->GetRepeatedBool(msg, field, fieldNum), doc.GetAllocator());
				}
				break;
			case FieldDescriptor::CPPTYPE_DOUBLE:
				for(int fieldNum = 0; fieldNum < reflection->FieldSize(msg, field); fieldNum++){
					jsonValue.PushBack(reflection->GetRepeatedDouble(msg, field, fieldNum), doc.GetAllocator());
				}
				break;
			case FieldDescriptor::CPPTYPE_MESSAGE:
				{
					for(int fieldNum = 0; fieldNum < reflection->FieldSize(msg, field); fieldNum++){
						Value jsonMsg;
						jsonMsg.SetObject();
						_pb2json(reflection->GetRepeatedMessage(msg, field, fieldNum), jsonMsg, doc);
						jsonValue.PushBack(jsonMsg, doc.GetAllocator());
					}
				}
				break;
			default:
				break;
			}
			json.AddMember(field->name().c_str(), doc.GetAllocator(), jsonValue, doc.GetAllocator());
		} else {
			Value jsonValue;
			switch(field->cpp_type())
			{
			case FieldDescriptor::CPPTYPE_INT32:
				jsonValue.SetInt(reflection->GetInt32(msg, field));
				break;
			case FieldDescriptor::CPPTYPE_INT64:
				jsonValue.SetInt64(reflection->GetInt64(msg, field));
				break;
			case FieldDescriptor::CPPTYPE_STRING:
				jsonValue.SetString(reflection->GetString(msg, field).c_str());
				break;
			case FieldDescriptor::CPPTYPE_BOOL:
				jsonValue.SetBool(reflection->GetBool(msg, field));
				break;
			case FieldDescriptor::CPPTYPE_DOUBLE:
				jsonValue.SetDouble(reflection->GetDouble(msg, field));
				break;
			case FieldDescriptor::CPPTYPE_MESSAGE:
				{
					jsonValue.SetObject();
					_pb2json(reflection->GetMessage(msg, field), jsonValue, doc);
				}
				break;
			default:
				break;
			}
			json.AddMember(field->name().c_str(), doc.GetAllocator(), jsonValue, doc.GetAllocator());
		}
	}
}

string pb2json(const Message &msg)
{
	Document jsonDoc;
	jsonDoc.SetObject();
	_pb2json(msg, jsonDoc, jsonDoc);

	string jsonStr;
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	jsonDoc.Accept(writer);
	jsonStr.assign(buffer.GetString(), buffer.Size());

	return jsonStr;
}