#pragma once
#include <string>

namespace nc
{
	class Resource
	{
		virtual bool Load(const std::string& filename, void* data = nullptr) = 0;
	};
}