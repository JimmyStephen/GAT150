#pragma once
#include <string>

class Resource
{
	virtual bool Load(const std::string& filename, void* data = nullptr) = 0;
};
