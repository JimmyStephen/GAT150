#pragma once
#include "Core/Serializable.h"
#include "Vector2.h"
#include "Matrix33.h"


namespace nc
{
	struct Transform : public Serializable
	{
		Vector2 position;
		float rotation = 0;
		Vector2 scale = 1;

		Vector2 localPosition;
		float localRotation = 0;
		Vector2 localScale = 1;

		// Inherited via Serializable
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

		Matrix33 matrix;

		Transform() {}
		Transform(const Vector2& position, float rotation = 0, float scale = 1) : position{ position }
			, rotation{ rotation }, scale{ scale } {}

		void Update();
		void Update(const Matrix33& mx);

	};
}