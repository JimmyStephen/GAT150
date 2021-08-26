#pragma once
#include "Json.h"

namespace nc
{
	class Serializable
	{
	public:
		virtual ~Serializable() {};
		virtual bool Write(const rapidjson::Value& value) const = 0;
		virtual bool Read(const rapidjson::Value& value) = 0;
	};
}