#pragma once
#include "Engine.h"
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
		std::variant<int, bool, float, std::string> data;
	};

	class EventSystem : public System
	{
	public:
		using function_t = std::function<void(const Event&)>;

		virtual void Startup() override;
		virtual void Shutdown() override;
		virtual void Update(float dt) override;

		void Subscribe(const std::string& name, function_t function);
		void Notify(const Event& event);

	private:
		struct Observer
		{
			function_t function;
		};

		std::map<std::string, std::list<Observer>> observers;

		void UpdateTitle(float dt);
		void UpdateStartLevel(float dt);
		void OnAddPoints(const nc::Event& event);
	};
}