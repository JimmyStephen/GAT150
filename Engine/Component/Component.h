#pragma once
#include "Object/Object.h"
#include "Core/Serializable.h"

namespace nc {
	class Actor;

	class Component : public Object, public Serializable
	{
	public:
		virtual void Update() = 0;

	public:
		Actor* owner{ nullptr };

	};
}