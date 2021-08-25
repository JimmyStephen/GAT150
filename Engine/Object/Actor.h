#pragma once
#include "Object.h"
#include "Math/Transform.h"
#include "Component/Component.h"
#include "Core/Serializable.h"
#include <vector>
#include <memory>

namespace nc
{
	class Scene;
	class Renderer;

	class Actor : public Object, public Serializable
	{
	public:
		Actor() {}
		Actor(const Transform& transform) : transform{ transform }{}

		virtual void Update(float dt);
		virtual void Draw(Renderer* renderer);

		virtual void OnCollision(Actor* actor) {}
		void AddChild(std::unique_ptr<Actor> actor);

		float GetRadius();
		template<class T>
		T* AddComponent();
		
		template<class T>
		T* GetComponent();


		void AddComponent(std::unique_ptr<Component> component);

		// Inherited via Serializable
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;
	public:
		bool destroy = false;
		std::string tag;



		Transform transform;
		Scene* scene{ nullptr };

		Actor* parent{ nullptr };
		std::vector<std::unique_ptr<Actor>> children;
		std::vector<std::unique_ptr<Component>> components;

	};
	template<class T>
	inline T* Actor::GetComponent()
	{
		for (auto& actor : actors)
		{
			if (dynamic_cast<T*>(actor.get())) return dynamic_cast<T*>(actor.get());
		}

		return nullptr;
	}
}