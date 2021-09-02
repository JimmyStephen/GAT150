#pragma once
#include "Framework/System.h"
#include "Object/Object.h"
#include <functional>
#include <variant>
#include <string>
#include <list>
#include <map>

namespace nc
{
	struct Event
	{
		std::string name;
		Object* receiver{ nullptr };
		std::variant<int, bool, float, std::string, void*> data;
	};

	class EventSystem : public System
	{
	public:
		void Unsubscribe(const std::string& name, Object* receiver);
		using function_t = std::function<void(const Event&)>;

		virtual void Startup() override;
		virtual void Shutdown() override;
		virtual void Update(float dt) override;

		void Subscribe(const std::string& name, function_t function, Object* receiver = nullptr);
		void Notify(const Event& event);

	private:
		struct Observer
		{
			function_t function;
			Object* receiver;
		};

		std::map<std::string, std::list<Observer>> observers;

		void UpdateTitle(float dt);
		void UpdateStartLevel(float dt);
		void OnAddPoints(const nc::Event& event);
	};
}