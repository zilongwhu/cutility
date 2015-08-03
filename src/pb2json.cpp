#include "pb2json.h"
#include <google/protobuf/descriptor.h>
#include <stdlib.h>
#include <string>

using namespace google::protobuf;

static cJSON *_pb2json(const Message& msg);
static cJSON *_field2json(const Message& msg, const FieldDescriptor *field, size_t index)
{
    const Reflection *ref = msg.GetReflection();
    const bool repeated = field->is_repeated();
    cJSON *jf = 0;
    switch (field->cpp_type())
    {
#define _CONVERT(type, ctype, fmt, sfunc, afunc)        \
        case FieldDescriptor::type: {           \
            const ctype value = (repeated)?     \
                ref->afunc(msg, field, index):  \
                ref->sfunc(msg, field);     \
            jf = fmt(value);            \
            break;                  \
        }

        _CONVERT(CPPTYPE_DOUBLE, double, cJSON_CreateNumber, GetDouble, GetRepeatedDouble);
        _CONVERT(CPPTYPE_FLOAT, double, cJSON_CreateNumber, GetFloat, GetRepeatedFloat);
        _CONVERT(CPPTYPE_INT64, double, cJSON_CreateNumber, GetInt64, GetRepeatedInt64);
        _CONVERT(CPPTYPE_UINT64, double, cJSON_CreateNumber, GetUInt64, GetRepeatedUInt64);
        _CONVERT(CPPTYPE_INT32, double, cJSON_CreateNumber, GetInt32, GetRepeatedInt32);
        _CONVERT(CPPTYPE_UINT32, double, cJSON_CreateNumber, GetUInt32, GetRepeatedUInt32);
        _CONVERT(CPPTYPE_BOOL, bool, cJSON_CreateBool, GetBool, GetRepeatedBool);
#undef _CONVERT
        case FieldDescriptor::CPPTYPE_STRING: {
            std::string scratch;
            const std::string &value = (repeated)?
                ref->GetRepeatedStringReference(msg, field, index, &scratch):
                ref->GetStringReference(msg, field, &scratch);
            if (field->type() == FieldDescriptor::TYPE_BYTES) {
                int outlen = (value.length() + 2)/3*4 + 1;
                std::string str(outlen, '\0');
                std_base64_encode(&str[0], &outlen, value.c_str(), value.length());
                jf = cJSON_CreateString(str.c_str());
            } else {
                jf = cJSON_CreateString(value.c_str());
            }
            break;
        }
        case FieldDescriptor::CPPTYPE_MESSAGE: {
            const Message& mf = (repeated)?
                ref->GetRepeatedMessage(msg, field, index):
                ref->GetMessage(msg, field);
            jf = _pb2json(mf);
            break;
        }
        case FieldDescriptor::CPPTYPE_ENUM: {
            const EnumValueDescriptor* ef = (repeated)?
                ref->GetRepeatedEnum(msg, field, index):
                ref->GetEnum(msg, field);

            jf = cJSON_CreateNumber(ef->number());
            break;
        }
        default:
            break;
    }
    return jf;
}

static cJSON *_pb2json(const Message& msg)
{
    const Descriptor *d = msg.GetDescriptor();
    const Reflection *ref = msg.GetReflection();
    if (!d || !ref) return 0;

    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;
    cJSON *jf = 0;
    bool ret = true;

    std::vector<const FieldDescriptor *> fields;
    ref->ListFields(msg, &fields);

    for (size_t i = 0; i != fields.size(); i++)
    {
        const FieldDescriptor *field = fields[i];

        jf = 0;
        if(field->is_repeated()) {
            size_t count = ref->FieldSize(msg, field);
            if (!count) continue;

            jf = cJSON_CreateArray();
            for (size_t j = 0; j < count; j++) {
                cJSON *aj = _field2json(msg, field, j);
                if (!aj) {
                    ret = false;
                    break;
                }
                cJSON_AddItemToArray(jf, aj);
            }
        } else if (ref->HasField(msg, field))
            jf = _field2json(msg, field, 0);
        else
            continue;

        const std::string &name = (field->is_extension())?field->full_name():field->name();
        cJSON_AddItemToObject(root, name.c_str(), jf);
    }
    if (ret) return root;
    if (jf) cJSON_Delete(jf);
    if (root) cJSON_Delete(root);
    return NULL;
}

bool pb2json(const Message &msg, std::string &json, bool fmt)
{
    cJSON *js = _pb2json(msg);
    if (js) {
        char *str = NULL;
        if (fmt) {
            str = cJSON_Print(js);
        } else {
            str = cJSON_PrintUnformatted(js);
        }
        cJSON_Delete(js);
        if (str) {
            json = str;
            ::free(str);
            return true;
        }
    }
    json.clear();
    return false;
}
