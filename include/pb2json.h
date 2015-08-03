#ifndef __CUTILITY_PB2JSON_H__
#define __CUTILITY_PB2JSON_H__

#include "cJSON.h"
#include "encode_util.h"
#include <google/protobuf/message.h>

bool pb2json(const google::protobuf::Message &msg, std::string &json, bool fmt = false);

#endif

