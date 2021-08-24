#pragma once
#include "document.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include <string>

#define JSON_READ(value, data) 	nc::json::Get(value, #data, data);;

namespace nc
{
	namespace json
	{
		bool Load(const std::string& filename, rapidjson::Document& document);
		bool Get(const rapidjson::Value& value, const std::string& name, int& key);
	}
}