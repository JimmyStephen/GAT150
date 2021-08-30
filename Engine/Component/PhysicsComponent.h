#pragma once
#include "Component.h"
#include "Math/Vector2.h"
namespace nc {

	class PhysicsComponent : public Component {
	public:
		void Update() override;
		std::unique_ptr<Object> Clone() const { return std::make_unique<PhysicsComponent>(*this); };

		virtual void ApplyForce(const Vector2& force) { acceleration += force; };
		// Inherited via Component
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		Vector2 velocity;
		Vector2 acceleration;
		float damping = 1;

	};
}
